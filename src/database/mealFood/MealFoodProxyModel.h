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

#ifndef DATABASE_MEAL_FOOD_PROXY_MODEL_H
#define DATABASE_MEAL_FOOD_PROXY_MODEL_H

#include "src/tool/proxyModel/SortFilterProxyModel.hpp"
#include "MealFoodListModel.h"
class DateModel;


class MealFoodProxyModel: public SortFilterProxyModel {
Q_OBJECT
	Q_PROPERTY(MealFoodListModel *meals READ getMealFoodListModel WRITE setMealFoodListModel NOTIFY mealFoodListModelChanged)
	Q_PROPERTY(qint64 mealGroupId READ getMealGroupId WRITE setMealGroupId NOTIFY mealGroupIdChanged)
public:
	MealFoodProxyModel(QObject * parent = nullptr);

	MealFoodListModel *getMealFoodListModel() const;
	void setMealFoodListModel(MealFoodListModel *data);
	qint64 getMealGroupId() const;
	void setMealGroupId(qint64 data);

	virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

signals:
	void mealFoodListModelChanged();
	void mealGroupIdChanged();
	void update();
	void updateFromDate(DateModel*);

private:
	qint64 mMealGroupId = 0;

};

#endif
