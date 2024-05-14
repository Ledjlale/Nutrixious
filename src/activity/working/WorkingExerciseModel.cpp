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

#include "WorkingExerciseModel.h"

WorkingExerciseModel::WorkingExerciseModel(QObject *parent)
	: QObject{parent}
{
	connect(this, &WorkingExerciseModel::finished, [this](){
		setIsRunning(false);
		setIsDone(true);
	});
	connect(this, &WorkingExerciseModel::targetExerciseModelChanged, [this](){
		for(auto i : mSeries) i->deleteLater();
		mSeries.clear();
		for(int count = 0 ; count < mTargetExerciseModel->getSeries().size() ; ++count) {
			mSeries << new WorkingSerieModel(mTargetExerciseModel->getSeries()[count], mResultExerciseModel->getSeries()[count], this);
			connect(mSeries.back(), &WorkingSerieModel::finished, this, &WorkingExerciseModel::nextSerie);
		}
		emit seriesChanged();
	});
}

WorkingExerciseModel::WorkingExerciseModel(TrainingExerciseModel * target, TrainingExerciseModel * result, QObject *parent) : WorkingExerciseModel(parent){
	setResultExerciseModel(result);
	setTargetExerciseModel(target);
}

WorkingExerciseModel::~WorkingExerciseModel(){
}

TrainingExerciseModel *WorkingExerciseModel::getTargetExerciseModel() const{
	return mTargetExerciseModel;
}

void WorkingExerciseModel::setTargetExerciseModel(TrainingExerciseModel *data){
	if(mTargetExerciseModel != data){
		mTargetExerciseModel = data;
		emit targetExerciseModelChanged();
	}
}

TrainingExerciseModel *WorkingExerciseModel::getResultExerciseModel() const{
	return mResultExerciseModel;
}

void WorkingExerciseModel::setResultExerciseModel(TrainingExerciseModel *data){
	if(mResultExerciseModel != data){
		mResultExerciseModel = data;
		emit resultExerciseModelChanged();
	}
}



QVariantList WorkingExerciseModel::getVariantSeries() const {
	QVariantList series;
	for(auto serie : mSeries){
		series << QVariant::fromValue(serie);
	 }
	return series;
}

QList<WorkingSerieModel*> WorkingExerciseModel::getSeries()const{
	return mSeries;
}

bool WorkingExerciseModel::getIsRunning() const {
	return mIsRunning;
}

void WorkingExerciseModel::setIsRunning(bool data) {
	if(mIsRunning != data){
		mIsRunning = data;
		emit isRunningChanged();
	}
}

bool WorkingExerciseModel::getIsResting() const {
	return mIsResting;
}

void WorkingExerciseModel::setIsResting(bool data) {
	if(mIsResting != data){
		mIsResting = data;
		if(mIsResting)
			mStart = QDateTime::currentDateTime();
		else
			endOfCurrentRest();
		emit isRestingChanged();
	}
}

bool WorkingExerciseModel::getIsDone() const {
	return mIsDone;
}

void WorkingExerciseModel::setIsDone(bool data) {
	if(mIsDone != data){
		mIsDone = data;
		emit isDoneChanged();
	}
}


void WorkingExerciseModel::startWork(){
	mStart = QDateTime::currentDateTime();
	setIsRunning(true);
	setIsResting(false);
	setIsDone(false);
	if(mSeries.size() > 0){
		mCurrentSerie = mSeries.begin();
		(*mCurrentSerie)->startWork();
	}
	emit workStarted();
}

void WorkingExerciseModel::nextSerie(){
	if(++mCurrentSerie == mSeries.end())
		emit finished();
	else {
		(*mCurrentSerie)->startWork();
		emit workStarted();
	}
}


void WorkingExerciseModel::endOfCurrentWork(){
	if(mSeries.size() > 0){
		(*mCurrentSerie)->endOfCurrentWork();
	}else{
		mResultExerciseModel->setWorkTime(mStart.secsTo(QDateTime::currentDateTime()));
		setIsResting(true);
	}
}

void WorkingExerciseModel::endOfCurrentRest() {
	if(mSeries.size() > 0){
		(*mCurrentSerie)->endOfCurrentRest();
	}else{
		mResultExerciseModel->setRestTime(mStart.secsTo(QDateTime::currentDateTime()));
		emit finished();
	}
}

QVariant WorkingExerciseModel::getCurrentWork() {
	if(mSeries.size() > 0)
		return QVariant::fromValue(*mCurrentSerie);
	else
		return QVariant::fromValue(this);
}

bool WorkingExerciseModel::save(){
	return false;
}

//-------------------------------------------------------------------------------------------------------------------

void WorkingExerciseModel::fillRandomValues(){
}
