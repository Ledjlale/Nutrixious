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

#include "StepsModel.h"

#include <QObject>
#include <QVector>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>

#include "src/activity/training/sport/StepsModel.h"
#include "src/database/DatabaseQuery.h"

#include <QQmlApplicationEngine>
extern QQmlApplicationEngine * gEngine;
using namespace Description;

StepsModel::StepsModel() : StepsModel(nullptr){
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}

StepsModel::StepsModel(QObject *parent) : ExerciseModel(parent){
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	mName = "Steps";
	mInvalidName = false;
}

StepsModel::StepsModel(const StepsModel * model, QObject *parent) : ExerciseModel(model, parent){
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	mSteps = model->getSteps();
	mWeight = model->getWeight();
	mRestTime = model->getRestTime();
}

ExerciseModel * StepsModel::clone(qint64 programId, QObject *parent)const{
	StepsModel *model = new StepsModel(this, parent);
	model->setProgramId(programId);
	return model;
}

Training::ExerciseModel * StepsModel::cloneTraining(qint64 trainId, QObject *parent){
	Training::StepsModel * model = new Training::StepsModel(parent);
	model->setTargetExercise(this);
	model->setTrainId(trainId);
	model->setOrder(getOrder());
	return model;
}

int StepsModel::getType() const{
	return 2;
}


bool StepsModel::save() {
	if(!ExerciseModel::save()) return false;
	qDebug() << "Saving Steps" << mName << mDescription;
	DatabaseQuery query;

	query.begin(mExerciseId == 0 ? DatabaseQuery::Insert : DatabaseQuery::Update, isProgramLinked() ? "pgrm_ex_steps" : "ex_steps");
	if(isProgramLinked()){
		query.add("program_id", getProgramId());
		query.add("program_order", getOrder());
		if(getDescriptionExerciseId() >= 0) query.add("exercise_id", getDescriptionExerciseId());
	}
	query.add("name", mName);
	query.add("description", mDescription);
	query.add("steps", getSteps());
	query.add("rest_time", getRestTime());
	query.addConditionnal("id",mExerciseId);
	if(mExerciseId == 0){
		if(!query.exec()) qCritical() << "Cannot save"<< (isProgramLinked() ? "program" : "") <<"steps : "  << query.mQuery.lastError().text();
		auto fieldNo = query.mQuery.record().indexOf("id");
		while (query.mQuery.next()) {
			setExerciseId(query.mQuery.value(fieldNo).toInt());
			if(!isProgramLinked()) setDescriptionExerciseId(getExerciseId());
			qDebug() << "Insert"<< (isProgramLinked() ? "program" : "") <<"steps exercise: " << mExerciseId;
		}
	}else{
		if(!query.exec()) qCritical() << "Cannot update"<< (isProgramLinked() ? "program" : "") <<"steps : "  << query.mQuery.lastError().text();
		else qDebug() << "Update"<< (isProgramLinked() ? "program" : "") <<"steps exercise: " << mExerciseId;
	}
	return true;
}

void StepsModel::remove(){
	if(mExerciseId > 0){
		DatabaseQuery query;
		query.begin(DatabaseQuery::Delete, isProgramLinked() ? "pgrm_ex_steps" : "ex_steps");
		query.addConditionnal("id",mExerciseId);
		if(!query.exec()){
			if(!query.exec()) qCritical() << "Cannot delete"<< (isProgramLinked() ? "program" : "") <<"steps : "  << query.mQuery.lastError().text();
		}
	}
	emit removed(this);
}

QList<ExerciseModel*> StepsModel::load(QObject * parent){
	QList<ExerciseModel*> models;
	QSqlQuery query( "SELECT * FROM ex_steps ORDER BY id ASC");

	while (query.next()) {
		models << load(query, parent);
	}
	return models;
}

StepsModel *StepsModel::load(QSqlQuery &query, QObject * parent) {
	StepsModel * model = new StepsModel(parent);
// TODO optimize
	auto idField = query.record().indexOf("id");
	auto nameField = query.record().indexOf("name");
	auto descriptionField = query.record().indexOf("description");
	auto stepsField = query.record().indexOf("steps");
	auto restTimeField = query.record().indexOf("rest_time");
	auto programIdField = query.record().indexOf("program_id");
	auto orderField = query.record().indexOf("program_order");
	auto descriptionExerciseIdField = query.record().indexOf("exercise_id");
	model->setExerciseId(query.value(idField).toInt());
	model->setName(query.value(nameField).toString());
	model->setDescription(query.value(descriptionField).toString());
	model->setSteps(query.value(stepsField).toInt());
	model->setRestTime(query.value(restTimeField).toInt());
	model->setRestTime(query.value(restTimeField).toInt());
	if(programIdField>=0){
		model->setProgramId(query.value(programIdField).toInt());
	}
	if(orderField>=0){
		model->setOrder(query.value(orderField).toInt());
	}
	if(descriptionExerciseIdField>=0){
		model->setDescriptionExerciseId(query.value(descriptionExerciseIdField).toInt());
	}
	return model;
}

int StepsModel::getSteps() const{
	return mSteps;
}

void StepsModel::setSteps(int data){
	if(mSteps != data){
		mSteps = data;
		emit stepsChanged();
	}
}

int StepsModel::getWeight()const{
	return mWeight;
}

void StepsModel::setWeight(int data){
	if(mWeight != data){
		mWeight = data;
		emit weightChanged();
	}
}

int StepsModel::getRestTime()const{
	return mRestTime;
}

void StepsModel::setRestTime(int data){
	if(mRestTime != data){
		mRestTime = data;
		emit restTimeChanged();
	}
}
