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

#include "ProgramModel.h"

#include <QQmlApplicationEngine>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include "../sport/StrengthModel.h"
#include "../sport/StrengthWorkModel.h"
#include "../sport/DistanceModel.h"
#include "../sport/StepsModel.h"
#include "src/database/DatabaseQuery.h"

using namespace Description;

extern QQmlApplicationEngine * gEngine;

ProgramModel::ProgramModel() : ProgramModel(nullptr){	// QML
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}
ProgramModel::ProgramModel(QObject *parent)
	: QObject{parent}
{
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	mName = "Program";
	mInvalidName = false;
	connect(this, &ProgramModel::nameChanged, [this](){
		setInvalidName( mName == "");
	});
	//connect(this, &ProgramModel::descriptionChanged, [this](){
	//	setInvalidDescription( false);
	//});
	connect(this, &ProgramModel::exercisesChanged, [this](){
		setInvalidExercises( mExercises.size() == 0);
	});
	connect(this, &ProgramModel::programIdChanged, [this](){
		for(auto &i: mExercises) i->setProgramId(mProgramId);
	});
}

qint64 ProgramModel::getProgramId()const{
	return mProgramId;
}

void ProgramModel::setProgramId(qint64 id) {
	if(mProgramId != id) {
		mProgramId = id;
		emit programIdChanged();
	}

}

QString ProgramModel::getName() const{
	return mName;
}

void ProgramModel::setName(QString name) {
	if(mName != name){
		mName = name;
		emit nameChanged();
	}
}
void ProgramModel::setDescription(QString description) {
	if(mDescription != description){
		mDescription = description;
		emit descriptionChanged();
	}
}

QVariantList ProgramModel::getVariantExercises() const {
	QVariantList exercises;
	for(auto exercise: mExercises){
		exercises << QVariant::fromValue(exercise);
	 }
	return exercises;
}

const QList<ExerciseModel*>& ProgramModel::getExercises()const {
	return mExercises;
}

void ProgramModel::addExercise(ExerciseModel *model, bool keepId) {
	int programOrder = model->getProgramOrder();
	ExerciseModel * insertedModel = nullptr;
	if(programOrder < 0) {
		mExercises.push_back(model->clone(mProgramId, this));
		insertedModel = mExercises.back();
		insertedModel->setProgramOrder(mExercises.size());
	}else{
		if( mExercises.size() == 0){
			mExercises.push_back(model->clone(mProgramId, this));
			insertedModel = mExercises.back();
		}else {
			auto it = mExercises.begin();
			while(it != mExercises.end() && (*it)->getProgramOrder() <= programOrder)
				++it;
			insertedModel = *mExercises.insert(it, model->clone(mProgramId, this));
		}
	}
	if(keepId)
		insertedModel->setExerciseId(model->getExerciseId());
	emit exercisesChanged();
}

void ProgramModel::removeExercise(ExerciseModel *model) {
	mExercises.removeOne(model);
	model->deleteLater();
	emit exercisesChanged();
}

void ProgramModel::clearExercises(){
	for(auto item : mExercises)
		item->deleteLater();
	mExercises.clear();
	emit exercisesChanged();
}

//-------------------------------------------------------------------------------------------------------------------

void ProgramModel::setInvalidName(bool invalid) {
	if(mInvalidName != invalid){
		mInvalidName = invalid;
		emit invalidNameChanged();
	}
}

void ProgramModel::setInvalidDescription(bool invalid){
	if(mInvalidDescription != invalid){
		mInvalidDescription = invalid;
		emit invalidDescriptionChanged();
	}
}

void ProgramModel::setInvalidExercises(bool invalid) {
	if(mInvalidExercises != invalid){
		mInvalidExercises = invalid;
		emit invalidExercisesChanged();
	}
}
//-------------------------------------------------------------------------------------------------------------------


