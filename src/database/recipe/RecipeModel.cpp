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

#include "RecipeModel.h"

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

RecipeModel::RecipeModel() : RecipeModel(nullptr){	// QML
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}
RecipeModel::RecipeModel(QObject *parent)
	: QmlModel{parent} {
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	connect(this, &RecipeModel::idChanged, this, &RecipeModel::updateIsSaved);
	//connect(this, &RecipeModel::servingSizeChanged, this, &RecipeModel::recomputeFromServingSize);
	mTablePrefix = "recipe";
}

RecipeModel::RecipeModel(const RecipeModel * model, QObject *parent)
	: QmlModel{parent} {
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	connect(this, &RecipeModel::idChanged, this, &RecipeModel::updateIsSaved);
	//connect(this, &RecipeModel::servingSizeChanged, this, &RecipeModel::recomputeFromServingSize);
	mTablePrefix = "recipe";
	if(model){
		mId = initiBackup(model, &model->mId, model->mId, &mId).toLongLong();
		mBrand = initiBackup(model, &model->mBrand, model->mBrand, &mBrand).toString();
		mDescription = initiBackup(model, &model->mDescription, model->mDescription, &mDescription).toString();
		mRecipeUnitId = initiBackup(model, &model->mRecipeUnitId, model->mRecipeUnitId, &mRecipeUnitId).toLongLong();
		mRecipeSize = initiBackup(model, &model->mRecipeSize, model->mRecipeSize, &mRecipeSize).toDouble();
	}else {
		mDescription = "Custom";
		
	}
	
}

RecipeModel* RecipeModel::clone(QObject*parent) {
	auto model = new RecipeModel(this, parent);
	model->clearBackupValues();
	return model;
}

void RecipeModel::initRandomValues(){
	static int count = 0;
	mId = 0;
	mBrand = "Custom Recipe";
	mDescription = "A random recipe for debugging or example";
	mRecipeUnitId = 1;
	mRecipeSize = std::rand() * 400.0 / RAND_MAX;
}

DEFINE_GETSET(RecipeModel,qint64,id,Id)
DEFINE_GETSET(RecipeModel,QString,brand,Brand)
DEFINE_GETSET(RecipeModel,QString,description,Description)
DEFINE_GETSET(RecipeModel,qint64,recipeUnitId,RecipeUnitId)
DEFINE_GETSET(RecipeModel,double,recipeSize,RecipeSize)

void RecipeModel::addIngredient(RecipeIngredientModel * data) {
	data->setRecipeId(mId);
	mIngredients.push_back(data);
	emit ingredientsAdded(data);
}

void RecipeModel::removeIngredient(RecipeIngredientModel * data) {
	auto it = std::find_if(mIngredients.begin(), mIngredients.end(), [data](RecipeIngredientModel*i){
		return i == data;
	});
	if(it != mIngredients.end()){
		mIngredients.erase(it);
		if(data->getIsSaved())
			mRemovedIngredients.push_back(data);
	}
	emit ingredientsRemoved(data);
}

//-------------------------------------------------------------------------------------------------------------------

void RecipeModel::updateIsSaved(){
	setIsSaved(getId() > 0);
}
void RecipeModel::undo(){
	DEFINE_UNDO_LONGLONG(Id)
	DEFINE_UNDO_STRING(Brand)
	DEFINE_UNDO_STRING(Description)
	DEFINE_UNDO_LONGLONG(RecipeUnitId)
	DEFINE_UNDO_DOUBLE(RecipeSize)
	for(auto i : mRemovedIngredients)
		mIngredients.push_back(i);
	mRemovedIngredients.clear();
	QmlModel::undo();
}
int RecipeModel::save(){
	if(mId>0 && !mIsEdited) return 1;// Avoid update for nothing

	qDebug() << "Saving " << mTablePrefix  << mBrand << mDescription;
	DatabaseQuery query;

	query.begin(mId == 0 ? DatabaseQuery::Insert : DatabaseQuery::Update, mTablePrefix+"s" );
	query.add("brand", mBrand);
	query.add("description", mDescription);
	query.add("recipe_size", mRecipeSize);
	query.add("recipe_unit_id", mRecipeUnitId);
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
	for(auto item : mIngredients)
		item->save();
	for(auto item : mRemovedIngredients)
		item->remove();
	mRemovedIngredients.clear();
	emit saved();
	return 1;
}

void RecipeModel::saveValues(DatabaseQuery &query){
}

void RecipeModel::remove(){
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

RecipeModel *RecipeModel::build(QSqlQuery &query, QObject * parent) {
	RecipeModel * model = new RecipeModel(parent);
	model->load(query);
	return model;
}

void RecipeModel::load(QSqlQuery &query){
	// TODO optimize
	for(int i = 0 ; i < query.record().count() ; ++i){
		QString fieldName = query.record().field(i).name();
		if(fieldName == mTablePrefix+"_id") setId(query.value(i).toInt());
		else if(fieldName == "brand") setBrand(query.value(i).toString());
		else if(fieldName == "description") setDescription(query.value(i).toString());
		else if(fieldName == "recipe_size") setRecipeSize(query.value(i).toDouble());
		else if(fieldName == "recipe_unit_id") setRecipeUnitId(query.value(i).toLongLong());
	}
	clearBackupValues();
}

QList<RecipeModel*> RecipeModel::buildAll(QObject * parent){
	QList<RecipeModel*> models;
	QSqlQuery query;
	if(!query.exec( "SELECT * FROM recipes ORDER BY recipe_id DESC")) qCritical() << "Cannot select all recipes : "  << query.lastError().text();

	while (query.next()) {
		auto model = build(query, parent);
		models << model;
	}

	return models;
}
