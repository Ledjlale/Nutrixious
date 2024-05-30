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

#include "FoodModel.h"

#include <QQmlApplicationEngine>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include "src/database/DatabaseQuery.h"

#include "src/tool/Utils.h"


extern QQmlApplicationEngine * gEngine;

FoodModel::FoodModel() : FoodModel(nullptr){	// QML
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}
FoodModel::FoodModel(QObject *parent)
	: QmlModel{parent}
{
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE

}

FoodModel::FoodModel(FoodModel * model, QObject *parent)
	: QmlModel{parent}
{
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE

}

FoodModel* FoodModel::clone(QObject*parent) {
	return new FoodModel(this, parent);

}
#define DEFINE_GETSET(Class, type, x, X) type Class::get##X() const{ \
	return m##X;\
	}\
	void Class::set##X(type data){\
		if(m##X!= data){\
			m##X = data;\
			emit x##Changed();\
		}\
	}

DEFINE_GETSET(FoodModel,qint64,foodId,FoodId)
DEFINE_GETSET(FoodModel,QString,openFoodFactsCode,OpenFoodFactsCode)
DEFINE_GETSET(FoodModel,QString,brand,Brand)
DEFINE_GETSET(FoodModel,QString,imageUrl,ImageUrl)
DEFINE_GETSET(FoodModel,QString,description,Description)
DEFINE_GETSET(FoodModel,QString,servingUnit,ServingUnit)
DEFINE_GETSET(FoodModel,double,servingSize,ServingSize)
DEFINE_GETSET(FoodModel,double,servingsPerContainer,ServingsPerContainer)
DEFINE_GETSET(FoodModel,double,calories,Calories)
DEFINE_GETSET(FoodModel,double,totalFat,TotalFat)
DEFINE_GETSET(FoodModel,double,saturatedFat,SaturatedFat)
DEFINE_GETSET(FoodModel,double,transFat,TransFat)
DEFINE_GETSET(FoodModel,double,polyUnsaturatedFat,PolyUnsaturatedFat)
DEFINE_GETSET(FoodModel,double,monoUnsaturatedFat,MonoUnsaturatedFat)
DEFINE_GETSET(FoodModel,double,cholesterol,Cholesterol)
DEFINE_GETSET(FoodModel,double,sodium,Sodium)
DEFINE_GETSET(FoodModel,double,totalCarbohydrate,TotalCarbohydrate)
DEFINE_GETSET(FoodModel,double,dietaryFiber,DietaryFiber)
DEFINE_GETSET(FoodModel,double,sugar,Sugar)
DEFINE_GETSET(FoodModel,double,protein,Protein)
DEFINE_GETSET(FoodModel,double,calcium,Calcium)
DEFINE_GETSET(FoodModel,double,iron,Iron)
DEFINE_GETSET(FoodModel,double,potassium,Potassium)
DEFINE_GETSET(FoodModel,double,vitaminA,VitaminA)
DEFINE_GETSET(FoodModel,double,vitaminC,VitaminC)

//-------------------------------------------------------------------------------------------------------------------
/*

bool FoodModel::save(){
	if(mExerciseId>0 && !mIsEdited) return true;// Avoid update for nothing
	qDebug() << "Saving exercise " << mName << mDescription;
	DatabaseQuery query;

	query.begin(mExerciseId == 0 ? DatabaseQuery::Insert : DatabaseQuery::Update, "exercises" );

	query.add("name", mName);
	query.add("description", mDescription);
	query.addConditionnal("exercise_id",mExerciseId);
	if(mExerciseId == 0){
		if(!query.exec()) qCritical() << "Cannot save exercise: "  << query.mQuery.lastError().text();
		auto fieldNo = query.mQuery.record().indexOf("exercise_id");
		while (query.mQuery.next()) {
			setExerciseId(query.mQuery.value(fieldNo).toInt());
			qDebug() << "Insert exercise: " << mExerciseId;
		}
	}else{
		if(!query.exec()) qCritical() << "Cannot update exercise: "  << query.mQuery.lastError().text();
		else {
			qDebug() << "Update exercise: " << mExerciseId;
		}
	}
	return true;
}

void FoodModel::remove(){
	if(mExerciseId > 0){
		DatabaseQuery query;
		query.begin(DatabaseQuery::Delete, "exercises");
		query.addConditionnal("exercise_id",mExerciseId);
		if(!query.exec()){
			if(!query.exec()) qCritical() << "Cannot delete exercise  : "  << query.mQuery.lastError().text();
		}
	}
	emit removed(this);
}

FoodModel *FoodModel::build(QSqlQuery &query, QObject * parent) {
	auto idField = query.record().indexOf("exercises.exercise_id");
	auto nameField = query.record().indexOf("exercises.name");
	auto descriptionField = query.record().indexOf("exercises.description");
	if( idField>= 0 && nameField >= 0 && descriptionField >= 0){
		FoodModel * model = new FoodModel(parent);
	// TODO optimize

		model->setExerciseId(query.value(idField).toInt());
		model->setName(query.value(nameField).toString());
		model->setDescription(query.value(descriptionField).toString());
		return model;
	}else return nullptr;
}

QList<FoodModel*> FoodModel::buildAll(QObject * parent){
	QList<FoodModel*> models;
	QSqlQuery query;
	if(!query.exec( "SELECT * FROM exercises ORDER BY exercise_id DESC")) qCritical() << "Cannot select all exercises  : "  << query.lastError().text();

	while (query.next()) {
		auto model = build(query, parent);
		models << model;
	}

	return models;
}
*/
