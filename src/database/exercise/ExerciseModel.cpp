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


//-------------------------------------------------------------------------------------------------------------------


bool ExerciseModel::save(){
	if(mExerciseId>0 && !mIsEdited) return true;// Avoid update for nothing
	qDebug() << "Saving exercise " << mName;
	DatabaseQuery query;

	query.begin(mExerciseId == 0 ? DatabaseQuery::Insert : DatabaseQuery::Update, "exercises" );

	query.add("name", mName);
	query.add("met", mMet);
	query.addConditionnal("exercise_id",mExerciseId);
	if(mExerciseId == 0){
		if(!query.exec()) qCritical() << "Cannot save exercise: "  << query.mQuery.lastError().text();
		auto fieldNo = query.mQuery.record().indexOf("exercise_id");
		while (query.mQuery.next()) {
			setExerciseId(query.mQuery.value(fieldNo).toInt());
			qDebug() << "Insert exercise: " << mExerciseId;
		}
	}else{
		if(!query.exec()) qCritical() << "Cannot update exercise: "  << query.mQuery.lastError().text();
		else {
			qDebug() << "Update exercise: " << mExerciseId;
		}
	}
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
	if( idField>= 0 && nameField >= 0 && metField >= 0){
		ExerciseModel * model = new ExerciseModel(parent);
	// TODO optimize
		model->setExerciseId(query.value(idField).toInt());
		model->setName(query.value(nameField).toString());
		model->setMet(query.value(metField).toDouble());
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
