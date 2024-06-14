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

#include "UnitProxyModel.h"
#include "UnitListModel.h"

UnitProxyModel::UnitProxyModel(QObject *parent)
	: SortFilterProxyModel{parent}{
	auto list = UnitListModel::getInstance();
	connect(this, &UnitProxyModel::update, list, &UnitListModel::update);
	setSourceModel(list);
}

bool UnitProxyModel::filterAcceptsRow(int sourceRow,
											   const QModelIndex &sourceParent) const {
	QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
	auto model = sourceModel()->data(index0).value<UnitModel*>();
	bool show = true;
	if(model && mFilterType == 0 )
		return show;
	else {
		show = ( (mFilterType & WEIGHT) == WEIGHT) && model->getGramValue() > 0.0
					|| ( (mFilterType & DISTANCE) == DISTANCE) && model->getMeterValue() > 0.0
					|| ( (mFilterType & TIME) == TIME) && model->getSecondValue() > 0.0
					|| ( (mFilterType & ENERGY) == ENERGY) && model->getKcalValue() > 0.0
					|| ( (mFilterType & VOLUME) == VOLUME) && model->getMilliliterValue() > 0.0;

	}
	return show;
}
