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

using namespace Description;

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
}

ExerciseModel::ExerciseModel(const ExerciseModel * model, QObject *parent) : QObject(parent){
	mName = model->getName();
	mDescription = model->getDescription();
	mProgramOrder = model->getProgramOrder();
	mDescriptionExerciseId = model->getDescriptionExerciseId();
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
}

ExerciseModel * ExerciseModel::clone(qint64 programId, QObject *parent)const{
	ExerciseModel *model = new ExerciseModel(this, parent);
	model->setProgramId(programId);
	return model;
}

int ExerciseModel::getType() const{
	return 0;
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


qint64 ExerciseModel::getProgramId() const{
	return mProgramId;
}

void ExerciseModel::setProgramId(qint64 id){
	if(mProgramId != id){
		mProgramId = id;
		emit programIdChanged();
	}
}

bool ExerciseModel::isProgramLinked() const {
	return mProgramId >= 0;
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

int ExerciseModel::getProgramOrder() const {
	return mProgramOrder;
}

void ExerciseModel::setProgramOrder(int data){
	if(mProgramOrder != data){
		mProgramOrder = data;
		emit programOrderChanged();
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

bool ExerciseModel::save(){
	return !mInvalidName && !mInvalidDescription;
}
