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

#ifndef DATABASE_RECIPE_INGREDIENT_MODEL_H
#define DATABASE_RECIPE_INGREDIENT_MODEL_H

#include <QObject>
#include <QVector>
#include <QSqlQuery>

#include "src/tool/QmlModel.h"
#include "src/database/DatabaseQuery.h"

#include "src/database/food/FoodModel.h"
#include "src/tool/date/DateModel.h"

class RecipeIngredientModel : public QmlModel{
Q_OBJECT
	DECLARE_GETSET(qint64,id,Id)
	DECLARE_GETSET(qint64,recipeId,RecipeId)
	DECLARE_GETSET(qint64,foodId,FoodId)
	DECLARE_GETSET(qint64,ingredientUnitId,IngredientUnitId)
	DECLARE_GETSET(double,ingredientSize,IngredientSize)
	
	DECLARE_GETSET(FoodModel*,foodModel,FoodModel)
public:
	RecipeIngredientModel();	// QML
	RecipeIngredientModel(QObject *parent);
	RecipeIngredientModel(const RecipeIngredientModel * model, QObject *parent);
	RecipeIngredientModel* clone(QObject*parent);

	void initRandomValues();
	
	virtual void addQueryValues(DatabaseQuery &query){}
	Q_INVOKABLE virtual int save();
	
	virtual void saveValues(DatabaseQuery &query);
	Q_INVOKABLE virtual void remove();
	virtual void updateIsSaved();
	Q_INVOKABLE virtual void undo();

	static QList<RecipeIngredientModel*> buildAll(QObject * parent);
	static QList<RecipeIngredientModel*> build(qint64 recipeId, bool withFoodModel, QObject * parent);

	static RecipeIngredientModel *build(QSqlQuery &query, QObject * parent);
	void load(QSqlQuery &query);

signals:
	void removed(RecipeIngredientModel *model);
	void saved();

protected:
protected:
	qint64 mId = -1;
	qint64 mRecipeId = -1;
	qint64 mFoodId = -1;
	double mIngredientSize = 100.0;
	qint64 mIngredientUnitId = 1;
	FoodModel *mFoodModel = nullptr;
// Internal
	QString mTablePrefix = "";
};


#endif
