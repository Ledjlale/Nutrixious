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

#include "FoodProxyModel.h"
#include "FoodListModel.h"

FoodProxyModel::FoodProxyModel(QObject *parent)
	: SortFilterProxyModel{parent}
{
	auto list = new FoodListModel( this);
	connect(this, &FoodProxyModel::update, list, &FoodListModel::update);
	connect(this,&FoodProxyModel::nameChanged,this, &FoodProxyModel::invalidateFilter);
	setSourceModel(list);
}

DEFINE_SIMPLE_GETSET(FoodProxyModel,QString,name,Name)


bool FoodProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const{
	if(mName == "") return true;
	else{
		QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
		auto model = sourceModel()->data(index0).value<FoodModel*>();
		return model->getBrand().contains(mName) || model->getDescription().contains(mName);
	}
}
