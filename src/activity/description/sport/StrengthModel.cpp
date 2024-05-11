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
#include <QSqlError>

#include "src/database/DatabaseQuery.h"

#include "StrengthWorkModel.h"
#include "src/activity/training/sport/StrengthModel.h"

#include <QQmlApplicationEngine>
extern QQmlApplicationEngine * gEngine;

using namespace Description;

StrengthModel::StrengthModel() : StrengthModel(nullptr){
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}
StrengthModel::StrengthModel(QObject *parent) : ExerciseModel(parent){
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	mName = "Strength";
	mInvalidName = false;
	connect(this, &StrengthModel::setsChanged, [this](){
		setInvalidSets( mSets.size() == 0);
	});
	connect(this, &ExerciseModel::exerciseIdChanged, [this](){
		for(auto &i: mSets) i->setStrengthId(mExerciseId);
	});
	connect(this, &ExerciseModel::programIdChanged, [this](){
		for(auto &i: mSets) i->setProgramId(mProgramId);
	});
}

StrengthModel::StrengthModel(const StrengthModel * model, QObject *parent) : ExerciseModel(model, parent){
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	for(auto set : model->getSets()) {
		mSets << set->clone(mExerciseId, this);
	}
	connect(this, &ExerciseModel::exerciseIdChanged, [this](){
		for(auto &i: mSets) i->setStrengthId(mExerciseId);
	});
	connect(this, &ExerciseModel::programIdChanged, [this](){
		for(auto &i: mSets) i->setProgramId(mProgramId);
	});
	setInvalidSets( mSets.size() == 0);
}

StrengthModel::~StrengthModel(){
	for(auto set : mSets)
		set->deleteLater();
}

ExerciseModel * StrengthModel::clone(qint64 programId, QObject *parent)const{
	StrengthModel *model = new StrengthModel(this, parent);
	model->setProgramId(programId);
	return model;
}

Training::ExerciseModel * StrengthModel::cloneTraining(qint64 trainId, QObject *parent){
	Training::StrengthModel * model = new Training::StrengthModel(parent);
	model->setTargetExercise(this);
	model->setTrainId(trainId);
	model->setTrainOrder(getProgramOrder());
	return model;
}

int StrengthModel::getType() const{
	return 3;
}

bool StrengthModel::save() {
	if(!ExerciseModel::save() || mInvalidSets) return false;
	qDebug() << "Saving " << mName << mDescription;

	DatabaseQuery query;

	query.begin(mExerciseId == 0 ? DatabaseQuery::Insert : DatabaseQuery::Update, isProgramLinked() ? "pgrm_ex_strength" : "ex_strength");
	if(isProgramLinked()){
		query.add("program_id", getProgramId());
		query.add("program_order", getProgramOrder());
	}
	query.add("name", mName);
	query.add("description", mDescription);
	query.addConditionnal("id",mExerciseId);
	if(mExerciseId == 0){
		if(!query.exec()) qCritical() << "Cannot save"<< (isProgramLinked() ? "program" : "") <<"strength : "  << query.mQuery.lastError().text();
		auto fieldNo = query.mQuery.record().indexOf("id");
		while (query.mQuery.next()) {
			setExerciseId(query.mQuery.value(fieldNo).toInt());
			qDebug() << "Insert"<< (isProgramLinked() ? "program" : "") <<"strength exercise: " << mExerciseId;
		}
		for(auto set : mSets){
			set->save();
		}
	}else{
		if(!query.exec()) qCritical() << "Cannot update"<< (isProgramLinked() ? "program" : "") <<"strength : "  << query.mQuery.lastError().text();
		else {
			qDebug() << "Update"<< (isProgramLinked() ? "program" : "") <<"strength exercise: " << mExerciseId;
			for(auto set : mSets){
				set->save();
			}
		}
	}

	return true;
}

QList<ExerciseModel*> StrengthModel::load(QObject * parent){
	QList<ExerciseModel*> models;
	QSqlQuery query( "SELECT * FROM ex_strength ORDER BY id ASC");

	auto idField = query.record().indexOf("id");
	auto nameField = query.record().indexOf("name");
	auto descriptionField = query.record().indexOf("description");
	QStringList ids;

	while (query.next()) {
		StrengthModel * model = new StrengthModel(parent);
		qint64 id = query.value(idField).toInt();
		ids << QString::number(id);
		model->setExerciseId(id);
		model->setName(query.value(nameField).toString());
		model->setDescription(query.value(descriptionField).toString());
		models << model;
	}

	query.exec("SELECT * FROM ex_strength_set WHERE strength_id IN(" + ids.join(",") + ") ORDER BY strength_id ASC");
	idField = query.record().indexOf("strength_id");
	auto currentModel = models.begin();
	while (query.next()) {
		if(query.value(idField).toInt() != (*currentModel)->getExerciseId()) ++currentModel;
		dynamic_cast<StrengthModel*>(*currentModel)->addSet(StrengthWorkModel::load(query, *currentModel), true);
	}
	return models;
}

StrengthModel *StrengthModel::load(QSqlQuery &query, QObject * parent) {
	StrengthModel * model = new StrengthModel(parent);
// TODO optimize
	auto idField = query.record().indexOf("id");
	auto nameField = query.record().indexOf("name");
	auto descriptionField = query.record().indexOf("description");
	auto programIdField = query.record().indexOf("program_id");
	auto programOrderField = query.record().indexOf("program_order");
	model->setExerciseId(query.value(idField).toInt());
	model->setName(query.value(nameField).toString());
	model->setDescription(query.value(descriptionField).toString());
	if(programIdField>=0){
		model->setProgramId(query.value(programIdField).toInt());
	}
	if(programOrderField>=0){
		model->setProgramOrder(query.value(programOrderField).toInt());
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
	mSets.push_back(model->clone(mExerciseId, this));
	mSets.back()->setProgramId(getProgramId());
	if(keepId) {
		mSets.back()->setWorkId(model->getWorkId());
	}
	emit setsChanged();
}

void StrengthModel::removeSet(StrengthWorkModel *model) {
	mSets.removeOne(model);
	model->deleteLater();
	emit setsChanged();
}

//-------------------------------------------------------------------------------------------------------------------


void StrengthModel::setInvalidSets(bool invalid){
	if(mInvalidSets != invalid){
		mInvalidSets = invalid;
		emit invalidSetsChanged();
	}
}

//-------------------------------------------------------------------------------------------------------------------
