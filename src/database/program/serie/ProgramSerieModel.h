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

#ifndef DATABASE_SERIE_MODEL_H
#define DATABASE_SERIE_MODEL_H

#include <QObject>
#include <QVector>
#include <QSqlQuery>

#include "../../exerciseSerie/ExerciseSerieModel.h"

class ProgramSerieModel : public ExerciseSerieModel{
Q_OBJECT
public:
	ProgramSerieModel();
	ProgramSerieModel(QObject *parent);
	ProgramSerieModel(int type, QObject *parent);
	ProgramSerieModel(const ProgramSerieModel * model, QObject *parent);
	virtual ProgramSerieModel * clone( QObject *parent)const;

	static ProgramSerieModel *build(QSqlQuery &query, QObject * parent);
};

#endif
