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
#include "src/database/DatabaseQuery.h"

#include "src/tool/Utils.h"


extern QQmlApplicationEngine * gEngine;

ProgramModel::ProgramModel() : ProgramModel(nullptr){	// QML
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}
ProgramModel::ProgramModel(QObject *parent)
	: QmlModel{parent}
{
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	mName = "Program";
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

const QList<ProgramExerciseModel*>& ProgramModel::getExercises()const {
	return mExercises;
}

ProgramExerciseModel* ProgramModel::addExercise(ProgramExerciseModel *model, bool keepId) {
	auto insertedModel = Utils::add<ProgramExerciseModel>(model, this, mExercises);
	insertedModel->setProgramId(mProgramId);
	if(keepId)
		insertedModel->setProgramExerciseId(model->getProgramExerciseId());
	emit exercisesChanged();
	return insertedModel;
}

void ProgramModel::removeExercise(ProgramExerciseModel *model) {
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

void ProgramModel::updateProgramOrder(){
	for(size_t i = 0 ; i < mExercises.size() ; ++i)
		mExercises[i]->setOrder(i);
}


void ProgramModel::decrementExerciseOrder(ProgramExerciseModel *model) {
	if(Utils::decrementOrder<ProgramExerciseModel>(model, mExercises)){
		emit exercisesChanged();
		save();
	}
}
void ProgramModel::incrementExerciseOrder(ProgramExerciseModel *model){
	if(Utils::incrementOrder<ProgramExerciseModel>(model, mExercises)){
		emit exercisesChanged();
		save();
	}
}


//-------------------------------------------------------------------------------------------------------------------


bool ProgramModel::save(){
	qDebug() << "Saving Program " << mName << mDescription;
	DatabaseQuery query;

	query.begin(mProgramId == 0 ? DatabaseQuery::Insert : DatabaseQuery::Update, "programs" );

	query.add("name", mName);
	query.add("description", mDescription);
	query.addConditionnal("program_id",mProgramId);
	if(mProgramId == 0){
		if(!query.exec()) qCritical() << "Cannot save program: "  << query.mQuery.lastError().text();
		auto fieldNo = query.mQuery.record().indexOf("program_id");
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

void ProgramModel::remove(){
	if(mProgramId > 0){
		DatabaseQuery query;
		query.begin(DatabaseQuery::Delete, "programs");
		query.addConditionnal("program_id",mProgramId);
		if(!query.exec()){
			if(!query.exec()) qCritical() << "Cannot delete program  : "  << query.mQuery.lastError().text();
		}
	}
	emit removed(this);
}

ProgramModel *ProgramModel::load(QSqlQuery &query, QObject * parent) {
	ProgramModel * model = new ProgramModel(parent);
// TODO optimize
	auto idField = query.record().indexOf("program_id");
	auto nameField = query.record().indexOf("name");
	auto descriptionField = query.record().indexOf("description");
	model->setProgramId(query.value(idField).toInt());
	model->setName(query.value(nameField).toString());
	model->setDescription(query.value(descriptionField).toString());
	return model;
}

QList<ProgramModel*> ProgramModel::load(QObject * parent){
	QList<ProgramModel*> models;

	QMap<qint64, ExerciseModel*> exercises;
	QSqlQuery query;
	if(!query.exec("SELECT * FROM exercises"))
		 qCritical() << "Cannot select exercises: "  << query.lastError().text();
	while (query.next()) {
		auto model = ExerciseModel::load(query, nullptr);
		exercises[model->getExerciseId()] = model;
	}


	QMap<qint64, QList<ProgramSerieModel*>> series;
	if(!query.exec("SELECT * FROM prgm_exercise_series"))
		 qCritical() << "Cannot select series: "  << query.lastError().text();
	while (query.next()) {
		auto model = ProgramSerieModel::load(query, nullptr);
		series[model->getProgramExerciseId()] << model;
	}


	QMap<qint64, QList<ProgramExerciseModel*>> exerciseModels;
	if(!query.exec("SELECT * FROM prgm_exercises"))
		 qCritical() << "Cannot select series: "  << query.lastError().text();
	while (query.next()) {
		auto model = ProgramExerciseModel::load(query, nullptr);
		model->setExerciseModel(exercises[model->getExerciseId()]);
		if( series.contains(model->getProgramExerciseId())){
			for(auto it : series[model->getProgramExerciseId()])
				model->addSerie(it, true);
		}
		exerciseModels[model->getProgramId()] << model;
	}

	query.exec( "SELECT * FROM programs ORDER BY program_id ASC");
	QStringList ids, programExerciseIds;

	while (query.next()) {
		auto model = load(query, parent);
		if(exerciseModels.contains(model->getProgramId())){
			for(auto it : exerciseModels[model->getProgramId()]){
				model->addExercise(it, true);
			}
		}
		models << model;
	}
	for(auto i : exerciseModels)
		for(auto j : i)
			j->deleteLater();
	for(auto i : series)
		for(auto j : i)
			j->deleteLater();
	for(auto i : exercises) i->deleteLater();

//--------------------------------
/*
	if(models.size() == 0) return models;
	if(!query.exec("SELECT * FROM prgm_exercises, exercises WHERE program_id IN(" + ids.join(",") + ")  AND prgm_exercises.exercise_id=exercises.exercise_id ORDER BY program_id ASC, exercise_order ASC"))
		 qCritical() << "Cannot select exercise series: "  << query.lastError().text();
	auto programIdField = query.record().indexOf("program_id");
	auto currentModel = models.begin();
	while (query.next()) {
		auto id = query.value(programIdField).toInt() ;
		while(id != (*currentModel)->getProgramId()) ++currentModel;
		auto insertedExercise = (*currentModel)->addExercise(ProgramExerciseModel::load(query, *currentModel), true);
		programExerciseIds << QString::number(insertedExercise->getProgramExerciseId());
	}
	if(!query.exec("SELECT * FROM prgm_exercise_series WHERE prgm_exercise_id IN(" + programExerciseIds.join(",") + ") ORDER BY pgrm_exercise_id ASC, serie_order ASC"))
		 qCritical() << "Cannot select exercise series: "  << query.lastError().text();
	auto programExerciseIdField = query.record().indexOf("prgm_exercise_id");
	currentModel = models.begin();
	auto currentExercise = (*currentModel)->mExercises.begin();
	bool found = false;

	while(query.next()){
		int id = query.value(programExerciseIdField).toInt();
		if(found) found = (*currentExercise)->getProgramExerciseId() == id;
		while(!found && currentModel != models.end()) {
			while(!found && currentExercise != (*currentModel)->mExercises.end()) {
				found = (*currentExercise)->getProgramExerciseId() == id;
				if(!found) ++currentExercise;
			}
			if(!found && (++currentModel != models.end())) currentExercise = (*currentModel)->mExercises.begin();
		}
		if(found){
			(*currentExercise)->addSerie(ProgramSerieModel::load(query, *currentExercise), true);
		}

	}*/
	return models;
}
