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

#include "UnitModel.h"

#include <QQmlApplicationEngine>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include "src/database/DatabaseQuery.h"

#include "src/tool/Utils.h"


extern QQmlApplicationEngine * gEngine;

UnitModel::UnitModel() : UnitModel(nullptr){	// QML
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}
UnitModel::UnitModel(QObject *parent)
	: QmlModel{parent}
{
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE

}
UnitModel::UnitModel(QString name, double gram, double meter, double second, double kcal, double ml, QObject *parent) : UnitModel(parent){
	mName = name;
	mGramValue = gram;
	mMeterValue = meter;
	mSecondValue = second;
	mKcalValue = kcal;
	mMilliliterValue = ml;
}

DEFINE_GETSET(UnitModel,qint64,unitId,UnitId)
DEFINE_GETSET(UnitModel,QString,name,Name)
DEFINE_GETSET(UnitModel,double,gramValue,GramValue)
DEFINE_GETSET(UnitModel,double,meterValue,MeterValue)
DEFINE_GETSET(UnitModel,double,secondValue,SecondValue)
DEFINE_GETSET(UnitModel,double,kcalValue,KcalValue)
DEFINE_GETSET(UnitModel,double,milliliterValue,MilliliterValue)


int UnitModel::save(){
	if(mUnitId>0 && !mIsEdited) return true;// Avoid update for nothing
	qDebug() << "Saving unit " << mName;
	DatabaseQuery query;

	query.begin(mUnitId == 0 ? DatabaseQuery::Insert : DatabaseQuery::Update, "units" );

	query.add("name", mName);
	query.add("gram_value", mGramValue);
	query.add("meter_value", mMeterValue);
	query.add("second_value", mSecondValue);
	query.add("kcal_value", mKcalValue);
	query.add("milliliter_value", mMilliliterValue);
	query.addConditionnal("unit_id",mUnitId);
	if(mUnitId == 0){
		if(!query.exec()){
			qCritical() << "Cannot save unit: "  << query.mQuery.lastError().text();
			return false;
		}
		auto fieldNo = query.mQuery.record().indexOf("unit_id");
		while (query.mQuery.next()) {
			setUnitId(query.mQuery.value(fieldNo).toInt());
			qDebug() << "Insert unit: " << mUnitId;
		}
	}else{
		if(!query.exec()){
			qCritical() << "Cannot update unit: "  << query.mQuery.lastError().text();
			return false;
		} else {
			qDebug() << "Update unit: " << mUnitId;
		}
	}
	clearBackupValues();
	return true;

}

UnitModel *UnitModel::build(QSqlQuery &query, QObject * parent) {
	auto idField = query.record().indexOf("units.unit_id");
	auto nameField = query.record().indexOf("units.name");
	auto gramValueField = query.record().indexOf("units.gram_value");
	auto meterValueField = query.record().indexOf("units.meter_value");
	auto secondValueField = query.record().indexOf("units.second_value");
	auto kcalValueField = query.record().indexOf("units.kcal_value");
	auto mlValueField = query.record().indexOf("units.milliliter_value");
	if( idField>= 0 && nameField >= 0 && gramValueField >= 0){
		UnitModel * model = new UnitModel(parent);
	// TODO optimize
		model->setUnitId(query.value(idField).toInt());
		model->setName(query.value(nameField).toString());
		model->setGramValue(query.value(gramValueField).toDouble());
		model->setMeterValue(query.value(meterValueField).toDouble());
		model->setSecondValue(query.value(secondValueField).toDouble());
		model->setKcalValue(query.value(kcalValueField).toDouble());
		model->setMilliliterValue(query.value(mlValueField).toDouble());
		model->clearBackupValues();
		return model;
	}else return nullptr;
}

QList<UnitModel*> UnitModel::buildAll(QObject * parent){
	QList<UnitModel*> models;
	QSqlQuery query;
	if(!query.exec( "SELECT * FROM units ORDER BY name ASC")) qCritical() << "Cannot select all units  : "  << query.lastError().text();

	while (query.next()) {
		auto model = build(query, parent);
		models << model;
	}

	return models;
}
