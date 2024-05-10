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
	setInvalidName( mName == "");
	connect(this, &ExerciseModel::nameChanged, [this](){
		setInvalidName( mName == "");
	});
	//connect(this, &ExerciseModel::descriptionChanged, [this](modelData){
	//	setInvalidDescription( false);
	//});
}

ExerciseModel * ExerciseModel::clone(qint64 programId)const{
	ExerciseModel *model = new ExerciseModel(this, nullptr);
	model->setProgramId(programId);
	return model;
}

int ExerciseModel::getType() const{
	return 0;
}

bool ExerciseModel::save(){
	return !mInvalidName && !mInvalidDescription;
}

qint64 ExerciseModel::getId()const{
	return mDbId;
}

void ExerciseModel::setId(qint64 id) {
	if(mDbId != id){
		mDbId = id;
		emit idChanged();
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
