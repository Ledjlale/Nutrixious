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

#include "StrengthWorkModel.h"

#include <QObject>
#include <QVector>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

#include "src/database/DatabaseQuery.h"
#include "src/activity/training/sport/StrengthWorkModel.h"

#include <QQmlApplicationEngine>
extern QQmlApplicationEngine * gEngine;

using namespace Description;

StrengthWorkModel::StrengthWorkModel() : StrengthWorkModel(nullptr){
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}

StrengthWorkModel::StrengthWorkModel(QObject *parent) : QObject(parent){
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
}

StrengthWorkModel::StrengthWorkModel(const StrengthWorkModel * model, QObject *parent) : QObject(parent){
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	mRepetitions = model->getRepetitions();
	mWeight = model->getWeight();
	mRestTime = model->getRestTime();
}

StrengthWorkModel * StrengthWorkModel::clone(qint64 strengthId, QObject *parent)const{
	StrengthWorkModel *model = new StrengthWorkModel(this, parent);
	model->setStrengthId(strengthId);
	return model;
}

Training::StrengthWorkModel * StrengthWorkModel::cloneTraining(qint64 trainId, qint64 strengthId, QObject *parent){
	Training::StrengthWorkModel * model = new Training::StrengthWorkModel(parent);
	model->setTargetWork(this);
	model->setTrainId(trainId);
	model->setStrengthId(strengthId);
	return model;
}

int StrengthWorkModel::getRepetitions() const {
	return mRepetitions;
}
void StrengthWorkModel::setRepetitions(int reps){
	if(mRepetitions != reps){
		mRepetitions = reps;
		emit repetitionsChanged();
	}
}

int StrengthWorkModel::getWeight() const{
	return mWeight;
}
void StrengthWorkModel::setWeight(int weight){
	if( mWeight != weight){
		mWeight = weight;
		emit weightChanged();
	}
}

int StrengthWorkModel::getRestTime() const{
	return mRestTime;
}
void StrengthWorkModel::setRestTime(int restTime){
	if( mRestTime != restTime){
		mRestTime = restTime;
		emit restTimeChanged();
	}
}

qint64 StrengthWorkModel::getWorkId() const{
	return mWorkId;
}

void StrengthWorkModel::setWorkId(qint64 id){
	if(mWorkId != id){
		mWorkId = id;
		emit workIdChanged();
	}
}

qint64 StrengthWorkModel::getStrengthId() const{
	return mStrengthId;
}

void StrengthWorkModel::setStrengthId(qint64 id){
	if(mStrengthId != id){
		mStrengthId = id;
		emit strengthIdChanged();
	}
}

qint64 StrengthWorkModel::getProgramId() const{
	return mProgramId;
}

void StrengthWorkModel::setProgramId(qint64 id){
	if(mProgramId != id){
		mProgramId = id;
		emit programIdChanged();
	}
}

bool StrengthWorkModel::isProgramLinked() const {
	return mProgramId >= 0;
}

void StrengthWorkModel::save() {
	if(getStrengthId() <=0) {
		qCritical() << "Strength ID was not set for an update!";
		return;
	}

	DatabaseQuery query;

	query.begin(mWorkId == 0 ? DatabaseQuery::Insert : DatabaseQuery::Update, isProgramLinked() ? "pgrm_ex_strength_set" : "ex_strength_set");
	if(isProgramLinked()){
		query.add("program_id", getProgramId());
	}
	query.add("strength_id", getStrengthId());
	query.add("reps", getRepetitions());
	query.add("weight", getWeight());
	query.add("rest_time", getRestTime());
	query.addConditionnal("id",mWorkId);
	if(mWorkId == 0) {
		if(!query.exec()) qCritical() << "Cannot save"<< (isProgramLinked() ? "program" : "") <<"set : "  << query.mQuery.lastError().text();
		auto fieldNo = query.mQuery.record().indexOf("id");
		while (query.mQuery.next()) {
			setWorkId(query.mQuery.value(fieldNo).toInt());
			qDebug() << "Insert"<< (isProgramLinked() ? "program" : "") <<"strength exercise set: " << mWorkId;
		}
	}else{
		if(!query.exec()) qCritical() << "Cannot update"<< (isProgramLinked() ? "program" : "") <<"set : "  << query.mQuery.lastError().text();
		else qDebug() << "Update"<< (isProgramLinked() ? "program" : "") <<"strength exercise set: " << mWorkId;
	}
}


StrengthWorkModel *StrengthWorkModel::load(QSqlQuery &query, QObject * parent) {
	StrengthWorkModel * model = new StrengthWorkModel(parent);
// TODO optimize
	auto idField = query.record().indexOf("id");
	auto programIdField = query.record().indexOf("program_id");
	auto strengthIdField = query.record().indexOf("strength_id");
	auto repsField = query.record().indexOf("reps");
	auto weightField = query.record().indexOf("weight");
	auto restTimeField = query.record().indexOf("rest_time");
	model->setWorkId(query.value(idField).toInt());
	if(programIdField >= 0) model->setProgramId(query.value(programIdField).toInt());
	model->setStrengthId(query.value(strengthIdField).toInt());
	model->setRepetitions(query.value(repsField).toInt());
	model->setWeight(query.value(weightField).toInt());
	model->setRestTime(query.value(restTimeField).toInt());
	return model;
}
