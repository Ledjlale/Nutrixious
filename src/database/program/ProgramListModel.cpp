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


ProgramListModel::ProgramListModel(QObject *parent)
	: ProxyAbstractListModel<ProgramModel*>{parent}
{
	mList << ProgramModel::buildAll(parent);
	//mList << StrengthModel::load();
	//mList << DistanceModel::load();
	//mList << StepsModel::load();
	for(auto p : mList)
		connect(p, &ProgramModel::removed, this, &ProgramListModel::handleRemoved);
}

QVariant ProgramListModel::getMainProgram() const{
	for(auto i : mList){
		if(i->getIsMain()) return QVariant::fromValue(i);
	}
	return QVariant();
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
	auto model = mList[row];
	if (role == Qt::DisplayRole) {
		return QVariant::fromValue(model);
	}else{
		return model->getName() + (model->getDescription().isEmpty() ? "" : "-" +model->getDescription());
	}

	return QVariant();
}

void ProgramListModel::create(){
	beginInsertRows(QModelIndex(), mList.size(),mList.size());
	auto model = new ProgramModel(this);
	connect(model, &ProgramModel::removed, this, &ProgramListModel::handleRemoved);
	mList.push_back(model);
	endInsertRows();
}

void ProgramListModel::handleRemoved(){
	auto model = dynamic_cast<ProgramModel*>(sender());
	auto it = std::find(mList.begin(), mList.end(), model);
	if( it != mList.end()){
		int row = it - mList.begin();
		beginRemoveRows(QModelIndex(), row, row);
		mList.erase(it);
		endRemoveRows();
	}
}

