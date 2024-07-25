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

#include "RecipeIngredientModel.h"

#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QQmlApplicationEngine>
#include <QSqlError>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSslSocket>
#include "qstandardpaths.h"
#include "src/database/DatabaseQuery.h"
#include "src/database/unit/UnitListModel.h"

#include "src/tool/Utils.h"

extern QQmlApplicationEngine * gEngine;

RecipeIngredientModel::RecipeIngredientModel() : RecipeIngredientModel(nullptr){	// QML
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}
RecipeIngredientModel::RecipeIngredientModel(QObject *parent)
	: QmlModel{parent} {
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	connect(this, &RecipeIngredientModel::idChanged, this, &RecipeIngredientModel::updateIsSaved);
	//connect(this, &RecipeIngredientModel::servingSizeChanged, this, &RecipeIngredientModel::recomputeFromServingSize);
	mTablePrefix = "recipe";
}

RecipeIngredientModel::RecipeIngredientModel(const RecipeIngredientModel * model, QObject *parent)
	: QmlModel{parent} {
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	connect(this, &RecipeIngredientModel::idChanged, this, &RecipeIngredientModel::updateIsSaved);
	//connect(this, &RecipeIngredientModel::servingSizeChanged, this, &RecipeIngredientModel::recomputeFromServingSize);
	mTablePrefix = "recipe";
	if(model){
		mId = initiBackup(model, &model->mId, model->mId, &mId).toLongLong();
		mRecipeId = initiBackup(model, &model->mRecipeId, model->mRecipeId, &mRecipeId).toLongLong();
		mFoodId = initiBackup(model, &model->mFoodId, model->mFoodId, &mFoodId).toLongLong();
		mIngredientUnitId = initiBackup(model, &model->mIngredientUnitId, model->mIngredientUnitId, &mIngredientUnitId).toLongLong();
		mIngredientSize = initiBackup(model, &model->mIngredientSize, model->mIngredientSize, &mIngredientSize).toDouble();
	}else {
	}
	
}

RecipeIngredientModel* RecipeIngredientModel::clone(QObject*parent) {
	auto model = new RecipeIngredientModel(this, parent);
	model->clearBackupValues();
	return model;
}

void RecipeIngredientModel::initRandomValues(){
	static int count = 0;
	mId = 0;
	mRecipeId = 0;
	mIngredientUnitId = 1;
	mIngredientSize = std::rand() * 400.0 / RAND_MAX;
}

DEFINE_GETSET(RecipeIngredientModel,qint64,id,Id)
DEFINE_GETSET(RecipeIngredientModel,qint64,recipeId,RecipeId)
DEFINE_GETSET(RecipeIngredientModel,qint64,foodId,FoodId)
DEFINE_GETSET(RecipeIngredientModel,qint64,ingredientUnitId,IngredientUnitId)
DEFINE_GETSET(RecipeIngredientModel,double,ingredientSize,IngredientSize)

DEFINE_SIMPLE_GETSET(RecipeIngredientModel,FoodModel*,foodModel,FoodModel)
//-------------------------------------------------------------------------------------------------------------------


void RecipeIngredientModel::updateIsSaved(){
	setIsSaved(getId() > 0);
}
void RecipeIngredientModel::undo(){
	DEFINE_UNDO_LONGLONG(Id)
	DEFINE_UNDO_LONGLONG(RecipeId)
	DEFINE_UNDO_LONGLONG(FoodId)
	DEFINE_UNDO_LONGLONG(IngredientUnitId)
	DEFINE_UNDO_DOUBLE(IngredientSize)
	QmlModel::undo();
}
int RecipeIngredientModel::save(){
	if(mId>0 && !mIsEdited) return 1;// Avoid update for nothing

	qDebug() << "Saving " << mTablePrefix;
	DatabaseQuery query;

	query.begin(mId == 0 ? DatabaseQuery::Insert : DatabaseQuery::Update, mTablePrefix+"s" );
	query.add("recipe_id", mRecipeId);
	query.add("food_id", mFoodId);
	query.add("ingredient_size", mIngredientSize);
	query.add("ingredient_unit_id", mIngredientUnitId);
	addQueryValues(query);
	query.addConditionnal(mTablePrefix+"_id",mId);
	if(mId == 0){
		if(!query.exec()){
			qCritical() << "Cannot save " << mTablePrefix << " : " << query.mQuery.lastError().text();
			return 0;
		}
		auto fieldNo = query.mQuery.record().indexOf(mTablePrefix+"_id");
		while (query.mQuery.next()) {
			setId(query.mQuery.value(fieldNo).toInt());
			qDebug() << "Insert " << mTablePrefix  << ": " << mId;
		}
	}else{
		if(!query.exec()) {
			qCritical() << "Cannot update " << mTablePrefix << query.mQuery.lastError().text();
			return 0;
		}else {
			qDebug() << "Update " << mTablePrefix  << ": " << mId;
		}
	}
	clearBackupValues();
	emit saved();
	return 1;
}

