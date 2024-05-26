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
	connect(this, &ProgramModel::idChanged, [this](){
		for(auto &i: mExercises) i->setParentId(mId);
	});
}
ProgramModel::ProgramModel(ProgramModel * model, QObject *parent) : ProgramModel(parent) {
	mId = model->getId();
	mName = model->getName();
	mDescription = model->getDescription();
	for(auto i : model->getExercises()){
		insertExercise(new ProgramExerciseModel(i, this));
	}
}

qint64 ProgramModel::getId()const{
	return mId;
}

void ProgramModel::setId(qint64 id) {
	if(mId != id) {
		mId = id;
		setIsSaved(mId > 0);
		emit idChanged();
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

QString ProgramModel::getDescription() const{
	return mDescription;
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

void ProgramModel::addNewExercise(ProgramExerciseModel *model) {
	insertExercise(model->clone(this))->makeNew();
}

ProgramExerciseModel* ProgramModel::insertNewExercise(ProgramExerciseModel *model) {
	auto newModel = insertExercise(model->clone(this));
	newModel->makeNew();
	return newModel;
}

ProgramExerciseModel* ProgramModel::insertExercise(ProgramExerciseModel *model) {
	auto insertedModel = Utils::add<ProgramExerciseModel>(model, mExercises);
	insertedModel->setParentId(mId);
	connect(insertedModel, &ExerciseUnitModel::removed, this, &ProgramModel::handleRemoved);
	emit exercisesChanged();
	return insertedModel;
}

void ProgramModel::removeExercise(ProgramExerciseModel *model) {
	mExercises.removeOne(model);
	model->deleteLater();
	emit exercisesChanged();
}

void ProgramModel::handleRemoved(ExerciseUnitModel *model){
	auto it = std::find(mExercises.begin(), mExercises.end(), model);
	if( it != mExercises.end()){
		int row = it - mExercises.begin();
		mExercises.erase(it);
		emit exercisesChanged();
	}
}

void ProgramModel::clearExercises(){
	for(auto item : mExercises)
		item->deleteLater();
	mExercises.clear();
	emit exercisesChanged();
}

void ProgramModel::makeNew(){
	setId(0);
	for(auto i : mExercises)
		i->makeNew();
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
	qDebug() << "Saving " << mTablePrefix << mName << mDescription;
	DatabaseQuery query;
	if( mId > 0 && !getIsEdited()){
		bool saved = false;
		for(auto e : mExercises) {
			saved = saved || e->save();
		}
		return saved;
	}
	query.begin(mId == 0 ? DatabaseQuery::Insert : DatabaseQuery::Update, mTablePrefix +"s");

	query.add("name", mName);
	query.add("description", mDescription);
	addQueryValues(query);
	query.addConditionnal(mTablePrefix+"_id",mId);
	if(mId == 0){
		if(!query.exec()) qCritical() << "Cannot save program: "  << query.mQuery.lastError().text();
		auto fieldNo = query.mQuery.record().indexOf(mTablePrefix+"_id");
		while (query.mQuery.next()) {
			setId(query.mQuery.value(fieldNo).toInt());
			qDebug() << "Insert " + mTablePrefix +": " << mId;
		}
		for(auto exercise : mExercises){
			exercise->save();
		}
		clearBackupValues();
	}else{
		if(!query.exec()) qCritical() << "Cannot update "+mTablePrefix+": "  << query.mQuery.lastError().text();
		else {
			qDebug() << "Update "+mTablePrefix+": " << mId;
			for(auto exercise : mExercises){
				exercise->save();
			}
		clearBackupValues();
		}
	}

	return true;
}

void ProgramModel::remove(){
	if(mId > 0){
		DatabaseQuery query;
		query.begin(DatabaseQuery::Delete, mTablePrefix+"s");
		query.addConditionnal(mTablePrefix+"_id",mId);
		if(!query.exec()){
			if(!query.exec()) qCritical() << "Cannot delete "+mTablePrefix+"  : "  << query.mQuery.lastError().text();
		}
	}
	emit removed();
}

ProgramModel *ProgramModel::build(QSqlQuery &query, QObject * parent) {
	ProgramModel * model = new ProgramModel(parent);
// TODO optimize
	model->load(query);
	return model;
}

void ProgramModel::load(QSqlQuery &query){
	auto idField = query.record().indexOf(mTablePrefix+"_id");
	auto nameField = query.record().indexOf("name");
	auto descriptionField = query.record().indexOf("description");
	setId(query.value(idField).toInt());
	setName(query.value(nameField).toString());
	setDescription(query.value(descriptionField).toString());
	clearBackupValues();
}

QList<ProgramModel*> ProgramModel::buildAll(QObject * parent){
	QList<ProgramModel*> models;
	QString tablePrefix = "program";
	QMap<qint64, ExerciseModel*> exercises;
	QSqlQuery query;
	if(!query.exec("SELECT * FROM exercises"))
		 qCritical() << "Cannot select exercises: "  << query.lastError().text();
	while (query.next()) {
		auto model = ExerciseModel::build(query, nullptr);
		exercises[model->getExerciseId()] = model;
	}


	QMap<qint64, QList<ProgramSerieModel*>> series;
	if(!query.exec("SELECT * FROM "+tablePrefix+"_exercise_series"))
		 qCritical() << "Cannot select series: "  << query.lastError().text();
	while (query.next()) {
		auto model = ProgramSerieModel::build(query, nullptr);
		series[model->getExerciseUnitId()] << model;
	}


	QMap<qint64, QList<ProgramExerciseModel*>> exerciseModels;
	if(!query.exec("SELECT * FROM "+tablePrefix+"_exercise_units"))
		 qCritical() << "Cannot select series: "  << query.lastError().text();
	while (query.next()) {
		auto model = ProgramExerciseModel::build(query, nullptr);
		if(exercises.contains(model->getExerciseId()))
			model->setExerciseModel(exercises[model->getExerciseId()]);
		else
			model->setExerciseModel(nullptr);
		if( series.contains(model->getExerciseUnitId())){
			for(auto it : series[model->getExerciseUnitId()])
				model->ExerciseUnitModel::insertSerie(it->clone(model));
		}
		exerciseModels[model->getParentId()] << model;
	}

	query.exec( "SELECT * FROM "+tablePrefix+"s ORDER BY "+tablePrefix+"_id ASC");
	QStringList ids, programExerciseIds;

	while (query.next()) {
		auto model = build(query, parent);
		if(exerciseModels.contains(model->getId())){
			for(auto it : exerciseModels[model->getId()]){
				model->insertExercise(it->clone(model));
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