bool ProgramModel::save(){
	if( mInvalidName || mInvalidDescription || mInvalidExercises) return false;
	qDebug() << "Saving Program " << mName << mDescription;
	DatabaseQuery query;

	query.begin(mProgramId == 0 ? DatabaseQuery::Insert : DatabaseQuery::Update, "programs" );

	query.add("name", mName);
	query.add("description", mDescription);
	query.addConditionnal("id",mProgramId);
	if(mProgramId == 0){
		if(!query.exec()) qCritical() << "Cannot save program: "  << query.mQuery.lastError().text();
		auto fieldNo = query.mQuery.record().indexOf("id");
		while (query.mQuery.next()) {
			setProgramId(query.mQuery.value(fieldNo).toInt());
			qDebug() << "Insert program: " << mProgramId;
		}
		for(auto exercise : mExercises){
			exercise->save();
		}
	}else{
		if(!query.exec()) qCritical() << "Cannot update program: "  << query.mQuery.lastError().text();
		else {
			qDebug() << "Update program: " << mProgramId;
			for(auto exercise : mExercises){
			exercise->save();
		}
		}
	}

	return true;
}
ProgramModel *ProgramModel::load(QSqlQuery &query, QObject * parent) {
	ProgramModel * model = new ProgramModel(parent);
// TODO optimize
	auto idField = query.record().indexOf("id");
	auto nameField = query.record().indexOf("name");
	auto descriptionField = query.record().indexOf("description");
	model->setProgramId(query.value(idField).toInt());
	model->setName(query.value(nameField).toString());
	model->setDescription(query.value(descriptionField).toString());
	return model;
}

QList<ProgramModel*> ProgramModel::load(QObject * parent){
	QList<ProgramModel*> models;
	QSqlQuery query( "SELECT * FROM programs ORDER BY id ASC");

	auto idField = query.record().indexOf("id");
	auto nameField = query.record().indexOf("name");
	auto descriptionField = query.record().indexOf("description");
	QStringList ids;

	while (query.next()) {
		auto model = load(query, parent);
		ids << QString::number(model->getProgramId());
		models << model;
	}
	if(models.size() == 0) return models;

//----------------------		Strength
	if(!query.exec("SELECT * FROM pgrm_ex_strength WHERE program_id IN(" + ids.join(",") + ") ORDER BY program_id ASC, id ASC"))
		qCritical() << "Cannot request program strength :" << query.lastError().text();
	auto programIdField = query.record().indexOf("program_id");
	auto currentModel = models.begin();
	while (query.next()) {
		while(query.value(programIdField).toInt() != (*currentModel)->getProgramId()) ++currentModel;
		auto model = StrengthModel::load(query, *currentModel);
		(*currentModel)->addExercise(model, true);
	}

	if(!query.exec("SELECT * FROM pgrm_ex_strength_set WHERE program_id IN(" + ids.join(",") + ") ORDER BY program_id ASC, strength_id ASC"))
		qCritical() << "Cannot request program strength set :" << query.lastError().text();
	programIdField = query.record().indexOf("program_id");
	auto strengthId = query.record().indexOf("strength_id");
	currentModel = models.begin();
	auto currentExercise = (*currentModel)->getExercises().begin();
	while (query.next()) {
		while(query.value(programIdField).toInt() != (*currentModel)->getProgramId()){
			++currentModel;
			currentExercise = (*currentModel)->getExercises().begin();
		}
		while(query.value(strengthId).toInt() != (*currentExercise)->getExerciseId()) {
			++currentExercise;
		}
		auto model = StrengthWorkModel::load(query, *currentExercise);
		dynamic_cast<StrengthModel*>(*currentExercise)->addSet(model, true);
	}

//----------------------		Distance
	if(!query.exec("SELECT * FROM pgrm_ex_distance WHERE program_id IN(" + ids.join(",") + ") ORDER BY program_id ASC, id ASC"))
		qCritical() << "Cannot request program distance :" << query.lastError().text();
	programIdField = query.record().indexOf("program_id");
	currentModel = models.begin();
	while (query.next()) {
		while(query.value(programIdField).toInt() != (*currentModel)->getProgramId()) ++currentModel;
		auto model = DistanceModel::load(query, *currentModel);
		(*currentModel)->addExercise(model, true);
	}

//----------------------		Steps
	if(!query.exec("SELECT * FROM pgrm_ex_steps WHERE program_id IN(" + ids.join(",") + ") ORDER BY program_id ASC, id ASC"))
		qCritical() << "Cannot request program steps :" << query.lastError().text();
	programIdField = query.record().indexOf("program_id");
	currentModel = models.begin();
	while (query.next()) {
		while(query.value(programIdField).toInt() != (*currentModel)->getProgramId()) ++currentModel;
		auto model = StepsModel::load(query, *currentModel);
		(*currentModel)->addExercise(model, true);
	}

//--------------------------------

	return models;
}
