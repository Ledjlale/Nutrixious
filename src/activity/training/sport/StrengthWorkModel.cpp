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

#include <QQmlApplicationEngine>
extern QQmlApplicationEngine * gEngine;

using namespace Training;



StrengthWorkModel::StrengthWorkModel() : StrengthWorkModel(nullptr){
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}

StrengthWorkModel::StrengthWorkModel(QObject *parent) : QObject(parent){
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	connect(this, &StrengthWorkModel::finished, [this](){
		setIsRunning(false);
		setIsDone(true);
	});
	connect(this, &StrengthWorkModel::workIdChanged, [this](){
		setIsSaved(getWorkId() > 0);
	});
}

StrengthWorkModel::StrengthWorkModel(const StrengthWorkModel * model, QObject *parent) : QObject(parent){
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	mRepetitions = model->getRepetitions();
	mWeight = model->getWeight();
	mWorkTime = model->getWorkTime();
	mRestTime = model->getRestTime();
	mIsSaved = mWorkId > 0;

	connect(this, &StrengthWorkModel::finished, [this](){
		setIsRunning(false);
		setIsDone(true);
	});
	connect(this, &StrengthWorkModel::workIdChanged, [this](){
		setIsSaved(getWorkId() > 0);
	});
}

StrengthWorkModel * StrengthWorkModel::clone(qint64 trainId, qint64 strengthId, QObject *parent)const{
	StrengthWorkModel *model = new StrengthWorkModel(this, parent);
	model->setTrainId(trainId);
	model->setStrengthId(strengthId);
	return model;
}

void StrengthWorkModel::setTargetWork(Description::StrengthWorkModel * data) {
	if(mTargetWork != data) {
		mTargetWork = data;
		setRepetitions(mTargetWork->getRepetitions());
		setWeight(mTargetWork->getWeight());
		setRestTime(mTargetWork->getRestTime());
		emit targetWorkChanged();
	}
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

int StrengthWorkModel::getWorkTime() const{
	return mWorkTime;
}
void StrengthWorkModel::setWorkTime(int workTime){
	if( mWorkTime != workTime){
		mWorkTime = workTime;
		emit workTimeChanged();
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

qint64 StrengthWorkModel::getTrainId() const{
	return mTrainId;
}

void StrengthWorkModel::setTrainId(qint64 id){
	if(mTrainId != id){
		mTrainId = id;
		emit trainIdChanged();
	}
}

bool StrengthWorkModel::getIsRunning() const {
	return mIsRunning;
}

void StrengthWorkModel::setIsRunning(bool data) {
	if(mIsRunning != data){
		mIsRunning = data;
		emit isRunningChanged();
	}
}

bool StrengthWorkModel::getIsResting() const {
	return mIsResting;
}

void StrengthWorkModel::setIsResting(bool data) {
	if(mIsResting != data){
		mIsResting = data;
		if(mIsResting)
			mStart = QDateTime::currentDateTime();
		else
			endOfCurrentRest();
		emit isRestingChanged();
	}
}

bool StrengthWorkModel::getIsDone() const {
	return mIsDone;
}

void StrengthWorkModel::setIsDone(bool data) {
	if(mIsDone != data){
		mIsDone = data;
		emit isDoneChanged();
	}
}

bool StrengthWorkModel::getIsSaved() const {
	return mIsSaved;
}

void StrengthWorkModel::setIsSaved(bool data) {
	if(mIsSaved != data){
		mIsSaved = data;
		emit isSavedChanged();
	}
}

void StrengthWorkModel::startWork(){
	mStart = QDateTime::currentDateTime();
	setIsRunning(true);
	setIsResting(false);
	setIsDone(false);
}

void StrengthWorkModel::endOfCurrentWork(){
	setWorkTime(mStart.secsTo(QDateTime::currentDateTime()));
	setIsResting(true);
}

void StrengthWorkModel::endOfCurrentRest() {
	setRestTime(mStart.secsTo(QDateTime::currentDateTime()));
	emit finished();
}

void StrengthWorkModel::save() {
	if(getStrengthId() <=0 || getTrainId() <= 0) {
		qCritical() << "Strength ID or Train ID was not set for an update!";
		return;
	}

	DatabaseQuery query;

	query.begin(mWorkId == 0 ? DatabaseQuery::Insert : DatabaseQuery::Update, "tr_ex_strength_set");

	query.add("train_id", getTrainId());
	query.add("strength_id", getStrengthId());
	query.add("reps", getRepetitions());
	query.add("weight", getWeight());
	query.add("rest_time", getRestTime());
	query.add("work_time", getWorkTime());
	query.addConditionnal("id",mWorkId);
	if(mWorkId == 0) {
		if(!query.exec()) qCritical() << "Cannot save train set : "  << query.mQuery.lastError().text();
		auto fieldNo = query.mQuery.record().indexOf("id");
		while (query.mQuery.next()) {
			setWorkId(query.mQuery.value(fieldNo).toInt());
			qDebug() << "Insert train strength exercise set: " << mWorkId;
		}
	}else{
		if(!query.exec()) qCritical() << "Cannot update train set : "  << query.mQuery.lastError().text();
		else qDebug() << "Update train strength exercise set: " << mWorkId;
	}
}

StrengthWorkModel *StrengthWorkModel::load(QSqlQuery &query, QObject * parent) {
	StrengthWorkModel * model = new StrengthWorkModel(parent);
// TODO optimize
	auto idField = query.record().indexOf("id");
	auto trainIdField = query.record().indexOf("train_id");
	auto strengthIdField = query.record().indexOf("strength_id");
	auto repsField = query.record().indexOf("reps");
	auto weightField = query.record().indexOf("weight");
	auto workTimeField = query.record().indexOf("work_time");
	auto restTimeField = query.record().indexOf("rest_time");
	model->setWorkId(query.value(idField).toInt());
	if(trainIdField >= 0) model->setTrainId(query.value(trainIdField).toInt());
	model->setStrengthId(query.value(strengthIdField).toInt());
	model->setRepetitions(query.value(repsField).toInt());
	model->setWeight(query.value(weightField).toInt());
	model->setWorkTime(query.value(workTimeField).toInt());
	model->setRestTime(query.value(restTimeField).toInt());
	return model;
}

void StrengthWorkModel::fillRandomValues(){
	setWeight(100.0 * std::rand() / RAND_MAX);
}
