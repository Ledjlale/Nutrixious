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

#include "ExerciseModel.h"
#include "../serie/SerieModel.h"
#include "src/tool/Utils.h"
#include <QVariantList>
#include <QSqlRecord>
#include <QSqlError>
#include <QEvent>
//#include <QDynamicPropertyChange>

#include <QQmlApplicationEngine>
extern QQmlApplicationEngine * gEngine;

using namespace Base;

ExerciseModel::ExerciseModel()
	: ExerciseModel{nullptr} {
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}

ExerciseModel::ExerciseModel(QObject *parent)
	: QObject{parent}
{
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	connect(this, &ExerciseModel::nameChanged, [this](){
		setInvalidName( mName == "");
	});
	connect(this, &ExerciseModel::exerciseIdChanged, [this](){
		for(auto &i: mSeries) i->setExerciseId(mExerciseId);
		updateIsSaved();
	});
	//connect(this, &ExerciseModel::descriptionChanged, [this](){
	//	setInvalidDescription( false);
	//});
}

ExerciseModel::ExerciseModel(int type, QObject * parent)
	: ExerciseModel{parent} {
	setType(type);
}


ExerciseModel::ExerciseModel(const ExerciseModel * model, QObject *parent) : QObject(parent){
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	mName = model->getName();
	mDescription = model->getDescription();
	mType = model->getType();
	mWorkTime = model->getWorkTime();
	mRestTime = model->getRestTime();
	for(auto i : model->getSeries())
		mSeries << i->clone(this);
	setInvalidName( mName == "");
	connect(this, &ExerciseModel::exerciseIdChanged, [this](){
		for(auto &i: mSeries) i->setExerciseId(mExerciseId);
		updateIsSaved();
	});
	connect(this, &ExerciseModel::nameChanged, [this](){
		setInvalidName( mName == "");
	});
	//connect(this, &ExerciseModel::descriptionChanged, [this](modelData){
	//	setInvalidDescription( false);
	//});
}

ExerciseModel * ExerciseModel::clone(QObject *parent)const{
	ExerciseModel *model = new ExerciseModel(this, parent);
	model->buildData();
	model->setData(this->getData());
	model->clearBackupValues();
	return model;
}

qint64 ExerciseModel::getExerciseId()const{
	return mExerciseId;
}

void ExerciseModel::setExerciseId(qint64 id) {
	if(mExerciseId != id){
		mExerciseId = id;
		emit exerciseIdChanged();
	}
}

QString ExerciseModel::getName() const{
	return mName;
}

void ExerciseModel::setName(QString data) {
	if(mName != data){
		addBackup(&mWorkTime, mWorkTime, data);
		mName = data;
		emit nameChanged();
	}
}

QString ExerciseModel::getDescription() const{
	return mDescription;
}

void ExerciseModel::setDescription(QString data) {
	if(mDescription != data){
		addBackup(&mWorkTime, mWorkTime, data);
		mDescription = data;
		emit descriptionChanged();
	}
}

int ExerciseModel::getType() const{
	return mType;
}

void ExerciseModel::setType(int data){
	if(mType != data){
		mType = data;
		buildData();
		emit typeChanged();
	}
}

int ExerciseModel::getWorkTime() const{
	return mWorkTime;
}
void ExerciseModel::setWorkTime(int data){
	if( mWorkTime != data){
		addBackup(&mWorkTime, mWorkTime, data);
		mWorkTime = data;
		emit workTimeChanged();
	}
}

int ExerciseModel::getRestTime() const{
	return mRestTime;
}

void ExerciseModel::setRestTime(int data){
	if( mRestTime != data){
		addBackup(&mRestTime, mRestTime, data);
		mRestTime = data;
		emit restTimeChanged();
	}
}
/*
QVariantList ExerciseModel::getData() const {
	return mData;
}

void ExerciseModel::setData(QVariantList data) {
	if(mData != data) {
		if(data.size() != mQmlData.size()){
			qCritical() << "QmlData and data size are different. : Data cannot be set before type";
			return;
		}
		mData = data;
		for(size_t i = 0 ; i < mData.size() ; ++i){
			mQmlData[i]->setValue(mData[i]);
		}
		emit dataChanged();
	}
}
*/
void ExerciseModel::setDbData(QVariant data){
	switch(getType()){
		case 1: case 2 :{
			int count = -1;
			for(auto value : data.toString().split(',')){
				mData[++count]->setValue(value.toInt());
			}
		}break;

		case 3 : {


		}break;
	default:{
	}
	}
}

const QList<QmlData*>& ExerciseModel::getData()const {
	return mData;
}

