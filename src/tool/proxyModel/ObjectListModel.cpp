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

#include "ObjectListModel.h"
#include <QObject>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>

ObjectListModel::ObjectListModel(QObject *parent)
	: ProxyAbstractListModel<QObject*>{parent}
{
}

ObjectListModel::ObjectListModel(QVariantList data, QObject *parent)
	: ProxyAbstractListModel<QObject*>{parent}
{
	for(auto d : data){
		auto e = d.value<QObject*>();
		connect(e, SIGNAL(removed), this, SLOT(handleRemoved));
		mList << e;
	}
}

ObjectListModel::ObjectListModel(QList<QObject*> data, QObject * parent): ProxyAbstractListModel<QObject*>{parent}{
	mList = data;
	for(auto i : mList){
		connect(i, SIGNAL(removed), this, SLOT(handleRemoved));
	}
}

QHash<int, QByteArray> ObjectListModel::roleNames () const {
	QHash<int, QByteArray> roles;
	roles[Qt::DisplayRole] = "$modelData";
	return roles;
}

QVariant ObjectListModel::data (const QModelIndex &index, int role) const {
	int row = index.row();

	if (!index.isValid() || row < 0 || row >= mList.count())
		return QVariant();
	auto model = mList[row];
	if (role == Qt::DisplayRole) {
		return QVariant::fromValue(model);
	}
	return QVariant();
}

QVariantList ObjectListModel::getData() const{
	QVariantList models;
	for(auto d : mList){
		models << QVariant::fromValue(d);
	}
	return models;
}

void ObjectListModel::handleRemoved(QObject * model){
	auto it = std::find(mList.begin(), mList.end(), model);
	if( it != mList.end()){
		int row = it - mList.begin();
		beginRemoveRows(QModelIndex(), row, row);
		mList.erase(it);
		endRemoveRows();
	}
}
