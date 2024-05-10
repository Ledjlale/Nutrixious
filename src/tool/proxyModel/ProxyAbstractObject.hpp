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

#ifndef PROXY_ABSTRACT_OBJECT_H_
#define PROXY_ABSTRACT_OBJECT_H_

#include <QAbstractListModel>
#include <QDebug>

// Use a regular declaration for Qt signal/slots handling
class ProxyAbstractObject : public QAbstractListModel{
	Q_OBJECT
public:
	Q_PROPERTY(int count READ getCount NOTIFY countChanged)
	Q_PROPERTY(int length READ getCount NOTIFY countChanged)

	ProxyAbstractObject(QObject * parent = nullptr) : QAbstractListModel(parent){
		connect(this, &ProxyAbstractObject::rowsInserted, this, &ProxyAbstractObject::countChanged);
		connect(this, &ProxyAbstractObject::rowsRemoved, this, &ProxyAbstractObject::countChanged);
	}
	Q_INVOKABLE virtual int getCount() const{
		return rowCount();
	}
	Q_INVOKABLE virtual bool remove(QObject *itemToRemove){return false;}
	Q_INVOKABLE virtual void clearData(){}
	Q_INVOKABLE virtual void resetData(){}
	
signals:
	void countChanged();
};

#endif
