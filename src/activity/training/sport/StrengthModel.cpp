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

#include "StrengthModel.h"

#include <QObject>
#include <QVector>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>

#include "StrengthWorkModel.h"
#include "src/activity/description/sport/StrengthModel.h"
#include "src/database/DatabaseQuery.h"

using namespace Training;

StrengthModel::StrengthModel(QObject *parent) : ExerciseModel(parent){
	mName = "Strength";
	mInvalidName = false;
	mType = 3;
	connect(this, &StrengthModel::setsChanged, [this](){
		setInvalidSets( mSets.size() == 0);
	});
}
StrengthModel::StrengthModel(const StrengthModel * model, QObject *parent) : ExerciseModel(model, parent){
	for(auto set : model->getSets()) {
		auto newSet = set->clone(mTrainId, mDbId);
		connect(newSet, &StrengthWorkModel::finished, this, &StrengthModel::nextSet);
		mSets << newSet;
	}
	setInvalidSets( mSets.size() == 0);
	mType = 3;
}

StrengthModel::~StrengthModel(){
	for(auto set : mSets)
		set->deleteLater();
}

ExerciseModel * StrengthModel::clone(qint64 trainId)const{
	StrengthModel *model = new StrengthModel(this, nullptr);
	model->setTrainId(trainId);
	return model;
}

void StrengthModel::setTargetExercise(Description::ExerciseModel * data){
	ExerciseModel::setTargetExercise(data);
	if(data){
		auto model = dynamic_cast<Description::StrengthModel*>(data);
		mSets.clear();
		for(auto set : dynamic_cast<Description::StrengthModel*>(mTargetExercise)->getSets()){
			auto newSet = set->cloneTraining(mTrainId, mDbId);
			connect(newSet, &StrengthWorkModel::finished, this, &StrengthModel::nextSet);
			mSets << newSet;
		}
		setInvalidSets( mSets.size() == 0);
		emit setsChanged();
	}
}


bool StrengthModel::save() {
	if(!ExerciseModel::save() || mInvalidSets) return false;
	qDebug() << "Saving " << mName << mDescription;

	DatabaseQuery query;

	query.begin(mDbId == 0 ? DatabaseQuery::Insert : DatabaseQuery::Update, "tr_ex_strength");
	query.add("train_id", getTrainId());
	query.add("train_order", getTrainOrder());
	query.add("name", mName);
	query.add("description", mDescription);
	query.addConditionnal("id",mDbId);
	if(mDbId == 0){
		if(!query.exec()) qCritical() << "Cannot save train strength : "  << query.mQuery.lastError().text();
		auto fieldNo = query.mQuery.record().indexOf("id");
		while (query.mQuery.next()) {
			setId(query.mQuery.value(fieldNo).toInt());
			qDebug() << "Insert train strength exercise: " << mDbId;
		}
		for(auto set : mSets){
			set->save();
		}
	}else{
		if(!query.exec()) qCritical() << "Cannot update train strength : "  << query.mQuery.lastError().text();
		else {
			qDebug() << "Update train strength exercise: " << mDbId;
			for(auto set : mSets){
				set->save();
			}
		}
	}

	return true;
}

QList<ExerciseModel*> StrengthModel::load(){
	QList<ExerciseModel*> models;
	QSqlQuery query( "SELECT * FROM tr_ex_strength ORDER BY id ASC");
	QStringList ids;

	while (query.next()) {
		auto model = load(query);
		models << model;
		ids << QString::number(model->getId());
	}

	query.exec("SELECT * FROM tr_ex_strength_set WHERE strength_id IN(" + ids.join(",") + ") ORDER BY strength_id ASC");
	auto idField = query.record().indexOf("strength_id");
	auto currentModel = models.begin();
	while (query.next()) {
		if(query.value(idField).toInt() != (*currentModel)->getId()) ++currentModel;
		dynamic_cast<StrengthModel*>(*currentModel)->addSet(StrengthWorkModel::load(query), true);
	}
	return models;
}

StrengthModel *StrengthModel::load(QSqlQuery &query) {
	StrengthModel * model = new StrengthModel();
// TODO optimize
	auto idField = query.record().indexOf("id");
	auto nameField = query.record().indexOf("name");
	auto descriptionField = query.record().indexOf("description");
	auto trainIdField = query.record().indexOf("train_id");
	auto trainOrderField = query.record().indexOf("train_order");
	model->setId(query.value(idField).toInt());
	model->setName(query.value(nameField).toString());
	model->setDescription(query.value(descriptionField).toString());
	if(trainIdField>=0){
		model->setTrainId(query.value(trainIdField).toInt());
	}
	if(trainOrderField>=0){
		model->setTrainOrder(query.value(trainOrderField).toInt());
	}
	return model;
}

//-------------------------------------------------------------------------------------------------------------------

QVariantList StrengthModel::getVariantSets() const {
	QVariantList sets;
	for(auto set : mSets){
		sets << QVariant::fromValue(set);
	 }
	return sets;
}

QList<StrengthWorkModel*> StrengthModel::getSets() const{
	return mSets;
}

void StrengthModel::addSet(StrengthWorkModel *model, bool keepId) {
	mSets.push_back(model->clone(mTrainId, mDbId));
	if(keepId) {
		mSets.back()->setId(model->getId());
	}
	emit setsChanged();
}

void StrengthModel::removeSet(StrengthWorkModel *model) {
	mSets.removeOne(model);
	model->deleteLater();
	emit setsChanged();
}

void StrengthModel::startWork(){
	mStart = QDateTime::currentDateTime();
	setIsRunning(true);
	setIsResting(false);
	setIsDone(false);
	mCurrentSet = mSets.begin();
	(*mCurrentSet)->startWork();
}

void StrengthModel::nextSet(){
	if(++mCurrentSet == mSets.end())
		emit finished();
	else
		(*mCurrentSet)->startWork();
}

void StrengthModel::endOfCurrentWork(){
	(*mCurrentSet)->endOfCurrentWork();
}

void StrengthModel::endOfCurrentRest() {
	(*mCurrentSet)->endOfCurrentRest();
}


QVariant StrengthModel::getCurrentWork() {
	return QVariant::fromValue(*mCurrentSet);
}

//-------------------------------------------------------------------------------------------------------------------


void StrengthModel::setInvalidSets(bool invalid){
	if(mInvalidSets != invalid){
		mInvalidSets = invalid;
		emit invalidSetsChanged();
	}
}

//-------------------------------------------------------------------------------------------------------------------
