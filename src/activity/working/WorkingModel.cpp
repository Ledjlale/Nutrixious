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

#include "WorkingModel.h"

#include <QQmlApplicationEngine>
extern QQmlApplicationEngine * gEngine;

WorkingModel::WorkingModel() : WorkingModel(nullptr){
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
	mCurrentExercise = mExercises.end();
	mExercises.reserve(256);// Because of new implementation on Qt6, list will be resized if appending more object : iterators will be undefined.
}

WorkingModel::WorkingModel(QObject *parent)
	: QObject{parent} {
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	mExercises.reserve(256);
	mCurrentExercise = mExercises.end();
//	mTrainModel = new TrainModel(this);
//	connect(mTrainModel, &TrainModel::exercisesChanged, this, &WorkingModel::exercisesChanged);
//	connect(mTrainModel, &TrainModel::nextExercise, this, &WorkingModel::nextExercise);
//	connect(mTrainModel, &TrainModel::workStarted, this, &WorkingModel::currentWorkChanged);

	mRestTimer.setInterval(1000);
	connect(&mRestTimer, &QTimer::timeout, this, &WorkingModel::refresh);
	connect(this, &WorkingModel::targetModelChanged, [this](){
		for(auto i : mExercises) i->deleteLater();
		mExercises.clear();
		auto t = new TrainingModel(mTargetModel, this);
		t->makeNew();
		setResultModel(t);
		for(int count = 0 ;  count < mTargetModel->getExercises().size() ; ++count) {
			mExercises << new WorkingExerciseModel(dynamic_cast<TrainingExerciseModel*>(mTargetModel->getExercises()[count])
							, dynamic_cast<TrainingExerciseModel*>(mResultModel->getExercises()[count])
							, this);
			connect(mExercises.back(), &WorkingExerciseModel::finished, this, &WorkingModel::nextExercise);
			connect(mExercises.back(), &WorkingExerciseModel::workStarted, this, &WorkingModel::currentWorkChanged);
		}
		mCurrentExercise = mExercises.end();
		emit exercisesChanged();
	});
}

WorkingModel::~WorkingModel(){
}

TrainingModel *WorkingModel::getTargetModel() const {
	return mTargetModel;
}

void WorkingModel::setTargetModel(TrainingModel *data){
	if(mTargetModel != data){
		mTargetModel = data;
		emit targetModelChanged();
	}
}

TrainingModel *WorkingModel::getResultModel() const {
	return mResultModel;
}

void WorkingModel::setResultModel(TrainingModel *data){
	if(mResultModel != data){
		mResultModel = data;
		emit resultModelChanged();
	}
}

void WorkingModel::loadFromProgram(ProgramModel *data) {
	setTargetModel(new TrainingModel(data, this));
}

void WorkingModel::addExercise(ExerciseModel * data){
	auto build = mTargetModel->buildExercise(data);
	auto targetExercise = dynamic_cast<TrainingExerciseModel*>(mTargetModel->insertExercise(build));
	auto resultExercise = dynamic_cast<TrainingExerciseModel*>(mResultModel->insertNewExercise(targetExercise));
	mExercises << new WorkingExerciseModel(targetExercise, resultExercise, this);
	connect(mExercises.back(), &WorkingExerciseModel::finished, this, &WorkingModel::nextExercise);
	connect(mExercises.back(), &WorkingExerciseModel::workStarted, this, &WorkingModel::currentWorkChanged);
	emit exercisesChanged();
}

QVariantList WorkingModel::getVariantExercises() const {
	QVariantList exercises;
	for(auto exercise: mExercises){
		exercises << QVariant::fromValue(exercise);
	 }
	return exercises;
}

bool WorkingModel::isWorking()const{
	return mIsWorking;
}
void WorkingModel::setIsWorking(bool data){
	if(mIsWorking != data){
		mIsWorking = data;
		emit isWorkingChanged();
	}
}

bool WorkingModel::isResting()const{
	return mIsResting;
}
void WorkingModel::setIsResting(bool data){
	if(mIsResting!= data){
		mIsResting = data;
		emit isRestingChanged();
	}
}

bool WorkingModel::start(){
	mResultModel->setStartDateTime(QDateTime::currentDateTime());
	if(mExercises.size() == 0){
		qCritical() << "Cannot start : no exercises in working model";
		return false;
	}
	mCurrentExercise = mExercises.begin();
	(*mCurrentExercise)->startWork();
	emit workingNextExercise(0);
	emit currentWorkChanged();// because affectation of iterator is done before the WorkingModel signal.
	return true;
}
void WorkingModel::stop(){
}
void WorkingModel::resume(){
}

void WorkingModel::save(){
	mResultModel->save();
}

void WorkingModel::endOfCurrentWork(){
	if(mCurrentExercise != mExercises.end())
		(*mCurrentExercise)->endOfCurrentWork();
	setIsWorking(false);
	setIsResting(true);
}

void WorkingModel::endOfCurrentRest(){
	(*mCurrentExercise)->endOfCurrentRest();
}

QVariant WorkingModel::getCurrentExercise() {
	if(mTargetModel && mCurrentExercise != mExercises.end())
		return QVariant::fromValue(*mCurrentExercise);
	else
		return QVariant();
}

QVariant WorkingModel::getCurrentWork() {
	if(mTargetModel && mCurrentExercise != mExercises.end())
		return (*mCurrentExercise)->getCurrentWork();
	else
		return QVariant();
}

void WorkingModel::refresh(){
	//if(isWorking()) setWorkTime(QTime::currentTime());
	//else setRestTime(QTime::currentTime());
}

void WorkingModel::nextExercise(){
	if(mCurrentExercise == mExercises.end() || ++mCurrentExercise == mExercises.end()){
		emit finished();
	}else {
		(*mCurrentExercise)->startWork();
		emit workingNextExercise(mCurrentExercise - mExercises.begin());
		emit currentWorkChanged();
	}
	emit currentExerciseChanged();
}
