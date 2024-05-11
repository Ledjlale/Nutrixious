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

#include "StatsModel.h"
#include <QVariant>
#include <QPoint>
#include <QDateTime>
#include <QMap>

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

StatsModel::StatsModel(QObject *parent)
	: QObject{parent}
{}


void StatsModel::setExercise(Description::ExerciseModel *exercise) {
	if( mExerciseModel != exercise ){
		mExerciseModel = exercise;
		emit exerciseModelChanged();
	}
}

QVariantMap getPoint(double x, double y) {
	QVariantMap m;
	m["x"] = x;
	m["y"] = y;
	return m;
}

QVariantList StatsModel::computeWeights() const {
	QVariantList points;
	QSqlQuery query;
	QMap<QDate, double> weights;

	auto id = mExerciseModel->getExerciseId();
	if (mExerciseModel->getType() == 3 ){
		if(!query.exec("SELECT start_date_time, weight FROM trains, tr_ex_strength, tr_ex_strength_set  WHERE trains.id = tr_ex_strength.train_id AND  exercise_id="+QString::number(id) +" AND tr_ex_strength_set.strength_id = tr_ex_strength.id"  )){
			qCritical() << "Bad stats! "  << query.lastError().text();
		}
		auto tField = query.record().indexOf("start_date_time");
		auto wField = query.record().indexOf("weight");
		while(query.next()){
			weights[QDateTime::fromMSecsSinceEpoch(query.value(tField).toULongLong()).date()] += query.value(wField).toDouble();
		}

	}else if( mExerciseModel->getType() == 2 ){
		if(!query.exec("SELECT start_date_time, steps FROM trains, tr_ex_steps WHERE trains.id = tr_ex_steps.train_id AND exercise_id="+QString::number(id)  )){
			qCritical() << "Bad stats! "  << query.lastError().text();
		}
		auto tField = query.record().indexOf("start_date_time");
		auto wField = query.record().indexOf("steps");
		while(query.next()){
			weights[QDateTime::fromMSecsSinceEpoch(query.value(tField).toULongLong()).date()] += query.value(wField).toDouble();
		}
	}else if( mExerciseModel->getType() == 1 ){
		if(!query.exec("SELECT start_date_time, distance FROM trains, tr_ex_distance WHERE trains.id = tr_ex_distance.train_id AND exercise_id="+QString::number(id)  )){
			qCritical() << "Bad stats! "  << query.lastError().text();
		}
		auto tField = query.record().indexOf("start_date_time");
		auto wField = query.record().indexOf("distance");
		while(query.next()){
			weights[QDateTime::fromMSecsSinceEpoch(query.value(tField).toULongLong()).date()] += query.value(wField).toDouble();
		}
	}

	for(auto p = weights.begin() ; p != weights.end() ; ++p) {
			points << getPoint(p.key().toJulianDay(), p.value() );
		}
	return points;
}
