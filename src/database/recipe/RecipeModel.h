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

#ifndef DATABASE_RECIPE_MODEL_H
#define DATABASE_RECIPE_MODEL_H

#include <QObject>
#include <QVector>
#include <QVariantList>
#include <QSqlQuery>
#include <QSslError>
#include <QNetworkReply>

#include "src/tool/QmlModel.h"
#include "src/database/DatabaseQuery.h"
#include "src/database/unit/UnitListModel.h"
#include "src/database/recipeIngredient/RecipeIngredientModel.h"

class RecipeModel : public QmlModel{
Q_OBJECT
// DB
	DECLARE_GETSET(qint64,id,Id)
	DECLARE_GETSET(QString,brand,Brand)
	DECLARE_GETSET(QString,description,Description)
	DECLARE_GETSET(qint64,recipeUnitId,RecipeUnitId)
	DECLARE_GETSET(double,recipeSize,RecipeSize)
public:
	RecipeModel();	// QML
	RecipeModel(QObject *parent);
	RecipeModel(const RecipeModel * model, QObject *parent);
	RecipeModel* clone(QObject*parent);

	void initRandomValues();
	
	Q_INVOKABLE void addIngredient(RecipeIngredientModel * data);
	Q_INVOKABLE void removeIngredient(RecipeIngredientModel * data);
	
	virtual void addQueryValues(DatabaseQuery &query){}
	Q_INVOKABLE virtual int save();
	
	virtual void saveValues(DatabaseQuery &query);
	Q_INVOKABLE virtual void remove();
	virtual void updateIsSaved();
	Q_INVOKABLE virtual void undo();

	static QList<RecipeModel*> buildAll(QObject * parent);

	static RecipeModel *build(QSqlQuery &query, QObject * parent);
	void load(QSqlQuery &query);


signals:
	void removed(RecipeModel *model);
	void saved();
	void ingredientsAdded(RecipeIngredientModel*data);
	void ingredientsRemoved(RecipeIngredientModel*data);

protected:
	qint64 mId = 0;
	QString mBrand;
	QString mDescription;
	double mRecipeSize = 100.0;
	qint64 mRecipeUnitId = 1;
	
	QList<RecipeIngredientModel*> mIngredients, mRemovedIngredients;
	
// Internal
	QString mTablePrefix = "";

};

#endif
