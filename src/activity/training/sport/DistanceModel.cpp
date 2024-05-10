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

#include "DistanceModel.h"

#include <QObject>
#include <QVector>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

#include "src/activity/description/sport/DistanceModel.h"

using namespace Training;

DistanceModel::DistanceModel(QObject *parent) : ExerciseModel(parent){
	mName = "Distance";
	mInvalidName = false;
	mType = 1;
}

DistanceModel::DistanceModel(const DistanceModel * model, QObject *parent) : ExerciseModel(model, parent){
	mDistance = model->getDistance();
	mWeight = model->getWeight();
	mWorkTime = model->getWorkTime();
	mRestTime = model->getRestTime();
	mType = 1;
}

ExerciseModel * DistanceModel::clone()const{
	ExerciseModel *model = new DistanceModel(this, nullptr);
	return model;
}

void DistanceModel::setTargetExercise(Description::ExerciseModel * data){
	ExerciseModel::setTargetExercise(data);
	if(data){
		auto model = dynamic_cast<Description::DistanceModel*>(data);
		setDistance(model->getDistance());
		setWeight(model->getWeight());
		setRestTime(model->getRestTime());
	}
}

bool DistanceModel::save() {
/*
	if(!ExerciseModel::save()) return false;
	qDebug() << "Saving Distance" << mName << mDescription;
	QSqlQuery query;
	if(mDbId == 0){
		query.prepare("INSERT INTO ex_distance(name,description, distance, work_time) VALUES(?,?,?,?) RETURNING id");
		query.addBindValue(mName);
		query.addBindValue(mDescription);
		query.addBindValue(mDistance);
		query.addBindValue(mWorkTime);
		query.exec();
		auto fieldNo = query.record().indexOf("id");
		while (query.next()) {
			mDbId = query.value(fieldNo).toInt();
			qDebug() << "Insert Distance exercise : " << mDbId;
		}
	}else{
		query.prepare("UPDATE ex_distance SET name=?, description=?, distance=?, work_time=? WHERE id=?");
		query.addBindValue(mName);
		query.addBindValue(mDescription);
		query.addBindValue(mDistance);
		query.addBindValue(mWorkTime);
		query.addBindValue(mDbId);
		qDebug() << "Update Distance exercise : " << mDbId << query.exec();;
	}
	return true;
	*/
	return false;
}

bool DistanceModel::saveProgram(qint64 programId){
/*
	if(!ExerciseModel::saveProgram(programId)) return false;
	qDebug() << "Saving " << mName << mDescription;
	QSqlQuery query;
	if(mDbId == 0){
		query.prepare("INSERT INTO pgrm_ex_distance(program_id, name,description, distance, work_time, program_order) VALUES(?,?,?,?,?,?) RETURNING id");
		query.addBindValue(programId);
		query.addBindValue(mName);
		query.addBindValue(mDescription);
		query.addBindValue(mDistance);
		query.addBindValue(mWorkTime);
		query.addBindValue(mProgramOrder);
		if(!query.exec()) qCritical() << "Cannot insert distance in program : " << query.lastError().text();
		auto fieldNo = query.record().indexOf("id");
		while (query.next()) {
			mDbId = query.value(fieldNo).toInt();
			qDebug() << "Insert program distance exercise : " << mDbId << " at " << mProgramOrder;
		}
	}else{
		query.prepare("UPDATE pgrm_ex_distance SET name=?, description=?, distance=?, work_time=?, program_order=? WHERE id=?");
		query.addBindValue(mName);
		query.addBindValue(mDescription);
		query.addBindValue(mDistance);
		query.addBindValue(mWorkTime);
		query.addBindValue(mProgramOrder);
		query.addBindValue(mDbId);
		qDebug() << "Update distance exercise : " << mDbId << " at " << mProgramOrder << query.exec();
	}
	return true;
	*/
	return false;
}

QList<ExerciseModel*> DistanceModel::load(){
	QList<ExerciseModel*> models;
	QSqlQuery query( "SELECT * FROM ex_distance ORDER BY id ASC");

	auto idField = query.record().indexOf("id");
	auto nameField = query.record().indexOf("name");
	auto descriptionField = query.record().indexOf("description");
	auto distanceField = query.record().indexOf("distance");
	auto workTimeField = query.record().indexOf("work_time");
	QStringList ids;

	while (query.next()) {
		DistanceModel * model = new DistanceModel();
		qint64 id = query.value(idField).toInt();
		ids << QString::number(id);
		model->setId(id);
		model->setName(query.value(nameField).toString());
		model->setDescription(query.value(descriptionField).toString());
		model->setDistance(query.value(distanceField).toInt());
		model->setWorkTime(query.value(workTimeField).toInt());
		models << model;
	}
	return models;
}

DistanceModel *DistanceModel::load(QSqlQuery &query) {
	DistanceModel * model = new DistanceModel();
// TODO optimize
	auto idField = query.record().indexOf("id");
	auto nameField = query.record().indexOf("name");
	auto descriptionField = query.record().indexOf("description");
	auto distanceField = query.record().indexOf("distance");
	auto workTimeField = query.record().indexOf("work_time");
	auto programOrderField = query.record().indexOf("program_order");
	model->setId(query.value(idField).toInt());
	model->setName(query.value(nameField).toString());
	model->setDescription(query.value(descriptionField).toString());
	model->setDistance(query.value(distanceField).toInt());
	model->setWorkTime(query.value(workTimeField).toInt());
	if(programOrderField>=0){
		model->setProgramOrder(query.value(programOrderField).toInt());
	}
	return model;
}

int DistanceModel::getDistance() const{
	return mDistance;
}

void DistanceModel::setDistance(int data){
	if(mDistance != data){
		mDistance = data;
		emit distanceChanged();
	}
}

int DistanceModel::getWeight()const{
	return mWeight;
}

void DistanceModel::setWeight(int data){
	if(mWeight != data){
		mWeight = data;
		emit weightChanged();
	}
}

int DistanceModel::getWorkTime()const{
	return mWorkTime;
}

void DistanceModel::setWorkTime(int data){
	if(mWorkTime != data){
		mWorkTime = data;
		emit workTimeChanged();
	}
}

int DistanceModel::getRestTime()const{
	return mRestTime;
}

void DistanceModel::setRestTime(int data){
	if(mRestTime != data){
		mRestTime = data;
		emit restTimeChanged();
	}
}

void DistanceModel::endOfCurrentWork(){
	setWorkTime(mStart.secsTo(QDateTime::currentDateTime()));
	setIsResting(true);
}

void DistanceModel::endOfCurrentRest() {
	setRestTime(mStart.secsTo(QDateTime::currentDateTime()));
	emit finished();
}
