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

#include "QmlModel.h"

QmlModel::QmlModel(QObject * parent) : QObject(parent){

}


bool QmlModel::getIsSaved() const {
	return mIsSaved;
}

void QmlModel::setIsSaved(bool data) {
	if(mIsSaved != data){
		mIsSaved = data;
		emit isSavedChanged();
	}
}

void QmlModel::updateIsSaved() {
}

void QmlModel::updateIsEdited() {
	setIsEdited(mBackupValues.size() > 0);
}

bool QmlModel::getIsEdited() const {
	return mIsEdited;
}

void QmlModel::setIsEdited(bool data) {
	if(mIsEdited != data){
		mIsEdited = data;
		emit isEditedChanged();
	}
}

QVariant QmlModel::initiBackup(const QmlModel *fromModel, const void *from, const QVariant fromValue, void *to){
	if(fromModel->mBackupValues.contains(from))
		addBackup(&to, fromValue, fromValue);
	return fromValue;
}

void QmlModel::addBackup(void *key, QVariant keyValue, QVariant newValue){
	if(mBackupValues.contains(key)) {
		if(mBackupValues[key]  == newValue) {
			mBackupValues.remove(key);
			updateIsEdited();
		}
	}else{
		mBackupValues[key] = keyValue;
		setIsEdited(true);
	}
}

void QmlModel::clearBackupValues(){
	mBackupValues.clear();
	setIsEdited(false);
}

void QmlModel::undo(){
	clearBackupValues();
}
