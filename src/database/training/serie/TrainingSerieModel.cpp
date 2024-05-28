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

#include "TrainingSerieModel.h"
#include <QSqlQuery>
#include <QSqlRecord>

#include "src/database/DatabaseQuery.h"
#include "src/database/program/serie/ProgramSerieModel.h"

#include <QQmlApplicationEngine>
extern QQmlApplicationEngine * gEngine;

TrainingSerieModel::TrainingSerieModel() : TrainingSerieModel(nullptr){
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}

TrainingSerieModel::TrainingSerieModel(QObject *parent)
	: ProgramSerieModel{parent}
{
	mTablePrefix = "training";
}

TrainingSerieModel::TrainingSerieModel(const ProgramSerieModel * model, QObject *parent) : ProgramSerieModel(model, parent) {
	mTablePrefix = "training";
	clearBackupValues();
}

TrainingSerieModel::TrainingSerieModel(const TrainingSerieModel * model, QObject *parent) : ProgramSerieModel(model, parent) {
	mTablePrefix = "training";
	mCalories = model->mCalories;
	clearBackupValues();
}

TrainingSerieModel * TrainingSerieModel::clone(QObject *parent)const{
	TrainingSerieModel *model = new TrainingSerieModel(this, parent);
	model->setCalories(model->getCalories());
	model->clearBackupValues();
	return model;
}

//-------------------------------------------------------------------------------------------------------------------

void TrainingSerieModel::addQueryValues(DatabaseQuery &query){
	query.add("calories", mCalories);
}

void TrainingSerieModel::load(QSqlQuery &query) {
// TODO optimize
	ProgramSerieModel::load(query);
	auto caloriesField = query.record().indexOf("calories");
	setCalories(query.value(caloriesField).toDouble());
	clearBackupValues();
}
TrainingSerieModel *TrainingSerieModel::build(QSqlQuery &query, QObject * parent) {
	TrainingSerieModel * model = new TrainingSerieModel(parent);
	model->load(query);
	return model;
}

double TrainingSerieModel::getCalories() const{
	return mCalories;
}
void TrainingSerieModel::setCalories(double data){
	if(mCalories != data){
		addBackup(&mCalories, mCalories, data);
		mCalories = data;
		emit caloriesChanged();
	}
}

void TrainingSerieModel::computeCalories(){
	emit requestComputeCalories(this);
}
