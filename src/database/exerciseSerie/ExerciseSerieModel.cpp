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

#include "ExerciseSerieModel.h"
#include <QSqlQuery>
#include <QSqlRecord>

#include "src/database/DatabaseQuery.h"

#include <QQmlApplicationEngine>
extern QQmlApplicationEngine * gEngine;

ExerciseSerieModel::ExerciseSerieModel() : QmlModel(nullptr){
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}

ExerciseSerieModel::ExerciseSerieModel(QObject *parent)
	: QmlModel{parent}
{
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
}

ExerciseSerieModel::ExerciseSerieModel(const ExerciseSerieModel * model, QObject *parent) : ExerciseSerieModel(parent){
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	setSerieId(model->getSerieId());
	setExerciseUnitId(model->getExerciseUnitId());
	setOrder(model->getOrder());
	setRestTime(model->getRestTime());
	setWorkTime(model->getWorkTime());

	setDistance(model->getDistance());
	setSpeed(model->getSpeed());
	setWeight(model->getWeight());
	setRepetitions(model->getRepetitions());
}

ExerciseSerieModel::~ExerciseSerieModel(){
}

ExerciseSerieModel * ExerciseSerieModel::clone(QObject *parent)const{
	ExerciseSerieModel *model = new ExerciseSerieModel(this, parent);
	model->clearBackupValues();
	return model;
}


qint64 ExerciseSerieModel::getSerieId()const{
	return mSerieId;
}

void ExerciseSerieModel::setSerieId(qint64 id) {
	if(mSerieId != id){
		mSerieId = id;
		setIsSaved(getSerieId() > 0);
		emit serieIdChanged();
	}
}

qint64 ExerciseSerieModel::getExerciseUnitId()const{
	return mExerciseUnitId;
}

void ExerciseSerieModel::setExerciseUnitId(qint64 id) {
	if(mExerciseUnitId != id){
		mExerciseUnitId = id;
		emit exerciseUnitIdChanged();
	}
}

int ExerciseSerieModel::getOrder() const {
	return mOrder;
}

void ExerciseSerieModel::setOrder(int data){
	if(mOrder != data){
		addBackup(&mOrder, mOrder, data);
		mOrder = data;
		emit orderChanged();
	}
}

int ExerciseSerieModel::getWorkTime() const{
	return mWorkTime;
}
void ExerciseSerieModel::setWorkTime(int data){
	if( mWorkTime != data){
		addBackup(&mWorkTime, mWorkTime, data);
		mWorkTime = data;
		emit workTimeChanged();
	}
}

int ExerciseSerieModel::getRestTime() const{
	return mRestTime;
}

void ExerciseSerieModel::setRestTime(int data){
	if( mRestTime != data){
		addBackup(&mRestTime, mRestTime, data);
		mRestTime = data;
		emit restTimeChanged();
	}
}

//-------------------------------------------------------------------------------------------------------------------

int ExerciseSerieModel::getDistance() const{
	return mDistance;
}

void ExerciseSerieModel::setDistance(int data){
	if(mDistance != data){
		addBackup(&mDistance, mDistance, data);
		mDistance = data;
		emit distanceChanged();
	}
}

double ExerciseSerieModel::getSpeed() const{
	return mSpeed;
}

void ExerciseSerieModel::setSpeed(double data){
	if(mSpeed!= data){
		addBackup(&mSpeed, mSpeed, data);
		mSpeed= data;
		emit speedChanged();
	}
}

double ExerciseSerieModel::getWeight() const{
	return mWeight;
}

void ExerciseSerieModel::setWeight(double data){
	if(mWeight != data){
		addBackup(&mWeight, mWeight, data);
		mWeight= data;
		emit weightChanged();
	}
}

int ExerciseSerieModel::getRepetitions() const{
	return mRepetitions;
}

void ExerciseSerieModel::setRepetitions(int data){
	if(mRepetitions != data){
		addBackup(&mRepetitions, mRepetitions, data);
		mRepetitions= data;
		emit repetitionsChanged();
	}
}

