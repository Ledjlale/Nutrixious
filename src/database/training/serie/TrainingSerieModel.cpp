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

#include "TrainingSerieModel.h"
#include <QSqlQuery>
#include <QSqlRecord>

#include "src/database/DatabaseQuery.h"
#include "src/database/program/serie/ProgramSerieModel.h"

#include <QQmlApplicationEngine>
extern QQmlApplicationEngine * gEngine;

TrainingSerieModel::TrainingSerieModel() : QmlModel(nullptr){
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}

TrainingSerieModel::TrainingSerieModel(QObject *parent)
	: QmlModel{parent}
{
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
}

TrainingSerieModel::TrainingSerieModel(int type, QObject *parent)
	: QmlModel{parent}// Don't call Exercise parent on type because of virtuality issue in constructor
{
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	setType(type);
}

TrainingSerieModel::TrainingSerieModel(const TrainingSerieModel * model, QObject *parent) : TrainingSerieModel(model->getType(), parent){
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	setOrder(model->getOrder());
	setRestTime(model->getRestTime());
	setWorkTime(model->getWorkTime());
}

TrainingSerieModel::TrainingSerieModel(const ProgramSerieModel * model, QObject *parent) : TrainingSerieModel(model->getType(), parent) {
	setOrder(model->getOrder());
	setRestTime(model->getRestTime());
	setWorkTime(model->getWorkTime());
	setData(model->getData());
	clearBackupValues();
}

TrainingSerieModel * TrainingSerieModel::clone(QObject *parent)const{
	TrainingSerieModel *model = new TrainingSerieModel(this, parent);
	model->setData(this->getData());
	model->clearBackupValues();
	return model;
}


qint64 TrainingSerieModel::getTrainingSerieId()const{
	return mTrainingSerieId;
}

void TrainingSerieModel::setTrainingSerieId(qint64 id) {
	if(mTrainingSerieId != id){
		mTrainingSerieId = id;
		setIsSaved(getTrainingSerieId() > 0);
		emit serieIdChanged();
	}
}

qint64 TrainingSerieModel::getTrainingExerciseId()const{
	return mTrainingExerciseId;
}

void TrainingSerieModel::setTrainingExerciseId(qint64 id) {
	if(mTrainingExerciseId != id){
		mTrainingExerciseId = id;
		emit trainingExerciseIdChanged();
	}
}

int TrainingSerieModel::getOrder() const {
	return mOrder;
}

void TrainingSerieModel::setOrder(int data){
	if(mOrder != data){
		addBackup(&mOrder, mOrder, data);
		mOrder = data;
		emit orderChanged();
	}
}


int TrainingSerieModel::getType() const{
	return mType;
}

void TrainingSerieModel::setType(int data){
	if(mType != data){
		mType = data;
		buildData();
		emit typeChanged();
	}
}

int TrainingSerieModel::getWorkTime() const{
	return mWorkTime;
}
void TrainingSerieModel::setWorkTime(int data){
	if( mWorkTime != data){
		addBackup(&mWorkTime, mWorkTime, data);
		mWorkTime = data;
		emit workTimeChanged();
	}
}

int TrainingSerieModel::getRestTime() const{
	return mRestTime;
}

void TrainingSerieModel::setRestTime(int data){
	if( mRestTime != data){
		addBackup(&mRestTime, mRestTime, data);
		mRestTime = data;
		emit restTimeChanged();
	}
}

const QList<QmlData*>& TrainingSerieModel::getData()const {
	return mData;
}

void TrainingSerieModel::setData(const QList<QmlData *> data) {
	if(data.size() == mData.size()){
		for(int i = 0 ; i < data.size() ; ++i)
			mData[i]->setValue(data[i]->getValue());
	}else
		qCritical() << "Cannot set Data : sizes are different. You must build them before.";
}

QVariantList TrainingSerieModel::getQmlVariantData()const {
	QVariantList l;
	for(auto i : mData)
		l << QVariant::fromValue(i);
	return l;
}

void TrainingSerieModel::setDbData(QVariant data) {
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

void TrainingSerieModel::buildData() {
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
	emit dataChanged();
}

//-------------------------------------------------------------------------------------------------------------------

void TrainingSerieModel::updateIsSaved() {
	setIsSaved(getTrainingSerieId() > 0);
}

void TrainingSerieModel::undo(){
	if(mBackupValues.contains(&mOrder)) mOrder = mBackupValues[&mOrder].toInt();
	if(mBackupValues.contains(&mRestTime)) mRestTime = mBackupValues[&mRestTime].toInt();
	if(mBackupValues.contains(&mWorkTime)) mWorkTime = mBackupValues[&mWorkTime].toInt();

	QmlModel::undo();
}

//-------------------------------------------------------------------------------------------------------------------

bool TrainingSerieModel::save(){
	DatabaseQuery query;
	if( mTrainingSerieId > 0 && !getIsEdited()) return false;
	query.begin(mTrainingSerieId == 0 ? DatabaseQuery::Insert : DatabaseQuery::Update, "tr_exercise_series");
	query.add("type", mType);
	query.add("rest_time", mRestTime);
	query.add("work_time", mWorkTime);
	query.add("tr_exercise_id", mTrainingExerciseId);
	query.add("serie_order", mOrder);
	query.addConditionnal("tr_serie_id", mTrainingSerieId);

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
	if(!result) qCritical() << "Cannot " << (mTrainingSerieId == 0 ? "save" : "update") << " series: " << query.mQuery.lastError().text();
	else {
		if(mTrainingSerieId == 0){
			auto idField = query.mQuery.record().indexOf("tr_serie_id");
			query.mQuery.next();
			setTrainingSerieId(query.mQuery.value(idField).toInt());
		}
		clearBackupValues();
	}

	return result;
}

TrainingSerieModel *TrainingSerieModel::load(QSqlQuery &query, QObject * parent) {
	TrainingSerieModel * model = new TrainingSerieModel(parent);
// TODO optimize
	auto serieIdField = query.record().indexOf("tr_serie_id");
	auto trainingExerciseIdField = query.record().indexOf("tr_exercise_id");
	auto serieOrderField = query.record().indexOf("serie_order");
	auto typeField = query.record().indexOf("type");
	auto restTimeField = query.record().indexOf("rest_time");
	auto workTimeField = query.record().indexOf("work_time");
	auto dataField = query.record().indexOf("data");


	model->setTrainingSerieId(query.value(serieIdField).toInt());
	model->setOrder(query.value(serieOrderField).toInt());
	model->setType(query.value(typeField).toInt());
	model->setTrainingExerciseId(query.value(trainingExerciseIdField).toInt());
	model->setWorkTime(query.value(workTimeField).toInt());
	model->setRestTime(query.value(restTimeField).toInt());

	model->setDbData(query.value(dataField));

	model->clearBackupValues();
	return model;
}


void TrainingSerieModel::remove(){
	if(mTrainingSerieId > 0){
		DatabaseQuery query;
		query.begin(DatabaseQuery::Delete, "tr_exercise_series");
		query.addConditionnal("tr_serie_id", mTrainingSerieId);
		if(!query.exec()){
			if(!query.exec()) qCritical() << "Cannot delete serie: "  << query.mQuery.lastError().text();
		}
	}
	emit removed(this);
}