void RecipeIngredientModel::saveValues(DatabaseQuery &query){
}

void RecipeIngredientModel::remove(){
	if(mId > 0){
		DatabaseQuery query;
		query.begin(DatabaseQuery::Delete, mTablePrefix+"s");
		query.addConditionnal(mTablePrefix+"_id",mId);
		if(!query.exec()){
			if(!query.exec()) qCritical() << "Cannot delete " << mTablePrefix << " : "  << query.mQuery.lastError().text();
		}
	}
	emit removed(this);
}

RecipeIngredientModel *RecipeIngredientModel::build(QSqlQuery &query, QObject * parent) {
	RecipeIngredientModel * model = new RecipeIngredientModel(parent);
	model->load(query);
	return model;
}

void RecipeIngredientModel::load(QSqlQuery &query){
	// TODO optimize
	for(int i = 0 ; i < query.record().count() ; ++i){
		QString fieldName = query.record().field(i).name();
		if(fieldName == mTablePrefix+"_id") setId(query.value(i).toInt());
		else if(fieldName == "recipe_id") setRecipeId(query.value(i).toInt());
		else if(fieldName == "food_id") setFoodId(query.value(i).toInt());
		else if(fieldName == "ingredient_size") setIngredientSize(query.value(i).toDouble());
		else if(fieldName == "ingredient_unit_id") setIngredientUnitId(query.value(i).toLongLong());
	}
	clearBackupValues();
}

QList<RecipeIngredientModel*> RecipeIngredientModel::buildAll(QObject * parent){
	QList<RecipeIngredientModel*> models;
	QSqlQuery query;
	if(!query.exec( "SELECT * FROM recipe_ingredients ORDER BY recipe_ingredient_id DESC")) qCritical() << "Cannot select all recipe ingredients : "  << query.lastError().text();

	while (query.next()) {
		auto model = build(query, parent);
		models << model;
	}

	return models;
}

QList<RecipeIngredientModel*> RecipeIngredientModel::build(qint64 recipeId, bool withFoodModel, QObject * parent){
	QList<RecipeIngredientModel*> models;
	QSqlQuery query;
	if(!query.exec( QStringLiteral("SELECT * FROM recipe_ingredients WHERE recipe_id = %1 ORDER BY recipe_ingredient_id DESC").arg(recipeId) )) qCritical() << "Cannot select all recipe ingredients : "  << query.lastError().text();
	QStringList ids;
	while (query.next()) {
		auto model = build(query, parent);
		models << model;
		ids << QString::number(model->getFoodId());
	}
	if(ids.size() > 0 && withFoodModel){
		if(!query.exec( QStringLiteral("SELECT * FROM foods WHERE food_id IN(%1)").arg(ids.join(",")) )) qCritical() << "Cannot select all foods for recipe ingredients : "  << query.lastError().text();
		while (query.next()) {
			auto model = FoodModel::build(query, parent);
			auto id = model->getId();
			for(int i = 0 ; i < models.size() ; ++i)
				if(models[i]->getFoodId() == id)
					models[i]->setFoodModel(model);
		}
	}
	return models;
}
