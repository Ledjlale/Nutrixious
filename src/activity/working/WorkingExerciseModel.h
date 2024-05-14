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

#ifndef WORKING_EXERCISE_MODEL_H
#define WORKING_EXERCISE_MODEL_H

#include <QObject>
#include <QVector>
#include <QDateTime>

#include "src/database/training/exercise/TrainingExerciseModel.h"
#include "WorkingSerieModel.h"

class WorkingExerciseModel : public QObject{
Q_OBJECT
	Q_PROPERTY(TrainingExerciseModel *targetExerciseModel MEMBER mTargetExerciseModel NOTIFY targetExerciseModelChanged)
	Q_PROPERTY(TrainingExerciseModel *resultExerciseModel READ getResultExerciseModel NOTIFY resultExerciseModelChanged)
	Q_PROPERTY(QVariantList series READ getVariantSeries NOTIFY seriesChanged)

	Q_PROPERTY(bool isRunning READ getIsRunning WRITE setIsRunning NOTIFY isRunningChanged)
	Q_PROPERTY(bool isResting READ getIsResting WRITE setIsResting NOTIFY isRestingChanged)
	Q_PROPERTY(bool isDone READ getIsDone WRITE setIsDone NOTIFY isDoneChanged)
	Q_PROPERTY(bool isTraining MEMBER mIsTraining CONSTANT)
public:

	WorkingExerciseModel(QObject *parent);
	WorkingExerciseModel(TrainingExerciseModel * target, TrainingExerciseModel * result, QObject *parent);
	virtual ~WorkingExerciseModel();

	TrainingExerciseModel *getTargetExerciseModel() const;
	void setTargetExerciseModel(TrainingExerciseModel *data);

	TrainingExerciseModel *getResultExerciseModel() const;
	void setResultExerciseModel(TrainingExerciseModel *data);


	QVariantList getVariantSeries() const;
	QList<WorkingSerieModel*> getSeries()const;

	bool getIsRunning() const;
	void setIsRunning(bool data);

	bool getIsResting() const;
	void setIsResting(bool data);

	bool getIsDone() const;
	void setIsDone(bool data);

	bool getIsSaved() const;
	void setIsSaved(bool data);

	virtual void startWork();
	void nextSerie();
	virtual void endOfCurrentWork();
	virtual void endOfCurrentRest();
	virtual QVariant getCurrentWork();

	Q_INVOKABLE virtual bool save();

	virtual void fillRandomValues(); // Debug


signals:
	void targetExerciseModelChanged();
	void resultExerciseModelChanged();
	void seriesChanged();
	void isRunningChanged();
	void isRestingChanged();
	void isDoneChanged();
	void finished();
	void workStarted();

protected:
	TrainingExerciseModel *mTargetExerciseModel = nullptr;
	TrainingExerciseModel *mResultExerciseModel = nullptr;

	QList<WorkingSerieModel*> mSeries;
	QList<WorkingSerieModel*>::Iterator mCurrentSerie;

	bool mIsRunning = false;
	bool mIsResting = false;
	bool mIsDone = false;
	bool mIsSaved = false;
	bool mIsTraining = true;

	QDateTime mStart;

};

#endif
