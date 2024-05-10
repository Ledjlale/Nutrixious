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

#ifndef TRAINING_STEPS_MODEL_H
#define TRAINING_STEPS_MODEL_H

#include <QObject>
#include <QVector>
#include <QSqlQuery>

#include "src/activity/training/exercise/ExerciseModel.h"

namespace Training{
class StepsModel : public ExerciseModel {
	Q_OBJECT
	Q_PROPERTY(int steps MEMBER mSteps NOTIFY stepsChanged)
	Q_PROPERTY(int weight MEMBER mWeight NOTIFY weightChanged)
	Q_PROPERTY(int workTime MEMBER mWorkTime NOTIFY workTimeChanged)
	Q_PROPERTY(int restTime MEMBER mRestTime NOTIFY restTimeChanged)
public:
	explicit StepsModel(QObject *parent = nullptr);
	StepsModel(const StepsModel * model, QObject *parent = nullptr);
	virtual ExerciseModel * clone()const;

	Q_INVOKABLE virtual bool save();
	virtual bool saveProgram(qint64 programId);
	static QList<ExerciseModel*> load();
	static StepsModel *load(QSqlQuery &query);	// Create a model from the current query.

	virtual void setTargetExercise(Description::ExerciseModel * data);

	int getSteps() const;
	void setSteps(int data);

	int getWeight()const;
	void setWeight(int data);

	int getWorkTime()const;
	void setWorkTime(int data);

	int getRestTime()const;
	void setRestTime(int restTime);

	virtual void endOfCurrentWork();
	virtual void endOfCurrentRest();

signals:
	void nameChanged();
	void descriptionChanged();
	void stepsChanged();
	void weightChanged();
	void workTimeChanged();
	void restTimeChanged();

protected:
	int mSteps = 1000;
	int mWeight = 0;
	int mWorkTime = 0;
	int mRestTime = 2;
};
}

#endif
