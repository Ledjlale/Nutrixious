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

#include "ProgramExerciseModel.h"
#include "../serie/ProgramSerieModel.h"
#include "src/tool/Utils.h"
#include <QVariantList>
#include <QSqlRecord>
#include <QSqlError>
#include <QEvent>
//#include <QDynamicPropertyChange>

#include <QQmlApplicationEngine>
extern QQmlApplicationEngine * gEngine;


ProgramExerciseModel::ProgramExerciseModel()
	: ProgramExerciseModel{nullptr} {
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}

ProgramExerciseModel::ProgramExerciseModel(QObject *parent)
	: QmlModel{parent}
{
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE

	connect(this, &ProgramExerciseModel::exerciseIdChanged, [this](){
		for(auto &i: mSeries) i->setProgramExerciseId(mProgramExerciseId);
		updateIsSaved();
	});
	setExerciseModel(new ExerciseModel(this));
}

ProgramExerciseModel::ProgramExerciseModel(int type, QObject * parent)
	: QmlModel{parent} {
	setType(type);
}


ProgramExerciseModel::ProgramExerciseModel(const ProgramExerciseModel * model, QObject *parent) : ProgramExerciseModel(model->getType(), parent){
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
	connect(this, &ProgramExerciseModel::exerciseIdChanged, [this](){
		for(auto &i: mSeries) i->setProgramExerciseId(mProgramExerciseId);
		updateIsSaved();
	});
}

ProgramExerciseModel * ProgramExerciseModel::clone(QObject *parent)const{
	ProgramExerciseModel *model = new ProgramExerciseModel(this, parent);
	model->buildData();
	model->setData(this->getData());
	model->clearBackupValues();
	return model;
}

qint64 ProgramExerciseModel::getProgramExerciseId()const{
	return mProgramExerciseId;
}

void ProgramExerciseModel::setProgramExerciseId(qint64 id) {
	if(mProgramExerciseId != id){
		mProgramExerciseId = id;
		emit exerciseIdChanged();
	}
}

qint64 ProgramExerciseModel::getExerciseId()const{
	return mExercise.second ? mExercise.second->getExerciseId() : mExercise.first;
}

void ProgramExerciseModel::setExerciseId(qint64 id) {
	if(mExercise.first != id){
		mExercise.first = id;
		emit exerciseIdChanged();
	}
}

ExerciseModel * ProgramExerciseModel::getExerciseModel()const{
	return mExercise.second;
}

void ProgramExerciseModel::setExerciseModel(ExerciseModel *model) {
	if(mExercise.second != model){
		if(mExercise.second) mExercise.second->deleteLater();
		mExercise.second = model;
		if( mExercise.second) connect(mExercise.second, &ExerciseModel::exerciseIdChanged, [this](){setExerciseId(mExercise.second->getExerciseId());});
		if(model)
			setExerciseId(model->getExerciseId());
		emit exerciseModelChanged();
	}
}


qint64 ProgramExerciseModel::getProgramId()const{
	return mProgramId;
}

void ProgramExerciseModel::setProgramId(qint64 id) {
	if(mProgramId != id){
		mProgramId = id;
		emit programIdChanged();
	}
}

QString ProgramExerciseModel::getDescription() const{
	return mDescription;
}

void ProgramExerciseModel::setDescription(QString data) {
	if(mDescription != data){
		addBackup(&mWorkTime, mWorkTime, data);
		mDescription = data;
		emit descriptionChanged();
	}
}

int ProgramExerciseModel::getType() const{
	return mType;
}

void ProgramExerciseModel::setType(int data){
	if(mType != data){
		mType = data;
		buildData();
		emit typeChanged();
	}
}

int ProgramExerciseModel::getWorkTime() const{
	return mWorkTime;
}
void ProgramExerciseModel::setWorkTime(int data){
	if( mWorkTime != data){
		addBackup(&mWorkTime, mWorkTime, data);
		mWorkTime = data;
		emit workTimeChanged();
	}
}

int ProgramExerciseModel::getRestTime() const{
	return mRestTime;
}

