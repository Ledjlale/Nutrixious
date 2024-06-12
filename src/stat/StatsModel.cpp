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

#include "src/database/training/exercise/TrainingExerciseModel.h"
#include "src/database/training/TrainingModel.h"
#include "src/database/training/serie/TrainingSerieModel.h"

StatsModel::StatsModel(QObject *parent)
	: QObject{parent}
{}


void StatsModel::setExercise(ExerciseModel *exercise) {
	if( mExerciseModel != exercise ){
		mExerciseModel = exercise;
		if(mExerciseModel)
			setTrainings(TrainingModel::buildAll({mExerciseModel}, nullptr));
		emit exerciseModelChanged();
	}
}

void StatsModel::setTrainings(QList<TrainingModel*> data){
	if(mTrainings != data){
		for(auto i : mTrainings) i->deleteLater();
		mTrainings = data;
		emit trainingsChanged();
	}
}

QVariantList StatsModel::getAvailableSerieModes()const{
	QVariantList result;
	QVariantMap mode;

	bool distance=false, repetition=false, weight=false, speed=false;

	// TODO optimization
	for(auto i : mTrainings){
		for(auto j : i->getExercises()){
			distance |= j->getUseDistance();
			repetition |= j->getUseRepetitions();
			weight |= j->getUseWeight();
			speed |= j->getUseSpeed();
			if( distance && repetition && weight && speed) break;
		}
	}
	if(weight) {
		mode["text"] = "Weight";
		mode["value"] = WEIGHT;
		result << mode;
	}
	if(distance){
		mode["text"] = "Distance";
		mode["value"] = DISTANCE;
		result << mode;
	}
	if(speed){
		mode["text"] = "Speed";
		mode["value"] = SPEED;
		result << mode;
	}
	if(repetition){
		mode["text"] = "Repetition";
		mode["value"] = REPETITION;
		result << mode;
	}
	mode["text"] = "Calories";
	mode["value"] = CALORIES;
	result << mode;

	return result;
}

QVariantMap getPoint(QDate x, double y) {
	QVariantMap m;
	m["x"] = x;
	m["y"] = y;
	return m;
}

QVariantList StatsModel::computeOnSerie(ComputeMode mode) const {
	QVariantList points;
	QSqlQuery query;
	QMap<QDate, double> sums;
	QMap<QDate, QList<double>> average;

	for(auto i : mTrainings){
		for(auto j : i->getExercises()){
			for(auto k : j->getSeries()){
				switch(mode){
				case WEIGHT : if( j->getUseWeight()){
								sums[i->getStartDateTime().date()] += k->getWeight();
							} break;
				case DISTANCE : if( j->getUseDistance()){
									sums[i->getStartDateTime().date()] += k->getDistance();
								}
								break;
				case REPETITION : if( j->getUseRepetitions()){
									sums[i->getStartDateTime().date()] += k->getRepetitions();
								}
								break;
				case CALORIES : sums[i->getStartDateTime().date()] += dynamic_cast<TrainingSerieModel*>(k)->getCalories(); break;
				case SPEED : if( j->getUseSpeed()){
								average[i->getStartDateTime().date()] << k->getSpeed();
							}
							break;
				default:{}
				}
			}
		}
	}
	if(mode == SPEED){
		for(auto i = average.begin() ; i != average.end() ; ++i){
			for(auto j : i.value())
				sums[i.key()] += j;
			if(i.value().size()> 0)
				sums[i.key()] /= i.value().size();
		}
	}
	//auto exercises = TrainingExerciseModel::loadAll(id);
/*
// Get exercises
	if(!query.exec("SELECT start_date_time, training_exercise_units.data, training_exercise_units.type FROM trainings, training_exercise_units WHERE trainings.training_id = training_exercise_units.training_id AND training_exercise_units.exercise_id="+QString::number(id))){
		qCritical() << "Bad stats for exercises! "  << query.lastError().text();
	}else{
		auto tField = query.record().indexOf("start_date_time");
		auto wField = query.record().indexOf("training_exercise_units.data");
		auto typeField = query.record().indexOf("training_exercise_units.type");
		while(query.next()){
			switch(query.value(typeField).toInt()){
				case 1 : {
					QStringList fields = query.value(wField).toString().split(',');
					weights[QDateTime::fromMSecsSinceEpoch(query.value(tField).toULongLong()).date()] += fields[0].toDouble();
				}break;
				case 2 : {
					QStringList fields = query.value(wField).toString().split(',');
					weights[QDateTime::fromMSecsSinceEpoch(query.value(tField).toULongLong()).date()] += fields[0].toDouble();
				}break;
			default:{}
			}
		}
	}

// Get series
	if(!query.exec("SELECT start_date_time, training_exercise_units.data, training_exercise_series.data, training_exercise_series.type FROM trainings, tr_exercises,tr_exercise_series WHERE trainings.training_id = tr_exercises.training_id AND tr_exercise_series.tr_exercise_id=tr_exercises.tr_exercise_id AND tr_exercises.exercise_id="+QString::number(id))){
		qCritical() << "Bad stats for series! "  << query.lastError().text();
	}else{
		auto tField = query.record().indexOf("start_date_time");
		auto wField = query.record().indexOf("tr_exercise_series.data");
		auto typeField = query.record().indexOf("tr_exercise_series.type");
		while(query.next()){
			switch(query.value(typeField).toInt()){
				case 1 : {
					QStringList fields = query.value(wField).toString().split(',');
					weights[QDateTime::fromMSecsSinceEpoch(query.value(tField).toULongLong()).date()] += fields[1].toDouble();
				}break;
			default:{}
			}
		}
	}
*/
/*
	if(!query.exec("SELECT start_date_time, tr_exercise_series.data FROM trainings, tr_exercises, tr_exercise_series WHERE trainings.training_id = tr_exercises.training_id AND exercise_id="+QString::number(id) +" AND tr_exercise_series.tr_exercise_id = tr_exercises.tr_exercise_id"  )){
			qCritical() << "Bad stats! "  << query.lastError().text();
		}

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
*/
	for(auto p = sums.begin() ; p != sums.end() ; ++p) {
			//points << getPoint(p.key().toJulianDay(), p.value() );
			points << getPoint(p.key(), p.value() );
		}
	return points;
}
