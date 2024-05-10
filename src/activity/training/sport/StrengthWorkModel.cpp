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

using namespace Training;

StrengthWorkModel::StrengthWorkModel(QObject *parent) : QObject(parent){
	connect(this, &StrengthWorkModel::finished, [this](){
		setIsRunning(false);
		setIsDone(true);
	});
}

StrengthWorkModel::StrengthWorkModel(const StrengthWorkModel * model, QObject *parent) : QObject(parent){
	mRepetitions = model->getRepetitions();
	mWeight = model->getWeight();
	mWorkTime = model->getWorkTime();
	mRestTime = model->getRestTime();

	connect(this, &StrengthWorkModel::finished, [this](){
		setIsRunning(false);
		setIsDone(true);
	});
}

StrengthWorkModel * StrengthWorkModel::clone()const{
	StrengthWorkModel *model = new StrengthWorkModel(this, nullptr);
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

StrengthWorkModel *StrengthWorkModel::load(QSqlQuery &query) {
	StrengthWorkModel * model = new StrengthWorkModel();
// TODO optimize
	auto idField = query.record().indexOf("id");
	auto repsField = query.record().indexOf("reps");
	auto weightField = query.record().indexOf("weight");
	auto workTimeField = query.record().indexOf("work_time");
	auto restTimeField = query.record().indexOf("rest_time");
	model->setId(query.value(idField).toInt());
	model->setRepetitions(query.value(repsField).toInt());
	model->setWeight(query.value(weightField).toInt());
	model->setWorkTime(query.value(workTimeField).toInt());
	model->setRestTime(query.value(restTimeField).toInt());
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

qint64 StrengthWorkModel::getId() const{
	return mDbId;
}

void StrengthWorkModel::setId(qint64 id){
	if(mDbId != id){
		mDbId = id;
		emit idChanged();
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


void StrengthWorkModel::save(qint64 parentId) {
/*
	QSqlQuery query;
	if( mDbId == 0) {
		query.prepare("INSERT INTO ex_strength_set(strength_id, reps, weight, work_time, rest_time) VALUES(?,?,?,?,?) RETURNING id");
		query.addBindValue(parentId);
		query.addBindValue(getRepetitions());
		query.addBindValue(getWeight());
		query.addBindValue(getWorkTime());
		query.addBindValue(getRestTime());
		if(!query.exec()) qCritical() << "Cannot save steps : "  << query.lastError().text();
		auto fieldNo = query.record().indexOf("id");
		while (query.next()) {
			mDbId = query.value(fieldNo).toInt();
			qDebug() << "Insert strength exercise steps: " << mDbId;
		}
	}else{
		query.prepare("UPDATE ex_strength_set SET strength_id=?, reps=?, weight=?, work_time=?, rest_time=?  WHERE id=?");
		query.addBindValue(parentId);
		query.addBindValue(getRepetitions());
		query.addBindValue(getWeight());
		query.addBindValue(getWorkTime());
		query.addBindValue(getRestTime());
		query.addBindValue(mDbId);
		qDebug() << "Update strength exercise steps : " << mDbId << query.exec();
	}
	*/
}

void StrengthWorkModel::saveProgram(qint64 programId, qint64 parentId){
/*
	QSqlQuery query;
	if( mDbId == 0) {
		query.prepare("INSERT INTO pgrm_ex_strength_set(program_id, strength_id, reps, weight, work_time, rest_time) VALUES(?,?,?,?,?,?) RETURNING id");
		query.addBindValue(programId);
		query.addBindValue(parentId);
		query.addBindValue(getRepetitions());
		query.addBindValue(getWeight());
		query.addBindValue(getWorkTime());
		query.addBindValue(getRestTime());
		if(!query.exec()) qCritical() << "Cannot save steps : "  << query.lastError().text();
		auto fieldNo = query.record().indexOf("id");
		while (query.next()) {
			mDbId = query.value(fieldNo).toInt();
			qDebug() << "Insert program strength exercise steps: " << mDbId;
		}
	}else{
		query.prepare("UPDATE pgrm_ex_strength_set SET program_id=?, strength_id=?, reps=?, weight=?, work_time=?, rest_time=?  WHERE id=?");
		query.addBindValue(programId);
		query.addBindValue(parentId);
		query.addBindValue(getRepetitions());
		query.addBindValue(getWeight());
		query.addBindValue(getWorkTime());
		query.addBindValue(getRestTime());
		query.addBindValue(mDbId);
		qDebug() << "Update program strength exercise steps : " << mDbId << query.exec();
	}
	*/
}
