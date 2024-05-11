/*
 * Copyright (c) 2024 Wadel Julien.
 *
 * This file is part of Nutrixious
 * (see https://github.com/Ledjlale/Nutrixious).
 *
 * This train is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This train is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this train. If not, see <http://www.gnu.org/licenses/>.
 */

#include "TrainModel.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

#include "src/activity/description/program/ProgramModel.h"
#include "src/activity/training/sport/DistanceModel.h"
#include "src/activity/training/sport/StepsModel.h"
#include "src/activity/training/sport/StrengthModel.h"
#include "src/activity/training/sport/StrengthWorkModel.h"
#include "src/database/DatabaseQuery.h"

#include <QQmlApplicationEngine>
extern QQmlApplicationEngine * gEngine;

using namespace Training;

TrainModel::TrainModel() : TrainModel(nullptr){
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}

TrainModel::TrainModel(QObject *parent)
	: QObject{parent}
{
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	mName = "Train";
	mInvalidName = false;
	connect(this, &TrainModel::nameChanged, [this](){
		setInvalidName( mName == "");
	});
	//connect(this, &TrainModel::descriptionChanged, [this](){
	//	setInvalidDescription( false);
	//});
	connect(this, &TrainModel::exercisesChanged, [this](){
		setInvalidExercises( mExercises.size() == 0);
	});
	connect(this, &TrainModel::trainIdChanged, [this](){
		for(auto &i: mExercises) i->setTrainId(mTrainId);
	});
	connect(this, &TrainModel::targetProgramModelChanged, [this](){
		mExercises.clear();
		if(mTargetProgramModel){
			setName("Train : " + mTargetProgramModel->getName());
			for(auto model : mTargetProgramModel->getExercises()){
				auto newExercise = model->cloneTraining(mTrainId, this);
				connect(newExercise, &ExerciseModel::finished, this, &TrainModel::nextExercise);
				mExercises << newExercise;
			}
		}
		emit exercisesChanged();
	});
}

qint64 TrainModel::getTrainId()const{
	return mTrainId;
}

void TrainModel::setTrainId(qint64 id) {
	if(mTrainId != id){
		mTrainId = id;
		emit trainIdChanged();
	}
}

void TrainModel::setTargetProgramModel(Description::ProgramModel *data) {
	if(mTargetProgramModel != data){
		mTargetProgramModel = data;
		emit targetProgramModelChanged();
	}
}

QString TrainModel::getName() const{
	return mName;
}

void TrainModel::setName(QString name) {
	if(mName != name){
		mName = name;
		emit nameChanged();
	}
}
void TrainModel::setDescription(QString description) {
	if(mDescription != description){
		mDescription = description;
		emit descriptionChanged();
	}
}

QDateTime TrainModel::getStartDateTime() const{
	return mStartDateTime;
}

void TrainModel::setStartDateTime(const time_t& data_ms){
	setStartDateTime(QDateTime::fromMSecsSinceEpoch(data_ms));
}
void TrainModel::setStartDateTime(const QDateTime& data){
	if(mStartDateTime != data){
		mStartDateTime = data;
		emit startDateTimeChanged();
	}
}

QString TrainModel::getStartDateTimeStr()const{
	if(mStartDateTime.isValid())
		return QLocale().toString(mStartDateTime, QLocale().dateTimeFormat(QLocale::FormatType::ShortFormat));
	else
		return "";
}

QVariantList TrainModel::getVariantExercises() const {
	QVariantList exercises;
	for(auto exercise: mExercises){
		exercises << QVariant::fromValue(exercise);
	 }
	return exercises;
}

const QList<ExerciseModel*>& TrainModel::getExercises()const {
	return mExercises;
}
QList<ExerciseModel*>& TrainModel::getExercises(){
	return mExercises;
}

void TrainModel::addExercise(ExerciseModel *model, bool keepId) {
	int trainOrder = model->getTrainOrder();
	ExerciseModel * insertedModel = nullptr;
	if(trainOrder < 0) {
		mExercises.push_back(model->clone(mTrainId, this));
		insertedModel = mExercises.back();
		insertedModel->setTrainOrder(mExercises.size());
	}else{
		if( mExercises.size() == 0){
			mExercises.push_back(model->clone(mTrainId, this));
			insertedModel = mExercises.back();
		}else {
			auto it = mExercises.begin();
			while(it != mExercises.end() && (*it)->getTrainOrder() <= trainOrder)
				++it;
			insertedModel = *mExercises.insert(it, model->clone(mTrainId, this));
		}
	}
	if(keepId)
		insertedModel->setExerciseId(model->getExerciseId());
	emit exercisesChanged();
}

void TrainModel::removeExercise(ExerciseModel *model) {
	mExercises.removeOne(model);
	model->deleteLater();
	emit exercisesChanged();
}

void TrainModel::clearExercises(){
	for(auto item : mExercises)
		item->deleteLater();
	mExercises.clear();
	emit exercisesChanged();
}

//-------------------------------------------------------------------------------------------------------------------

void TrainModel::setInvalidName(bool invalid) {
	if(mInvalidName != invalid){
		mInvalidName = invalid;
		emit invalidNameChanged();
	}
}

void TrainModel::setInvalidDescription(bool invalid){
	if(mInvalidDescription != invalid){
		mInvalidDescription = invalid;
		emit invalidDescriptionChanged();
	}
}

