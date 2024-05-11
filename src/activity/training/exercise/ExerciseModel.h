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

#ifndef TRAINING_EXERCISE_MODEL_H
#define TRAINING_EXERCISE_MODEL_H

#include <QObject>
#include <QVector>
#include <QDateTime>

#include "src/activity/description/exercise/ExerciseModel.h"

namespace Training{
class ExerciseModel : public QObject{
Q_OBJECT
	Q_PROPERTY(Description::ExerciseModel *targetExercise MEMBER mTargetExercise NOTIFY targetExerciseChanged)
	Q_PROPERTY(QString name MEMBER mName NOTIFY nameChanged)
	Q_PROPERTY(QString description MEMBER mDescription NOTIFY descriptionChanged)

	Q_PROPERTY(bool invalidName MEMBER mInvalidName NOTIFY invalidNameChanged)
	Q_PROPERTY(bool invalidDescription MEMBER mInvalidDescription NOTIFY invalidDescriptionChanged)
	Q_PROPERTY(int type READ getType WRITE setType NOTIFY typeChanged)
	Q_PROPERTY(int trainOrder READ getTrainOrder WRITE setTrainOrder NOTIFY trainOrderChanged)

	Q_PROPERTY(bool isRunning READ getIsRunning WRITE setIsRunning NOTIFY isRunningChanged)
	Q_PROPERTY(bool isResting READ getIsResting WRITE setIsResting NOTIFY isRestingChanged)
	Q_PROPERTY(bool isDone READ getIsDone WRITE setIsDone NOTIFY isDoneChanged)
	Q_PROPERTY(bool isSaved READ getIsSaved WRITE setIsSaved NOTIFY isSavedChanged)
	Q_PROPERTY(bool isTraining MEMBER mIsTraining CONSTANT)
public:
	explicit ExerciseModel(QObject *parent = nullptr);
	ExerciseModel(const ExerciseModel * model, QObject *parent = nullptr);
	virtual ~ExerciseModel();
	virtual ExerciseModel * clone(qint64 trainId, QObject *parent)const;

	virtual int getType() const;	// For QML to know how to display it because of lacking object relationship.
	void setType(int data);

	virtual void setTargetExercise(Description::ExerciseModel * data);

	QString getName()const;
	void setName(QString name);

	QString getDescription() const;
	void setDescription(QString description);

	void setInvalidName(bool invalid);
	void setInvalidDescription(bool invalid);

	qint64 getExerciseId()const;
	void setExerciseId(qint64 id);

	int getTrainOrder() const;
	void setTrainOrder(int data);

	qint64 getTrainId() const;
	virtual void setTrainId(qint64 id);

	qint64 getDescriptionExerciseId() const;
	virtual void setDescriptionExerciseId(qint64 id);

	bool getIsRunning() const;
	void setIsRunning(bool data);

	bool getIsResting() const;
	void setIsResting(bool data);

	bool getIsDone() const;
	void setIsDone(bool data);

	bool getIsSaved() const;
	void setIsSaved(bool data);

	virtual void startWork();

	virtual void endOfCurrentWork(){}
	virtual void endOfCurrentRest(){}
	virtual QVariant getCurrentWork();



	Q_INVOKABLE virtual bool save();

	virtual void fillRandomValues(); // Debug


signals:
	void exerciseIdChanged();
	void trainIdChanged();
	void descriptionExerciseIdChanged();
	void targetExerciseChanged();
	void nameChanged();
	void descriptionChanged();
	void trainOrderChanged();
	void typeChanged();
	void isRunningChanged();
	void isRestingChanged();
	void isDoneChanged();
	void isSavedChanged();
	void finished();


	void invalidNameChanged();
	void invalidDescriptionChanged();

protected:
	Description::ExerciseModel *mTargetExercise = nullptr;
	qint64 mExerciseId = 0;
	qint64 mTrainId = -1;
	qint64 mDescriptionExerciseId = -1;
	QString mName;
	QString mDescription;
	int mTrainOrder = -1;			// Order in train
	int mType = 0;
	bool mIsRunning = false;
	bool mIsResting = false;
	bool mIsDone = false;
	bool mIsSaved = false;
	bool mIsTraining = true;

	QDateTime mStart;

	bool mInvalidName = true;
	bool mInvalidDescription = false;
};
}

#endif
