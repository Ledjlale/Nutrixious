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

#ifndef DISTANCE_MODEL_H
#define DISTANCE_MODEL_H

#include <QObject>
#include <QVector>
#include <QSqlQuery>

#include "src/activity/description/exercise/ExerciseModel.h"

namespace Description{
class DistanceModel : public ExerciseModel {
	Q_OBJECT
	Q_PROPERTY(int distance MEMBER mDistance NOTIFY distanceChanged)
	Q_PROPERTY(int weight MEMBER mWeight NOTIFY weightChanged)
	Q_PROPERTY(int restTime MEMBER mRestTime NOTIFY restTimeChanged)
public:
	explicit DistanceModel(QObject *parent = nullptr);
	DistanceModel(const DistanceModel * model, QObject *parent = nullptr);
	virtual ExerciseModel * clone(qint64 programId)const;
	virtual Training::ExerciseModel * cloneTraining();
	virtual int getType() const;

	Q_INVOKABLE virtual bool save();
	//virtual bool saveProgram(qint64 programId);
	static QList<ExerciseModel*> load();
	static DistanceModel *load(QSqlQuery &query);	// Create a model from the current query.

	int getDistance() const;
	void setDistance(int distance);

	int getWeight()const;
	void setWeight(int weight);

	int getRestTime()const;
	void setRestTime(int data);

signals:
	void nameChanged();
	void descriptionChanged();
	void distanceChanged();
	void weightChanged();
	void restTimeChanged();

protected:
	int mDistance = 10000;// 10 km
	int mWeight = 0;
	int mRestTime = 60*5;// 5 minutes
};
}
#endif
