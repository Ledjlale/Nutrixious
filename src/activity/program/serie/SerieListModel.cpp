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

#include "SerieListModel.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>


using namespace Base;

SerieListModel::SerieListModel(QObject *parent)
	: ProxyAbstractListModel<SerieModel*>{parent}
{
	//mList << StrengthModel::load(this);
	//mList << DistanceModel::load(this);
	//mList << StepsModel::load(this);
	//for(auto e : mList)
//		connect(e, &ExerciseModel::removed, this, &SerieListModel::handleRemoved);
}

SerieListModel::SerieListModel(QVariantList exercises, QObject *parent)
	: ProxyAbstractListModel<SerieModel*>{parent}
{
//	for(auto exercise : exercises){
//		auto e = exercise.value<ExerciseModel*>();
//		connect(e, &ExerciseModel::removed, this, &SerieListModel::handleRemoved);
//		mList << e;
//	}
}

QHash<int, QByteArray> SerieListModel::roleNames () const {
	QHash<int, QByteArray> roles;
	roles[Qt::DisplayRole] = "$modelData";
	roles[Qt::DisplayRole+1] = "displayText";
	return roles;
}

QVariant SerieListModel::data (const QModelIndex &index, int role) const {
	int row = index.row();

	if (!index.isValid() || row < 0 || row >= mList.count())
		return QVariant();
	auto model = mList[row];
	if (role == Qt::DisplayRole) {
		return QVariant::fromValue(model);
	}else{
		return model->getName();
	}

	return QVariant();
}



QVariantList SerieListModel::getSeries() const{
	QVariantList models;
	for(auto item : mList){
		models << QVariant::fromValue(item);
	}
	return models;
}


void SerieListModel::handleRemoved(SerieModel * model){
	auto it = std::find(mList.begin(), mList.end(), model);
	if( it != mList.end()){
		int row = it - mList.begin();
		beginRemoveRows(QModelIndex(), row, row);
		mList.erase(it);
		endRemoveRows();
	}
}
