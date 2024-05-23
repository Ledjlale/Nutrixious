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

#include "ProgramSerieModel.h"
#include <QSqlQuery>
#include <QSqlRecord>

#include "src/database/DatabaseQuery.h"

#include <QQmlApplicationEngine>
extern QQmlApplicationEngine * gEngine;

ProgramSerieModel::ProgramSerieModel() : QmlModel(nullptr){
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}

ProgramSerieModel::ProgramSerieModel(QObject *parent)
	: QmlModel{parent}
{
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
}

ProgramSerieModel::ProgramSerieModel(int type, QObject *parent)
	: QmlModel{parent}// Don't call Exercise parent on type because of virtuality issue in constructor
{
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	setType(type);
}

ProgramSerieModel::ProgramSerieModel(const ProgramSerieModel * model, QObject *parent) : ProgramSerieModel(model->getType(), parent){
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	setOrder(model->getOrder());
	setRestTime(model->getRestTime());
	setWorkTime(model->getWorkTime());
}

ProgramSerieModel * ProgramSerieModel::clone(QObject *parent)const{
	ProgramSerieModel *model = new ProgramSerieModel(this, parent);
	model->setData(this->getData());
	model->clearBackupValues();
	return model;
}


qint64 ProgramSerieModel::getProgramSerieId()const{
	return mProgramSerieId;
}

void ProgramSerieModel::setProgramSerieId(qint64 id) {
	if(mProgramSerieId != id){
		mProgramSerieId = id;
		setIsSaved(getProgramSerieId() > 0);
		emit serieIdChanged();
	}
}

qint64 ProgramSerieModel::getProgramExerciseId()const{
	return mProgramExerciseId;
}

void ProgramSerieModel::setProgramExerciseId(qint64 id) {
	if(mProgramExerciseId != id){
		mProgramExerciseId = id;
		emit programExerciseIdChanged();
	}
}

int ProgramSerieModel::getOrder() const {
	return mOrder;
}

void ProgramSerieModel::setOrder(int data){
	if(mOrder != data){
		addBackup(&mOrder, mOrder, data);
		mOrder = data;
		emit orderChanged();
	}
}


int ProgramSerieModel::getType() const{
	return mType;
}

void ProgramSerieModel::setType(int data){
	if(mType != data){
		mType = data;
		buildData();
		emit typeChanged();
	}
}

int ProgramSerieModel::getWorkTime() const{
	return mWorkTime;
}
void ProgramSerieModel::setWorkTime(int data){
	if( mWorkTime != data){
		addBackup(&mWorkTime, mWorkTime, data);
		mWorkTime = data;
		emit workTimeChanged();
	}
}

int ProgramSerieModel::getRestTime() const{
	return mRestTime;
}

void ProgramSerieModel::setRestTime(int data){
	if( mRestTime != data){
		addBackup(&mRestTime, mRestTime, data);
		mRestTime = data;
		emit restTimeChanged();
	}
}

const QList<QmlData*>& ProgramSerieModel::getData()const {
	return mData;
}

void ProgramSerieModel::setData(const QList<QmlData *> data) {
	if(data.size() == mData.size()){
		for(int i = 0 ; i < data.size() ; ++i)
			mData[i]->setValue(data[i]->getValue());
	}else
		qCritical() << "Cannot set Data : sizes are different. You must build them before.";
}

QVariantList ProgramSerieModel::getQmlVariantData()const {
	QVariantList l;
	for(auto i : mData)
		l << QVariant::fromValue(i);
	return l;
}

void ProgramSerieModel::setDbData(QVariant data) {
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

void ProgramSerieModel::buildData() {
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
				addBackup(&mData[1], mData[1]->getValue(), d->getValue());
				mData[1]->setValue(d->getValue());
			});

		}break;
		default:{}
	}
	emit dataChanged();
}

//-------------------------------------------------------------------------------------------------------------------

void ProgramSerieModel::updateIsSaved() {
	setIsSaved(getProgramSerieId() > 0);
}

void ProgramSerieModel::undo(){
	if(mBackupValues.contains(&mOrder)) mOrder = mBackupValues[&mOrder].toInt();
	if(mBackupValues.contains(&mRestTime)) mRestTime = mBackupValues[&mRestTime].toInt();
	if(mBackupValues.contains(&mWorkTime)) mWorkTime = mBackupValues[&mWorkTime].toInt();

	QmlModel::undo();
}

//-------------------------------------------------------------------------------------------------------------------

bool ProgramSerieModel::save(){
	DatabaseQuery query;
	query.begin(mProgramSerieId == 0 ? DatabaseQuery::Insert : DatabaseQuery::Update, "prgm_exercise_series");
	query.add("type", mType);
	query.add("rest_time", mRestTime);
	query.add("work_time", mWorkTime);
	query.add("prgm_exercise_id", mProgramExerciseId);
	query.add("serie_order", mOrder);
	query.addConditionnal("prgm_serie_id", mProgramSerieId);

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
	if(!result) qCritical() << "Cannot " << (mProgramSerieId == 0 ? "save" : "update") << " series: " << query.mQuery.lastError().text();
	else {
		if(mProgramSerieId == 0){
			auto idField = query.mQuery.record().indexOf("prgm_serie_id");
			query.mQuery.next();
			setProgramSerieId(query.mQuery.value(idField).toInt());
		}
		clearBackupValues();
	}

	return result;
}

ProgramSerieModel *ProgramSerieModel::load(QSqlQuery &query, QObject * parent) {
	ProgramSerieModel * model = new ProgramSerieModel(parent);
// TODO optimize
	auto serieIdField = query.record().indexOf("prgm_serie_id");
	auto programExerciseIdField = query.record().indexOf("prgm_exercise_id");
	auto serieOrderField = query.record().indexOf("serie_order");
	auto typeField = query.record().indexOf("type");
	auto restTimeField = query.record().indexOf("rest_time");
	auto workTimeField = query.record().indexOf("work_time");
	auto dataField = query.record().indexOf("data");


	model->setProgramSerieId(query.value(serieIdField).toInt());
	model->setOrder(query.value(serieOrderField).toInt());
	model->setType(query.value(typeField).toInt());
	model->setProgramExerciseId(query.value(programExerciseIdField).toInt());
	model->setWorkTime(query.value(workTimeField).toInt());
	model->setRestTime(query.value(restTimeField).toInt());

	model->setDbData(query.value(dataField));

	model->clearBackupValues();
	return model;
}


void ProgramSerieModel::remove(){
	if(mProgramSerieId > 0){
		DatabaseQuery query;
		query.begin(DatabaseQuery::Delete, "prgm_exercise_series");
		query.addConditionnal("prgm_serie_id", mProgramSerieId);
		if(!query.exec()){
			if(!query.exec()) qCritical() << "Cannot delete serie: "  << query.mQuery.lastError().text();
		}
	}
	emit removed(this);
}
