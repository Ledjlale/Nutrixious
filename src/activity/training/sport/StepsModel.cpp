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

#include "src/activity/description/sport/StepsModel.h"
#include "src/database/DatabaseQuery.h"

using namespace Training;

StepsModel::StepsModel(QObject *parent) : ExerciseModel(parent){
	mName = "Steps";
	mInvalidName = false;
	mType = 2;
}

StepsModel::StepsModel(const StepsModel * model, QObject *parent) : ExerciseModel(model, parent){
	mSteps = model->getSteps();
	mWeight = model->getWeight();
	mWorkTime = model->getWorkTime();
	mRestTime = model->getRestTime();
	mType = 2;
}

ExerciseModel * StepsModel::clone(qint64 trainId)const{
	StepsModel *model = new StepsModel(this, nullptr);
	model->setTrainId(trainId);
	return model;
}

void StepsModel::setTargetExercise(Description::ExerciseModel * data){
	ExerciseModel::setTargetExercise(data);
	if(data){
		auto model = dynamic_cast<Description::StepsModel*>(data);
		setSteps(model->getSteps());
		setWeight(model->getWeight());
		setRestTime(model->getRestTime());
	}
}

bool StepsModel::save() {
	if(!ExerciseModel::save()) return false;
	qDebug() << "Saving Steps" << mName << mDescription;
	DatabaseQuery query;

	query.begin(mDbId == 0 ? DatabaseQuery::Insert : DatabaseQuery::Update, "tr_ex_steps");
	query.add("train_id", getTrainId());
	query.add("train_order", getTrainOrder());
	query.add("name", mName);
	query.add("description", mDescription);
	query.add("steps", getSteps());
	query.add("rest_time", getRestTime());
	query.addConditionnal("id",mDbId);
	if(mDbId == 0){
		if(!query.exec()) qCritical() << "Cannot save train steps : "  << query.mQuery.lastError().text();
		auto fieldNo = query.mQuery.record().indexOf("id");
		while (query.mQuery.next()) {
			setId(query.mQuery.value(fieldNo).toInt());
			qDebug() << "Insert train steps exercise: " << mDbId;
		}
	}else{
		if(!query.exec()) qCritical() << "Cannot update train steps : "  << query.mQuery.lastError().text();
		else qDebug() << "Update train steps exercise: " << mDbId;
	}
	return true;
}

QList<ExerciseModel*> StepsModel::load(){
	QList<ExerciseModel*> models;
	QSqlQuery query( "SELECT * FROM tr_ex_steps ORDER BY id ASC");

	while (query.next()) {
		models << load(query);
	}
	return models;
}

StepsModel *StepsModel::load(QSqlQuery &query) {
	StepsModel * model = new StepsModel();
// TODO optimize
	auto idField = query.record().indexOf("id");
	auto nameField = query.record().indexOf("name");
	auto descriptionField = query.record().indexOf("description");
	auto stepsField = query.record().indexOf("steps");
	auto restTimeField = query.record().indexOf("rest_time");
	auto trainIdField = query.record().indexOf("train_id");
	auto trainOrderField = query.record().indexOf("train_order");
	model->setId(query.value(idField).toInt());
	model->setName(query.value(nameField).toString());
	model->setDescription(query.value(descriptionField).toString());
	model->setSteps(query.value(stepsField).toInt());
	model->setRestTime(query.value(restTimeField).toInt());
	model->setRestTime(query.value(restTimeField).toInt());
	if(trainIdField>=0){
		model->setTrainId(query.value(trainIdField).toInt());
	}
	if(trainOrderField>=0){
		model->setTrainOrder(query.value(trainOrderField).toInt());
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

int StepsModel::getWorkTime()const{
	return mWorkTime;
}

void StepsModel::setWorkTime(int data){
	if(mWorkTime != data){
		mWorkTime = data;
		emit workTimeChanged();
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

void StepsModel::endOfCurrentWork(){
	setWorkTime(mStart.secsTo(QDateTime::currentDateTime()));
	setIsResting(true);
}

void StepsModel::endOfCurrentRest() {
	setRestTime(mStart.secsTo(QDateTime::currentDateTime()));
	emit finished();
}
