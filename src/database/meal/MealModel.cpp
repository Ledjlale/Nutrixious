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

#include "MealModel.h"

#include <QQmlApplicationEngine>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlField>
#include <QJsonDocument>
#include "src/database/DatabaseQuery.h"

#include "src/tool/Utils.h"


extern QQmlApplicationEngine * gEngine;

MealModel::MealModel() : MealModel(nullptr){	// QML
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}

MealModel::MealModel(QObject *parent)
	: QmlModel{parent}
{
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	connect(this, &MealModel::mealIdChanged, this, &MealModel::updateIsSaved);
}

DEFINE_GETSET(MealModel,qint64,mealId,MealId)
DEFINE_GETSET(MealModel,QString,name,Name)

double MealModel::getCalories() const{

}

void MealModel::addFood(FoodModel * data){

}

//-------------------------------------------------------------------------------------------------------------------

void MealModel::updateIsSaved(){
	setIsSaved(getMealId() > 0);
}

int MealModel::save(){
	if(mMealId>0 && !mIsEdited) return true;// Avoid update for nothing
	qDebug() << "Saving meal " << mName;
	DatabaseQuery query;

	query.begin(mMealId == 0 ? DatabaseQuery::Insert : DatabaseQuery::Update, "meals" );

	query.add("name", mName);
	query.addConditionnal("meal_id",mMealId);
	if(mMealId == 0){
		if(!query.exec()){
			qCritical() << "Cannot save meal: "  << query.mQuery.lastError().text();
			return false;
		}
		auto fieldNo = query.mQuery.record().indexOf("meal_id");
		while (query.mQuery.next()) {
			setMealId(query.mQuery.value(fieldNo).toInt());
			qDebug() << "Insert food: " << mMealId;
		}
	}else{
		if(!query.exec()) {
			qCritical() << "Cannot update meal: "  << query.mQuery.lastError().text();
			return false;
		}else {
			qDebug() << "Update meal: " << mMealId;
		}
	}
	clearBackupValues();
	return true;
}

void MealModel::remove(){
	if(mMealId > 0){
		DatabaseQuery query;
		query.begin(DatabaseQuery::Delete, "meals");
		query.addConditionnal("meal_id",mMealId);
		if(!query.exec()){
			if(!query.exec()) qCritical() << "Cannot delete meal  : "  << query.mQuery.lastError().text();
		}
	}
	emit removed(this);
}

MealModel *MealModel::build(QSqlQuery &query, QObject * parent) {
	auto idField = query.record().indexOf("foods.food_id");

	if( idField>= 0){
		MealModel * model = new MealModel(parent);
	// TODO optimize
		for(int i = 0 ; i < query.record().count() ; ++i){
			QString fieldName = query.record().field(i).name();
			if(fieldName == "meal_id") model->setMealId(query.value(i).toInt());
			else if(fieldName == "name") model->setName(query.value(i).toString());
		}
		return model;
	}else return nullptr;
}

QList<MealModel*> MealModel::buildAll(QObject * parent){
	QList<MealModel*> models;
	QSqlQuery query;
	if(!query.exec( "SELECT * FROM meals ORDER BY meal_id DESC")) qCritical() << "Cannot select all meals  : "  << query.lastError().text();

	while (query.next()) {
		auto model = build(query, parent);
		models << model;
	}

	return models;
}


