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

#ifndef TRAINING_PROGRAM_MODEL_H
#define TRAINING_PROGRAM_MODEL_H

#include <QObject>
#include <QVector>
#include <QVariantList>
#include <QSqlQuery>

#include "src/activity/training/exercise/ExerciseModel.h"
namespace Description{
	class ProgramModel;
}
//#include "src/activity/description/program/ProgramModel.h"

namespace Training{
class TrainModel : public QObject{
Q_OBJECT
	Q_PROPERTY(Description::ProgramModel *targetProgramModel MEMBER mTargetProgramModel NOTIFY targetProgramModelChanged)
	Q_PROPERTY(QString name MEMBER mName NOTIFY nameChanged)
	Q_PROPERTY(QString description MEMBER mDescription NOTIFY descriptionChanged)
	Q_PROPERTY(QVariantList exercises READ getVariantExercises NOTIFY exercisesChanged)

	Q_PROPERTY(bool invalidName MEMBER mInvalidName NOTIFY invalidNameChanged)
	Q_PROPERTY(bool invalidDescription MEMBER mInvalidDescription NOTIFY invalidDescriptionChanged)
	Q_PROPERTY(bool invalidExercises MEMBER mInvalidExercises NOTIFY invalidExercisesChanged)
public:
	TrainModel();
	explicit TrainModel(QObject *parent );

	void setTargetProgramModel(Description::ProgramModel *data);

	QString getName() const;
	void setName(QString name);
	void setDescription(QString description);

	QDateTime getStartTime() const;
	void setStartTime(const time_t& data_ms);	// ms
	void setStartTime(const QDateTime& data);

	QVariantList getVariantExercises() const;
	const QList<ExerciseModel*>& getExercises() const;
	QList<ExerciseModel*>& getExercises();
	Q_INVOKABLE void addExercise(ExerciseModel *model, bool keepId = false);
	Q_INVOKABLE void removeExercise(ExerciseModel *model);
	void clearExercises();

	void setInvalidExercises(bool invalid);
	void setInvalidName(bool invalid);
	void setInvalidDescription(bool invalid);

	qint64 getTrainId()const;
	virtual void setTrainId(qint64 id);

	Q_INVOKABLE virtual bool save();
	static QList<TrainModel*> load(QObject * parent);
	static TrainModel *load(QSqlQuery &query, QObject * parent);
	QList<ExerciseModel*>::Iterator start();

signals:
	void targetProgramModelChanged();
	void trainIdChanged();
	void nameChanged();
	void descriptionChanged();
	void startTimeChanged();
	void exercisesChanged();
	void nextExercise();

	void invalidNameChanged();
	void invalidDescriptionChanged();
	void invalidExercisesChanged();

protected:
	Description::ProgramModel *mTargetProgramModel;
	qint64 mTrainId = 0;
	QString mName;
	QString mDescription;
	QDateTime mStartTime;
	QList<ExerciseModel*> mExercises;

	bool mInvalidName = true;
	bool mInvalidDescription = false;
	bool mInvalidExercises = false;
};
}
#endif
