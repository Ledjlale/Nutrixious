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

#include "MealFoodProxyModel.h"

MealFoodProxyModel::MealFoodProxyModel(QObject *parent)
	: SortFilterProxyModel{parent} {
	auto model = new MealFoodListModel(this);
	connect(this, &MealFoodProxyModel::updateFromDate, model, &MealFoodListModel::updateFromDate);
	setSourceModel(model);
}

MealFoodListModel *MealFoodProxyModel::getMealFoodListModel() const{
	auto model = sourceModel();
	return model ? dynamic_cast<MealFoodListModel*>(model) : nullptr;
}

void MealFoodProxyModel::setMealFoodListModel(MealFoodListModel *data){
	setSourceModel(data);
	emit mealFoodListModelChanged();
}

qint64 MealFoodProxyModel::getMealGroupId() const{
	return mMealGroupId;
}

void MealFoodProxyModel::setMealGroupId(qint64 data){
	if( mMealGroupId != data){
		mMealGroupId = data;
		mealGroupIdChanged();
	}
}

bool MealFoodProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const{
	if(mMealGroupId == 0) return true;
	else{
		QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
		auto model = sourceModel()->data(index0).value<MealFoodModel*>();
		return model->getMealGroupId() == mMealGroupId;
	}
}
