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

#ifndef DATABASE_MEAL_FOOD_MODEL_H
#define DATABASE_MEAL_FOOD_MODEL_H

#include <QObject>
#include <QVector>
#include <QSqlQuery>

#include "src/tool/QmlModel.h"
#include "src/database/DatabaseQuery.h"

#include "src/database/food/FoodModel.h"
#include "src/database/mealGroup/MealGroupModel.h"
#include "src/tool/date/DateModel.h"
class MealFoodModel : public FoodModel{
Q_OBJECT
	DECLARE_GETSET(qint64,mealGroupId,MealGroupId)
	DECLARE_GETSET(QDateTime,consumptionDateTime,ConsumptionDateTime)

public:
	MealFoodModel();
	explicit MealFoodModel(QObject *parent);
	MealFoodModel(const FoodModel * model, MealGroupModel *groupModel, QObject *parent);
	MealFoodModel(const MealFoodModel * model, QObject *parent);
	virtual ~MealFoodModel();
	virtual MealFoodModel * clone(QObject *parent)const;

// ----------------------
	Q_INVOKABLE virtual void undo();
	virtual void addQueryValues(DatabaseQuery &query);
	virtual void load(QSqlQuery &query);
	static MealFoodModel *build(QSqlQuery &query, QObject * parent);
	static QList<MealFoodModel*> buildAll(QDate date, QObject * parent);

protected:
	qint64 mMealGroupId = 0;
	QDateTime mConsumptionDateTime;

};


#endif
