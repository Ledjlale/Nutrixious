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

#ifndef CHANGE_DAY_TIMER_H
#define CHANGE_DAY_TIMER_H

#include <QObject>
#include <QDateTime>
#include <QTimer>

class ChangeDayTimer: public QObject{
Q_OBJECT
public:
	ChangeDayTimer(QObject* parent = nullptr);
	static ChangeDayTimer* getInstance();
	
	void onTimeout();
signals:
	void minuteChanged();
	void dayChanged();
protected:
	QTimer mTimer;
	static ChangeDayTimer* gInstance;
};

#endif
