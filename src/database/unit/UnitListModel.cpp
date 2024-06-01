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

#include "UnitListModel.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>

#include "UnitModel.h"

UnitListModel::UnitListModel(QObject *parent)
	: ProxyAbstractListModel<UnitModel*>{parent}
{}


QHash<int, QByteArray> UnitListModel::roleNames () const {
	QHash<int, QByteArray> roles;
	roles[Qt::DisplayRole] = "$modelData";
	roles[Qt::DisplayRole+1] = "displayText";
	roles[Qt::DisplayRole+2] = "id";
	return roles;
}

QVariant UnitListModel::data (const QModelIndex &index, int role) const {
	int row = index.row();

	if (!index.isValid() || row < 0 || row >= mList.count())
		return QVariant();
	auto model = mList[row];
	if (role == Qt::DisplayRole) {
		return QVariant::fromValue(model);
	}else if(role == Qt::DisplayRole +1){
		return model->getName();
	}else
		return model->getUnitId();

	return QVariant();
}

void UnitListModel::update(){
	beginResetModel();
	mList.clear();
	mList = UnitModel::buildAll(this);
	endResetModel();
}
