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

#include "Computation.h"
#include "src/database/DatabaseQuery.h"
#include "src/database/personal/PersonalDataModel.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

int Computation::ageAt(const QDate& birthday, const QDate& today) {
	auto age = today.year() - birthday.year();
	return today.month() >= birthday.month() && today.day() >= birthday.day() ? age : age - 1;
}

double Computation::targetDailyCalories(QDate date) {
	if(!date.isValid()) return 0.0;
	QSqlQuery query;
	if(!query.exec("SELECT * FROM personal_data ORDER by date_time ASC")){
		qCritical() << "Cannot compute calories from personal data:" << query.lastError().text();
		return 0.0;
	}
	auto dateTimeField = query.record().indexOf("date_time");

	QVector<QPair<qulonglong, PersonalDataModel*>> weights;
	while(query.next()){
		weights.push_back(QPair<qulonglong, PersonalDataModel*>(query.value(dateTimeField).toULongLong(), PersonalDataModel::load(query, nullptr)));
	}
	if(weights.size() == 0){
		qCritical() << "Cannot compute Calories : no personal data";
	}

	int weightIndex = 0;
	int bestWeightIndex = 0;
	qint64 dateT = QDateTime(date, QTime(0,0,0)).toMSecsSinceEpoch();
	if( weights.size() > 0){
		if( weights.size() > 1){
			while(weightIndex < weights.size() - 1 && dateT > weights[weightIndex+1].first)
				++weightIndex;
			bestWeightIndex = dateT - weights[weightIndex].first < weights[weightIndex+1].first - dateT ? weightIndex : weightIndex +1;
		}
		//Mifflin-St Jeor Equation
		int age = ageAt(weights[bestWeightIndex].second->getBirthday(), date);
		int weight = weights[bestWeightIndex].second->getWeight();
		int height = weights[bestWeightIndex].second->getHeight();
		if( weights[bestWeightIndex].second->getSex() == 0)
			return 10.0 * weight + 6.25 * height - 5.0 * age + 5;
		else
			return 10.0 * weights[bestWeightIndex].second->getWeight() + 6.25 * weights[bestWeightIndex].second->getHeight() - 5.0 * age - 161;
	}
	return 0.0;
}

double Computation::targetDailyCarbo(QDate date){
	return 35.0;
}

double Computation::targetDailyFat(QDate date){
	return 35.0;
}

double Computation::targetDailyProtein(QDate date){
	return 30.0;
}
