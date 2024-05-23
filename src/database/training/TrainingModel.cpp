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

#include "TrainingModel.h"

#include <QQmlApplicationEngine>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include "src/database/DatabaseQuery.h"

#include "src/tool/Utils.h"


extern QQmlApplicationEngine * gEngine;

TrainingModel::TrainingModel() : TrainingModel(nullptr){	// QML
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}
TrainingModel::TrainingModel(QObject *parent)
	: QmlModel{parent}
{
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	mName = "Training";
	connect(this, &TrainingModel::trainingIdChanged, [this](){
		for(auto &i: mExercises) i->setTrainingId(mTrainingId);
	});
}
TrainingModel::TrainingModel(ProgramModel * program, QObject *parent) : TrainingModel(parent) {
	mName = "Training";
	mDescription = program->getName();
	for(auto i : program->getExercises()){
		mExercises << new TrainingExerciseModel(i, this);
	}
}
TrainingModel::TrainingModel(TrainingModel * training, QObject *parent) : TrainingModel(parent) {
	mName = training->getName();
	mDescription = training->getDescription();
	for(auto i : training->getExercises()){
		mExercises << new TrainingExerciseModel(i, this);
	}
}
qint64 TrainingModel::getTrainingId()const{
	return mTrainingId;
}

void TrainingModel::setTrainingId(qint64 id) {
	if(mTrainingId != id) {
		mTrainingId = id;
		emit trainingIdChanged();
	}

}

QString TrainingModel::getName() const{
	return mName;
}

void TrainingModel::setName(QString name) {
	if(mName != name){
		mName = name;
		emit nameChanged();
	}
}

QString TrainingModel::getDescription() const{
	return mDescription;
}

void TrainingModel::setDescription(QString description) {
	if(mDescription != description){
		mDescription = description;
		emit descriptionChanged();
	}
}

QDateTime TrainingModel::getStartDateTime() const{
	return mStartDateTime;
}

void TrainingModel::setStartDateTime(const time_t& data_ms){
	setStartDateTime(QDateTime::fromMSecsSinceEpoch(data_ms));
}
void TrainingModel::setStartDateTime(const QDateTime& data){
	if(mStartDateTime != data){
		mStartDateTime = data;
		emit startDateTimeChanged();
	}
}

QString TrainingModel::getStartDateTimeStr()const{
	if(mStartDateTime.isValid())
		return QLocale().toString(mStartDateTime, QLocale().dateTimeFormat(QLocale::FormatType::ShortFormat));
	else
		return "";
}

QVariantList TrainingModel::getVariantExercises() const {
	QVariantList exercises;
	for(auto exercise: mExercises){
		exercises << QVariant::fromValue(exercise);
	 }
	return exercises;
}

const QList<TrainingExerciseModel*>& TrainingModel::getExercises()const {
	return mExercises;
}

TrainingExerciseModel* TrainingModel::addExercise(TrainingExerciseModel *model, bool keepId) {
	auto insertedModel = Utils::add<TrainingExerciseModel>(model, this, mExercises);
	insertedModel->setTrainingId(mTrainingId);
	if(keepId)
		insertedModel->setTrainingExerciseId(model->getTrainingExerciseId());
	emit exercisesChanged();
	return insertedModel;
}

TrainingExerciseModel* TrainingModel::buildExercise(ExerciseModel *model) {
	QSqlQuery query;

	TrainingExerciseModel* exerciseModel = nullptr;
	// Build from last training
	if(!query.exec("SELECT * FROM trainings, tr_exercises WHERE trainings.training_id=tr_exercises.training_id AND exercise_id=" +QString::number(model->getExerciseId()) + " ORDER BY start_date_time DESC LIMIT 1" ))
		qCritical() << "Cannot request training exercises to build exercise : " << query.lastError().text();
	else{
		if(query.next()){
			exerciseModel = TrainingExerciseModel::load(query,this);
			exerciseModel->setExerciseModel(model);
			if(!query.exec("SELECT * FROM tr_exercise_series WHERE tr_exercise_id = "+QString::number(exerciseModel->getTrainingExerciseId())))
				qCritical() << "Cannot request training series to build series : "  << query.lastError().text();
			else{
				while (query.next()) {
					exerciseModel->addSerie(TrainingSerieModel::load(query, exerciseModel), false);
				}
			}
		}
	}
	//Build from Program
	if( !exerciseModel){
		if(!query.exec("SELECT * FROM prgm_exercises WHERE exercise_id=" +QString::number(model->getExerciseId()) + " ORDER BY prgm_exercise_id DESC LIMIT 1" ))
			qCritical() << "Cannot request program exercises to build exercise : " << query.lastError().text();
		else{
			if(query.next()){
				auto programModel = ProgramExerciseModel::load(query,nullptr);
				programModel->setExerciseModel(model);
				if(!query.exec("SELECT * FROM prgm_exercise_series WHERE prgm_exercise_id = "+QString::number(programModel->getProgramExerciseId())))
					qCritical() << "Cannot request training series to build series : "  << query.lastError().text();
				else{
					while (query.next()) {
						programModel->addSerie(ProgramSerieModel::load(query, programModel), false);
					}
				}
				exerciseModel = new TrainingExerciseModel(programModel, this);
				programModel->deleteLater();
			}
		}
	}


	return exerciseModel;
}

