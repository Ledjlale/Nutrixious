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

#include "ProgramListModel.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>

using namespace Description;

ProgramListModel::ProgramListModel(QObject *parent)
	: ProxyAbstractListModel<ProgramModel*>{parent}
{
	mList << ProgramModel::load();
	//mList << StrengthModel::load();
	//mList << DistanceModel::load();
	//mList << StepsModel::load();
}

QHash<int, QByteArray> ProgramListModel::roleNames () const {
	QHash<int, QByteArray> roles;
	roles[Qt::DisplayRole] = "$modelData";
	roles[Qt::DisplayRole+1] = "displayText";
	return roles;
}

QVariant ProgramListModel::data (const QModelIndex &index, int role) const {
	int row = index.row();

	if (!index.isValid() || row < 0 || row >= mList.count())
		return QVariant();
	auto programModel = mList[row];
	if (role == Qt::DisplayRole) {
		return QVariant::fromValue(programModel);
	}else{
		return programModel->getName();
	}

	return QVariant();
}


