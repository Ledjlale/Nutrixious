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
	connect(this, &ExerciseSerieModel::distanceChanged, this, &ExerciseSerieModel::isSpeedComputableChanged);
	connect(this, &ExerciseSerieModel::workTimeChanged, this, &ExerciseSerieModel::isSpeedComputableChanged);
}

ExerciseSerieModel::ExerciseSerieModel(QObject *parent)
	: QmlModel{parent}
{
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	connect(this, &ExerciseSerieModel::distanceChanged, this, &ExerciseSerieModel::isSpeedComputableChanged);
	connect(this, &ExerciseSerieModel::workTimeChanged, this, &ExerciseSerieModel::isSpeedComputableChanged);
}

ExerciseSerieModel::ExerciseSerieModel(const ExerciseSerieModel * model, QObject *parent) : ExerciseSerieModel(parent){
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	mSerieId = initiBackup(model, &model->mSerieId, model->mSerieId, &mSerieId).toLongLong();
	mExerciseUnitId = initiBackup(model, &model->mExerciseUnitId, model->mExerciseUnitId, &mExerciseUnitId).toLongLong();

	mOrder = initiBackup(model, &model->mOrder, model->mOrder, &mOrder).toInt();
	mRestTime = initiBackup(model, &model->mRestTime, model->mRestTime, &mRestTime).toInt();
	mWorkTime = initiBackup(model, &model->mWorkTime, model->mWorkTime, &mWorkTime).toInt();
	mDistance = initiBackup(model, &model->mDistance, model->mDistance, &mDistance).toInt();
	mRepetitions = initiBackup(model, &model->mRepetitions, model->mRepetitions, &mRepetitions).toInt();

	mWeight = initiBackup(model, &model->mWeight, model->mWeight, &mWeight).toDouble();
	mSpeed = initiBackup(model, &model->mSpeed, model->mSpeed, &mSpeed).toDouble();
	updateIsSaved();
}

ExerciseSerieModel::~ExerciseSerieModel(){
}

ExerciseSerieModel * ExerciseSerieModel::clone(QObject *parent)const{
	ExerciseSerieModel *model = new ExerciseSerieModel(this, parent);
	model->clearBackupValues();
	return model;
}

void ExerciseSerieModel::initRandomValues() {
	mRestTime = std::rand() * 60.0 / RAND_MAX + 1.0;
	mWorkTime = std::rand() * 120.0 / RAND_MAX + 1.0;
	mDistance = std::rand() * 20000.0 / RAND_MAX + 1000.0;
	mRepetitions = std::rand() * 10.0 / RAND_MAX + 5.0;
	mWeight = std::rand() * 100.0 / RAND_MAX + 5.0;
	mSpeed = std::rand() * 10.0 / RAND_MAX + 10.0;
}

qint64 ExerciseSerieModel::getSerieId()const{
	return mSerieId;
}

void ExerciseSerieModel::setSerieId(qint64 id) {
	if(mSerieId != id){
		mSerieId = id;
		updateIsSaved();
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

QString ExerciseSerieModel::getRestTimeStr() const {
	return QLocale().toString(QTime::fromMSecsSinceStartOfDay(mRestTime*1000), "hh:mm:ss");
}
void ExerciseSerieModel::setRestTimeStr(QString data){
	setRestTime(QTime::fromString(data, "hh:mm:ss").msecsSinceStartOfDay() / 1000);
}

QString ExerciseSerieModel::getWorkTimeStr() const{
	return QLocale().toString(QTime::fromMSecsSinceStartOfDay(mWorkTime*1000), "hh:mm:ss");
}
void ExerciseSerieModel::setWorkTimeStr(QString data){
	setWorkTime(QTime::fromString(data, "hh:mm:ss").msecsSinceStartOfDay() / 1000);
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

bool ExerciseSerieModel::isSpeedComputable()const{
	return mWorkTime>0 && mDistance > 0;
}

//-------------------------------------------------------------------------------------------------------------------

void ExerciseSerieModel::updateIsSaved() {
	setIsSaved(getSerieId() > 0);
}

void ExerciseSerieModel::undo(){
	DEFINE_UNDO(Int,Order)
	DEFINE_UNDO(Int,RestTime)
	DEFINE_UNDO(Int,WorkTime)
	DEFINE_UNDO(Int,Distance)
	DEFINE_UNDO(Int,Repetitions)
	
	DEFINE_UNDO(Double,Speed)
	DEFINE_UNDO(Double,Weight)

	QmlModel::undo();
}

void ExerciseSerieModel::makeNew(){
	setSerieId(0);
}
//-------------------------------------------------------------------------------------------------------------------

int ExerciseSerieModel::save(){
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
	addQueryValues(query);
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

void ExerciseSerieModel::computeSpeed(){
	double hours = getWorkTime() / 3600.0;
	double km = getDistance() / 1000.0;
	if(hours > 0 )
		setSpeed(km / hours);
}
