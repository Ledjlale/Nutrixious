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

#include "ExerciseUnitModel.h"
#include "src/tool/Utils.h"
#include <QVariantList>
#include <QSqlRecord>
#include <QSqlError>
#include <QEvent>
//#include <QDynamicPropertyChange>

#include <QQmlApplicationEngine>
extern QQmlApplicationEngine * gEngine;


ExerciseUnitModel::ExerciseUnitModel()
	: ExerciseUnitModel{nullptr} {
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}

ExerciseUnitModel::ExerciseUnitModel(QObject *parent)
	: QmlModel{parent}
{
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE

	connect(this, &ExerciseUnitModel::exerciseUnitIdChanged, [this](){
		for(auto &i: mSeries) i->setExerciseUnitId(mExerciseUnitId);
		updateIsSaved();
	});
	setExerciseModel(new ExerciseModel(this));
	updateIsSaved();
}


ExerciseUnitModel::ExerciseUnitModel(const ExerciseUnitModel * model, QObject *parent) : ExerciseUnitModel( parent){
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	mParentId = initiBackup(model, &model->mParentId, model->mParentId, &mParentId).toLongLong();
	mExerciseUnitId = initiBackup(model, &model->mExerciseUnitId, model->mExerciseUnitId, &mExerciseUnitId).toLongLong();

	mDescription = initiBackup(model, &model->mDescription, model->mDescription, &mDescription).toString();

	mUseDistance = initiBackup(model, &model->mUseDistance, model->mUseDistance, &mUseDistance).toBool();
	mUseSpeed = initiBackup(model, &model->mUseSpeed, model->mUseSpeed, &mUseSpeed).toBool();
	mUseWeight = initiBackup(model, &model->mUseWeight, model->mUseWeight, &mUseWeight).toBool();
	mUseRepetitions = initiBackup(model, &model->mUseRepetitions, model->mUseRepetitions, &mUseRepetitions).toBool();

	mOrder = initiBackup(model, &model->mOrder, model->mOrder, &mOrder).toInt();
	mExercise.first = initiBackup(model, &model->mExercise.first, model->mExercise.first, &mExercise.first).toLongLong();

	if( model->getExerciseModel())
		setExerciseModel(model->getExerciseModel()->clone(this));
	else
		mExercise = model->mExercise;
	connect(this, &ExerciseUnitModel::exerciseUnitIdChanged, [this](){
		for(auto &i: mSeries) i->setExerciseUnitId(mExerciseUnitId);
		updateIsSaved();
	});

	updateIsSaved();
}

ExerciseUnitModel::~ExerciseUnitModel(){
}

ExerciseUnitModel * ExerciseUnitModel::clone(QObject *parent)const{
	ExerciseUnitModel *model = new ExerciseUnitModel(this, parent);

	for(auto i : model->getSeries())
		model->insertSerie(i->clone(model));

	model->clearBackupValues();
	return model;
}

void ExerciseUnitModel::initRandomValues(){
	for(auto i : mSeries)
		i->initRandomValues();
}

qint64 ExerciseUnitModel::getExerciseUnitId()const{
	return mExerciseUnitId;
}

void ExerciseUnitModel::setExerciseUnitId(qint64 id) {
	if(mExerciseUnitId != id){
		mExerciseUnitId = id;
		emit exerciseUnitIdChanged();
	}
}

qint64 ExerciseUnitModel::getExerciseId()const{
	return mExercise.second ? mExercise.second->getExerciseId() : mExercise.first;
}

void ExerciseUnitModel::setExerciseId(qint64 id) {
	if(mExercise.first != id){
		addBackup(&mExercise.first, mExercise.first, id);
		mExercise.first = id;
		emit exerciseIdChanged();
	}
}

ExerciseModel * ExerciseUnitModel::getExerciseModel()const{
	return mExercise.second;
}

void ExerciseUnitModel::setExerciseModel(ExerciseModel *model) {
	if(mExercise.second != model){
		if(mExercise.second) mExercise.second->deleteLater();
		mExercise.second = model;
		if( mExercise.second) connect(mExercise.second, &ExerciseModel::exerciseIdChanged, [this](){setExerciseId(mExercise.second->getExerciseId());});
		if(model)
			setExerciseId(model->getExerciseId());
		emit exerciseModelChanged();
	}
}


