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

using namespace Description;

DistanceModel::DistanceModel(QObject *parent) : ExerciseModel(parent){
	mName = "Distance";
	mInvalidName = false;
}

DistanceModel::DistanceModel(const DistanceModel * model, QObject *parent) : ExerciseModel(model, parent){

	mDistance = model->getDistance();
	mWeight = model->getWeight();
	mRestTime = model->getRestTime();
}

ExerciseModel * DistanceModel::clone(qint64 programId)const{
	ExerciseModel *model = new DistanceModel(this, nullptr);
	model->setProgramId(programId);
	return model;
}

Training::ExerciseModel * DistanceModel::cloneTraining(qint64 trainId){
	Training::DistanceModel * model = new Training::DistanceModel();
	model->setTargetExercise(this);
	model->setTrainId(trainId);
	return model;
}

int DistanceModel::getType() const{
	return 1;
}

bool DistanceModel::save() {
	if(!ExerciseModel::save()) return false;
	qDebug() << "Saving Distance" << mName << mDescription;
	DatabaseQuery query;

	query.begin(mDbId == 0 ? DatabaseQuery::Insert : DatabaseQuery::Update, isProgramLinked() ? "pgrm_ex_distance" : "ex_distance");
	if(isProgramLinked()){
		query.add("program_id", getProgramId());
		query.add("program_order", getProgramOrder());
	}
	query.add("name", mName);
	query.add("description", mDescription);
	query.add("distance", getDistance());
	query.add("rest_time", getRestTime());
	query.addConditionnal("id",mDbId);
	if(mDbId == 0){
		if(!query.exec()) qCritical() << "Cannot save"<< (isProgramLinked() ? "program" : "") <<"distance : "  << query.mQuery.lastError().text();
		auto fieldNo = query.mQuery.record().indexOf("id");
		while (query.mQuery.next()) {
			setId(query.mQuery.value(fieldNo).toInt());
			qDebug() << "Insert"<< (isProgramLinked() ? "program" : "") <<"distance exercise: " << mDbId;
		}
	}else{
		if(!query.exec()) qCritical() << "Cannot update"<< (isProgramLinked() ? "program" : "") <<"distance : "  << query.mQuery.lastError().text();
		else qDebug() << "Update"<< (isProgramLinked() ? "program" : "") <<"distance exercise: " << mDbId;
	}
	return true;
}

QList<ExerciseModel*> DistanceModel::load(){
	QList<ExerciseModel*> models;
	QSqlQuery query( "SELECT * FROM ex_distance ORDER BY id ASC");

	auto idField = query.record().indexOf("id");
	auto nameField = query.record().indexOf("name");
	auto descriptionField = query.record().indexOf("description");
	auto distanceField = query.record().indexOf("distance");
	auto restTimeField = query.record().indexOf("rest_time");
	QStringList ids;

	while (query.next()) {
		DistanceModel * model = new DistanceModel();
		qint64 id = query.value(idField).toInt();
		ids << QString::number(id);
		model->setId(id);
		model->setName(query.value(nameField).toString());
		model->setDescription(query.value(descriptionField).toString());
		model->setDistance(query.value(distanceField).toInt());
		model->setRestTime(query.value(restTimeField).toInt());
		models << model;
	}
	return models;
}

DistanceModel *DistanceModel::load(QSqlQuery &query) {
	DistanceModel * model = new DistanceModel();
// TODO optimize
	auto idField = query.record().indexOf("id");
	auto nameField = query.record().indexOf("name");
	auto descriptionField = query.record().indexOf("description");
	auto distanceField = query.record().indexOf("distance");
	auto restTimeField = query.record().indexOf("rest_time");
	auto programIdField = query.record().indexOf("program_id");
	auto programOrderField = query.record().indexOf("program_order");
	model->setId(query.value(idField).toInt());
	model->setName(query.value(nameField).toString());
	model->setDescription(query.value(descriptionField).toString());
	model->setDistance(query.value(distanceField).toInt());
	model->setRestTime(query.value(restTimeField).toInt());if(programIdField>=0){
		model->setProgramId(query.value(programIdField).toInt());
	}
	if(programOrderField>=0){
		model->setProgramOrder(query.value(programOrderField).toInt());
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
