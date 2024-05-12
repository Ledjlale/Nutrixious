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

#include "src/activity/description/sport/DistanceModel.h"
#include "src/database/DatabaseQuery.h"

#include <QQmlApplicationEngine>
extern QQmlApplicationEngine * gEngine;
using namespace Training;

DistanceModel::DistanceModel() : DistanceModel(nullptr){
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}

DistanceModel::DistanceModel(QObject *parent) : ExerciseModel(parent){
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	mName = "Distance";
	mInvalidName = false;
	mType = 1;
}

DistanceModel::DistanceModel(const DistanceModel * model, QObject *parent) : ExerciseModel(model, parent){
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	mDistance = model->getDistance();
	mWeight = model->getWeight();
	mWorkTime = model->getWorkTime();
	mRestTime = model->getRestTime();
	mIsDone = mExerciseId > 0;
	mType = 1;
}

ExerciseModel * DistanceModel::clone(qint64 trainId, QObject *parent)const{
	ExerciseModel *model = new DistanceModel(this, parent);
	model->setTrainId(trainId);
	return model;
}

void DistanceModel::setTargetExercise(Description::ExerciseModel * data){
	ExerciseModel::setTargetExercise(data);
	if(data){
		auto model = dynamic_cast<Description::DistanceModel*>(data);
		setDistance(model->getDistance());
		setWeight(model->getWeight());
		setRestTime(model->getRestTime());
	}
}

bool DistanceModel::save() {
	if(!ExerciseModel::save()) return false;
	qDebug() << "Saving Distance" << mName << mDescription;
	DatabaseQuery query;

	query.begin(mExerciseId == 0 ? DatabaseQuery::Insert : DatabaseQuery::Update, "tr_ex_distance");

	query.add("train_id", getTrainId());
	if(getDescriptionExerciseId() >= 0) query.add("exercise_id", getDescriptionExerciseId());
	query.add("train_order", getOrder());
	query.add("name", mName);
	query.add("description", mDescription);
	query.add("distance", getDistance());
	query.add("rest_time", getRestTime());
	query.addConditionnal("id",mExerciseId);
	if(mExerciseId == 0){
		if(!query.exec()) qCritical() << "Cannot save training distance : "  << query.mQuery.lastError().text();
		auto fieldNo = query.mQuery.record().indexOf("id");
		while (query.mQuery.next()) {
			setExerciseId(query.mQuery.value(fieldNo).toInt());
			qDebug() << "Insert training distance exercise: " << mExerciseId;
		}
	}else{
		if(!query.exec()) qCritical() << "Cannot update training distance : "  << query.mQuery.lastError().text();
		else qDebug() << "Update training distance exercise: " << mExerciseId;
	}

	return false;
}

QList<ExerciseModel*> DistanceModel::load(QObject * parent){
	QList<ExerciseModel*> models;
	QSqlQuery query( "SELECT * FROM tr_ex_distance ORDER BY id ASC");

	while (query.next()) {
		models  << load(query, parent);
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
	auto trainIdField = query.record().indexOf("train_id");
	auto orderField = query.record().indexOf("train_order");
	auto descriptionExerciseIdField = query.record().indexOf("exercise_id");
	model->setExerciseId(query.value(idField).toInt());
	model->setName(query.value(nameField).toString());
	model->setDescription(query.value(descriptionField).toString());
	model->setDistance(query.value(distanceField).toInt());
	model->setRestTime(query.value(restTimeField).toInt());
	if(trainIdField>=0){
		model->setTrainId(query.value(trainIdField).toInt());
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

int DistanceModel::getWorkTime()const{
	return mWorkTime;
}

void DistanceModel::setWorkTime(int data){
	if(mWorkTime != data){
		mWorkTime = data;
		emit workTimeChanged();
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

void DistanceModel::endOfCurrentWork(){
	setWorkTime(mStart.secsTo(QDateTime::currentDateTime()));
	setIsResting(true);
}

void DistanceModel::endOfCurrentRest() {
	setRestTime(mStart.secsTo(QDateTime::currentDateTime()));
	emit finished();
}

void DistanceModel::fillRandomValues(){
	setDistance(100000.0 * std::rand() / RAND_MAX);
}
