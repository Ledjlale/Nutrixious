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

#include "MealFoodListModel.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>

#include "MealFoodModel.h"

MealFoodListModel::MealFoodListModel(QObject *parent)
	: ProxyAbstractListModel<MealFoodModel*>{parent}
{

}


QHash<int, QByteArray> MealFoodListModel::roleNames () const {
	QHash<int, QByteArray> roles;
	roles[Qt::DisplayRole] = "$modelData";
	roles[Qt::DisplayRole+1] = "displayText";
	roles[Qt::DisplayRole+2] = "meal_group_id";
	return roles;
}

QVariant MealFoodListModel::data (const QModelIndex &index, int role) const {
	int row = index.row();

	if (!index.isValid() || row < 0 || row >= mList.count())
		return QVariant();
	auto model = mList[row];
	if (role == Qt::DisplayRole) {
		return QVariant::fromValue(model);
	}else if (role == Qt::DisplayRole+1) {
		return QVariant(model->getBrand());
	}else if (role == Qt::DisplayRole+2) {
		return QVariant(model->getMealGroupId());
	}

	return QVariant();
}

void MealFoodListModel::update(){
	beginResetModel();
	mList.clear();
	for(auto e : mList) {
		connect(e, &MealFoodModel::removed, this, &MealFoodListModel::handleRemoved);
		connect(e, &MealFoodModel::caloriesChanged, this, &MealFoodListModel::caloriesChanged);
	}
	endResetModel();
}

void MealFoodListModel::updateFromDate(DateModel * data){
	auto models = MealFoodModel::buildAll(data->getDate(), this);
	for(auto e : models) {
		connect(e, &MealFoodModel::removed, this, &MealFoodListModel::handleRemoved);
		connect(e, &MealFoodModel::caloriesChanged, this, &MealFoodListModel::caloriesChanged);
	}
	resetData();
	add(models);
	/*
	beginResetModel();
	mList.clear();
	mList << MealFoodModel::buildAll(data->getDate(), this);
	for(auto e : mList)
		connect(e, &MealFoodModel::removed, this, &MealFoodListModel::handleRemoved);
	endResetModel();
	*/
}

void MealFoodListModel::addFoodModel(FoodModel *model, MealGroupModel *groupModel ) {
	auto newModel = new MealFoodModel(model, groupModel, this);
	connect(newModel, &MealFoodModel::removed, this, &MealFoodListModel::handleRemoved);
	connect(newModel, &MealFoodModel::caloriesChanged, this, &MealFoodListModel::caloriesChanged);
	newModel->save();
	add(newModel);
}

double MealFoodListModel::calories(qint64 mealGroupId){
	double cal = 0.0;
	for(auto i : mList){
		if( i->getMealGroupId() == mealGroupId)
			cal += i->getCalories();
	}
	return cal;
}

void MealFoodListModel::handleRemoved(MealFoodModel * model){
	auto it = std::find(mList.begin(), mList.end(), model);
	if( it != mList.end()){
		int row = it - mList.begin();
		beginRemoveRows(QModelIndex(), row, row);
		mList.erase(it);
		endRemoveRows();
	}
}
