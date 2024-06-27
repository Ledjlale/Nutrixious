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

#include "serie/TrainingSerieModel.h"

extern QQmlApplicationEngine * gEngine;

TrainingModel::TrainingModel() : TrainingModel(nullptr){	// QML
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}
TrainingModel::TrainingModel(QObject *parent)
	: ProgramModel{parent}
{
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	mName = "Training";
	mTablePrefix = "training";
}
TrainingModel::TrainingModel(ProgramModel * model, QObject *parent) : ProgramModel(parent) {
	mTablePrefix = "training";
	mName = "Training";
	mDescription = model->getName();
	for(auto i : model->getExercises()){
		auto exercise = new TrainingExerciseModel(i, this);
		for(auto j : i->getSeries()) {
			auto s = new TrainingSerieModel(j, exercise);
			connect(s, &TrainingSerieModel::caloriesChanged, this, &TrainingModel::updateCalories);
			exercise->insertSerie(s);
		}
		connect(exercise, &TrainingExerciseModel::requestComputeCalories, this, &TrainingModel::computeCalorie);
		exercise->makeNew();
		mExercises << exercise;
	}
}
TrainingModel::TrainingModel(TrainingModel *model, QObject *parent) : TrainingModel((ProgramModel*)model, parent) {
	mStartDateTime = model->getStartDateTime();
	mDescription = model->getDescription();
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
		//return QLocale().toString(mStartDateTime, QLocale().dateTimeFormat(QLocale::FormatType::ShortFormat));
		return QLocale().toString(mStartDateTime, "yyyy/MM/dd hh:mm:ss");
	else
		return "";
}

void TrainingModel::setStartDateTimeStr(QString data) {
	QDateTime dateTime = QDateTime::fromString(data, "yyyy/MM/dd hh:mm:ss");
	if( dateTime.isValid() && dateTime != mStartDateTime){
		addBackup(&mStartDateTime, mStartDateTime, dateTime);
		mStartDateTime = dateTime;
		startDateTimeChanged();
	}
}

QString TrainingModel::getStartTimeStr()const{
	if(mStartDateTime.isValid())
		//return QLocale().toString(mStartDateTime, QLocale().dateTimeFormat(QLocale::FormatType::ShortFormat));
		return QLocale().toString(mStartDateTime, "hh:mm:ss");
	else
		return "";
}

void TrainingModel::setStartTimeStr(QString data) {
	QDateTime dateTime = QDateTime(mStartDateTime.date(), QTime::fromString(data, "hh:mm:ss"));
	if( dateTime.isValid() && dateTime != mStartDateTime){
		addBackup(&mStartDateTime, mStartDateTime, dateTime);
		mStartDateTime = dateTime;
		startDateTimeChanged();
	}
}



DEFINE_SIMPLE_GETSET(TrainingModel,double,calories,Calories)

ProgramExerciseModel* TrainingModel::insertNewExercise(ProgramExerciseModel *model){
	auto insertedModel = dynamic_cast<TrainingExerciseModel*>(ProgramModel::insertNewExercise(model));
	connect(insertedModel, &TrainingExerciseModel::requestComputeCalories, this, &TrainingModel::computeCalorie);
	return insertedModel;
}

ProgramExerciseModel* TrainingModel::insertExercise(ProgramExerciseModel *model){
	auto insertedModel = dynamic_cast<TrainingExerciseModel*>(ProgramModel::insertExercise(model));
	connect(insertedModel, &TrainingExerciseModel::requestComputeCalories, this, &TrainingModel::computeCalorie);
	connect(insertedModel, &TrainingExerciseModel::caloriesChanged, this, &TrainingModel::updateCalories);
	updateCalories();
	return insertedModel;
}




