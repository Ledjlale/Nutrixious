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

#include "ExerciseModel.h"

using namespace Training;

ExerciseModel::ExerciseModel(QObject *parent)
	: QObject{parent}
{
	connect(this, &ExerciseModel::nameChanged, [this](){
		setInvalidName( mName == "");
	});
	//connect(this, &ExerciseModel::descriptionChanged, [this](){
	//	setInvalidDescription( false);
	//});
	connect(this, &ExerciseModel::finished, [this](){
		setIsRunning(false);
		setIsDone(true);
	});
}

ExerciseModel::ExerciseModel(const ExerciseModel * model, QObject *parent) : QObject(parent){
	mName = model->getName();
	mDescription = model->getDescription();
	mProgramOrder = model->getProgramOrder();
	setInvalidName( mName == "");
	connect(this, &ExerciseModel::nameChanged, [this](){
		setInvalidName( mName == "");
	});
	//connect(this, &ExerciseModel::descriptionChanged, [this](modelData){
	//	setInvalidDescription( false);
	//});
	connect(this, &ExerciseModel::finished, [this](){
		setIsRunning(false);
		setIsDone(true);
	});
}

ExerciseModel * ExerciseModel::clone()const{
	ExerciseModel *model = new ExerciseModel(this, nullptr);
	return model;
}

int ExerciseModel::getType() const{
	return mType;
}

void ExerciseModel::setType(int data){
	if(mType != data){
		mType = data;
		emit typeChanged();
	}
}

bool ExerciseModel::save(){
	return !mInvalidName && !mInvalidDescription;
}

bool ExerciseModel::saveProgram(qint64 programId){
	return !mInvalidName && !mInvalidDescription;
}

void ExerciseModel::setTargetExercise(Description::ExerciseModel * data) {
	if(mTargetExercise != data) {
		mTargetExercise = data;
		setName(mTargetExercise->getName());
		setDescription(mTargetExercise->getDescription());
		setType(mTargetExercise->getType());
		emit targetExerciseChanged();
	}
}

qint64 ExerciseModel::getId()const{
	return mDbId;
}

void ExerciseModel::setId(qint64 id) {
	mDbId = id;
}

QString ExerciseModel::getName() const{
	return mName;
}

void ExerciseModel::setName(QString name) {
	if(mName != name){
		mName = name;
		emit nameChanged();
	}
}

QString ExerciseModel::getDescription() const{
	return mDescription;
}

void ExerciseModel::setDescription(QString description) {
	if(mDescription != description){
		mDescription = description;
		emit descriptionChanged();
	}
}

int ExerciseModel::getProgramOrder() const {
	return mProgramOrder;
}

void ExerciseModel::setProgramOrder(int data){
	if(mProgramOrder != data){
		mProgramOrder = data;
		emit programOrderChanged();
	}
}

bool ExerciseModel::getIsRunning() const {
	return mIsRunning;
}

void ExerciseModel::setIsRunning(bool data) {
	if(mIsRunning != data){
		mIsRunning = data;
		emit isRunningChanged();
	}
}

bool ExerciseModel::getIsResting() const {
	return mIsResting;
}

void ExerciseModel::setIsResting(bool data) {
	if(mIsResting != data){
		mIsResting = data;
		if(mIsResting)
			mStart = QDateTime::currentDateTime();
		else
			endOfCurrentRest();
		emit isRestingChanged();
	}
}

bool ExerciseModel::getIsDone() const {
	return mIsDone;
}

void ExerciseModel::setIsDone(bool data) {
	if(mIsDone != data){
		mIsDone = data;
		emit isDoneChanged();
	}
}

void ExerciseModel::startWork(){
	mStart = QDateTime::currentDateTime();
	setIsRunning(true);
	setIsResting(false);
	setIsDone(false);
}

QVariant ExerciseModel::getCurrentWork() {
	return QVariant::fromValue(this);
}

//-------------------------------------------------------------------------------------------------------------------

void ExerciseModel::setInvalidName(bool invalid) {
	if(mInvalidName != invalid){
		mInvalidName = invalid;
		emit invalidNameChanged();
	}
}

void ExerciseModel::setInvalidDescription(bool invalid){
	if(mInvalidDescription != invalid){
		mInvalidDescription = invalid;
		emit invalidDescriptionChanged();
	}
}

//-------------------------------------------------------------------------------------------------------------------
