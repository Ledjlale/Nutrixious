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

#ifndef TRAINING_MODEL_H
#define TRAINING_MODEL_H

#include <QObject>
#include <QTimer>
#include <QTime>

#include "src/activity/training/train/TrainModel.h"

namespace Training{
class TrainingModel : public QObject {
	Q_OBJECT
	Q_PROPERTY(TrainModel *trainModel MEMBER mTrainModel NOTIFY trainModelChanged)
	Q_PROPERTY(bool pause MEMBER mPause NOTIFY pauseChanged)
	Q_PROPERTY(bool resting READ isResting NOTIFY isRestingChanged)
	Q_PROPERTY(bool working READ isWorking NOTIFY isWorkingChanged)
	Q_PROPERTY(QVariant currentWork READ getCurrentWork NOTIFY currentWorkChanged)

public:
	TrainingModel();
	explicit TrainingModel(QObject *parent);
	~TrainingModel();


	bool isWorking()const;
	void setIsWorking(bool data);

	bool isResting()const;
	void setIsResting(bool data);

	Q_INVOKABLE void start();	// Start from 0
	Q_INVOKABLE void stop();
	Q_INVOKABLE void resume();	// Resume after pause
	Q_INVOKABLE void save();

	Q_INVOKABLE void endOfCurrentWork();
	Q_INVOKABLE void endOfCurrentRest();
	Q_INVOKABLE QVariant getCurrentWork();

	void refresh();
	void nextExercise();

signals:
	void trainModelChanged();
	void workTimeChanged();
	void restTimeChanged();
	void pauseChanged();
	void isRestingChanged();
	void isWorkingChanged();

	void exercisesChanged();
	void workingNextExercise(int index);
	void currentWorkChanged();
	void finished();

protected:
	TrainModel *mTrainModel;

	QList<ExerciseModel*>::Iterator mCurrentExercise;
	QList<ExerciseModel*>::Iterator mNextExercise;

	bool mPause = false;
	QTimer mRestTimer;
	QDateTime mTimeStart;

	bool mIsResting = false;
	bool mIsWorking = false;
};
}
#endif
