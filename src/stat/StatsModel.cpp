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

#include "src/database/personal/PersonalDataModel.h"
#include "src/database/mealFood/MealFoodModel.h"

StatsModel::StatsModel(QObject *parent)
	: QObject{parent}
{}


void StatsModel::setExercise(ExerciseModel *exercise) {
	if( mExerciseModel != exercise ){
		mExerciseModel = exercise;
		if(mExerciseModel)
			setTrainings(TrainingModel::buildAll({mExerciseModel}, nullptr));
		else
			setTrainings(QList<TrainingModel*>());
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

QVariantMap getPoint(QDateTime x, double y) {
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
	for(auto p = sums.begin() ; p != sums.end() ; ++p) {
		points << getPoint(p.key(), p.value() );
	}
	return points;
}

QVariantList StatsModel::computeBodyWeights() const{
	auto data = PersonalDataModel::loadAll(nullptr);
	QVariantList points;
	for(auto i : data){
		points << getPoint(i->getDateTime(), i->getWeight() );
		i->deleteLater();
	}
	return points;
}

QVariantList StatsModel::computeNutritionCalories() const {
	auto models = MealFoodModel::buildAll(QDate(), nullptr);
	QVariantList points;
	QMap<QDate, double> sums;
	for(auto i : models){
		sums[i->getConsumptionDateTime().date()] += i->computeNutriment(i->getCalories());
		i->deleteLater();
	}
	for(auto p = sums.begin() ; p != sums.end() ; ++p) {
		points << getPoint(p.key(), p.value() );
	}
	return points;
}
