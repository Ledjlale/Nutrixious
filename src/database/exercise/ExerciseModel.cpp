/*
 * Copyright (c) 2024 Wadel Julien.
 *
 * This file is part of Nutrixious
 * (see https://github.com/Ledjlale/Nutrixious).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ExerciseModel.h"

#include <QQmlApplicationEngine>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include "src/database/DatabaseQuery.h"

#include "src/tool/Utils.h"


extern QQmlApplicationEngine * gEngine;

ExerciseModel::ExerciseModel() : ExerciseModel(nullptr){	// QML
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}
ExerciseModel::ExerciseModel(QObject *parent)
	: QmlModel{parent}
{
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE

}

ExerciseModel::ExerciseModel(ExerciseModel * model, QObject *parent)
	: QmlModel{parent}
{
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE

	mName = initiBackup(model, &model->mName, model->mName, &mName).toString();
	mMet = initiBackup(model, &model->mMet, model->mMet, &mMet).toDouble();
	mMetMode = initiBackup(model, &model->mMetMode, model->mMetMode, &mMetMode).toInt();
	mExerciseId = initiBackup(model, &model->mExerciseId, model->mExerciseId, &mExerciseId).toLongLong();
}

ExerciseModel* ExerciseModel::clone(QObject*parent) {
	return new ExerciseModel(this, parent);

}

qint64 ExerciseModel::getExerciseId()const{
	return mExerciseId;
}

void ExerciseModel::setExerciseId(qint64 id) {
	if(mExerciseId != id) {
		mExerciseId = id;
		emit exerciseIdChanged();
	}

}

QString ExerciseModel::getName() const{
	return mName;
}

void ExerciseModel::setName(QString data) {
	if(mName != data){
		addBackup(&mName, mName, data);
		mName = data;
		emit nameChanged();
	}
}


double ExerciseModel::getMet() const {
	return mMet;
}

void ExerciseModel::setMet(double data) {
	if(mMet != data){
		addBackup(&mMet, mMet, data);
		mMet = data;
		emit metChanged();
	}
}

DEFINE_GETSET(ExerciseModel,int,metMode,MetMode)

void ExerciseModel::computeMet(){
	QSqlQuery query;
	if(!query.exec("SELECT date_time, weight FROM personal_data ORDER by date_time ASC")){
		qCritical() << "Cannot compute MET from personal data:" << query.lastError().text();
		return;
	}
	auto dateTimeField = query.record().indexOf("date_time");
	auto weightField = query.record().indexOf("weight");

	QVector<QPair<qulonglong, double>> weights;
	while(query.next()){
		weights.push_back(QPair<int, double>(query.value(dateTimeField).toULongLong(), query.value(weightField).toDouble()));
	}
	if(weights.size() == 0){
		qCritical() << "Cannot compute MET : no personal data";
	}
	query.prepare("SELECT calories, work_time, start_date_time FROM training_exercise_series,training_exercise_units, trainings  WHERE "
					"training_exercise_series.training_exercise_unit_id=training_exercise_units.training_exercise_unit_id"
					" AND trainings.training_id=training_exercise_units.training_id"
					" AND exercise_id=? AND calories > 0"
					" ORDER BY start_date_time ASC");
	query.addBindValue(mExerciseId);
	if(!query.exec()){
		qCritical() << "Cannot compute MET from trainings:" << query.lastError().text();
		return;
	}
	auto caloriesField = query.record().indexOf("calories");
	auto workField = query.record().indexOf("work_time");
	auto startField = query.record().indexOf("start_date_time");
	double mets = 0.0;
	int weightIndex = 0;
	int bestWeightIndex = 0;
	int count = 0;
	while(query.next()){
		double calories = query.value(caloriesField).toDouble();
		qulonglong trainingDate = query.value(startField).toULongLong();
		int workTime = query.value(workField).toInt();
		if( weights.size() > 0){
			if( weights.size() > 1){
				while(weightIndex < weights.size() - 1 && trainingDate > weights[weightIndex+1].first)
					++weightIndex;
				bestWeightIndex = trainingDate - weights[weightIndex].first < weights[weightIndex+1].first - trainingDate ? weightIndex : weightIndex +1;
			}
			mets += calories * 60.0/(workTime * 3.5 * weights[bestWeightIndex].second / 200.0);
			++count;
		}
	}
	if(count > 0){
		setMet(mets / count);
	}
}


//-------------------------------------------------------------------------------------------------------------------


int ExerciseModel::save(){
	if(mExerciseId>0 && !mIsEdited) return true;// Avoid update for nothing
	qDebug() << "Saving exercise " << mName;
	DatabaseQuery query;

	query.begin(mExerciseId == 0 ? DatabaseQuery::Insert : DatabaseQuery::Update, "exercises" );

	query.add("name", mName);
	query.add("met", mMet);
	query.add("met_mode", mMetMode);
	query.addConditionnal("exercise_id",mExerciseId);
	if(mExerciseId == 0){
		if(!query.exec()){
			qCritical() << "Cannot save exercise: "  << query.mQuery.lastError().text();
			return false;
		}
		auto fieldNo = query.mQuery.record().indexOf("exercise_id");
		while (query.mQuery.next()) {
			setExerciseId(query.mQuery.value(fieldNo).toInt());
			qDebug() << "Insert exercise: " << mExerciseId;
		}
	}else{
		if(!query.exec()){
			qCritical() << "Cannot update exercise: "  << query.mQuery.lastError().text();
			return false;
		} else {
			qDebug() << "Update exercise: " << mExerciseId;
		}
	}
	clearBackupValues();
	return true;
}

void ExerciseModel::remove(){
	if(mExerciseId > 0){
		DatabaseQuery query;
		query.begin(DatabaseQuery::Delete, "exercises");
		query.addConditionnal("exercise_id",mExerciseId);
		if(!query.exec()){
			if(!query.exec()) qCritical() << "Cannot delete exercise  : "  << query.mQuery.lastError().text();
		}
	}
	emit removed(this);
}

ExerciseModel *ExerciseModel::build(QSqlQuery &query, QObject * parent) {
	auto idField = query.record().indexOf("exercises.exercise_id");
	auto nameField = query.record().indexOf("exercises.name");
	auto metField = query.record().indexOf("exercises.met");
	auto metModeField = query.record().indexOf("exercises.met_mode");
	if( idField>= 0 && nameField >= 0 && metField >= 0){
		ExerciseModel * model = new ExerciseModel(parent);
	// TODO optimize
		model->setExerciseId(query.value(idField).toInt());
		model->setName(query.value(nameField).toString());
		model->setMet(query.value(metField).toDouble());
		model->setMetMode(query.value(metModeField).toInt());
		model->clearBackupValues();
		return model;
	}else return nullptr;
}

QList<ExerciseModel*> ExerciseModel::buildAll(QObject * parent){
	QList<ExerciseModel*> models;
	QSqlQuery query;
	if(!query.exec( "SELECT * FROM exercises ORDER BY exercise_id DESC")) qCritical() << "Cannot select all exercises  : "  << query.lastError().text();

	while (query.next()) {
		auto model = build(query, parent);
		models << model;
	}

	return models;
}