void TrainingModel::removeExercise(TrainingExerciseModel *model) {
	mExercises.removeOne(model);
	model->deleteLater();
	emit exercisesChanged();
}

void TrainingModel::clearExercises(){
	for(auto item : mExercises)
		item->deleteLater();
	mExercises.clear();
	emit exercisesChanged();
}

void TrainingModel::updateTrainingOrder(){
	for(size_t i = 0 ; i < mExercises.size() ; ++i)
		mExercises[i]->setOrder(i);
}


void TrainingModel::decrementExerciseOrder(TrainingExerciseModel *model) {
	if(Utils::decrementOrder<TrainingExerciseModel>(model, mExercises)){
		emit exercisesChanged();
		save();
	}
}
void TrainingModel::incrementExerciseOrder(TrainingExerciseModel *model){
	if(Utils::incrementOrder<TrainingExerciseModel>(model, mExercises)){
		emit exercisesChanged();
		save();
	}
}


//-------------------------------------------------------------------------------------------------------------------


bool TrainingModel::save(){
	qDebug() << "Saving Training " << mName << mDescription;
	DatabaseQuery query;

	query.begin(mTrainingId == 0 ? DatabaseQuery::Insert : DatabaseQuery::Update, "trainings" );

	query.add("name", mName);
	query.add("description", mDescription);
	query.add("start_date_time", mStartDateTime.toMSecsSinceEpoch());
	query.addConditionnal("training_id",mTrainingId);
	if(mTrainingId == 0){
		if(!query.exec()) qCritical() << "Cannot save training: "  << query.mQuery.lastError().text();
		auto fieldNo = query.mQuery.record().indexOf("training_id");
		while (query.mQuery.next()) {
			setTrainingId(query.mQuery.value(fieldNo).toInt());
			qDebug() << "Insert training: " << mTrainingId;
		}
		for(auto exercise : mExercises){
			exercise->save();
		}
	}else{
		if(!query.exec()) qCritical() << "Cannot update training: "  << query.mQuery.lastError().text();
		else {
			qDebug() << "Update training: " << mTrainingId;
			for(auto exercise : mExercises){
			exercise->save();
		}
		}
	}

	return true;
}

void TrainingModel::remove(){
	if(mTrainingId > 0){
		DatabaseQuery query;
		query.begin(DatabaseQuery::Delete, "trainings");
		query.addConditionnal("training_id",mTrainingId);
		if(!query.exec()){
			if(!query.exec()) qCritical() << "Cannot delete training  : "  << query.mQuery.lastError().text();
		}
	}
	emit removed(this);
}

TrainingModel *TrainingModel::load(QSqlQuery &query, QObject * parent) {
	TrainingModel * model = new TrainingModel(parent);
// TODO optimize
	auto idField = query.record().indexOf("training_id");
	auto nameField = query.record().indexOf("name");
	auto descriptionField = query.record().indexOf("description");
	auto startDateTimeField = query.record().indexOf("start_date_time");
	model->setTrainingId(query.value(idField).toInt());
	model->setName(query.value(nameField).toString());
	model->setDescription(query.value(descriptionField).toString());
	model->setStartDateTime(query.value(startDateTimeField).toULongLong());
	return model;
}

QList<TrainingModel*> TrainingModel::load(QObject * parent){
	QList<TrainingModel*> models;

	QMap<qint64, ExerciseModel*> exercises;
	QSqlQuery query;
	if(!query.exec("SELECT * FROM exercises"))
		 qCritical() << "Cannot select exercises: "  << query.lastError().text();
	while (query.next()) {
		auto model = ExerciseModel::load(query, nullptr);
		exercises[model->getExerciseId()] = model;
	}


	QMap<qint64, QList<TrainingSerieModel*>> series;
	if(!query.exec("SELECT * FROM tr_exercise_series"))
		 qCritical() << "Cannot select training series: "  << query.lastError().text();
	while (query.next()) {
		auto model = TrainingSerieModel::load(query, nullptr);
		series[model->getTrainingExerciseId()] << model;
	}


	QMap<qint64, QList<TrainingExerciseModel*>> exerciseModels;
	if(!query.exec("SELECT * FROM tr_exercises"))
		 qCritical() << "Cannot select series: "  << query.lastError().text();
	while (query.next()) {
		auto model = TrainingExerciseModel::load(query, nullptr);
		model->setExerciseModel(exercises[model->getExerciseId()]);
		if( series.contains(model->getTrainingExerciseId())){
			for(auto it : series[model->getTrainingExerciseId()])
				model->addSerie(it, true);
		}
		exerciseModels[model->getTrainingId()] << model;
	}

	query.exec( "SELECT * FROM trainings ORDER BY training_id ASC");
	QStringList ids, trainingExerciseIds;

	while (query.next()) {
		auto model = load(query, parent);
		if(exerciseModels.contains(model->getTrainingId())){
			for(auto it : exerciseModels[model->getTrainingId()])
				model->addExercise(it, true);
		}
		models << model;
	}
	for(auto i : exerciseModels)
		for(auto j : i)
			j->deleteLater();
	for(auto i : series)
		for(auto j : i)
			j->deleteLater();
	for(auto i : exercises) i->deleteLater();

//--------------------------------
	return models;
}
