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

	connect(this, &ExerciseModel::exerciseIdChanged, [this](){
		setIsSaved(getExerciseId() > 0);
	});
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
	connect(this, &ExerciseModel::targetExerciseChanged, [this](){
		if(mTargetExercise){
			setDescriptionExerciseId(mTargetExercise->getDescriptionExerciseId());
		}
	});
}

ExerciseModel::ExerciseModel(const ExerciseModel * model, QObject *parent) : QObject(parent){
	mName = model->getName();
	mDescription = model->getDescription();
	mTrainOrder = model->getTrainOrder();
	setInvalidName( mName == "");

	connect(this, &ExerciseModel::exerciseIdChanged, [this](){
		setIsSaved(getExerciseId() > 0);
	});
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
	connect(this, &ExerciseModel::targetExerciseChanged, [this](){
		if(mTargetExercise){
			setDescriptionExerciseId(mTargetExercise->getDescriptionExerciseId());
		}
	});
}

ExerciseModel::~ExerciseModel(){
}

ExerciseModel * ExerciseModel::clone(qint64 trainId, QObject *parent)const{
	ExerciseModel *model = new ExerciseModel(this, parent);
	model->setTrainId(trainId);
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

void ExerciseModel::setTargetExercise(Description::ExerciseModel * data) {
	if(mTargetExercise != data) {
		mTargetExercise = data;
		setName(mTargetExercise->getName());
		setDescription(mTargetExercise->getDescription());
		setType(mTargetExercise->getType());
		emit targetExerciseChanged();
	}
}

qint64 ExerciseModel::getExerciseId()const{
	return mExerciseId;
}

void ExerciseModel::setExerciseId(qint64 id) {
	if(mExerciseId != id){
		mExerciseId = id;
		emit exerciseIdChanged();
	}
}

qint64 ExerciseModel::getTrainId() const{
	return mTrainId;
}

void ExerciseModel::setTrainId(qint64 id){
	if(mTrainId != id){
		mTrainId = id;
		emit trainIdChanged();
	}
}

qint64 ExerciseModel::getDescriptionExerciseId() const {
	return mDescriptionExerciseId;
}
void ExerciseModel::setDescriptionExerciseId(qint64 id) {
	if(mDescriptionExerciseId != id){
		mDescriptionExerciseId = id;
		emit descriptionExerciseIdChanged();
	}
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

int ExerciseModel::getTrainOrder() const {
	return mTrainOrder;
}

void ExerciseModel::setTrainOrder(int data){
	if(mTrainOrder != data){
		mTrainOrder = data;
		emit trainOrderChanged();
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

bool ExerciseModel::getIsSaved() const {
	return mIsSaved;
}

void ExerciseModel::setIsSaved(bool data) {
	if(mIsSaved != data){
		mIsSaved = data;
		emit isSavedChanged();
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

void ExerciseModel::fillRandomValues(){
}