qint64 ExerciseUnitModel::getParentId()const{
	return mParentId;
}

void ExerciseUnitModel::setParentId(qint64 id) {
	if(mParentId != id){
		mParentId = id;
		emit parentIdChanged();
	}
}

DEFINE_GETSET(ExerciseUnitModel,QString,description,Description)
DEFINE_GETSET(ExerciseUnitModel,bool,useDistance,UseDistance)
DEFINE_GETSET(ExerciseUnitModel,bool,useSpeed,UseSpeed)
DEFINE_GETSET(ExerciseUnitModel,bool,useWeight,UseWeight)
DEFINE_GETSET(ExerciseUnitModel,bool,useRepetitions,UseRepetitions)

int ExerciseUnitModel::getOrder() const {
	return mOrder;
}

void ExerciseUnitModel::setOrder(int data){
	if(mOrder != data){
		addBackup(&mOrder, mOrder, data);
		mOrder = data;
		emit orderChanged();
	}
}

void ExerciseUnitModel::updateIsSaved() {
	setIsSaved(getExerciseUnitId() > 0);
}

void ExerciseUnitModel::undo(){
	if(mBackupValues.contains(&mDescription)) mDescription = mBackupValues[&mDescription].toString();
	if(mBackupValues.contains(&mUseDistance)) mUseDistance = mBackupValues[&mUseDistance].toBool();
	if(mBackupValues.contains(&mUseSpeed)) mUseSpeed = mBackupValues[&mUseSpeed].toBool();
	if(mBackupValues.contains(&mUseWeight)) mUseWeight = mBackupValues[&mUseWeight].toBool();
	if(mBackupValues.contains(&mUseRepetitions)) mUseRepetitions = mBackupValues[&mUseRepetitions].toBool();

	QmlModel::undo();
}

//-------------------------------------------------------------------------------------------------------------------

QVariantList ExerciseUnitModel::getVariantSeries() const {
	QVariantList series;
	for(auto serie : mSeries){
		series << QVariant::fromValue(serie);
	 }
	return series;
}

QList<ExerciseSerieModel*> ExerciseUnitModel::getSeries() const{
	return mSeries;
}

void ExerciseUnitModel::addNewSerie(ExerciseSerieModel *model) {
	insertSerie(model->clone(this))->makeNew();
}

ExerciseSerieModel *ExerciseUnitModel::insertSerie(ExerciseSerieModel *model) {
	auto insertedModel = Utils::add<ExerciseSerieModel>(model, mSeries);
	insertedModel->setExerciseUnitId(mExerciseUnitId);
	connect(insertedModel, &ExerciseSerieModel::removed, this, &ExerciseUnitModel::handleRemoved);
	emit seriesChanged();
	return insertedModel;
}

void ExerciseUnitModel::makeNew(){
	setExerciseUnitId(0);
	for(auto i : mSeries) i->makeNew();
	updateIsSaved();
}

void ExerciseUnitModel::removeSerie(ExerciseSerieModel *model) {
	mSeries.removeOne(model);
	model->deleteLater();
	emit seriesChanged();
}

void ExerciseUnitModel::handleRemoved(ExerciseSerieModel *model){
	auto it = std::find(mSeries.begin(), mSeries.end(), model);
	if( it != mSeries.end()){
		int row = it - mSeries.begin();
		mSeries.erase(it);
		emit seriesChanged();
	}
}

void ExerciseUnitModel::decrementSerieOrder(ExerciseSerieModel *model) {
	if(Utils::decrementOrder<ExerciseSerieModel>(model, mSeries)){
		emit seriesChanged();
		save();
	}
}
void ExerciseUnitModel::incrementSerieOrder(ExerciseSerieModel *model){
	if(Utils::incrementOrder<ExerciseSerieModel>(model, mSeries)){
		emit seriesChanged();
		save();
	}
}

bool ExerciseUnitModel::getCanHaveSeries() const {
	return mCanHaveSeries;
}