//-------------------------------------------------------------------------------------------------------------------

void ExerciseSerieModel::updateIsSaved() {
	setIsSaved(getSerieId() > 0);
}

void ExerciseSerieModel::undo(){
	if(mBackupValues.contains(&mOrder)) mOrder = mBackupValues[&mOrder].toInt();
	if(mBackupValues.contains(&mRestTime)) mRestTime = mBackupValues[&mRestTime].toInt();
	if(mBackupValues.contains(&mWorkTime)) mWorkTime = mBackupValues[&mWorkTime].toInt();

	if(mBackupValues.contains(&mDistance)) mDistance = mBackupValues[&mDistance].toInt();
	if(mBackupValues.contains(&mSpeed)) mSpeed = mBackupValues[&mSpeed].toDouble();
	if(mBackupValues.contains(&mWeight)) mWeight = mBackupValues[&mWeight].toDouble();
	if(mBackupValues.contains(&mRepetitions)) mRepetitions = mBackupValues[&mRepetitions].toInt();


	QmlModel::undo();
}

void ExerciseSerieModel::makeNew(){
	setSerieId(0);
}
//-------------------------------------------------------------------------------------------------------------------

bool ExerciseSerieModel::save(){
	DatabaseQuery query;

	if( mSerieId > 0 && !getIsEdited()){
		return false;
	}

	query.begin(mSerieId == 0 ? DatabaseQuery::Insert : DatabaseQuery::Update, mTablePrefix+"_exercise_series");
	query.add("rest_time", mRestTime);
	query.add("work_time", mWorkTime);
	query.add(mTablePrefix+"_exercise_unit_id", mExerciseUnitId);
	query.add("serie_order", mOrder);

	query.add("distance", mDistance);
	query.add("speed", mSpeed);
	query.add("weight", mWeight);
	query.add("repetitions", mRepetitions);

	query.addConditionnal(mTablePrefix+"_serie_id", mSerieId);
	bool result = query.exec();
	if(!result) qCritical() << "Cannot " << (mSerieId == 0 ? "save" : "update") << " series: " << query.mQuery.lastError().text();
	else {
		if(mSerieId == 0){
			auto idField = query.mQuery.record().indexOf(mTablePrefix+"_serie_id");
			query.mQuery.next();
			setSerieId(query.mQuery.value(idField).toInt());
		}
		clearBackupValues();
	}

	return result;
}

void ExerciseSerieModel::load(QSqlQuery &query){
// TODO optimize
	auto serieIdField = query.record().indexOf(mTablePrefix+"_serie_id");
	auto exerciseUnitIdField = query.record().indexOf(mTablePrefix+"_exercise_unit_id");
	auto serieOrderField = query.record().indexOf("serie_order");
	auto restTimeField = query.record().indexOf("rest_time");
	auto workTimeField = query.record().indexOf("work_time");


	auto distanceField = query.record().indexOf("distance");
	auto speedField = query.record().indexOf("speed");
	auto weightField = query.record().indexOf("weight");
	auto repetitionsField = query.record().indexOf("repetitions");


	setSerieId(query.value(serieIdField).toInt());
	setOrder(query.value(serieOrderField).toInt());
	setExerciseUnitId(query.value(exerciseUnitIdField).toInt());
	setWorkTime(query.value(workTimeField).toInt());
	setRestTime(query.value(restTimeField).toInt());

	setDistance(query.value(distanceField).toInt());
	setSpeed(query.value(speedField).toDouble());
	setWeight(query.value(weightField).toDouble());
	setRepetitions(query.value(repetitionsField).toInt());

	clearBackupValues();
}


void ExerciseSerieModel::remove(){
	if(mSerieId > 0){
		DatabaseQuery query;
		query.begin(DatabaseQuery::Delete, mTablePrefix + "_exercise_series");
		query.addConditionnal( mTablePrefix+"_serie_id", mSerieId);
		if(!query.exec()){
			if(!query.exec()) qCritical() << "Cannot delete serie: "  << query.mQuery.lastError().text();
		}
	}
	emit removed(this);
}
