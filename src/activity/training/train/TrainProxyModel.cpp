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

#include "TrainProxyModel.h"
#include "TrainListModel.h"

using namespace Training;

TrainProxyModel::TrainProxyModel(QObject *parent)
	: SortFilterProxyModel{parent}
{
	setSourceModel(new TrainListModel());
	sort(0);
}

void TrainProxyModel::update(){
	sourceModel()->deleteLater();
	setSourceModel(new TrainListModel());
	sort(0);
}


bool TrainProxyModel::lessThan (const QModelIndex &left, const QModelIndex &right) const {
		if( !sourceModel())
			return false;
		auto a = sourceModel()->data(left).value<TrainModel*>();
		auto b = sourceModel()->data(right).value<TrainModel*>();
		return a->getStartDateTime() > b->getStartDateTime();
	}

