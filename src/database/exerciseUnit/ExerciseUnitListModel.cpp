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

#include "ExerciseUnitListModel.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>

#include "ExerciseUnitModel.h"

ExerciseUnitListModel::ExerciseUnitListModel(QObject *parent)
	: ProxyAbstractListModel<ExerciseUnitModel*>{parent}
{
	for(auto e : mList)
		connect(e, &ExerciseUnitModel::removed, this, &ExerciseUnitListModel::handleRemoved);
}

ExerciseUnitListModel::ExerciseUnitListModel(QVariantList exercises, QObject *parent)
	: ProxyAbstractListModel<ExerciseUnitModel*>{parent}
{
	for(auto exercise : exercises){
		auto e = exercise.value<ExerciseUnitModel*>();
		connect(e, &ExerciseUnitModel::removed, this, &ExerciseUnitListModel::handleRemoved);
		mList << e;
	}
}

QHash<int, QByteArray> ExerciseUnitListModel::roleNames () const {
	QHash<int, QByteArray> roles;
	roles[Qt::DisplayRole] = "$modelData";
	roles[Qt::DisplayRole+1] = "displayText";
	return roles;
}

QVariant ExerciseUnitListModel::data (const QModelIndex &index, int role) const {
	int row = index.row();

	if (!index.isValid() || row < 0 || row >= mList.count())
		return QVariant();
	auto model = mList[row];
	if (role == Qt::DisplayRole) {
		return QVariant::fromValue(model);
	}else{
		return model->getExerciseModel() ? model->getExerciseModel()->getName() : "";
	}

	return QVariant();
}



QVariantList ExerciseUnitListModel::getExercises() const{
	QVariantList models;
	for(auto exercise : mList){
		models << QVariant::fromValue(exercise);
	}
	return models;
}


void ExerciseUnitListModel::handleRemoved(ExerciseUnitModel * model){
	auto it = std::find(mList.begin(), mList.end(), model);
	if( it != mList.end()){
		int row = it - mList.begin();
		beginRemoveRows(QModelIndex(), row, row);
		mList.erase(it);
		endRemoveRows();
	}
}