void ExerciseModel::setData(const QList<QmlData *> data) {
	if(data.size() == mData.size()){
		for(int i = 0 ; i < data.size() ; ++i)
			mData[i]->setValue(data[i]->getValue());
	}else
		qCritical() << "Cannot set Data : sizes are different. You must build them before.";
}

QVariantList ExerciseModel::getQmlVariantData()const {
	QVariantList l;
	for(auto i : mData)
		l << QVariant::fromValue(i);
	return l;
}

void ExerciseModel::buildData() {
	QVariantList defaultData;
	mData.clear();
	switch(mType){
	case 1 :{
			auto d = new QmlData("Distance (m)",10000, this);
			mData << d;
			connect(d, &QmlData::valueChanged, this, [this, d](){
				addBackup(&mData[0], mData[0]->getValue(), d->getValue());
				mData[0]->setValue(d->getValue());
			});
			setCanHaveSeries(false);
	}
	break;
	case 2 :{
			auto d = new QmlData("Steps",1000, this);
			mData << d;
			connect(d, &QmlData::valueChanged, this, [this, d](){
				addBackup(&mData[0], mData[0]->getValue(), d->getValue());
				mData[0]->setValue(d->getValue());
			});
			setCanHaveSeries(false);
	}
	break;
	case 3 :
		setCanHaveSeries(true);
	break;
		default:{}
	}
}

bool ExerciseModel::getIsSaved() const {
	return mIsSaved;
}

void ExerciseModel::setIsSaved(bool data) {
	if(mIsSaved != data){
		mIsSaved = data;
		emit isSavedChanged();
	}
}

void ExerciseModel::updateIsSaved() {
	setIsSaved(getExerciseId() > 0);
}

bool ExerciseModel::getIsEdited() const {
	return mIsEdited;
}

void ExerciseModel::setIsEdited(bool data) {
	if(mIsEdited != data){
		mIsEdited = data;
		emit isEditedChanged();
	}
}

void ExerciseModel::addBackup(void *key, QVariant keyValue, QVariant newValue){
	if(mBackupValues.contains(key)) {
		if(mBackupValues[key]  == newValue) {
			mBackupValues.remove(key);
			updateIsEdited();
		}
	}else{
		mBackupValues[key] = keyValue;
		setIsEdited(true);
	}
}

void ExerciseModel::updateIsEdited() {
	setIsEdited(mBackupValues.size() > 0);
}

void ExerciseModel::clearBackupValues(){
	mBackupValues.clear();
	setIsEdited(false);
}

void ExerciseModel::undo(){
	if(mBackupValues.contains(&mName)) mName = mBackupValues[&mName].toString();
	if(mBackupValues.contains(&mDescription)) mDescription = mBackupValues[&mDescription].toString();
	if(mBackupValues.contains(&mRestTime)) mRestTime = mBackupValues[&mRestTime].toInt();
	if(mBackupValues.contains(&mWorkTime)) mWorkTime = mBackupValues[&mWorkTime].toInt();
	clearBackupValues();
}

//-------------------------------------------------------------------------------------------------------------------

void ExerciseModel::setInvalidName(bool invalid) {
	if(mInvalidName != invalid){
		mInvalidName = invalid;
		emit invalidNameChanged();
	}
}

void ExerciseModel::setInvalidDescription(bool invalid){
	if(mInvalidDescription != invalid){
		mInvalidDescription = invalid;
		emit invalidDescriptionChanged();
	}
}
//-------------------------------------------------------------------------------------------------------------------

QVariantList ExerciseModel::getVariantSeries() const {
	QVariantList series;
	for(auto serie : mSeries){
		series << QVariant::fromValue(serie);
	 }
	return series;
}

QList<SerieModel*> ExerciseModel::getSeries() const{
	return mSeries;
}

void ExerciseModel::addSerie(SerieModel *model) {
	addSerie(model->clone(this), false);
}

void ExerciseModel::addSerie(SerieModel *model, bool keepId) {
	auto insertedModel = Utils::add<SerieModel>(model, mSeries);
	insertedModel->setExerciseId(mExerciseId);
	connect(insertedModel, &SerieModel::removed, this, &ExerciseModel::handleRemoved);
	if(keepId) {
		insertedModel->setSerieId(model->getSerieId());
	}
	emit seriesChanged();
}

void ExerciseModel::removeSerie(SerieModel *model) {
	mSeries.removeOne(model);
	model->deleteLater();
	emit seriesChanged();
}

void ExerciseModel::handleRemoved(SerieModel *model){
	auto it = std::find(mSeries.begin(), mSeries.end(), model);
	if( it != mSeries.end()){
		int row = it - mSeries.begin();
		mSeries.erase(it);
		emit seriesChanged();
	}
}

