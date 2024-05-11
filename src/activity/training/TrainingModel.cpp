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

#include "TrainingModel.h"

#include <QQmlApplicationEngine>
extern QQmlApplicationEngine * gEngine;

using namespace Training;

TrainingModel::TrainingModel() : TrainingModel(nullptr){
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}

TrainingModel::TrainingModel(QObject *parent)
	: QObject{parent} {
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	mTrainModel = new TrainModel(this);
	connect(mTrainModel, &TrainModel::exercisesChanged, this, &TrainingModel::exercisesChanged);
	connect(mTrainModel, &TrainModel::nextExercise, this, &TrainingModel::nextExercise);

	mRestTimer.setInterval(1000);
	connect(&mRestTimer, &QTimer::timeout, this, &TrainingModel::refresh);
}

TrainingModel::~TrainingModel(){
	mTrainModel->deleteLater();
}

bool TrainingModel::isWorking()const{
	return mIsWorking;
}
void TrainingModel::setIsWorking(bool data){
	if(mIsWorking != data){
		mIsWorking = data;
		emit isWorkingChanged();
	}
}

bool TrainingModel::isResting()const{
	return mIsResting;
}
void TrainingModel::setIsResting(bool data){
	if(mIsResting!= data){
		mIsResting = data;
		emit isRestingChanged();
	}
}

void TrainingModel::start(){
	mCurrentExercise = mTrainModel->start();
}
void TrainingModel::stop(){
}
void TrainingModel::resume(){
}

void TrainingModel::save(){
	mTrainModel->save();
}

void TrainingModel::endOfCurrentWork(){
	(*mCurrentExercise)->endOfCurrentWork();
	//(*mCurrentExercise)->endOfCurrentWork();
	//setIsWorking(false);
	//setIsResting(true);
}

void TrainingModel::endOfCurrentRest(){
	//(*mCurrentExercise)->endOfCurrentRest();
}
QVariant TrainingModel::getCurrentWork() {
	return (*mCurrentExercise)->getCurrentWork();
}

void TrainingModel::refresh(){
	//if(isWorking()) setWorkTime(QTime::currentTime());
	//else setRestTime(QTime::currentTime());
}

void TrainingModel::nextExercise(){
	if(++mCurrentExercise == mTrainModel->getExercises().end()){
		emit finished();
	}else {
		(*mCurrentExercise)->startWork();
		emit workingNextExercise(mCurrentExercise - mTrainModel->getExercises().begin());
	}
}
