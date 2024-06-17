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

#include "MealGroupModel.h"

#include <QQmlApplicationEngine>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlField>
#include "src/database/DatabaseQuery.h"

#include "src/tool/Utils.h"


extern QQmlApplicationEngine * gEngine;

MealGroupModel::MealGroupModel() : MealGroupModel(nullptr){	// QML
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}
MealGroupModel::MealGroupModel(QObject *parent)
	: QmlModel{parent}
{
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE

}

MealGroupModel::MealGroupModel(MealGroupModel * model, QObject *parent)
	: QmlModel{parent}
{
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE

	mName = initiBackup(model, &model->mName, model->mName, &mName).toString();
	mMealGroupId = initiBackup(model, &model->mMealGroupId, model->mMealGroupId, &mMealGroupId).toLongLong();
	mDefaultTime = initiBackup(model, &model->mDefaultTime, model->mDefaultTime, &mDefaultTime).toTime();
	mIsDisplayed = initiBackup(model, &model->mIsDisplayed, model->mIsDisplayed, &mIsDisplayed).toBool();
}

MealGroupModel::MealGroupModel(QString name, QTime defaultTime){
	mName = name;
	mDefaultTime = defaultTime;
}

MealGroupModel* MealGroupModel::clone(QObject*parent) {
	return new MealGroupModel(this, parent);
}

QString MealGroupModel::getDefaultTimeStr()const{
	if(mDefaultTime.isValid())
		//return QLocale().toString(mStartDateTime, QLocale().dateTimeFormat(QLocale::FormatType::ShortFormat));
		return QLocale().toString(mDefaultTime, "hh:mm:ss");
	else
		return "";
}
void MealGroupModel::setDefaultTimeStr(QString data){
	if(data == "")
		setDefaultTime(QTime());
	else{
		QTime time = QTime::fromString(data, "hh:mm:ss");
		if(time.isValid())
			setDefaultTime(time);
	}
}

DEFINE_GETSET(MealGroupModel,qint64,mealGroupId,MealGroupId)
DEFINE_GETSET(MealGroupModel,QString,name,Name)
DEFINE_GETSET(MealGroupModel,QTime,defaultTime,DefaultTime)
DEFINE_GETSET(MealGroupModel,bool,isDisplayed,IsDisplayed)

//-------------------------------------------------------------------------------------------------------------------


int MealGroupModel::save(){
	if(mMealGroupId>0 && !mIsEdited) return true;// Avoid update for nothing
	qDebug() << "Saving exercise " << mName;
	DatabaseQuery query;

	query.begin(mMealGroupId == 0 ? DatabaseQuery::Insert : DatabaseQuery::Update, "meal_groups" );

	query.add("name", mName);
	query.add("default_time", mDefaultTime.isValid() ? mDefaultTime.msecsSinceStartOfDay() : -1);
	query.add("is_displayed", mIsDisplayed);
	query.addConditionnal("meal_group_id",mMealGroupId);
	if(mMealGroupId == 0){
		if(!query.exec()){
			qCritical() << "Cannot save meal group: "  << query.mQuery.lastError().text();
			return false;
		}
		auto fieldNo = query.mQuery.record().indexOf("meal_group_id");
		while (query.mQuery.next()) {
			setMealGroupId(query.mQuery.value(fieldNo).toInt());
			qDebug() << "Insert meal group: " << mMealGroupId;
		}
	}else{
		if(!query.exec()){
			qCritical() << "Cannot update meal group: "  << query.mQuery.lastError().text();
			return false;
		} else {
			qDebug() << "Update meal group: " << mMealGroupId;
		}
	}
	clearBackupValues();
	return true;
}

void MealGroupModel::remove(){
	if(mMealGroupId > 0){
		DatabaseQuery query;
		query.begin(DatabaseQuery::Delete, "meal_groups");
		query.addConditionnal("meal_group_id",mMealGroupId);
		if(!query.exec()){
			if(!query.exec()) qCritical() << "Cannot delete meal group  : "  << query.mQuery.lastError().text();
		}
	}
	emit removed(this);
}

MealGroupModel *MealGroupModel::build(QSqlQuery &query, QObject * parent) {

	MealGroupModel * model = new MealGroupModel(parent);
// TODO optimize
	for(int i = 0 ; i < query.record().count() ; ++i){
		QString fieldName = query.record().field(i).name();
		if(fieldName == "meal_group_id") model->setMealGroupId(query.value(i).toLongLong());
		else if(fieldName == "name") model->setName(query.value(i).toString());
		else if(fieldName == "default_time") model->setDefaultTime(QTime::fromMSecsSinceStartOfDay(query.value(i).toInt()));
		else if(fieldName == "is_displayed") model->setIsDisplayed(query.value(i).toBool());
	}
	model->clearBackupValues();
	return model;
}

QList<MealGroupModel*> MealGroupModel::buildAll(QObject * parent){
	QList<MealGroupModel*> models;
	QSqlQuery query;
	if(!query.exec( "SELECT * FROM meal_groups ORDER BY default_time ASC")) qCritical() << "Cannot select all meal groups : "  << query.lastError().text();

	while (query.next()) {
		auto model = build(query, parent);
		models << model;
	}

	return models;
}
