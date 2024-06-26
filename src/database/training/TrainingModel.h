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

#ifndef DATABASE_TRAINING_MODEL_H
#define DATABASE_TRAINING_MODEL_H

#include <QObject>
#include <QVector>
#include <QVariantList>
#include <QSqlQuery>
#include <QDateTime>

#include "exercise/TrainingExerciseModel.h"
#include "serie/TrainingSerieModel.h"
#include "../program/ProgramModel.h"

class TrainingModel : public ProgramModel{
Q_OBJECT
	Q_PROPERTY(QString startDateTimeStr READ getStartDateTimeStr WRITE setStartDateTimeStr NOTIFY startDateTimeChanged)
	Q_PROPERTY(QString startTimeStr READ getStartTimeStr WRITE setStartTimeStr NOTIFY startDateTimeChanged)

	DECLARE_GETSET(double,calories,Calories)

public:
	TrainingModel();	// QML
	TrainingModel(QObject *parent);
	TrainingModel(ProgramModel * program, QObject *parent);
	TrainingModel(TrainingModel * program, QObject *parent);


	QDateTime getStartDateTime() const;
	void setStartDateTime(const time_t& data_ms);	// ms
	void setStartDateTime(const QDateTime& data);
	QString getStartDateTimeStr()const;
	void setStartDateTimeStr(QString data);
	QString getStartTimeStr()const;
	void setStartTimeStr(QString data);


	virtual ProgramExerciseModel* insertNewExercise(ProgramExerciseModel *model); // clone and make new
	virtual ProgramExerciseModel* insertExercise(ProgramExerciseModel *model);// Insert and set parent ID

/*
	QVariantList getVariantExercises() const;
	const QList<TrainingExerciseModel*>& getExercises() const;
	Q_INVOKABLE TrainingExerciseModel* addExercise(TrainingExerciseModel *model, bool keepId = false);

	Q_INVOKABLE void removeExercise(TrainingExerciseModel *model);
	void clearExercises();
*/

	Q_INVOKABLE TrainingExerciseModel* buildExercise(ExerciseModel *model);
	static TrainingExerciseModel* buildExercise(ExerciseModel *model, QObject *parent);
	virtual void addQueryValues(DatabaseQuery &query);

	void computeCalorie(TrainingExerciseModel * exercise, TrainingSerieModel * serie);
	void updateCalories();

	virtual void load(QSqlQuery &query);
	Q_INVOKABLE virtual void undo();
	static TrainingModel *build(QSqlQuery &query, QObject * parent);
	static QList<TrainingModel*> buildAll(QObject * parent);
	static QList<TrainingModel*> buildAll(QList<ExerciseModel *> exercises, QObject * parent);

signals:
	void startDateTimeChanged();
	void removed(TrainingModel *model);

protected:
	QDateTime mStartDateTime;
	double mCalories = 0.0;
};

#endif
