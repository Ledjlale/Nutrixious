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

#ifndef PROGRAM_EXERCISE_MODEL_H
#define PROGRAM_EXERCISE_MODEL_H

#include <QObject>
#include <QVector>
#include <QSqlQuery>
#include "src/activity/base/exercise/ExerciseModel.h"

namespace Program{
class SerieModel;

class ExerciseModel : public Base::ExerciseModel{
Q_OBJECT

public:
	ExerciseModel();
	ExerciseModel(QObject *parent);
	ExerciseModel(int type, QObject *parent);
	ExerciseModel(const ExerciseModel * model, QObject *parent);
	virtual ExerciseModel * clone(QObject *parent)const;

	qint64 getProgramId()const;
	virtual void setProgramId(qint64 id);

	virtual void saveValues(DatabaseQuery &query);
	static ExerciseModel *load(QSqlQuery &query, QObject * parent);
	static void load(ExerciseModel * model, QSqlQuery &query);

signals:
	void programIdChanged();

protected:
	qint64 mProgramId = -1;
};

}

#endif
