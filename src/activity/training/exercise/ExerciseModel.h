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
	Q_PROPERTY(int programOrder READ getProgramOrder WRITE setProgramOrder NOTIFY programOrderChanged)

	Q_PROPERTY(bool running READ getIsRunning WRITE setIsRunning NOTIFY isRunningChanged)
	Q_PROPERTY(bool resting READ getIsResting WRITE setIsResting NOTIFY isRestingChanged)
	Q_PROPERTY(bool done READ getIsDone WRITE setIsDone NOTIFY isDoneChanged)
public:
	explicit ExerciseModel(QObject *parent = nullptr);
	ExerciseModel(const ExerciseModel * model, QObject *parent = nullptr);
	virtual ExerciseModel * clone()const;

	virtual int getType() const;	// For QML to know how to display it because of lacking object relationship.
	void setType(int data);

	virtual void setTargetExercise(Description::ExerciseModel * data);

	QString getName()const;
	void setName(QString name);

	QString getDescription() const;
	void setDescription(QString description);

	void setInvalidName(bool invalid);
	void setInvalidDescription(bool invalid);

	qint64 getId()const;
	void setId(qint64 id);

	int getProgramOrder() const;
	void setProgramOrder(int data);

	bool getIsRunning() const;
	void setIsRunning(bool data);

	bool getIsResting() const;
	void setIsResting(bool data);

	bool getIsDone() const;
	void setIsDone(bool data);

	virtual void startWork();

	virtual void endOfCurrentWork(){}
	virtual void endOfCurrentRest(){}
	virtual QVariant getCurrentWork();



	Q_INVOKABLE virtual bool save();
	virtual bool saveProgram(qint64 programId);




signals:
	void targetExerciseChanged();
	void nameChanged();
	void descriptionChanged();
	void programOrderChanged();
	void typeChanged();
	void isRunningChanged();
	void isRestingChanged();
	void isDoneChanged();
	void finished();


	void invalidNameChanged();
	void invalidDescriptionChanged();

protected:
	Description::ExerciseModel *mTargetExercise = nullptr;
	qint64 mDbId = 0;
	QString mName;
	QString mDescription;
	int mProgramOrder = -1;			// Order in program
	int mType = 0;
	bool mIsRunning = false;
	bool mIsResting = false;
	bool mIsDone = false;

	QDateTime mStart;

	bool mInvalidName = true;
	bool mInvalidDescription = false;
};
}

#endif
