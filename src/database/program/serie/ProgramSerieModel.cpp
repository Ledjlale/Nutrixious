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

#include "ProgramSerieModel.h"
#include <QSqlQuery>
#include <QSqlRecord>

#include "src/database/DatabaseQuery.h"

#include <QQmlApplicationEngine>
extern QQmlApplicationEngine * gEngine;

ProgramSerieModel::ProgramSerieModel() : ExerciseSerieModel(nullptr){
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
	mTablePrefix = "program";
}

ProgramSerieModel::ProgramSerieModel(QObject *parent)
	: ExerciseSerieModel{parent} {
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	mTablePrefix = "program";
}

ProgramSerieModel::ProgramSerieModel(const ProgramSerieModel * model, QObject *parent) : ExerciseSerieModel(model, parent){
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	mTablePrefix = "program";
}

ProgramSerieModel * ProgramSerieModel::clone(QObject *parent)const{
	ProgramSerieModel *model = new ProgramSerieModel(this, parent);
	model->clearBackupValues();
	return model;
}

ProgramSerieModel *ProgramSerieModel::build(QSqlQuery &query, QObject * parent) {
	ProgramSerieModel * model = new ProgramSerieModel(parent);
	model->load(query);
	return model;
}
