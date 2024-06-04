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

#include "ProgramExerciseModel.h"
#include "../serie/ProgramSerieModel.h"
#include "src/tool/Utils.h"
#include <QVariantList>
#include <QSqlRecord>
#include <QSqlError>
#include <QEvent>
//#include <QDynamicPropertyChange>
#include "src/database/training/TrainingModel.h"

#include <QQmlApplicationEngine>
extern QQmlApplicationEngine * gEngine;


ProgramExerciseModel::ProgramExerciseModel()
	: ProgramExerciseModel{nullptr} {
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}

ProgramExerciseModel::ProgramExerciseModel(QObject *parent)
	: ExerciseUnitModel{parent} {
	mTablePrefix = "program";
}

ProgramExerciseModel::ProgramExerciseModel(const ProgramExerciseModel * model, QObject *parent) : ExerciseUnitModel(model, parent){
	mTablePrefix = "program";
}

ProgramExerciseModel * ProgramExerciseModel::clone(QObject *parent)const{
	ProgramExerciseModel *model = new ProgramExerciseModel(this, parent);

	for(auto i : mSeries)
		model->ExerciseUnitModel::insertSerie(i->clone(model));

	model->clearBackupValues();
	return model;
}

QList<ProgramSerieModel*> ProgramExerciseModel::getSeries() const {
	QList<ProgramSerieModel*> l;
	for(auto i : mSeries)
		l << dynamic_cast<ProgramSerieModel*>(i);
	return l;
}

void ProgramExerciseModel::addSerie(){
	ProgramSerieModel * model;
	if(mSeries.size() > 0)
		model = new ProgramSerieModel(dynamic_cast<ProgramSerieModel*>(mSeries.back()), nullptr);
	else
		model = new ProgramSerieModel(nullptr);
	ExerciseUnitModel::insertSerie(model);
}

void ProgramExerciseModel::updateFromLastProgram() {
	auto model = TrainingModel::buildExercise(mExercise.second, this);
	if(model){
		setUseDistance(model->getUseDistance());
		setUseRepetitions(model->getUseRepetitions());
		setUseSpeed(model->getUseSpeed());
		setUseWeight(model->getUseWeight());
		setDescription(model->getDescription());
		for(auto s : model->getSeries()){
			auto serie = new ProgramSerieModel(s, this);
			ExerciseUnitModel::insertSerie(serie);
		}
		model->deleteLater();
	}
	if(mSeries.size() == 0)
		addSerie();
}

ProgramExerciseModel *ProgramExerciseModel::build(QSqlQuery &query, QObject * parent) {
	ProgramExerciseModel * model = new ProgramExerciseModel(parent);
	model->load(query);
	return model;
}

QList<ProgramExerciseModel*> ProgramExerciseModel::buildAll(QObject * parent) {
	QList<ProgramExerciseModel*> exercises;

	QSqlQuery query( "SELECT * FROM exercises ORDER BY exercise_id ASC");

	QStringList ids;

	while (query.next()) {
		auto model = build(query, parent);
		qint64 id = model->getExerciseId();
		ids << QString::number(id);
		exercises << model;
	}
	if(ids.size() == 0) return exercises;
	if(!query.exec("SELECT * FROM program_exercise_units WHERE program_exercise_unit_id IN(" + ids.join(",") + ") ORDER BY program_exercise_unit_id ASC, serie_order ASC"))
		 qCritical() << "Cannot select exercise series: "  << query.lastError().text();
	auto exerciseIdField = query.record().indexOf("program_exercise_unit_id");
	auto currentModel = exercises.begin();
	while (query.next()) {
		if(query.value(exerciseIdField).toInt() != (*currentModel)->getExerciseId()) ++currentModel;
		auto model = ProgramSerieModel::build(query, *currentModel);
		(*currentModel)->ExerciseUnitModel::insertSerie(model);
	}

	return exercises;
}
