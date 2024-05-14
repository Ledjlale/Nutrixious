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

#include "ExerciseModel.h"
#include <QVariantList>
#include <QSqlRecord>
#include <QSqlError>
#include <QEvent>
//#include <QDynamicPropertyChange>

#include <QQmlApplicationEngine>
extern QQmlApplicationEngine * gEngine;

using namespace Program;

ExerciseModel::ExerciseModel()
	: Base::ExerciseModel{nullptr} {
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
	mDbTable = "pgrm_exercises";
}

ExerciseModel::ExerciseModel(QObject *parent) : Base::ExerciseModel(parent){
	mDbTable = "pgrm_exercises";
}

ExerciseModel::ExerciseModel(int type, QObject *parent) : Base::ExerciseModel(type, parent){
	mDbTable = "pgrm_exercises";
}

ExerciseModel::ExerciseModel(const ExerciseModel * model, QObject *parent) : Base::ExerciseModel(model, parent){
	mDbTable = "pgrm_exercises";
}

ExerciseModel * ExerciseModel::clone(QObject *parent)const{
	ExerciseModel * model = new ExerciseModel(parent);
	return model;
}

qint64 ExerciseModel::getProgramId()const{
	return mProgramId;
}

void ExerciseModel::setProgramId(qint64 id) {
	if(mProgramId != id){
		mProgramId = id;
		emit programIdChanged();
	}
}

void ExerciseModel::saveValues(DatabaseQuery &query) {
	query.add("program_id", mProgramId);
}

ExerciseModel *ExerciseModel::load(QSqlQuery &query, QObject * parent) {
	ExerciseModel * model = new ExerciseModel(parent);
	load(model, query);
	return model;
}

void ExerciseModel::load(ExerciseModel * model, QSqlQuery &query) {
// TODO optimize
	auto programIdField = query.record().indexOf("program_id");

	model->setProgramId(query.value(programIdField).toInt());
	Base::ExerciseModel::load(model, query);
}
