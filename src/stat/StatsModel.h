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

#ifndef STATS_MODEL_H
#define STATS_MODEL_H

#include <QObject>

#include "src/database/exercise/ExerciseModel.h"

class TrainingModel;

class StatsModel : public QObject {
	Q_OBJECT
	Q_PROPERTY(QVariantList availableSerieModes READ getAvailableSerieModes NOTIFY trainingsChanged)

public:
	StatsModel(QObject *parent = nullptr);
	Q_INVOKABLE void setExercise(ExerciseModel *exercise);

	void setTrainings(QList<TrainingModel*> data);

	QVariantList getAvailableSerieModes()const;
	typedef enum{WEIGHT=0, REPETITION=1,DISTANCE=2,SPEED=3,CALORIES=4} ComputeMode;
	Q_ENUM(ComputeMode);

	Q_INVOKABLE QVariantList computeOnSerie(ComputeMode mode) const;
	Q_INVOKABLE QVariantList computeBodyWeights() const;
	Q_INVOKABLE QVariantList computeNutritionCalories() const;


signals:
	void exerciseModelChanged();
	void trainingsChanged();

protected:
	ExerciseModel *mExerciseModel;
	QList<TrainingModel*> mTrainings;
};

#endif