void ProgramExerciseModel::setRestTime(int data){
	if( mRestTime != data){
		addBackup(&mRestTime, mRestTime, data);
		mRestTime = data;
		emit restTimeChanged();
	}
}

int ProgramExerciseModel::getOrder() const {
	return mOrder;
}

void ProgramExerciseModel::setOrder(int data){
	if(mOrder != data){
		addBackup(&mOrder, mOrder, data);
		mOrder = data;
		emit orderChanged();
	}
}

void ProgramExerciseModel::setDbData(QVariant data){
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

const QList<QmlData*>& ProgramExerciseModel::getData()const {
	return mData;
}

void ProgramExerciseModel::setData(const QList<QmlData *> data) {
	if(data.size() == mData.size()){
		for(int i = 0 ; i < data.size() ; ++i)
			mData[i]->setValue(data[i]->getValue());
	}else
		qCritical() << "Cannot set Data : sizes are different. You must build them before.";
}

QVariantList ProgramExerciseModel::getQmlVariantData()const {
	QVariantList l;
	for(auto i : mData)
		l << QVariant::fromValue(i);
	return l;
}

void ProgramExerciseModel::buildData() {
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


void ProgramExerciseModel::updateIsSaved() {
	setIsSaved(getExerciseId() > 0);
}

void ProgramExerciseModel::undo(){
	if(mBackupValues.contains(&mDescription)) mDescription = mBackupValues[&mDescription].toString();
	if(mBackupValues.contains(&mRestTime)) mRestTime = mBackupValues[&mRestTime].toInt();
	if(mBackupValues.contains(&mWorkTime)) mWorkTime = mBackupValues[&mWorkTime].toInt();
	QmlModel::undo();
}

//-------------------------------------------------------------------------------------------------------------------

QVariantList ProgramExerciseModel::getVariantSeries() const {
	QVariantList series;
	for(auto serie : mSeries){
		series << QVariant::fromValue(serie);
	 }
	return series;
}

QList<ProgramSerieModel*> ProgramExerciseModel::getSeries() const{
	return mSeries;
}

void ProgramExerciseModel::addSerie(ProgramSerieModel *model, bool keepId) {
	auto insertedModel = Utils::add<ProgramSerieModel>(model, this, mSeries);
	insertedModel->setProgramExerciseId(mProgramExerciseId);
	connect(insertedModel, &ProgramSerieModel::removed, this, &ProgramExerciseModel::handleRemoved);
	if(keepId) {
		insertedModel->setProgramSerieId(model->getProgramSerieId());
	}
	emit seriesChanged();
}

void ProgramExerciseModel::removeSerie(ProgramSerieModel *model) {
	mSeries.removeOne(model);
	model->deleteLater();
	emit seriesChanged();
}

void ProgramExerciseModel::handleRemoved(ProgramSerieModel *model){
	auto it = std::find(mSeries.begin(), mSeries.end(), model);
	if( it != mSeries.end()){
		int row = it - mSeries.begin();
		mSeries.erase(it);
		emit seriesChanged();
	}
}

void ProgramExerciseModel::decrementSerieOrder(ProgramSerieModel *model) {
	if(Utils::decrementOrder<ProgramSerieModel>(model, mSeries)){
		emit seriesChanged();
		save();
	}
}
void ProgramExerciseModel::incrementSerieOrder(ProgramSerieModel *model){
	if(Utils::incrementOrder<ProgramSerieModel>(model, mSeries)){
		emit seriesChanged();
		save();
	}
}

bool ProgramExerciseModel::getCanHaveSeries() const {
	return mCanHaveSeries;
}

void ProgramExerciseModel::setCanHaveSeries(bool data) {
	if(mCanHaveSeries != data){
		mCanHaveSeries = data;
		emit canHaveSeriesChanged();
	}
}



//-------------------------------------------------------------------------------------------------------------------

bool ProgramExerciseModel::save(){
	DatabaseQuery query;

	if(!mExercise.second) return false;
	mExercise.second->save();

	query.begin(mProgramExerciseId == 0 ? DatabaseQuery::Insert : DatabaseQuery::Update, "prgm_exercises");
	query.add("description", mDescription);
	query.add("type", mType);
	query.add("rest_time", mRestTime);
	query.add("work_time", mWorkTime);
	query.add("exercise_order", mOrder);
	query.add("program_id", mProgramId);
	query.add("exercise_id", mExercise.second->getExerciseId());
	query.addConditionnal("prgm_exercise_id", mProgramExerciseId);

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
	if(!result) qCritical() << "Cannot" << (mProgramExerciseId == 0 ? "save" : "update") << "program exercise: " << query.mQuery.lastError().text();
	else {
		if(mProgramExerciseId == 0){
			auto idField = query.mQuery.record().indexOf("prgm_exercise_id");
			query.mQuery.next();
			setProgramExerciseId(query.mQuery.value(idField).toInt());
		}
		for(auto serie : mSeries) {
			serie->save();
		}
		clearBackupValues();
	}


	return result;
}

void ProgramExerciseModel::saveValues(DatabaseQuery &query){
}

ProgramExerciseModel *ProgramExerciseModel::load(QSqlQuery &query, QObject * parent) {
	ProgramExerciseModel * model = new ProgramExerciseModel(parent);
	load(model, query);
	return model;
}
void ProgramExerciseModel::load(ProgramExerciseModel * model, QSqlQuery &query) {
// TODO optimize
	auto programExerciseIdField = query.record().indexOf("prgm_exercises.prgm_exercise_id");
	auto descriptionField = query.record().indexOf("prgm_exercises.description");
	auto typeField = query.record().indexOf("prgm_exercises.type");
	auto restTimeField = query.record().indexOf("prgm_exercises.rest_time");
	auto workTimeField = query.record().indexOf("prgm_exercises.work_time");
	auto dataField = query.record().indexOf("prgm_exercises.data");
	auto orderField = query.record().indexOf("prgm_exercises.exercise_order");
	auto programIdField = query.record().indexOf("prgm_exercises.program_id");
	auto exerciseIdField = query.record().indexOf("prgm_exercises.exercise_id");

	model->setExerciseModel(ExerciseModel::load(query, model));
	model->setType(query.value(typeField).toInt());
	model->setProgramExerciseId(query.value(programExerciseIdField).toInt());
	model->setDescription(query.value(descriptionField).toString());
	model->setWorkTime(query.value(workTimeField).toInt());
	model->setRestTime(query.value(restTimeField).toInt());


	model->setOrder(query.value(orderField).toInt());

	model->setProgramId(query.value(programIdField).toInt());
	model->setExerciseId(query.value(exerciseIdField).toInt());



	model->setDbData(query.value(dataField));
	model->clearBackupValues();
}

QList<ProgramExerciseModel*> ProgramExerciseModel::loadAll(QObject * parent) {
	QList<ProgramExerciseModel*> exercises;

	QSqlQuery query( "SELECT * FROM exercises ORDER BY exercise_id ASC");

	QStringList ids;

	while (query.next()) {
		auto model = load(query, parent);
		qint64 id = model->getExerciseId();
		ids << QString::number(id);
		exercises << model;
	}
	if(ids.size() == 0) return exercises;
	if(!query.exec("SELECT * FROM prgm_exercises WHERE prgm_exercise_id IN(" + ids.join(",") + ") ORDER BY prgm_exercise_id ASC, serie_order ASC"))
		 qCritical() << "Cannot select exercise series: "  << query.lastError().text();
	auto exerciseIdField = query.record().indexOf("prgm_exercise_id");
	auto currentModel = exercises.begin();
	while (query.next()) {
		if(query.value(exerciseIdField).toInt() != (*currentModel)->getExerciseId()) ++currentModel;
		(*currentModel)->addSerie(ProgramSerieModel::load(query, *currentModel), true);
	}

	return exercises;
}

void ProgramExerciseModel::remove(){
	if(mProgramExerciseId > 0){
		DatabaseQuery query;
		query.begin(DatabaseQuery::Delete, "prgm_exercises");
		query.addConditionnal("prgm_exercise_id",mProgramExerciseId);
		if(!query.exec()){
			if(!query.exec()) qCritical() << "Cannot delete exercise : "  << query.mQuery.lastError().text();
		}
	}
	emit removed(this);
}
