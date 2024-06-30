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

#ifndef DATE_MODEL_H
#define DATE_MODEL_H

#include <QObject>
#include <QDateTime>

class DateModel: public QObject{
Q_OBJECT
	Q_PROPERTY(QString date READ getDateStr NOTIFY dateChanged)
	Q_PROPERTY(QString time READ getTimeStr NOTIFY timeChanged)
public:
	DateModel();

	QString getDateStr()const;
	QString getTimeStr()const;
	QDate getDate() const;
	void setDate(QDate data);
	
	QTime getTime() const;
	QDateTime getDateTime() const;

	Q_INVOKABLE void nextDay();
	Q_INVOKABLE void previousDay();
signals:
	void dateChanged();
	void timeChanged();
protected:
	QDate mDate;
	QTime mTime;
};

#endif
