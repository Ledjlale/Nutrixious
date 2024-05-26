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

#ifndef WORKING_MODEL_H
#define WORKING_MODEL_H

#include <QObject>
#include <QTimer>
#include <QTime>

#include "src/database/program/ProgramModel.h"
#include "src/database/training/TrainingModel.h"
#include "WorkingExerciseModel.h"

class WorkingModel : public QObject {
	Q_OBJECT

	Q_PROPERTY(bool pause MEMBER mPause NOTIFY pauseChanged)
	Q_PROPERTY(bool resting READ isResting NOTIFY isRestingChanged)
	Q_PROPERTY(bool working READ isWorking NOTIFY isWorkingChanged)
	Q_PROPERTY(QVariant currentExercise READ getCurrentExercise NOTIFY currentExerciseChanged)
	Q_PROPERTY(QVariant currentWork READ getCurrentWork NOTIFY currentWorkChanged)

	Q_PROPERTY(TrainingModel *targetModel READ getTargetModel WRITE setTargetModel NOTIFY targetModelChanged)
	Q_PROPERTY(TrainingModel *resultModel READ getResultModel WRITE setResultModel NOTIFY resultModelChanged)
	Q_PROPERTY(QVariantList exercises READ getVariantExercises NOTIFY exercisesChanged)

public:
	WorkingModel();
	WorkingModel(QObject *parent);
	~WorkingModel();

	TrainingModel *getTargetModel() const;
	void setTargetModel(TrainingModel *data);

	TrainingModel *getResultModel() const;
	void setResultModel(TrainingModel *data);

	Q_INVOKABLE void loadFromProgram(ProgramModel *data);	// Build a TrainingModel
	Q_INVOKABLE void addExercise(ExerciseModel * data);

	QVariantList getVariantExercises() const;

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
	Q_INVOKABLE QVariant getCurrentExercise();
	Q_INVOKABLE QVariant getCurrentWork();

	void refresh();
	void nextExercise();

signals:
	void targetModelChanged();
	void resultModelChanged();
	void workTimeChanged();
	void restTimeChanged();
	void pauseChanged();
	void isRestingChanged();
	void isWorkingChanged();

	void exercisesChanged();
	void workingNextExercise(int index);
	void currentExerciseChanged();
	void currentWorkChanged();
	void finished();

protected:
	TrainingModel *mTargetModel;
	TrainingModel *mResultModel;

	QList<WorkingExerciseModel*> mExercises;
	QList<WorkingExerciseModel*>::Iterator mCurrentExercise;
	QList<WorkingExerciseModel*>::Iterator mNextExercise;

	bool mPause = false;
	QTimer mRestTimer;
	QDateTime mTimeStart;

	bool mIsResting = false;
	bool mIsWorking = false;
};
#endif
