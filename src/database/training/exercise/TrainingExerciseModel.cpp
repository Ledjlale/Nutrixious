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

#include "TrainingExerciseModel.h"
#include "../serie/TrainingSerieModel.h"
#include "src/tool/Utils.h"
#include <QVariantList>
#include <QSqlRecord>
#include <QSqlError>
#include <QEvent>
//#include <QDynamicPropertyChange>

#include <QQmlApplicationEngine>
extern QQmlApplicationEngine * gEngine;


TrainingExerciseModel::TrainingExerciseModel()
	: TrainingExerciseModel{nullptr} {
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}

TrainingExerciseModel::TrainingExerciseModel(QObject *parent)
	: QmlModel{parent}
{
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE

	connect(this, &TrainingExerciseModel::exerciseIdChanged, [this](){
		for(auto &i: mSeries) i->setTrainingExerciseId(mTrainingExerciseId);
		updateIsSaved();
	});
}

TrainingExerciseModel::TrainingExerciseModel(int type, QObject * parent)
	: QmlModel{parent} {
	setType(type);
}


TrainingExerciseModel::TrainingExerciseModel(const TrainingExerciseModel * model, QObject *parent) : TrainingExerciseModel(model->getType(), parent){
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	mDescription = model->getDescription();
	mWorkTime = model->getWorkTime();
	mRestTime = model->getRestTime();
	mOrder = model->getOrder();
	if( model->getExerciseModel())
		setExerciseModel(model->getExerciseModel()->clone(this));
	else
		mExercise = model->mExercise;
	for(auto i : model->getSeries())
		mSeries << i->clone(this);
	setData(model->getData());
	clearBackupValues();
	connect(this, &TrainingExerciseModel::exerciseIdChanged, [this](){
		for(auto &i: mSeries) i->setTrainingExerciseId(mTrainingExerciseId);
		updateIsSaved();
	});
}

TrainingExerciseModel::TrainingExerciseModel(const ProgramExerciseModel * model, QObject *parent) : TrainingExerciseModel(model->getType(), parent){
	mDescription = model->getDescription();
	mWorkTime = model->getWorkTime();
	mRestTime = model->getRestTime();
	mOrder = model->getOrder();
	if(model->getExerciseModel()) setExerciseModel(model->getExerciseModel()->clone(this));
	else mExercise.first = model->getExerciseId();
	for(auto i : model->getSeries())
		mSeries << new TrainingSerieModel(i, this);
	setData(model->getData());
	connect(this, &TrainingExerciseModel::exerciseIdChanged, [this](){
		for(auto &i: mSeries) i->setTrainingExerciseId(mTrainingExerciseId);
		updateIsSaved();
	});
}

TrainingExerciseModel * TrainingExerciseModel::clone(QObject *parent)const{
	TrainingExerciseModel *model = new TrainingExerciseModel(this, parent);
	model->buildData();
	model->setData(this->getData());
	model->clearBackupValues();
	return model;
}

qint64 TrainingExerciseModel::getTrainingExerciseId()const{
	return mTrainingExerciseId;
}

void TrainingExerciseModel::setTrainingExerciseId(qint64 id) {
	if(mTrainingExerciseId != id){
		mTrainingExerciseId = id;
		emit exerciseIdChanged();
	}
}

qint64 TrainingExerciseModel::getExerciseId()const{
	return mExercise.second ? mExercise.second->getExerciseId() : mExercise.first;
}

void TrainingExerciseModel::setExerciseId(qint64 id) {
	if(mExercise.first != id){
		mExercise.first = id;
		emit exerciseIdChanged();
	}
}

ExerciseModel * TrainingExerciseModel::getExerciseModel()const{
	return mExercise.second;
}

void TrainingExerciseModel::setExerciseModel(ExerciseModel *model) {
	if(mExercise.second != model){
		mExercise.second = model;
		connect(mExercise.second, &ExerciseModel::exerciseIdChanged, [this](){setExerciseId(mExercise.second->getExerciseId());});
		setExerciseId(model->getExerciseId());
		emit exerciseModelChanged();
	}
}


qint64 TrainingExerciseModel::getTrainingId()const{
	return mTrainingId;
}

void TrainingExerciseModel::setTrainingId(qint64 id) {
	if(mTrainingId != id){
		mTrainingId = id;
		emit trainingIdChanged();
	}
}

QString TrainingExerciseModel::getDescription() const{
	return mDescription;
}

void TrainingExerciseModel::setDescription(QString data) {
	if(mDescription != data){
		addBackup(&mWorkTime, mWorkTime, data);
		mDescription = data;
		emit descriptionChanged();
	}
}

int TrainingExerciseModel::getType() const{
	return mType;
}

