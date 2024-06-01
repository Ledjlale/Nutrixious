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
#include <QSslError>
#include <QNetworkReply>

#include "src/tool/QmlModel.h"



class FoodModel : public QmlModel{
Q_OBJECT
// DB
	DECLARE_GETSET(qint64,foodId,FoodId)
	DECLARE_GETSET(QString,openFoodFactsCode,OpenFoodFactsCode)
	DECLARE_GETSET(QString,brand,Brand)
	DECLARE_GETSET(QString,imageUrl,ImageUrl)
	DECLARE_GETSET(QString,description,Description)
	DECLARE_GETSET(qint64,servingUnitId,ServingUnitId)
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

	Q_INVOKABLE virtual bool save();
	Q_INVOKABLE virtual void remove();
	virtual void updateIsSaved();

	static QList<FoodModel*> buildAll(QObject * parent);
	static FoodModel *build(QSqlQuery &query, QObject * parent);

	Q_INVOKABLE void loadFromOpenFoodFacts(const QString& code);

	void openFoodFactsDownloaded();
	void handleSslErrors (const QList<QSslError> &sslErrors);
	void handleError (QNetworkReply::NetworkError code);

signals:
	void removed(FoodModel *model);

protected:
	qint64 mFoodId = 0;

	QString mBrand = "";
	QString mDescription = "";
	QString mImageUrl;

	QString mOpenFoodFactsCode;
	double mServingSize = 0.0;
	double mServingsPerContainer = 1.0;
	qint64 mServingUnitId = 1;

	double mCalories = -1.0;
	double mTotalFat = -1.0;
	double mSaturatedFat = -1.0;
	double mTransFat = -1.0;
	double mPolyUnsaturatedFat = -1.0;
	double mMonoUnsaturatedFat = -1.0;
	double mCholesterol = -1.0;
	double mSodium = -1.0;
	double mTotalCarbohydrate = -1.0;
	double mDietaryFiber = -1.0;
	double mSugar = -1.0;
	double mProtein = -1.0;
	double mCalcium = -1.0;
	double mIron = -1.0;
	double mPotassium = -1.0;
	double mVitaminA = -1.0;
	double mVitaminC = -1.0;

};

#endif
