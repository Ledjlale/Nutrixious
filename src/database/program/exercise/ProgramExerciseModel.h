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

#ifndef DATABASE_PROGRAM_EXERCISE_MODEL_H
#define DATABASE_PROGRAM_EXERCISE_MODEL_H

#include <QObject>
#include <QVector>
#include <QSqlQuery>

#include "src/database/DatabaseQuery.h"

#include "../../exercise/ExerciseModel.h"
#include "../../exerciseUnit/ExerciseUnitModel.h"
#include "../serie/ProgramSerieModel.h"

class ProgramExerciseModel : public ExerciseUnitModel{
Q_OBJECT
// DB

public:
	ProgramExerciseModel();
	explicit ProgramExerciseModel(QObject *parent);
	ProgramExerciseModel(int type, QObject *parent);
	ProgramExerciseModel(const ProgramExerciseModel * model, QObject *parent);
	virtual ProgramExerciseModel * clone(QObject *parent)const;

	QList<ProgramSerieModel*> getSeries() const;
	Q_INVOKABLE void addSerie();
	Q_INVOKABLE void updateFromLastProgram();

	static QList<ProgramExerciseModel*> buildAll(QObject * parent);
	static ProgramExerciseModel *build(QSqlQuery &query, QObject * parent);

};


#endif