void ExerciseModel::decrementSerieOrder(SerieModel *model) {
	if(Utils::decrementOrder<SerieModel>(model, mSeries)){
		emit seriesChanged();
		save();
	}
}
void ExerciseModel::incrementSerieOrder(SerieModel *model){
	if(Utils::incrementOrder<SerieModel>(model, mSeries)){
		emit seriesChanged();
		save();
	}
}

bool ExerciseModel::getCanHaveSeries() const {
	return mCanHaveSeries;
}

void ExerciseModel::setCanHaveSeries(bool data) {
	if(mCanHaveSeries != data){
		mCanHaveSeries = data;
		emit canHaveSeriesChanged();
	}
}



//-------------------------------------------------------------------------------------------------------------------

bool ExerciseModel::save(){
	if( mInvalidName || mInvalidDescription) return false;

	DatabaseQuery query;
	query.begin(mExerciseId == 0 ? DatabaseQuery::Insert : DatabaseQuery::Update, mDbTable);
	query.add("name", mName);
	query.add("description", mDescription);
	query.add("type", mType);
	query.add("rest_time", mRestTime);
	query.add("work_time", mWorkTime);
	int exerciseId = mExerciseId;
	query.addConditionnal("exercise_id", exerciseId);

	switch(mType){
		case 1: case 2:{
			QStringList parser;
			for(auto i : mData){
				parser << QString::number(i->getValue().toInt());
			}
			query.add("data", parser.join(','));
		}
		break;
		default: {}
	}
	saveValues(query);
	bool result = query.exec();
	if(!result) qCritical() << "Cannot " << (mExerciseId == 0 ? "save" : "update") << "exercise: " << query.mQuery.lastError().text();
	else {
		if(mExerciseId == 0){
			auto idField = query.mQuery.record().indexOf("exercise_id");
			query.mQuery.next();
			setExerciseId(query.mQuery.value(idField).toInt());
		}
		for(auto serie : mSeries) {
			serie->save();
		}
		clearBackupValues();
	}


	return result;
}

void ExerciseModel::saveValues(DatabaseQuery &query){
}

ExerciseModel *ExerciseModel::load(QSqlQuery &query, QObject * parent) {
	ExerciseModel * model = new ExerciseModel(parent);
	load(model, query);
	return model;
}
void ExerciseModel::load(ExerciseModel * model, QSqlQuery &query) {
// TODO optimize
	auto exerciseIdField = query.record().indexOf("exercise_id");
	auto nameField = query.record().indexOf("name");
	auto descriptionField = query.record().indexOf("description");
	auto typeField = query.record().indexOf("type");
	auto restTimeField = query.record().indexOf("rest_time");
	auto workTimeField = query.record().indexOf("work_time");
	auto dataField = query.record().indexOf("data");

	model->setType(query.value(typeField).toInt());
	model->setExerciseId(query.value(exerciseIdField).toInt());
	model->setName(query.value(nameField).toString());
	model->setDescription(query.value(descriptionField).toString());
	model->setWorkTime(query.value(workTimeField).toInt());
	model->setRestTime(query.value(restTimeField).toInt());

	model->setDbData(query.value(dataField));
	model->clearBackupValues();
}

QList<ExerciseModel*> ExerciseModel::loadAll(QObject * parent) {
	QList<ExerciseModel*> exercises;

	QSqlQuery query( "SELECT * FROM exercises ORDER BY exercise_id ASC");

	QStringList ids;

	while (query.next()) {
		auto model = load(query, parent);
		qint64 id = model->getExerciseId();
		ids << QString::number(id);
		exercises << model;
	}
	if(ids.size() == 0) return exercises;
	if(!query.exec("SELECT * FROM exercise_series WHERE exercise_id IN(" + ids.join(",") + ") ORDER BY exercise_id ASC, serie_order ASC"))
		 qCritical() << "Cannot select exercise series: "  << query.lastError().text();
	auto exerciseIdField = query.record().indexOf("exercise_id");
	auto currentModel = exercises.begin();
	while (query.next()) {
		if(query.value(exerciseIdField).toInt() != (*currentModel)->getExerciseId()) ++currentModel;
		(*currentModel)->addSerie(SerieModel::load(query, *currentModel), true);
	}

	return exercises;
}

void ExerciseModel::remove(){
	if(mExerciseId > 0){
		DatabaseQuery query;
		query.begin(DatabaseQuery::Delete, mDbTable);
		query.addConditionnal("exercise_id",mExerciseId);
		if(!query.exec()){
			if(!query.exec()) qCritical() << "Cannot delete exercise : "  << query.mQuery.lastError().text();
		}
	}
	emit removed(this);
}
