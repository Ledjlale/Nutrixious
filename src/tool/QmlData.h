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

#ifndef QML_DATA_H
#define QML_DATA_H

#include <QObject>
#include <QVariant>

class QmlData: public QObject{
Q_OBJECT
	Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
	Q_PROPERTY(QVariant value READ getValue WRITE setValue NOTIFY valueChanged)
public:
	QmlData(QString name, QVariant value, QObject * parent);
	QmlData * clone(QObject * parent);
	QString getName()const;
	void setName(QString data);
	QVariant getValue()const;
	void setValue(QVariant data);

signals:
	void nameChanged();
	void valueChanged(QVariant data);
protected:
	QVariant mValue;
	QString mName;
};

#endif
