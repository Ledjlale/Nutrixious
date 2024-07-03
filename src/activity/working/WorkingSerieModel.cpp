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


#include "WorkingSerieModel.h"

WorkingSerieModel::WorkingSerieModel(QObject *parent)
	: QObject{parent}
{
	connect(this, &WorkingSerieModel::finished, [this](){
		setIsRunning(false);
		setIsDone(true);
	});

}

WorkingSerieModel::WorkingSerieModel(TrainingSerieModel *target, TrainingSerieModel *result, QObject *parent) : WorkingSerieModel(parent){
	setResultSerieModel(result);
	setTargetSerieModel(target);
}

WorkingSerieModel::~WorkingSerieModel(){
}


TrainingSerieModel *WorkingSerieModel::getTargetSerieModel() const{
	return mTargetSerieModel;
}

void WorkingSerieModel::setTargetSerieModel(TrainingSerieModel *data){
	if(mTargetSerieModel != data){
		mTargetSerieModel = data;
		emit targetSerieModelChanged();
	}
}

TrainingSerieModel *WorkingSerieModel::getResultSerieModel() const{
	return mResultSerieModel;
}

void WorkingSerieModel::setResultSerieModel(TrainingSerieModel *data){
	if(mResultSerieModel != data){
		mResultSerieModel = data;
		emit resultSerieModelChanged();
	}
}


bool WorkingSerieModel::getIsRunning() const {
	return mIsRunning;
}

void WorkingSerieModel::setIsRunning(bool data) {
	if(mIsRunning != data){
		mIsRunning = data;
		emit isRunningChanged();
	}
}

bool WorkingSerieModel::getIsResting() const {
	return mIsResting;
}

void WorkingSerieModel::setIsResting(bool data) {
	if(mIsResting != data){
		mIsResting = data;
		if(mIsResting)
			mStart = QDateTime::currentDateTime();
		else
			endOfCurrentRest();
		emit isRestingChanged();
	}
}

bool WorkingSerieModel::getIsDone() const {
	return mIsDone;
}

void WorkingSerieModel::setIsDone(bool data) {
	if(mIsDone != data){
		mIsDone = data;
		emit isDoneChanged();
	}
}


int WorkingSerieModel::getOrder()const{
	return mTargetSerieModel->getOrder();
}

void WorkingSerieModel::setOrder(int data){
	mTargetSerieModel->setOrder(data);
	mResultSerieModel->setOrder(data);
	emit orderChanged();
}


void WorkingSerieModel::startWork(){
	mStart = QDateTime::currentDateTime();
	mResultSerieModel->setWorkTime(0);
	mResultSerieModel->setRestTime(0);
	setIsRunning(true);
	setIsResting(false);
	setIsDone(false);
	emit workStarted();
}

void WorkingSerieModel::pause(){
	mResultSerieModel->setWorkTime(mStart.secsTo(QDateTime::currentDateTime()) + mResultSerieModel->getWorkTime());
	mStart = QDateTime::currentDateTime();
}

void WorkingSerieModel::unpause(){
	mResultSerieModel->setRestTime(mStart.secsTo(QDateTime::currentDateTime()) + mResultSerieModel->getRestTime());
	mStart = QDateTime::currentDateTime();
}

int WorkingSerieModel::getElapsedWorkTime() const{
	return mStart.secsTo(QDateTime::currentDateTime()) + mResultSerieModel->getWorkTime();
}

int WorkingSerieModel::getElapsedRestTime() const{
	return mStart.secsTo(QDateTime::currentDateTime()) + mResultSerieModel->getRestTime();
}

void WorkingSerieModel::endOfCurrentWork(){
	mResultSerieModel->setWorkTime(mStart.secsTo(QDateTime::currentDateTime()) + mResultSerieModel->getWorkTime());
	setIsResting(true);
}

void WorkingSerieModel::endOfCurrentRest() {
	mResultSerieModel->setRestTime(mStart.secsTo(QDateTime::currentDateTime()) + mResultSerieModel->getRestTime());
	emit finished();
}

QVariant WorkingSerieModel::getCurrentWork() {
	return QVariant::fromValue(this);
}

int WorkingSerieModel::save(){
	return false;
}
//-------------------------------------------------------------------------------------------------------------------

void WorkingSerieModel::fillRandomValues(){
}


