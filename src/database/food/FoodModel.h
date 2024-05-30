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

#ifndef DATABASE_FOOD_MODEL_H
#define DATABASE_FOOD_MODEL_H

#include <QObject>
#include <QVector>
#include <QVariantList>
#include <QSqlQuery>

#include "src/tool/QmlModel.h"

#define DECLARE_GETSET(type, x, X) public:\
	Q_PROPERTY(type x MEMBER m##X WRITE set##X NOTIFY x##Changed)\
	type get##X() const; \
	void set##X(type data);\
	Q_SIGNAL void x##Changed();

class FoodModel : public QmlModel{
Q_OBJECT
// DB
	DECLARE_GETSET(qint64,foodId,FoodId)
	DECLARE_GETSET(QString,openFoodFactsCode,OpenFoodFactsCode)
	DECLARE_GETSET(QString,brand,Brand)
	DECLARE_GETSET(QString,imageUrl,ImageUrl)
	DECLARE_GETSET(QString,description,Description)
	DECLARE_GETSET(QString,servingUnit,ServingUnit)
	DECLARE_GETSET(double,servingSize,ServingSize)
	DECLARE_GETSET(double,servingsPerContainer,ServingsPerContainer)
	DECLARE_GETSET(double,calories,Calories)
	DECLARE_GETSET(double,totalFat,TotalFat)
	DECLARE_GETSET(double,saturatedFat,SaturatedFat)
	DECLARE_GETSET(double,transFat,TransFat)
	DECLARE_GETSET(double,polyUnsaturatedFat,PolyUnsaturatedFat)
	DECLARE_GETSET(double,monoUnsaturatedFat,MonoUnsaturatedFat)
	DECLARE_GETSET(double,cholesterol,Cholesterol)
	DECLARE_GETSET(double,sodium,Sodium)
	DECLARE_GETSET(double,totalCarbohydrate,TotalCarbohydrate)
	DECLARE_GETSET(double,dietaryFiber,DietaryFiber)
	DECLARE_GETSET(double,sugar,Sugar)
	DECLARE_GETSET(double,protein,Protein)
	DECLARE_GETSET(double,calcium,Calcium)
	DECLARE_GETSET(double,iron,Iron)
	DECLARE_GETSET(double,potassium,Potassium)
	DECLARE_GETSET(double,vitaminA,VitaminA)
	DECLARE_GETSET(double,vitaminC,VitaminC)

public:
	FoodModel();	// QML
	FoodModel(QObject *parent);
	FoodModel(FoodModel * model, QObject *parent);
	FoodModel* clone(QObject*parent);

public:

	//Q_INVOKABLE virtual bool save();
	//Q_INVOKABLE virtual void remove();

	static QList<FoodModel*> buildAll(QObject * parent);
	static FoodModel *build(QSqlQuery &query, QObject * parent);

signals:
	void removed(FoodModel *model);

protected:
	qint64 mFoodId = 0;

	QString mBrand = "CustomFood";
	QString mDescription = "Desc";
	QString mImageUrl;

	QString mOpenFoodFactsCode;
	double mServingSize = 0.0;
	double mServingsPerContainer = 0.0;
	QString mServingUnit = "g";

	double mCalories = 0.0;
	double mTotalFat = 0.0;
	double mSaturatedFat = 0.0;
	double mTransFat = 0.0;
	double mPolyUnsaturatedFat = 0.0;
	double mMonoUnsaturatedFat = 0.0;
	double mCholesterol = 0.0;
	double mSodium = 0.0;
	double mTotalCarbohydrate = 0.0;
	double mDietaryFiber = 0.0;
	double mSugar = 0.0;
	double mProtein = 0.0;
	double mCalcium = 0.0;
	double mIron = 0.0;
	double mPotassium = 0.0;
	double mVitaminA = 0.0;
	double mVitaminC = 0.0;

};

#endif