TrainingExerciseModel* TrainingModel::buildExercise(ExerciseModel *model) {
	return buildExercise(model, this);
}
TrainingExerciseModel* TrainingModel::buildExercise(ExerciseModel *model, QObject *parent){

	if(model->getExerciseId() == 0) return nullptr;
	QSqlQuery query;
	QString trainingTablePrefix = "training";
	QString programTablePrefix = "program";
	TrainingExerciseModel* exerciseModel = nullptr;
	// Build from last training
	if(!query.exec("SELECT * FROM "+trainingTablePrefix+"s, "+trainingTablePrefix+"_exercise_units WHERE "+trainingTablePrefix+"s."+trainingTablePrefix+"_id="+trainingTablePrefix+"_exercise_units."+trainingTablePrefix+"_id AND exercise_id=" +QString::number(model->getExerciseId()) + " ORDER BY start_date_time DESC LIMIT 1" ))
		qCritical() << "Cannot request training exercises to build exercise : " << query.lastError().text();
	else{
		if(query.next()){
			exerciseModel = new TrainingExerciseModel(parent);
			exerciseModel->ProgramExerciseModel::load(query);
			exerciseModel->setExerciseModel(model);
			if(!query.exec("SELECT * FROM "+trainingTablePrefix+"_exercise_series WHERE "+trainingTablePrefix+"_exercise_unit_id = "+QString::number(exerciseModel->getExerciseUnitId()))) {
				exerciseModel->deleteLater();
				exerciseModel = nullptr;
				qCritical() << "Cannot request training series to build series : "  << query.lastError().text();
			}else{
				while (query.next()) {
					auto serie = new TrainingSerieModel(exerciseModel);
					serie->load(query);
					exerciseModel->insertSerie(serie);
				}
			}
		}
	}
	//Build from Program
	if( !exerciseModel){
		if(!query.exec("SELECT * FROM "+programTablePrefix+"_exercise_units WHERE exercise_id=" +QString::number(model->getExerciseId()) + " ORDER BY "+programTablePrefix+"_exercise_unit_id DESC LIMIT 1" ))
			qCritical() << "Cannot request "+programTablePrefix+" exercises to build exercise : " << query.lastError().text();
		else{
			if(query.next()){
				auto programModel = ProgramExerciseModel::build(query,nullptr);
				programModel->setExerciseModel(model);
				if(!query.exec("SELECT * FROM "+programTablePrefix+"_exercise_series WHERE "+programTablePrefix+"_exercise_unit_id = "+QString::number(programModel->getExerciseUnitId())))
					qCritical() << "Cannot request "+programTablePrefix+" series to build series : "  << query.lastError().text();
				else{
					while (query.next()) {
						programModel->ExerciseUnitModel::insertSerie(ProgramSerieModel::build(query, programModel));
					}
				}
				exerciseModel = new TrainingExerciseModel(programModel, parent);
				for(auto i : programModel->getSeries())
					exerciseModel->insertSerie(new TrainingSerieModel(i, exerciseModel));
				programModel->deleteLater();
			}
		}
	}


	return exerciseModel;
}

//-------------------------------------------------------------------------------------------------------------------

void TrainingModel::addQueryValues(DatabaseQuery &query){
	query.add("start_date_time", mStartDateTime.toMSecsSinceEpoch());
}


void TrainingModel::load(QSqlQuery &query) {
// TODO optimize
	ProgramModel::load(query);
	auto startDateTimeField = query.record().indexOf("start_date_time");
	setStartDateTime(query.value(startDateTimeField).toULongLong());
	clearBackupValues();
}

void TrainingModel::undo(){
	DEFINE_UNDO(Double,Calories)
	
	ProgramModel::undo();
}

TrainingModel *TrainingModel::build(QSqlQuery &query, QObject * parent) {
	TrainingModel * model = new TrainingModel(parent);
// TODO optimize
	model->load(query);
	return model;
}


