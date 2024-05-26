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
#include "../program/ProgramModel.h"

class TrainingModel : public ProgramModel{
Q_OBJECT
	Q_PROPERTY(QString startDateTimeStr READ getStartDateTimeStr WRITE setStartDateTimeStr NOTIFY startDateTimeChanged)

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
/*
	QVariantList getVariantExercises() const;
	const QList<TrainingExerciseModel*>& getExercises() const;
	Q_INVOKABLE TrainingExerciseModel* addExercise(TrainingExerciseModel *model, bool keepId = false);

	Q_INVOKABLE void removeExercise(TrainingExerciseModel *model);
	void clearExercises();
*/

	Q_INVOKABLE TrainingExerciseModel* buildExercise(ExerciseModel *model);

	virtual void addQueryValues(DatabaseQuery &query);

	virtual void load(QSqlQuery &query);
	static TrainingModel *build(QSqlQuery &query, QObject * parent);
	static QList<TrainingModel*> buildAll(QObject * parent);

signals:
	void startDateTimeChanged();
	void removed(TrainingModel *model);

protected:
	QDateTime mStartDateTime;
};

#endif
