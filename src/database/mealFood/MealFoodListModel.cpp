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
#include "src/computation/Computation.h"

MealFoodListModel::MealFoodListModel(QObject *parent)
	: ProxyAbstractListModel<MealFoodModel*>{parent}
{
	connect(this, &MealFoodListModel::caloriesChanged, this, &MealFoodListModel::totalCaloriesChanged);
	connect(this, &MealFoodListModel::currentDateChanged, this, &MealFoodListModel::targetCaloriesChanged);
	connect(this, &MealFoodListModel::caloriesChanged, this, &MealFoodListModel::computeTotalRatios);
	connect(this, &MealFoodListModel::currentDateChanged, this, &MealFoodListModel::computeTotalRatios);
	connect(this, &MealFoodListModel::countChanged, this, &MealFoodListModel::computeTotalRatios);

	connect(this, &MealFoodListModel::caloriesChanged, [](){
		qDebug() << "toto";
	});
	connect(this, &MealFoodListModel::updateTotals, &MealFoodListModel::totalCaloriesChanged);
	connect(this, &MealFoodListModel::updateTotals, &MealFoodListModel::computeTotalRatios);
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
	setCurrentDate(data->getDate());
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
			cal += i->computeNutriment(i->getCalories());
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

void MealFoodListModel::setCurrentDate(QDate data){
	if(mCurrentDate != data){
		mCurrentDate = data;
		emit currentDateChanged();
	}
}


double MealFoodListModel::getTotalCalories()const{
	double sum = 0.0;
	for(auto i : mList){
		if(i->getCalories() > 0) sum += i->computeNutriment(i->getCalories());
	}
	return sum;
}

double MealFoodListModel::getTargetCalories()const{
	return Computation::targetDailyCalories(mCurrentDate);
}

void MealFoodListModel::computeTotalRatios(){
	double carbo  = 0.0;
	double fat = 0.0;
	double protein = 0.0;
	for(auto i : mList){
		if(i->getTotalCarbohydrate() > 0) carbo += i->computeNutriment(i->getTotalCarbohydrate());
		if(i->getTotalFat() > 0)  fat += i->computeNutriment(i->getTotalFat());
		if(i->getProtein() > 0)  protein += i->computeNutriment(i->getProtein());
	}
	double total = carbo + fat + protein;
	if(total > 0){
		setTotalCarboRatio( 100.0 * carbo / total);
		setTotalFatRatio( 100.0 * fat / total);
		setTotalProteinRatio( 100.0 * protein / total);
	}else{
		setTotalCarboRatio(0);
		setTotalFatRatio(0);
		setTotalProteinRatio(0);
	}
}

void MealFoodListModel::setTotalCarboRatio(double data){
	if(mTotalCarboRatio != data){
		mTotalCarboRatio = data;
		totalCarboRatioChanged();
	}
}

void MealFoodListModel::setTotalFatRatio(double data){
	if(mTotalFatRatio != data){
		mTotalFatRatio = data;
		totalFatRatioChanged();
	}
}

void MealFoodListModel::setTotalProteinRatio(double data){
	if(mTotalProteinRatio != data){
		mTotalProteinRatio = data;
		totalProteinRatioChanged();
	}
}


double MealFoodListModel::getTotalCarboRatio()const{
	return mTotalCarboRatio;
}

double MealFoodListModel::getTargetCarboRatio()const{
	return Computation::targetDailyCarbo(mCurrentDate);
}

double MealFoodListModel::getTotalFatRatio()const{
	return mTotalFatRatio;
}

double MealFoodListModel::getTargetFatRatio()const{
	return Computation::targetDailyFat(mCurrentDate);
}

double MealFoodListModel::getTotalProteinRatio()const{
	return mTotalProteinRatio;
}

double MealFoodListModel::getTargetProteinRatio()const{
	return Computation::targetDailyProtein(mCurrentDate);
}
