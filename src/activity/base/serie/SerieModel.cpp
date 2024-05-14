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

#include "SerieModel.h"
#include <QSqlQuery>
#include <QSqlRecord>

#include "src/database/DatabaseQuery.h"

#include <QQmlApplicationEngine>
extern QQmlApplicationEngine * gEngine;

using namespace Base;

SerieModel::SerieModel() : SerieModel(nullptr){
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
	mDbTable = "exercise_series";
}

SerieModel::SerieModel(QObject *parent)
	: ExerciseModel{parent}
{
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	mDbTable = "exercise_series";
}

SerieModel::SerieModel(int type, QObject *parent)
	: ExerciseModel{parent}// Don't call Exercise parent on type because of virtuality issue in constructor
{
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	mDbTable = "exercise_series";
	setType(type);
}

SerieModel::SerieModel(const SerieModel * model, QObject *parent) : ExerciseModel(model, parent){
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	mDbTable = "exercise_series";
	setOrder(model->getOrder());
}

SerieModel * SerieModel::clone(QObject *parent)const{
	SerieModel *model = new SerieModel(this, parent);
	model->buildData();
	model->setData(this->getData());
	model->clearBackupValues();
	return model;
}

void SerieModel::setDbData(QVariant data) {
	switch(getType()){
		case 1:{
			int count = 0;
			for(auto value : data.toString().split(',')){
				mData[count++]->setValue(value.toInt());
			}
		}break;
		case 2 : {}break;
		case 3 : {


		}break;
	default:{
	}
	}
}

void SerieModel::buildData() {
	QVariantList defaultData;
	if(mData.size() != 0) qCritical() << "Data is not empty while building it. Check this case!";
	switch(mType){
		case 1 :{
			auto d = new QmlData("Reps",10, this);
			mData << d;
			connect(d, &QmlData::valueChanged, this, [this, d](){
				addBackup(&mData[0], mData[0]->getValue(), d->getValue());
				mData[0]->setValue(d->getValue());
			});
			d = new QmlData("Weight (kg)",50, this);
			mData << d;
			connect(d, &QmlData::valueChanged, this, [this, d](){
				addBackup(&mData[0], mData[0]->getValue(), d->getValue());
				mData[0]->setValue(d->getValue());
			});

		}break;
		default:{}
	}
}

qint64 SerieModel::getSerieId()const{
	return mSerieId;
}

void SerieModel::setSerieId(qint64 id) {
	if(mSerieId != id){
		mSerieId = id;
		setIsSaved(getSerieId() > 0);
		emit serieIdChanged();
	}
}

int SerieModel::getOrder() const {
	return mOrder;
}

void SerieModel::setOrder(int data){
	if(mOrder != data){
		addBackup(&mOrder, mOrder, data);
		mOrder = data;
		emit orderChanged();
	}
}

void SerieModel::updateIsSaved() {
	setIsSaved(getSerieId() > 0);
}

void SerieModel::undo(){
	if(mBackupValues.contains(&mOrder)) mOrder = mBackupValues[&mOrder].toInt();
	ExerciseModel::undo();
}

//-------------------------------------------------------------------------------------------------------------------

bool SerieModel::save(){
	DatabaseQuery query;
	query.begin(mSerieId == 0 ? DatabaseQuery::Insert : DatabaseQuery::Update, mDbTable);
	query.add("name", mName);
	query.add("description", mDescription);
	query.add("type", mType);
	query.add("rest_time", mRestTime);
	query.add("work_time", mWorkTime);
	query.add("exercise_id", mExerciseId);
	query.add("serie_order", mOrder);
	query.addConditionnal("serie_id", mSerieId);

	switch(mType){
		case 1:{// Strength
			QStringList parser;
			for(auto i : mData){
				parser << QString::number(i->getValue().toInt());
			}
			query.add("data", parser.join(','));
		}
		break;
		default: {}
	}
	bool result = query.exec();
	if(!result) qCritical() << "Cannot " << (mSerieId == 0 ? "save" : "update") << " series: " << query.mQuery.lastError().text();
	else {
		if(mSerieId == 0){
			auto idField = query.mQuery.record().indexOf("serie_id");
			query.mQuery.next();
			setSerieId(query.mQuery.value(idField).toInt());
		}
		clearBackupValues();


	}

	return result;
}

SerieModel *SerieModel::load(QSqlQuery &query, QObject * parent) {
	SerieModel * model = new SerieModel(parent);
// TODO optimize
	ExerciseModel::load(model, query);
	auto serieIdField = query.record().indexOf("serie_id");
	auto serieOrderField = query.record().indexOf("serie_order");
	model->setSerieId(query.value(serieIdField).toInt());
	model->setOrder(query.value(serieOrderField).toInt());
	model->clearBackupValues();
	return model;
}


void SerieModel::remove(){
	if(mSerieId > 0){
		DatabaseQuery query;
		query.begin(DatabaseQuery::Delete, mDbTable);
		query.addConditionnal("serie_id",mSerieId);
		if(!query.exec()){
			if(!query.exec()) qCritical() << "Cannot delete serie: "  << query.mQuery.lastError().text();
		}
	}
	emit removed(this);
}
