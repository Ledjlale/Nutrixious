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

#include "DistanceModel.h"

#include <QObject>
#include <QVector>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include "src/database/DatabaseQuery.h"

#include "src/activity/training/sport/DistanceModel.h"

#include <QQmlApplicationEngine>
extern QQmlApplicationEngine * gEngine;

using namespace Description;

DistanceModel::DistanceModel() : DistanceModel(nullptr){
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}

DistanceModel::DistanceModel(QObject *parent) : ExerciseModel(parent){
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	mName = "Distance";
	mInvalidName = false;
}

DistanceModel::DistanceModel(const DistanceModel * model, QObject *parent) : ExerciseModel(model, parent){
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	mDistance = model->getDistance();
	mWeight = model->getWeight();
	mRestTime = model->getRestTime();
}

ExerciseModel * DistanceModel::clone(qint64 programId, QObject *parent)const{
	ExerciseModel *model = new DistanceModel(this, parent);
	model->setProgramId(programId);
	return model;
}

Training::ExerciseModel * DistanceModel::cloneTraining(qint64 trainId, QObject *parent){
	Training::DistanceModel * model = new Training::DistanceModel(parent);
	model->setTargetExercise(this);
	model->setTrainId(trainId);
	model->setOrder(getOrder());
	return model;
}

int DistanceModel::getType() const{
	return 1;
}

bool DistanceModel::save() {
	if(!ExerciseModel::save()) return false;
	qDebug() << "Saving Distance" << mName << mDescription;
	DatabaseQuery query;

	query.begin(mExerciseId == 0 ? DatabaseQuery::Insert : DatabaseQuery::Update, isProgramLinked() ? "pgrm_ex_distance" : "ex_distance");
	if(isProgramLinked()){
		query.add("program_id", getProgramId());
		query.add("program_order", getOrder());
		if(getDescriptionExerciseId() >= 0) query.add("exercise_id", getDescriptionExerciseId());
	}
	query.add("name", mName);
	query.add("description", mDescription);
	query.add("distance", getDistance());
	query.add("rest_time", getRestTime());
	query.addConditionnal("id",mExerciseId);
	if(mExerciseId == 0){
		if(!query.exec()) qCritical() << "Cannot save"<< (isProgramLinked() ? "program" : "") <<"distance : "  << query.mQuery.lastError().text();
		auto fieldNo = query.mQuery.record().indexOf("id");
		while (query.mQuery.next()) {
			setExerciseId(query.mQuery.value(fieldNo).toInt());
			if(!isProgramLinked()) setDescriptionExerciseId(getExerciseId());
			qDebug() << "Insert"<< (isProgramLinked() ? "program" : "") <<"distance exercise: " << mExerciseId;
		}
	}else{
		if(!query.exec()) qCritical() << "Cannot update"<< (isProgramLinked() ? "program" : "") <<"distance : "  << query.mQuery.lastError().text();
		else qDebug() << "Update"<< (isProgramLinked() ? "program" : "") <<"distance exercise: " << mExerciseId;
	}
	return true;
}

void DistanceModel::remove(){
	if(mExerciseId > 0){
		DatabaseQuery query;
		query.begin(DatabaseQuery::Delete, isProgramLinked() ? "pgrm_ex_distance" : "ex_distance");
		query.addConditionnal("id",mExerciseId);
		if(!query.exec()){
			if(!query.exec()) qCritical() << "Cannot delete"<< (isProgramLinked() ? "program" : "") <<"distance : "  << query.mQuery.lastError().text();
		}
	}
	emit removed(this);
}

QList<ExerciseModel*> DistanceModel::load(QObject * parent){
	QList<ExerciseModel*> models;
	QSqlQuery query( "SELECT * FROM ex_distance ORDER BY id ASC");

	while (query.next()) {
		models << load(query, parent);
	}
	return models;
}

DistanceModel *DistanceModel::load(QSqlQuery &query, QObject * parent) {
	DistanceModel * model = new DistanceModel(parent);
// TODO optimize
	auto idField = query.record().indexOf("id");
	auto nameField = query.record().indexOf("name");
	auto descriptionField = query.record().indexOf("description");
	auto distanceField = query.record().indexOf("distance");
	auto restTimeField = query.record().indexOf("rest_time");
	auto programIdField = query.record().indexOf("program_id");
	auto orderField = query.record().indexOf("program_order");
	auto descriptionExerciseIdField = query.record().indexOf("exercise_id");
	model->setExerciseId(query.value(idField).toInt());
	model->setName(query.value(nameField).toString());
	model->setDescription(query.value(descriptionField).toString());
	model->setDistance(query.value(distanceField).toInt());
	model->setRestTime(query.value(restTimeField).toInt());if(programIdField>=0){
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

int DistanceModel::getDistance() const{
	return mDistance;
}

void DistanceModel::setDistance(int data){
	if(mDistance != data){
		mDistance = data;
		emit distanceChanged();
	}
}

int DistanceModel::getWeight()const{
	return mWeight;
}

void DistanceModel::setWeight(int data){
	if(mWeight != data){
		mWeight = data;
		emit weightChanged();
	}
}

int DistanceModel::getRestTime()const{
	return mRestTime;
}

void DistanceModel::setRestTime(int data){
	if(mRestTime != data){
		mRestTime = data;
		emit restTimeChanged();
	}
}