QList<TrainingModel*> TrainingModel::buildAll(QObject * parent){
	QList<ExerciseModel *> exercises;
	QSqlQuery query;
	if(!query.exec("SELECT * FROM exercises"))
		 qCritical() << "Cannot select exercises: "  << query.lastError().text();
	while (query.next()) {
		exercises << ExerciseModel::build(query, nullptr);
	}
	auto result = buildAll(exercises, parent);
	for(auto i : exercises) i->deleteLater();
	return result;
}

QList<TrainingModel*> TrainingModel::buildAll(QList<ExerciseModel *> exercises, QObject * parent){
	QString tablePrefix = "training";
	QList<TrainingModel*> models;
	QMap<qint64, QList<TrainingSerieModel*>> series;
	QMap<qint64, ExerciseModel*> exercisesMap;
	QStringList ids;
	for(auto i : exercises) {
		exercisesMap[i->getExerciseId()] = i;
		ids << QString::number(i->getExerciseId());
	}
	QSqlQuery query;
	if(!query.exec("SELECT * FROM "+tablePrefix+"_exercise_series"))
		 qCritical() << "Cannot select series: "  << query.lastError().text();
	while (query.next()) {
		auto model = TrainingSerieModel::build(query, nullptr);
		series[model->getExerciseUnitId()] << model;
	}


	QMap<qint64, QList<TrainingExerciseModel*>> exerciseModels;
	if(!query.exec("SELECT * FROM "+tablePrefix+"_exercise_units WHERE exercise_id IN("+ids.join(',')+")"))
		 qCritical() << "Cannot select exercises: "  << query.lastError().text();
	while (query.next()) {
		auto model = TrainingExerciseModel::build(query, nullptr);
		if(exercisesMap.contains(model->getExerciseId()))
			model->setExerciseModel(exercisesMap[model->getExerciseId()]->clone(model));
		else
			model->setExerciseModel(nullptr);
		if( series.contains(model->getExerciseUnitId())){
			for(auto it : series[model->getExerciseUnitId()])
				model->insertSerie(it->clone(model));
		}
		exerciseModels[model->getParentId()] << model;
	}

	query.exec( "SELECT * FROM "+tablePrefix+"s ORDER BY "+tablePrefix+"_id ASC");

	while (query.next()) {
		auto model = build(query, parent);
		if(exerciseModels.contains(model->getId())){
			for(auto it : exerciseModels[model->getId()]){
				model->insertExercise(it->clone(model));
			}
		}
		models << model;
	}

	for(auto i : exerciseModels)
		for(auto j : i)
			j->deleteLater();
	for(auto i : series)
		for(auto j : i)
			j->deleteLater();
//--------------------------------
	return models;
}



void TrainingModel::computeCalorie(TrainingExerciseModel * exercise, TrainingSerieModel * serie) {
	QSqlQuery query;

	if(serie){
		double met = exercise->getExerciseModel()->getMet();
		int metMode = exercise->getExerciseModel()->getMetMode();
		query.prepare("SELECT weight FROM personal_data ORDER BY ABS(? - date_time) LIMIT 1");
		query.addBindValue(mStartDateTime.toMSecsSinceEpoch());
		if(query.exec()){
			if(query.next()){
				auto weightField = query.record().indexOf("weight");
				double weight = query.value(weightField).toDouble();
				double speed = serie->getSpeed();
				if(metMode == 1 && speed > 0){
					met = speed;
				}
				double factor = met * 3.5 * weight / 200.0;
				int workTime = serie->getWorkTime();
				double calories = factor * workTime / 60.0;
				serie->setCalories(calories);
			}else
			qCritical() << "Cannot compute calorie because of empty personal data ";
		}else{
			qCritical() << "Cannot compute calorie because of personal data : "  << query.lastError().text();
		}
	}
}

void TrainingModel::updateCalories() {
	double calories = 0.0;
	for(auto e : mExercises){
		calories += dynamic_cast<TrainingExerciseModel*>(e)->getCalories();
	}
	setCalories(calories);
}
