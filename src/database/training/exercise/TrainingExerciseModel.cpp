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

#include "TrainingExerciseModel.h"
#include "../serie/TrainingSerieModel.h"
#include "src/tool/Utils.h"
#include <QVariantList>
#include <QSqlRecord>
#include <QSqlError>
#include <QEvent>
//#include <QDynamicPropertyChange>

#include <QQmlApplicationEngine>
extern QQmlApplicationEngine * gEngine;


TrainingExerciseModel::TrainingExerciseModel()
	: TrainingExerciseModel{nullptr} {
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}

TrainingExerciseModel::TrainingExerciseModel(QObject *parent)
	: ProgramExerciseModel{parent}
{
	mTablePrefix = "training";
}

TrainingExerciseModel::TrainingExerciseModel(const ProgramExerciseModel * model, QObject *parent) : ProgramExerciseModel(model, parent){
	mTablePrefix = "training";

	clearBackupValues();
}


TrainingExerciseModel * TrainingExerciseModel::clone(QObject *parent)const{
	TrainingExerciseModel *model = new TrainingExerciseModel((ProgramExerciseModel*)this, parent);
	for(auto i : mSeries)
		model->insertSerie(i->clone(model));

	model->clearBackupValues();
	return model;
}


DEFINE_SIMPLE_GETSET(TrainingExerciseModel,double,calories,Calories)

void TrainingExerciseModel::addSerie(){
	auto model = new TrainingSerieModel(nullptr);
	insertSerie(model);
}

ExerciseSerieModel* TrainingExerciseModel::insertSerie(ExerciseSerieModel *model){
	connect(dynamic_cast<TrainingSerieModel*>(model), &TrainingSerieModel::requestComputeCalories, this, &TrainingExerciseModel::computeCalories);
	connect(dynamic_cast<TrainingSerieModel*>(model), &TrainingSerieModel::caloriesChanged, this, &TrainingExerciseModel::updateCalories);
	ExerciseUnitModel::insertSerie(model);
	updateCalories();
	return model;
}

QList<TrainingSerieModel*> TrainingExerciseModel::getSeries() const {
	QList<TrainingSerieModel*> l;
	for(auto i : mSeries)
		l << dynamic_cast<TrainingSerieModel*>(i);
	return l;
}

void TrainingExerciseModel::computeCalories(TrainingSerieModel * serie){
	emit requestComputeCalories(this, serie);
}

void TrainingExerciseModel::updateCalories() {
	double calories = 0.0;
	for(auto s : mSeries) {
		calories += dynamic_cast<TrainingSerieModel*>(s)->getCalories();
	}
	setCalories(calories);
}

void TrainingExerciseModel::undo(){
	DEFINE_UNDO(Double,Calories)
	
	ProgramExerciseModel::undo();
}

TrainingExerciseModel *TrainingExerciseModel::build(QSqlQuery &query, QObject * parent) {
	TrainingExerciseModel * model = new TrainingExerciseModel(parent);
	model->ProgramExerciseModel::load(query);
	return model;
}
