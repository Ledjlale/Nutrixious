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

#include "MealFoodModel.h"
#include "src/tool/Utils.h"
#include <QVariantList>
#include <QSqlRecord>
#include <QSqlError>
#include <QEvent>
//#include <QDynamicPropertyChange>

#include <QQmlApplicationEngine>
extern QQmlApplicationEngine * gEngine;


MealFoodModel::MealFoodModel()
	: MealFoodModel{nullptr} {
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}

MealFoodModel::MealFoodModel(QObject *parent)
	: FoodModel{parent} {
	mTablePrefix = "meal_food";
}

MealFoodModel::MealFoodModel(const FoodModel * model, MealGroupModel *groupModel, QObject *parent) : FoodModel(model, parent){
	mTablePrefix = "meal_food";
	mId = 0;
	mConsumptionDateTime = QDateTime(QDate::currentDate(), groupModel->getDefaultTime());
	mMealGroupId= groupModel->getMealGroupId();
}

MealFoodModel::MealFoodModel(const MealFoodModel * model, QObject *parent) : FoodModel(model, parent){
	mTablePrefix = "meal_food";
	mId = 0;
	mConsumptionDateTime = initiBackup(model, &model->mConsumptionDateTime, model->mConsumptionDateTime, &mConsumptionDateTime).toDateTime();
	mMealGroupId= initiBackup(model, &model->mMealGroupId, model->mMealGroupId, &mMealGroupId).toLongLong();
}


MealFoodModel::~MealFoodModel(){
}

MealFoodModel * MealFoodModel::clone(QObject *parent)const{
	MealFoodModel *model = new MealFoodModel(this, parent);

	model->clearBackupValues();
	return model;
}

//-------------------------------------------------------------------------------------------------------------------

DEFINE_GETSET(MealFoodModel,qint64,mealGroupId,MealGroupId)
DEFINE_GETSET(MealFoodModel,QDateTime,consumptionDateTime,ConsumptionDateTime)

//-------------------------------------------------------------------------------------------------------------------
void MealFoodModel::undo(){
	DEFINE_UNDO_LONGLONG(MealGroupId)
	DEFINE_UNDO_DATETIME(ConsumptionDateTime)
	FoodModel::undo();
}
//-------------------------------------------------------------------------------------------------------------------

void MealFoodModel::addQueryValues(DatabaseQuery &query){
	qDebug() << mConsumptionDateTime.toMSecsSinceEpoch();
	query.add("consumption_date_time", mConsumptionDateTime.toMSecsSinceEpoch());
	query.add("meal_group_id", mMealGroupId);
}

void MealFoodModel::load(QSqlQuery &query) {
// TODO optimize
	FoodModel::load(query);
	auto consumptionField = query.record().indexOf("consumption_date_time");
	auto mealGroupIdField = query.record().indexOf("meal_group_id");
	setConsumptionDateTime(QDateTime::fromMSecsSinceEpoch(query.value(consumptionField).toLongLong()));
	setMealGroupId(query.value(mealGroupIdField).toLongLong());
	clearBackupValues();
}

MealFoodModel *MealFoodModel::build(QSqlQuery &query, QObject * parent) {
	MealFoodModel * model = new MealFoodModel(parent);
	model->load(query);
	return model;
}


QList<MealFoodModel*> MealFoodModel::buildAll(QDate date, QObject * parent){
	QList<MealFoodModel*> models;
	QSqlQuery query;
	QDateTime day(date, QTime(0,0,0));
	query.prepare("SELECT * FROM meal_foods WHERE consumption_date_time BETWEEN ? AND ? ORDER BY consumption_date_time ASC, meal_food_id ASC");
	query.addBindValue(day.toMSecsSinceEpoch());
	query.addBindValue(day.addDays(1).toMSecsSinceEpoch()-1);

	QString str = query.executedQuery();
	const QVariantList list = query.boundValues();
	for (qsizetype i = list.size() - 1 ; i >= 0; --i){
		str.replace(":"+QString::number(i),list.at(i).toString());
	}

	qDebug() << str;
	if(!query.exec( )) qCritical() << "Cannot select meal foods  : "  << query.lastError().text();
	while (query.next()) {
		auto model = build(query, parent);
		models << model;
	}

	return models;
}
