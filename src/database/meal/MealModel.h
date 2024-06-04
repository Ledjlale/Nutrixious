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

#ifndef DATABASE_MEAL_MODEL_H
#define DATABASE_MEAL_MODEL_H

#include <QObject>
#include <QVector>
#include <QVariantList>
#include <QSqlQuery>
#include <QSslError>
#include <QNetworkReply>

#include "src/tool/QmlModel.h"
#include "src/database/food/FoodModel.h"


class MealModel : public QmlModel{
Q_OBJECT
// DB
	DECLARE_GETSET(qint64,mealId,MealId)
	DECLARE_GETSET(QString,name,Name)

	Q_PROPERTY(double calories READ getCalories NOTIFY caloriesChanged)

public:
	MealModel();	// QML
	MealModel(QObject *parent);

public:

	double getCalories() const;

	Q_INVOKABLE void addFood(FoodModel * data);

	Q_INVOKABLE virtual bool save();
	Q_INVOKABLE virtual void remove();
	virtual void updateIsSaved();

	static QList<MealModel*> buildAll(QObject * parent);
	static MealModel *build(QSqlQuery &query, QObject * parent);


signals:
	void caloriesChanged();
	void removed(MealModel *model);

protected:
	qint64 mMealId = 0;
	QString mName = "";
	QList<FoodModel*> mFoods;

};

#endif
