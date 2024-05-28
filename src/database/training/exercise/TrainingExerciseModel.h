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

#ifndef DATABASE_TRAINING_EXERCISE_MODEL_H
#define DATABASE_TRAINING_EXERCISE_MODEL_H

#include <QObject>
#include <QVector>
#include <QSqlQuery>

#include "../../program/exercise/ProgramExerciseModel.h"
#include "../serie/TrainingSerieModel.h"

class TrainingExerciseModel : public ProgramExerciseModel{
Q_OBJECT

public:
	TrainingExerciseModel();
	TrainingExerciseModel(QObject *parent);
	TrainingExerciseModel(const TrainingExerciseModel * model, QObject *parent);
	TrainingExerciseModel(const ProgramExerciseModel * model, QObject *parent);

	virtual TrainingExerciseModel * clone(QObject *parent)const;

	Q_INVOKABLE void addSerie();
	virtual ExerciseSerieModel* insertSerie(ExerciseSerieModel *model) override;
	QList<TrainingSerieModel*> getSeries() const;

	void computeCalories(TrainingSerieModel * serie);

	static TrainingExerciseModel *build(QSqlQuery &query, QObject * parent);

signals:
	void requestComputeCalories(TrainingExerciseModel* exercise, TrainingSerieModel *serie);

};


#endif
