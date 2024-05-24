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

#include "src/tool/QmlModel.h"
#include "exercise/TrainingExerciseModel.h"
#include "../program/ProgramModel.h"

class TrainingModel : public QmlModel{
Q_OBJECT
// DB
	Q_PROPERTY(QString name MEMBER mName NOTIFY nameChanged)
	Q_PROPERTY(QString description MEMBER mDescription NOTIFY descriptionChanged)
	Q_PROPERTY(QString startDateTimeStr READ getStartDateTimeStr WRITE setStartDateTimeStr NOTIFY startDateTimeChanged)
	Q_PROPERTY(QVariantList exercises READ getVariantExercises NOTIFY exercisesChanged)

public:
	TrainingModel();	// QML
	TrainingModel(QObject *parent);
	TrainingModel(ProgramModel * program, QObject *parent);
	TrainingModel(TrainingModel * program, QObject *parent);


	Q_INVOKABLE qint64 getTrainingId()const;
	virtual void setTrainingId(qint64 id);

	QString getName() const;
	void setName(QString name);

	QString getDescription() const;
	void setDescription(QString description);

	QDateTime getStartDateTime() const;
	void setStartDateTime(const time_t& data_ms);	// ms
	void setStartDateTime(const QDateTime& data);
	QString getStartDateTimeStr()const;
	void setStartDateTimeStr(QString data);

	QVariantList getVariantExercises() const;
	const QList<TrainingExerciseModel*>& getExercises() const;
	Q_INVOKABLE TrainingExerciseModel* addExercise(TrainingExerciseModel *model, bool keepId = false);
	Q_INVOKABLE TrainingExerciseModel* buildExercise(ExerciseModel *model);
	Q_INVOKABLE void removeExercise(TrainingExerciseModel *model);
	void clearExercises();

	Q_INVOKABLE void decrementExerciseOrder(TrainingExerciseModel *model);
	Q_INVOKABLE void incrementExerciseOrder(TrainingExerciseModel *model);

	Q_INVOKABLE void updateTrainingOrder();



	Q_INVOKABLE virtual bool save();
	Q_INVOKABLE virtual void remove();
	static QList<TrainingModel*> load(QObject * parent);
	static TrainingModel *load(QSqlQuery &query, QObject * parent);

signals:
	void trainingIdChanged();
	void nameChanged();
	void descriptionChanged();
	void startDateTimeChanged();
	void exercisesChanged();
	void removed(TrainingModel *model);

protected:
	qint64 mTrainingId = 0;
	QString mName;
	QString mDescription;
	QDateTime mStartDateTime;
	QList<TrainingExerciseModel*> mExercises;

};

#endif
