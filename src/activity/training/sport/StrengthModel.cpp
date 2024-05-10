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
		auto newSet = set->clone();
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

ExerciseModel * StrengthModel::clone()const{
	StrengthModel *model = new StrengthModel(this, nullptr);
	return model;
}

void StrengthModel::setTargetExercise(Description::ExerciseModel * data){
	ExerciseModel::setTargetExercise(data);
	if(data){
		auto model = dynamic_cast<Description::StrengthModel*>(data);
		mSets.clear();
		for(auto set : dynamic_cast<Description::StrengthModel*>(mTargetExercise)->getSets()){
			auto newSet = set->cloneTraining();
			connect(newSet, &StrengthWorkModel::finished, this, &StrengthModel::nextSet);
			mSets << newSet;
		}
		setInvalidSets( mSets.size() == 0);
		emit setsChanged();
	}
}



bool StrengthModel::save() {
/*
	if(!ExerciseModel::save() || mInvalidSets) return false;
	qDebug() << "Saving " << mName << mDescription;
	QSqlQuery query;
	if(mDbId == 0){
		query.prepare("INSERT INTO ex_strength(name,description) VALUES(?,?) RETURNING id");
		query.addBindValue(mName);
		query.addBindValue(mDescription);
		query.exec();
		auto fieldNo = query.record().indexOf("id");
		while (query.next()) {
			mDbId = query.value(fieldNo).toInt();
			qDebug() << "Insert strength exercise : " << mDbId;
		}
		for(auto set : mSets){
			set->save(mDbId);
		}
	}else{
		query.prepare("UPDATE ex_strength SET name=?, description=? WHERE id=?");
		query.addBindValue(mName);
		query.addBindValue(mDescription);
		query.addBindValue(mDbId);
		qDebug() << "Update strength exercise : " << mDbId << query.exec();
		for(auto set : mSets){
			set->save(mDbId);
		}
	}
	return true;
	*/
	return false;
}

bool StrengthModel::saveProgram(qint64 programId){
/*
	if(!ExerciseModel::saveProgram(programId) || mInvalidSets) return false;
	qDebug() << "Saving " << mName << mDescription;
	QSqlQuery query;
	if(mDbId == 0){
		query.prepare("INSERT INTO pgrm_ex_strength(program_id, name,description,program_order) VALUES(?,?,?,?) RETURNING id");
		query.addBindValue(programId);
		query.addBindValue(mName);
		query.addBindValue(mDescription);
		query.addBindValue(mProgramOrder);
		query.exec();
		auto fieldNo = query.record().indexOf("id");
		while (query.next()) {
			mDbId = query.value(fieldNo).toInt();
			qDebug() << "Insert program strength exercise : " << mDbId << " at " << mProgramOrder;
		}
		for(auto set : mSets){
			set->saveProgram(programId, mDbId);
		}
	}else{
		query.prepare("UPDATE pgrm_ex_strength SET name=?, description=?, program_id=?, program_order=? WHERE id=?");
		query.addBindValue(mName);
		query.addBindValue(mDescription);
		query.addBindValue(programId);
		query.addBindValue(mProgramOrder);
		query.addBindValue(mDbId);
		qDebug() << "Update strength exercise : " << mDbId << " at " << mProgramOrder << query.exec();
		for(auto set : mSets){
			set->saveProgram(programId, mDbId);
		}
	}
	return true;
	*/
	return false;
}

QList<ExerciseModel*> StrengthModel::load(){
	QList<ExerciseModel*> models;
	/*
	QSqlQuery query( "SELECT * FROM ex_strength ORDER BY id ASC");

	auto idField = query.record().indexOf("id");
	auto nameField = query.record().indexOf("name");
	auto descriptionField = query.record().indexOf("description");
	QStringList ids;

	while (query.next()) {
		StrengthModel * model = new StrengthModel();
		qint64 id = query.value(idField).toInt();
		ids << QString::number(id);
		model->setId(id);
		model->setName(query.value(nameField).toString());
		model->setDescription(query.value(descriptionField).toString());
		models << model;
	}

	query.exec("SELECT * FROM ex_strength_set WHERE strength_id IN(" + ids.join(",") + ") ORDER BY strength_id ASC");
	idField = query.record().indexOf("strength_id");
	auto currentModel = models.begin();
	while (query.next()) {
		if(query.value(idField).toInt() != (*currentModel)->getId()) ++currentModel;
		dynamic_cast<StrengthModel*>(*currentModel)->addSet(StrengthWorkModel::load(query), true);
	}*/
	return models;
}

StrengthModel *StrengthModel::load(QSqlQuery &query) {
/*
	StrengthModel * model = new StrengthModel();
// TODO optimize
	auto idField = query.record().indexOf("id");
	auto nameField = query.record().indexOf("name");
	auto descriptionField = query.record().indexOf("description");
	auto programOrderField = query.record().indexOf("program_order");
	model->setId(query.value(idField).toInt());
	model->setName(query.value(nameField).toString());
	model->setDescription(query.value(descriptionField).toString());
	if(programOrderField>=0){
		model->setProgramOrder(query.value(programOrderField).toInt());
	}
	return model;
	*/
	return nullptr;
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

void StrengthModel::addSet(StrengthWorkModel *model) {
	mSets.push_back(model->clone());
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
