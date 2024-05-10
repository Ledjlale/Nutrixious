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

#include "StepsModel.h"

#include <QObject>
#include <QVector>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>

#include "src/activity/description/sport/StepsModel.h"

using namespace Training;

StepsModel::StepsModel(QObject *parent) : ExerciseModel(parent){
	mName = "Steps";
	mInvalidName = false;
	mType = 2;
}

StepsModel::StepsModel(const StepsModel * model, QObject *parent) : ExerciseModel(model, parent){
	mSteps = model->getSteps();
	mWeight = model->getWeight();
	mWorkTime = model->getWorkTime();
	mRestTime = model->getRestTime();
	mType = 2;
}

ExerciseModel * StepsModel::clone()const{
	StepsModel *model = new StepsModel(this, nullptr);
	return model;
}

void StepsModel::setTargetExercise(Description::ExerciseModel * data){
	ExerciseModel::setTargetExercise(data);
	if(data){
		auto model = dynamic_cast<Description::StepsModel*>(data);
		setSteps(model->getSteps());
		setWeight(model->getWeight());
		setRestTime(model->getRestTime());
	}
}


bool StepsModel::save() {
	if(!ExerciseModel::save()) return false;
	qDebug() << "Saving Steps" << mName << mDescription;
	QSqlQuery query;
	if(mDbId == 0){
		query.prepare("INSERT INTO ex_steps(name,description, steps, work_time) VALUES(?,?,?,?) RETURNING id");
		query.addBindValue(mName);
		query.addBindValue(mDescription);
		query.addBindValue(mSteps);
		query.addBindValue(mWorkTime);
		query.exec();
		auto fieldNo = query.record().indexOf("id");
		while (query.next()) {
			mDbId = query.value(fieldNo).toInt();
			qDebug() << "Insert Steps exercise : " << mDbId;
		}
	}else{
		query.prepare("UPDATE ex_steps SET name=?, description=?, steps=?, work_time=? WHERE id=?");
		query.addBindValue(mName);
		query.addBindValue(mDescription);
		query.addBindValue(mSteps);
		query.addBindValue(mWorkTime);
		query.addBindValue(mDbId);
		qDebug() << "Update Steps exercise : " << mDbId << query.exec();;
	}
	return true;
}

bool StepsModel::saveProgram(qint64 programId){
	if(!ExerciseModel::saveProgram(programId)) return false;
	qDebug() << "Saving " << mName << mDescription;
	QSqlQuery query;
	if(mDbId == 0){
		query.prepare("INSERT INTO pgrm_ex_steps(program_id, name,description, steps, work_time, program_order) VALUES(?,?,?,?,?,?) RETURNING id");
		query.addBindValue(programId);
		query.addBindValue(mName);
		query.addBindValue(mDescription);
		query.addBindValue(mSteps);
		query.addBindValue(mWorkTime);
		query.addBindValue(mProgramOrder);
		query.exec();
		auto fieldNo = query.record().indexOf("id");
		while (query.next()) {
			mDbId = query.value(fieldNo).toInt();
			qDebug() << "Insert program steps exercise : " << mDbId << " at " << mProgramOrder;
		}
	}else{
		query.prepare("UPDATE pgrm_ex_steps SET name=?, description=?, steps=?, work_time=?, program_order=? WHERE id=?");
		query.addBindValue(mName);
		query.addBindValue(mDescription);
		query.addBindValue(mSteps);
		query.addBindValue(mWorkTime);
		query.addBindValue(mProgramOrder);
		query.addBindValue(mDbId);
		qDebug() << "Update steps exercise : " << mDbId << " at " << mProgramOrder << query.exec();
	}
	return true;
}

QList<ExerciseModel*> StepsModel::load(){
	QList<ExerciseModel*> models;
	QSqlQuery query( "SELECT * FROM ex_steps ORDER BY id ASC");

	auto idField = query.record().indexOf("id");
	auto nameField = query.record().indexOf("name");
	auto descriptionField = query.record().indexOf("description");
	auto stepsField = query.record().indexOf("steps");
	auto workTimeField = query.record().indexOf("work_time");
	QStringList ids;

	while (query.next()) {
		StepsModel * model = new StepsModel();
		qint64 id = query.value(idField).toInt();
		ids << QString::number(id);
		model->setId(id);
		model->setName(query.value(nameField).toString());
		model->setDescription(query.value(descriptionField).toString());
		model->setSteps(query.value(stepsField).toInt());
		model->setWorkTime(query.value(workTimeField).toInt());
		models << model;
	}
	return models;
}

StepsModel *StepsModel::load(QSqlQuery &query) {
	StepsModel * model = new StepsModel();
// TODO optimize
	auto idField = query.record().indexOf("id");
	auto nameField = query.record().indexOf("name");
	auto descriptionField = query.record().indexOf("description");
	auto stepsField = query.record().indexOf("steps");
	auto workTimeField = query.record().indexOf("work_time");
	auto programOrderField = query.record().indexOf("program_order");
	model->setId(query.value(idField).toInt());
	model->setName(query.value(nameField).toString());
	model->setDescription(query.value(descriptionField).toString());
	model->setSteps(query.value(stepsField).toInt());
	model->setWorkTime(query.value(workTimeField).toInt());
	if(programOrderField>=0){
		model->setProgramOrder(query.value(programOrderField).toInt());
	}
	return model;
}

int StepsModel::getSteps() const{
	return mSteps;
}

void StepsModel::setSteps(int data){
	if(mSteps != data){
		mSteps = data;
		emit stepsChanged();
	}
}

int StepsModel::getWeight()const{
	return mWeight;
}

void StepsModel::setWeight(int data){
	if(mWeight != data){
		mWeight = data;
		emit weightChanged();
	}
}

int StepsModel::getWorkTime()const{
	return mWorkTime;
}

void StepsModel::setWorkTime(int data){
	if(mWorkTime != data){
		mWorkTime = data;
		emit workTimeChanged();
	}
}

int StepsModel::getRestTime()const{
	return mRestTime;
}

void StepsModel::setRestTime(int data){
	if(mRestTime != data){
		mRestTime = data;
		emit restTimeChanged();
	}
}

void StepsModel::endOfCurrentWork(){
	setWorkTime(mStart.secsTo(QDateTime::currentDateTime()));
	setIsResting(true);
}

void StepsModel::endOfCurrentRest() {
	setRestTime(mStart.secsTo(QDateTime::currentDateTime()));
	emit finished();
}