void TrainModel::setInvalidExercises(bool invalid) {
	if(mInvalidExercises != invalid){
		mInvalidExercises = invalid;
		emit invalidExercisesChanged();
	}
}
//-------------------------------------------------------------------------------------------------------------------



bool TrainModel::save(){
	if( mInvalidName || mInvalidDescription || mInvalidExercises) return false;
	qDebug() << "Saving Training Train " << mName << mDescription;
	DatabaseQuery query;

	query.begin(mTrainId == 0 ? DatabaseQuery::Insert : DatabaseQuery::Update, "trains" );

	query.add("name", mName);
	query.add("description", mDescription);
	query.add("start_date_time", mStartDateTime.toMSecsSinceEpoch());
	query.addConditionnal("id",mTrainId);
	if(mTrainId == 0){
		if(!query.exec()) qCritical() << "Cannot save train: "  << query.mQuery.lastError().text();
		auto fieldNo = query.mQuery.record().indexOf("id");
		while (query.mQuery.next()) {
			setTrainId(query.mQuery.value(fieldNo).toInt());
			qDebug() << "Insert train: " << mTrainId;
		}
		for(auto exercise : mExercises){
			exercise->save();
		}
	}else{
		if(!query.exec()) qCritical() << "Cannot update train: "  << query.mQuery.lastError().text();
		else {
			qDebug() << "Update train: " << mTrainId;
			for(auto exercise : mExercises){
			exercise->save();
		}
		}
	}
	return false;
}

QList<TrainModel*> TrainModel::load(QObject * parent){
QList<TrainModel*> models;
	QSqlQuery query( "SELECT * FROM trains ORDER BY id ASC");
	QStringList ids;

	while (query.next()) {
		auto model = load(query, parent);
		models << model;
		ids << QString::number(model->getTrainId());
	}
	if(models.size() == 0) return models;

//----------------------		Strength
	if(!query.exec("SELECT * FROM tr_ex_strength WHERE train_id IN(" + ids.join(",") + ") ORDER BY train_id ASC, id ASC"))
		qCritical() << "Cannot request train strength :" << query.lastError().text();
	auto trainIdField = query.record().indexOf("train_id");
	auto currentModel = models.begin();
	while (query.next()) {
		while(query.value(trainIdField).toInt() != (*currentModel)->getTrainId()) ++currentModel;
		auto model = StrengthModel::load(query, *currentModel);
		(*currentModel)->addExercise(model, true);
	}

	if(!query.exec("SELECT * FROM tr_ex_strength_set WHERE train_id IN(" + ids.join(",") + ") ORDER BY train_id ASC, strength_id ASC"))
		qCritical() << "Cannot request train strength set :" << query.lastError().text();
	trainIdField = query.record().indexOf("train_id");
	auto strengthId = query.record().indexOf("strength_id");
	currentModel = models.begin();
	auto currentExercise = (*currentModel)->getExercises().begin();
	while (query.next()) {
		while(query.value(trainIdField).toInt() != (*currentModel)->getTrainId()){
			++currentModel;
			currentExercise = (*currentModel)->getExercises().begin();
		}
		while(query.value(strengthId).toInt() != (*currentExercise)->getExerciseId()) {
			++currentExercise;
		}
		auto model = StrengthWorkModel::load(query, *currentExercise);
		dynamic_cast<StrengthModel*>(*currentExercise)->addSet(model, true);
	}

//----------------------		Distance
	if(!query.exec("SELECT * FROM tr_ex_distance WHERE train_id IN(" + ids.join(",") + ") ORDER BY train_id ASC, id ASC"))
		qCritical() << "Cannot request train distance :" << query.lastError().text();
	trainIdField = query.record().indexOf("train_id");
	currentModel = models.begin();
	while (query.next()) {
		while(query.value(trainIdField).toInt() != (*currentModel)->getTrainId()) ++currentModel;
		auto model = DistanceModel::load(query, *currentModel);
		(*currentModel)->addExercise(model, true);
	}

//----------------------		Steps
	if(!query.exec("SELECT * FROM tr_ex_steps WHERE train_id IN(" + ids.join(",") + ") ORDER BY train_id ASC, id ASC"))
		qCritical() << "Cannot request train steps :" << query.lastError().text();
	trainIdField = query.record().indexOf("train_id");
	currentModel = models.begin();
	while (query.next()) {
		while(query.value(trainIdField).toInt() != (*currentModel)->getTrainId()) ++currentModel;
		auto model = StepsModel::load(query, *currentModel);
		(*currentModel)->addExercise(model, true);
	}

//--------------------------------

	return models;

}

TrainModel *TrainModel::load(QSqlQuery &query, QObject * parent) {
	TrainModel * model = new TrainModel(parent);
// TODO optimize
	auto idField = query.record().indexOf("id");
	auto nameField = query.record().indexOf("name");
	auto descriptionField = query.record().indexOf("description");
	auto startDateTimeField = query.record().indexOf("start_date_time");
	model->setTrainId(query.value(idField).toInt());
	model->setName(query.value(nameField).toString());
	model->setDescription(query.value(descriptionField).toString());
	model->setStartDateTime(query.value(startDateTimeField).toULongLong());
	return model;
}

QList<ExerciseModel*>::Iterator TrainModel::start(){
	setStartDateTime(QDateTime::currentDateTime());
	auto f = mExercises.begin();
	(*f)->startWork();
	return f;
}


