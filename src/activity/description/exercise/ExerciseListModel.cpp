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

#include "ExerciseListModel.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>

#include "src/activity/description/sport/StrengthModel.h"
#include "src/activity/description/sport/DistanceModel.h"
#include "src/activity/description/sport/StepsModel.h"

using namespace Description;

ExerciseListModel::ExerciseListModel(QObject *parent)
	: ProxyAbstractListModel<ExerciseModel*>{parent}
{
	mList << StrengthModel::load(this);
	mList << DistanceModel::load(this);
	mList << StepsModel::load(this);
}

ExerciseListModel::ExerciseListModel(QVariantList exercises, QObject *parent)
	: ProxyAbstractListModel<ExerciseModel*>{parent}
{
	for(auto exercise : exercises){
		mList << exercise.value<ExerciseModel*>();
	}
}

QHash<int, QByteArray> ExerciseListModel::roleNames () const {
	QHash<int, QByteArray> roles;
	roles[Qt::DisplayRole] = "$modelData";
	roles[Qt::DisplayRole+1] = "displayText";
	return roles;
}

QVariant ExerciseListModel::data (const QModelIndex &index, int role) const {
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



QVariantList ExerciseListModel::getExercises() const{
	QVariantList models;
	for(auto exercise : mList){
		models << QVariant::fromValue(exercise);
	}
	return models;
}

