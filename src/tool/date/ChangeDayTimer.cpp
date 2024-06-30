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

#include "ChangeDayTimer.h"

ChangeDayTimer * ChangeDayTimer::gInstance = nullptr;

ChangeDayTimer::ChangeDayTimer(QObject *parent) : QObject(parent){
	connect(&mTimer, &QTimer::timeout, this, &ChangeDayTimer::onTimeout);
	auto current = QTime::currentTime();
	int interval = (59-current.second()) * 1000 + (1000 - current.msec());
	mTimer.setInterval(interval);
	mTimer.setSingleShot(false);
	mTimer.start();
}

ChangeDayTimer* ChangeDayTimer::getInstance(){
	if(gInstance) return gInstance;
	gInstance = new ChangeDayTimer();
	return gInstance;	
}

void ChangeDayTimer::onTimeout(){
	auto current = QTime::currentTime();
	if(current.second() != 0){// recalibration
		mTimer.setInterval((59-current.second()) * 1000 + (1000- current.msec()));
	}else if( mTimer.interval() != 60000){
		mTimer.setInterval(60000);
	}
	emit minuteChanged();
	if(current == QTime(0,0,0))
		emit dayChanged();
}
