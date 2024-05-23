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

#include "TrainingProxyModel.h"
#include "TrainingListModel.h"


TrainingProxyModel::TrainingProxyModel(QObject *parent)
	: SortFilterProxyModel{parent}
{
	setSourceModel(new TrainingListModel());
	sort(0);
}

void TrainingProxyModel::update(){
	sourceModel()->deleteLater();
	setSourceModel(new TrainingListModel());
	sort(0);
}

bool TrainingProxyModel::lessThan (const QModelIndex &left, const QModelIndex &right) const{
	auto l = sourceModel()->data(left);
	auto r = sourceModel()->data(right);
	return l.value<TrainingModel*>()->getStartDateTime() > r.value<TrainingModel*>()->getStartDateTime();
}