void TrainingExerciseModel::setType(int data){
	if(mType != data){
		mType = data;
		buildData();
		emit typeChanged();
	}
}

int TrainingExerciseModel::getWorkTime() const{
	return mWorkTime;
}
void TrainingExerciseModel::setWorkTime(int data){
	if( mWorkTime != data){
		addBackup(&mWorkTime, mWorkTime, data);
		mWorkTime = data;
		emit workTimeChanged();
	}
}

int TrainingExerciseModel::getRestTime() const{
	return mRestTime;
}

void TrainingExerciseModel::setRestTime(int data){
	if( mRestTime != data){
		addBackup(&mRestTime, mRestTime, data);
		mRestTime = data;
		emit restTimeChanged();
	}
}

int TrainingExerciseModel::getOrder() const {
	return mOrder;
}

void TrainingExerciseModel::setOrder(int data){
	if(mOrder != data){
		addBackup(&mOrder, mOrder, data);
		mOrder = data;
		emit orderChanged();
	}
}

void TrainingExerciseModel::setDbData(QVariant data){
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

const QList<QmlData*>& TrainingExerciseModel::getData()const {
	return mData;
}

void TrainingExerciseModel::setData(const QList<QmlData *> data) {
	if(data.size() == mData.size()){
		for(int i = 0 ; i < data.size() ; ++i)
			mData[i]->setValue(data[i]->getValue());
	}else
		qCritical() << "Cannot set Data : sizes are different. You must build them before.";
}

QVariantList TrainingExerciseModel::getQmlVariantData()const {
	QVariantList l;
	for(auto i : mData)
		l << QVariant::fromValue(i);
	return l;
}

void TrainingExerciseModel::buildData() {
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
	emit dataChanged();
}


void TrainingExerciseModel::updateIsSaved() {
	setIsSaved(getExerciseId() > 0);
}

void TrainingExerciseModel::undo(){
	if(mBackupValues.contains(&mDescription)) mDescription = mBackupValues[&mDescription].toString();
	if(mBackupValues.contains(&mRestTime)) mRestTime = mBackupValues[&mRestTime].toInt();
	if(mBackupValues.contains(&mWorkTime)) mWorkTime = mBackupValues[&mWorkTime].toInt();
	QmlModel::undo();
}

//-------------------------------------------------------------------------------------------------------------------

QVariantList TrainingExerciseModel::getVariantSeries() const {
	QVariantList series;
	for(auto serie : mSeries){
		series << QVariant::fromValue(serie);
	 }
	return series;
}

QList<TrainingSerieModel*> TrainingExerciseModel::getSeries() const{
	return mSeries;
}

void TrainingExerciseModel::addSerie(TrainingSerieModel *model, bool keepId) {
	auto insertedModel = Utils::add<TrainingSerieModel>(model, this, mSeries);
	insertedModel->setTrainingExerciseId(mTrainingExerciseId);
	connect(insertedModel, &TrainingSerieModel::removed, this, &TrainingExerciseModel::handleRemoved);
	if(keepId) {
		insertedModel->setTrainingSerieId(model->getTrainingSerieId());
	}
	emit seriesChanged();
}

void TrainingExerciseModel::removeSerie(TrainingSerieModel *model) {
	mSeries.removeOne(model);
	model->deleteLater();
	emit seriesChanged();
}

void TrainingExerciseModel::handleRemoved(TrainingSerieModel *model){
	auto it = std::find(mSeries.begin(), mSeries.end(), model);
	if( it != mSeries.end()){
		int row = it - mSeries.begin();
		mSeries.erase(it);
		emit seriesChanged();
	}
}

void TrainingExerciseModel::decrementSerieOrder(TrainingSerieModel *model) {
	if(Utils::decrementOrder<TrainingSerieModel>(model, mSeries)){
		emit seriesChanged();
		save();
	}
}
void TrainingExerciseModel::incrementSerieOrder(TrainingSerieModel *model){
	if(Utils::incrementOrder<TrainingSerieModel>(model, mSeries)){
		emit seriesChanged();
		save();
	}
}

bool TrainingExerciseModel::getCanHaveSeries() const {
	return mCanHaveSeries;
}

void TrainingExerciseModel::setCanHaveSeries(bool data) {
	if(mCanHaveSeries != data){
		mCanHaveSeries = data;
		emit canHaveSeriesChanged();
	}
}



//-------------------------------------------------------------------------------------------------------------------

bool TrainingExerciseModel::save(){
	DatabaseQuery query;

	if(!mExercise.second) return false;
	mExercise.second->save();

	if( mTrainingExerciseId > 0 && !getIsEdited()){
		bool saved = false;
		for(auto serie : mSeries) {
			saved = saved || serie->save();
		}
		return saved;
	}
	query.begin(mTrainingExerciseId == 0 ? DatabaseQuery::Insert : DatabaseQuery::Update, "tr_exercises");
	query.add("description", mDescription);
	query.add("type", mType);
	query.add("rest_time", mRestTime);
	query.add("work_time", mWorkTime);
	query.add("exercise_order", mOrder);
	query.add("training_id", mTrainingId);
	query.add("exercise_id", mExercise.second->getExerciseId());
	query.addConditionnal("tr_exercise_id", mTrainingExerciseId);

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
	bool result = query.exec();
	if(!result) qCritical() << "Cannot" << (mTrainingExerciseId == 0 ? "save" : "update") << "training exercise: " << query.mQuery.lastError().text();
	else {
		if(mTrainingExerciseId == 0){
			auto idField = query.mQuery.record().indexOf("tr_exercise_id");
			query.mQuery.next();
			setTrainingExerciseId(query.mQuery.value(idField).toInt());
		}
		for(auto serie : mSeries) {
			serie->save();
		}
		clearBackupValues();
	}


	return result;
}

void TrainingExerciseModel::saveValues(DatabaseQuery &query){
}

TrainingExerciseModel *TrainingExerciseModel::load(QSqlQuery &query, QObject * parent) {
	TrainingExerciseModel * model = new TrainingExerciseModel(parent);
	load(model, query);
	return model;
}
void TrainingExerciseModel::load(TrainingExerciseModel * model, QSqlQuery &query) {
// TODO optimize
	auto trainingExerciseIdField = query.record().indexOf("tr_exercises.tr_exercise_id");
	auto descriptionField = query.record().indexOf("tr_exercises.description");
	auto typeField = query.record().indexOf("tr_exercises.type");
	auto restTimeField = query.record().indexOf("tr_exercises.rest_time");
	auto workTimeField = query.record().indexOf("tr_exercises.work_time");
	auto dataField = query.record().indexOf("tr_exercises.data");
	auto orderField = query.record().indexOf("tr_exercises.exercise_order");
	auto trainingIdField = query.record().indexOf("tr_exercises.training_id");
	auto exerciseIdField = query.record().indexOf("tr_exercises.exercise_id");

	model->setExerciseModel(ExerciseModel::load(query, model));
	model->setType(query.value(typeField).toInt());
	model->setTrainingExerciseId(query.value(trainingExerciseIdField).toInt());
	model->setDescription(query.value(descriptionField).toString());
	model->setWorkTime(query.value(workTimeField).toInt());
	model->setRestTime(query.value(restTimeField).toInt());


	model->setOrder(query.value(orderField).toInt());

	model->setTrainingId(query.value(trainingIdField).toInt());
	model->setExerciseId(query.value(exerciseIdField).toInt());



	model->setDbData(query.value(dataField));
	model->clearBackupValues();
}


QList<TrainingExerciseModel*> TrainingExerciseModel::loadAll(qint64 exerciseId, QObject * parent){
	QList<TrainingExerciseModel*> exercises;

	QSqlQuery query( "SELECT * FROM exercises " +(exerciseId > 0 ? QString("WHERE exercise_id=")+QString::number(exerciseId) : "") +"ORDER BY exercise_id ASC");

	QStringList ids;

	while (query.next()) {
		auto model = load(query, parent);
		qint64 id = model->getExerciseId();
		ids << QString::number(id);
		exercises << model;
	}
	if(ids.size() == 0) return exercises;
	if(!query.exec("SELECT * FROM tr_exercises WHERE tr_exercise_id IN(" + ids.join(",") + ") ORDER BY tr_exercise_id ASC, serie_order ASC"))
		 qCritical() << "Cannot select exercise series: "  << query.lastError().text();
	auto exerciseIdField = query.record().indexOf("tr_exercise_id");
	auto currentModel = exercises.begin();
	while (query.next()) {
		if(query.value(exerciseIdField).toInt() != (*currentModel)->getExerciseId()) ++currentModel;
		(*currentModel)->addSerie(TrainingSerieModel::load(query, *currentModel), true);
	}

	return exercises;
}

QList<TrainingExerciseModel*> TrainingExerciseModel::loadAll(QObject * parent) {
	return loadAll(0, parent);
}

void TrainingExerciseModel::remove(){
	if(mTrainingExerciseId > 0){
		DatabaseQuery query;
		query.begin(DatabaseQuery::Delete, "tr_exercises");
		query.addConditionnal("tr_exercise_id",mTrainingExerciseId);
		if(!query.exec()){
			if(!query.exec()) qCritical() << "Cannot delete exercise : "  << query.mQuery.lastError().text();
		}
	}
	emit removed(this);
}