void ExerciseUnitModel::setCanHaveSeries(bool data) {
	if(mCanHaveSeries != data){
		mCanHaveSeries = data;
		emit canHaveSeriesChanged();
	}
}



//-------------------------------------------------------------------------------------------------------------------

int ExerciseUnitModel::save(){
	DatabaseQuery query;

	if(!mExercise.second) return false;
	mExercise.second->save();

	if( mExerciseUnitId > 0 && !getIsEdited()){
		bool saved = false;
		for(auto e : mSeries) {
			saved = saved || e->save();
		}
		return saved;
	}

	query.begin(mExerciseUnitId == 0 ? DatabaseQuery::Insert : DatabaseQuery::Update, mTablePrefix+"_exercise_units");
	query.add("description", mDescription);
	query.add("use_distance", mUseDistance);
	query.add("use_speed", mUseSpeed);
	query.add("use_weight", mUseWeight);
	query.add("use_repetitions", mUseRepetitions);
	query.add("exercise_order", mOrder);
	query.add(mTablePrefix+"_id", mParentId);
	query.add("exercise_id", mExercise.second->getExerciseId());
	addQueryValues(query);
	query.addConditionnal(mTablePrefix+"_exercise_unit_id", mExerciseUnitId);

	bool result = query.exec();
	if(!result) qCritical() << "Cannot" << (mExerciseUnitId == 0 ? "save" : "update") << mTablePrefix+" exercise: " << query.mQuery.lastError().text();
	else {
		if(mExerciseUnitId == 0){
			auto idField = query.mQuery.record().indexOf(mTablePrefix+"_exercise_unit_id");
			query.mQuery.next();
			setExerciseUnitId(query.mQuery.value(idField).toInt());
		}
		for(auto serie : mSeries) {
			serie->save();
		}
		clearBackupValues();
	}

	return result;
}

void ExerciseUnitModel::saveValues(DatabaseQuery &query){
}

void ExerciseUnitModel::load(QSqlQuery &query){
// TODO optimize
	auto exerciseUnitIdField = query.record().indexOf(mTablePrefix+"_exercise_units."+mTablePrefix+"_exercise_unit_id");
	auto descriptionField = query.record().indexOf(mTablePrefix+"_exercise_units.description");
	auto useDistanceField = query.record().indexOf(mTablePrefix+"_exercise_units.use_distance");
	auto useSpeedField = query.record().indexOf(mTablePrefix+"_exercise_units.use_speed");
	auto useWeightField = query.record().indexOf(mTablePrefix+"_exercise_units.use_weight");
	auto useRepetitionsField = query.record().indexOf(mTablePrefix+"_exercise_units.use_repetitions");
	auto orderField = query.record().indexOf(mTablePrefix+"_exercise_units.exercise_order");
	auto parentIdField = query.record().indexOf(mTablePrefix+"_exercise_units."+mTablePrefix+"_id");
	auto exerciseIdField = query.record().indexOf(mTablePrefix+"_exercise_units.exercise_id");

	setExerciseModel(ExerciseModel::build(query, this));
	setUseDistance(query.value(useDistanceField).toBool());
	setUseSpeed(query.value(useSpeedField).toBool());
	setUseWeight(query.value(useWeightField).toBool());
	setUseRepetitions(query.value(useRepetitionsField).toBool());
	setExerciseUnitId(query.value(exerciseUnitIdField).toInt());
	setDescription(query.value(descriptionField).toString());
	setOrder(query.value(orderField).toInt());

	setParentId(query.value(parentIdField).toInt());
	setExerciseId(query.value(exerciseIdField).toInt());

	clearBackupValues();
}

void ExerciseUnitModel::remove(){
	if(mExerciseUnitId > 0){
		DatabaseQuery query;
		query.begin(DatabaseQuery::Delete, mTablePrefix+"_exercise_units");
		query.addConditionnal(mTablePrefix+"_exercise_unit_id",mExerciseUnitId);
		if(!query.exec()){
			if(!query.exec()) qCritical() << "Cannot delete exercise : "  << query.mQuery.lastError().text();
		}
	}else
		qDebug() << "Cannot delete an exercise because id is not positive : " << mExerciseUnitId;
	emit removed(this);
}
