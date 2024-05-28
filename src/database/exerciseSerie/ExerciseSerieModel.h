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

#ifndef DATABASE_EXERCISE_SERIE_MODEL_H
#define DATABASE_EXERCISE_SERIE_MODEL_H

#include <QObject>
#include <QVector>
#include <QSqlQuery>

#include "src/tool/QmlModel.h"
#include "src/database/DatabaseQuery.h"

class ExerciseSerieModel : public QmlModel{
Q_OBJECT
// DB
	Q_PROPERTY(int restTime READ getRestTime WRITE setRestTime NOTIFY restTimeChanged)
	Q_PROPERTY(int workTime READ getWorkTime WRITE setWorkTime NOTIFY workTimeChanged)
	Q_PROPERTY(QString restTimeStr READ getRestTimeStr WRITE setRestTimeStr NOTIFY restTimeChanged)
	Q_PROPERTY(QString workTimeStr READ getWorkTimeStr WRITE setWorkTimeStr NOTIFY workTimeChanged)
	Q_PROPERTY(int order READ getOrder WRITE setOrder NOTIFY orderChanged)

	Q_PROPERTY(int distance READ getDistance WRITE setDistance NOTIFY distanceChanged)
	Q_PROPERTY(double speed READ getSpeed WRITE setSpeed NOTIFY speedChanged)
	Q_PROPERTY(double weight READ getWeight WRITE setWeight NOTIFY weightChanged)
	Q_PROPERTY(int repetitions READ getRepetitions WRITE setRepetitions NOTIFY repetitionsChanged)

public:
	ExerciseSerieModel();
	ExerciseSerieModel(QObject *parent);
	ExerciseSerieModel(int type, QObject *parent);
	ExerciseSerieModel(const ExerciseSerieModel * model, QObject *parent);
	virtual ~ExerciseSerieModel();
	virtual ExerciseSerieModel * clone( QObject *parent)const;

	qint64 getSerieId() const;
	void setSerieId(qint64 data);

	qint64 getExerciseUnitId() const;
	void setExerciseUnitId(qint64 data);

	int  getRestTime() const;
	void setRestTime(int restTime);

	int  getWorkTime() const;
	void setWorkTime(int workTime);

	QString  getRestTimeStr() const;
	void setRestTimeStr(QString data);

	QString getWorkTimeStr() const;
	void setWorkTimeStr(QString data);

	int getOrder() const;
	void setOrder(int data);

	int getDistance() const;
	void setDistance(int data);

	double getSpeed() const;
	void setSpeed(double data);

	double getWeight() const;
	void setWeight(double data);

	int getRepetitions() const;
	void setRepetitions(int data);


	virtual void updateIsSaved();
	virtual void undo();

	Q_INVOKABLE void makeNew();

	virtual void addQueryValues(DatabaseQuery &query){}
	Q_INVOKABLE virtual bool save();
	virtual void load(QSqlQuery &query);
	Q_INVOKABLE virtual void remove();

signals:
	void serieIdChanged();
	void exerciseUnitIdChanged();
	void orderChanged();
	void restTimeChanged();
	void workTimeChanged();
	void distanceChanged();
	void speedChanged();
	void weightChanged();
	void repetitionsChanged();

	void removed(ExerciseSerieModel * model);

protected:
	qint64 mSerieId = 0;
	qint64 mExerciseUnitId = 0;
	int mWorkTime = 0;
	int mRestTime = 60;
	int mOrder = 0;
	int mDistance = -1;
	double mSpeed = -1.0;
	double mWeight = -1.0;
	int mRepetitions = -1;

//
	QString mTablePrefix = "";
};

#endif
