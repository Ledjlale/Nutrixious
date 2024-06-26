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

#include "MealGroupProxyModel.h"
#include "MealGroupListModel.h"

MealGroupProxyModel::MealGroupProxyModel(QObject *parent)
	: SortFilterProxyModel{parent} {
	auto model = new MealGroupListModel(this);
	setSourceModel(model);
	connect(this, &MealGroupProxyModel::update, model, &MealGroupListModel::update);
	connect(this, &MealGroupProxyModel::addNewMealGroup, model, &MealGroupListModel::addNewMealGroup);
	connect(this, &MealGroupProxyModel::showAllChanged, this, &MealGroupProxyModel::invalidateFilter);
}

QString MealGroupProxyModel::getNameFromId(int id) const{
	auto model = dynamic_cast<MealGroupListModel*>(sourceModel())->getModelFromId(id);
	if(model){
		return model->getName();
	}else return "";
}


bool MealGroupProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const{
	if(mShowAll) return true;
	else{
		QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
		auto model = sourceModel()->data(index0).value<MealGroupModel*>();
		return model->getIsDisplayed();
	}
}

