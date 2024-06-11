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

#ifndef DATABASE_MEAL_FOOD_LIST_MODEL_H
#define DATABASE_MEAL_FOOD_LIST_MODEL_H

#include "src/tool/proxyModel/ProxyAbstractListModel.hpp"
#include "MealFoodModel.h"
#include "src/database/mealGroup/MealGroupModel.h"
#include "src/tool/date/DateModel.h"

class MealFoodListModel: public ProxyAbstractListModel<MealFoodModel*> {
Q_OBJECT
	Q_PROPERTY(double totalCalories READ getTotalCalories NOTIFY totalCaloriesChanged)
	Q_PROPERTY(double targetCalories READ getTargetCalories NOTIFY targetCaloriesChanged)
	Q_PROPERTY(double totalCarboRatio READ getTotalCarboRatio NOTIFY totalCarboRatioChanged)
	Q_PROPERTY(double targetCarboRatio READ getTargetCarboRatio NOTIFY targetCarboRatioChanged)
	Q_PROPERTY(double totalFatRatio READ getTotalFatRatio NOTIFY totalFatRatioChanged)
	Q_PROPERTY(double targetFatRatio READ getTargetFatRatio NOTIFY targetFatRatioChanged)
	Q_PROPERTY(double totalProteinRatio READ getTotalProteinRatio NOTIFY totalProteinRatioChanged)
	Q_PROPERTY(double targetProteinRatio READ getTargetProteinRatio NOTIFY targetProteinRatioChanged)

public:
	MealFoodListModel(QObject * parent = nullptr);

	virtual QHash<int, QByteArray> roleNames () const override;
	QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;

	Q_INVOKABLE void update();
	Q_INVOKABLE void updateFromDate(DateModel * data);
	Q_INVOKABLE void addFoodModel(FoodModel *model, MealGroupModel *groupModel );
	Q_INVOKABLE double calories(qint64 mealGroupId);

	void handleRemoved(MealFoodModel *model);

	void setCurrentDate(QDate data);
	void computeTotalRatios();

	double getTotalCalories()const;
	double getTargetCalories()const;
	double getTotalCarboRatio()const;
	double getTargetCarboRatio()const;
	double getTotalFatRatio()const;
	double getTargetFatRatio()const;
	double getTotalProteinRatio()const;
	double getTargetProteinRatio()const;

	void setTotalCarboRatio(double data);
	void setTotalFatRatio(double data);
	void setTotalProteinRatio(double data);


signals:
	void caloriesChanged();
	void currentDateChanged();
	void totalCaloriesChanged();
	void targetCaloriesChanged();
	void totalCarboRatioChanged();
	void targetCarboRatioChanged();
	void totalFatRatioChanged();
	void targetFatRatioChanged();
	void totalProteinRatioChanged();
	void targetProteinRatioChanged();

protected:
	QDate mCurrentDate;

	double mTotalCarboRatio = 0.0;
	double mTotalFatRatio = 0.0;
	double mTotalProteinRatio = 0.0;

};

#endif
