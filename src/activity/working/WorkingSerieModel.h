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

#ifndef WORKING_SERIE_MODEL_H
#define WORKING_SERIE_MODEL_H

#include <QObject>
#include <QVector>
#include <QDateTime>

#include "src/database/training/serie/TrainingSerieModel.h"
#include "src/database/program/serie/ProgramSerieModel.h"

class WorkingSerieModel : public QObject{
Q_OBJECT
	Q_PROPERTY(TrainingSerieModel *targetSerieModel MEMBER mTargetSerieModel NOTIFY targetSerieModelChanged)
	Q_PROPERTY(TrainingSerieModel *resultSerieModel READ getResultSerieModel NOTIFY resultSerieModelChanged)

	Q_PROPERTY(bool isRunning READ getIsRunning WRITE setIsRunning NOTIFY isRunningChanged)
	Q_PROPERTY(bool isResting READ getIsResting WRITE setIsResting NOTIFY isRestingChanged)
	Q_PROPERTY(bool isDone READ getIsDone WRITE setIsDone NOTIFY isDoneChanged)
	Q_PROPERTY(bool isTraining MEMBER mIsTraining CONSTANT)
public:
	WorkingSerieModel(QObject *parent);
	WorkingSerieModel(TrainingSerieModel *target, TrainingSerieModel *result, QObject *parent);
	virtual ~WorkingSerieModel();

	TrainingSerieModel *getTargetSerieModel() const;
	void setTargetSerieModel(TrainingSerieModel *data);

	TrainingSerieModel *getResultSerieModel() const;
	void setResultSerieModel(TrainingSerieModel *data);

	bool getIsRunning() const;
	void setIsRunning(bool data);

	bool getIsResting() const;
	void setIsResting(bool data);

	bool getIsDone() const;
	void setIsDone(bool data);

	int getOrder()const;	// use for reordering Utils.
	void setOrder(int data);

	virtual void startWork();

	virtual void endOfCurrentWork();
	virtual void endOfCurrentRest();
	virtual QVariant getCurrentWork();

	Q_INVOKABLE virtual int save();

	virtual void fillRandomValues(); // Debug


signals:
	void targetSerieModelChanged();
	void resultSerieModelChanged();

	void isRunningChanged();
	void isRestingChanged();
	void isDoneChanged();
	void orderChanged();
	void finished();
	void workStarted();


protected:
	TrainingSerieModel *mTargetSerieModel = nullptr;
	TrainingSerieModel *mResultSerieModel = nullptr;

	bool mIsRunning = false;
	bool mIsResting = false;
	bool mIsDone = false;
	bool mIsSaved = false;
	bool mIsTraining = true;

	QDateTime mStart;

};

#endif
