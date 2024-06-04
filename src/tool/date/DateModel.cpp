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

#include "DateModel.h"


DateModel::DateModel() : QObject(nullptr){
	mDate = QDate::currentDate();
	mTime = QTime::currentTime();
}



QString DateModel::getDateStr()const {
	QDate today = QDate::currentDate();
	if(mDate == today)
		return "Today";
	else if(mDate == today.addDays(-1))
		return "Yesterday";
	else if( mDate == today.addDays(1))
		return "Tomorrow";
	else
		return mDate.toString("yyyy/MM/dd");
}

QString DateModel::getTimeStr()const {
	return mTime.toString("hh:mm:ss");
}

void DateModel::setDate(QDate data){
	if(mDate != data){
		mDate = data;
		emit dateChanged();
	}
}

void DateModel::nextDay(){
	setDate(mDate.addDays(1));
}

void DateModel::previousDay(){
	setDate(mDate.addDays(-1));
}
