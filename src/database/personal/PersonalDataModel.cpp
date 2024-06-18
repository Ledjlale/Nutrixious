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

#include "PersonalDataModel.h"

#include <QQmlApplicationEngine>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include "src/database/DatabaseQuery.h"

#include "src/tool/Utils.h"


extern QQmlApplicationEngine * gEngine;

PersonalDataModel::PersonalDataModel() : PersonalDataModel(nullptr){	// QML
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}
PersonalDataModel::PersonalDataModel(QObject *parent)
	: QmlModel{parent}
{
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	mDateTime = QDateTime::currentDateTime();
	mBirthday = QDate(1983,8,1);
}

PersonalDataModel::PersonalDataModel(PersonalDataModel * model, QObject *parent)
	: QmlModel{parent}
{
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	mSex = model->getSex();
	mWeight = model->getWeight();
	mHeight = model->getHeight();
	mDateTime = model->getDateTime();
	mBirthday = model->getBirthday();
}

PersonalDataModel* PersonalDataModel::clone(QObject*parent) {
	auto newModel = new PersonalDataModel(this, parent);
	newModel->setDateTime(QDateTime::currentDateTime());
	return newModel;
}

qint64 PersonalDataModel::getPersonalDataId()const{
	return mPersonalDataId;
}

void PersonalDataModel::setPersonalDataId(qint64 id) {
	if(mPersonalDataId != id) {
		mPersonalDataId = id;
		emit personalDataIdChanged();
	}

}



QDateTime PersonalDataModel::getDateTime() const{
	return mDateTime;
}

void PersonalDataModel::setDateTime(const time_t& data_ms){
	setDateTime(QDateTime::fromMSecsSinceEpoch(data_ms));
}
void PersonalDataModel::setDateTime(const QDateTime& data){
	if(mDateTime != data){
		mDateTime = data;
		emit dateTimeChanged();
	}
}

QString PersonalDataModel::getDateTimeStr()const{
	if(mDateTime.isValid())
		return QLocale().toString(mDateTime, "yyyy/MM/dd hh:mm:ss");
	else
		return "";
}

void PersonalDataModel::setDateTimeStr(QString data) {
	QDateTime dateTime = QDateTime::fromString(data, "yyyy/MM/dd hh:mm:ss");
	if( dateTime.isValid() && dateTime != mDateTime){
		addBackup(&mDateTime, mDateTime, dateTime);
		mDateTime = dateTime;
		dateTimeChanged();
	}
}


QDate PersonalDataModel::getBirthday() const{
	return mBirthday;
}

void PersonalDataModel::setBirthday(const qint64& days){
	setBirthday(QDate::fromJulianDay(days));
}

void PersonalDataModel::setBirthday(const QDate& data){
	if(mBirthday != data){
		mBirthday = data;
		emit birthdayChanged();
	}
}

QString PersonalDataModel::getBirthdayStr()const{
	if(mBirthday.isValid())
		return QLocale().toString(mBirthday, "yyyy/MM/dd");
	else
		return "";
}

void PersonalDataModel::setBirthdayStr(QString data){
	QDate birthday = QDate::fromString(data, "yyyy/MM/dd");
	if( birthday.isValid() && birthday != mBirthday){
		addBackup(&mBirthday, mBirthday, birthday);
		mBirthday = birthday;
		birthdayChanged();
	}
}


double PersonalDataModel::getWeight() const{
	return mWeight;
}

void PersonalDataModel::setWeight(double data) {
	if(mWeight != data){
		addBackup(&mWeight, mWeight, data);
		mWeight = data;
		emit weightChanged();
	}
}

int PersonalDataModel::getHeight() const{
	return mHeight;
}

void PersonalDataModel::setHeight(int data) {
	if(mHeight != data){
		addBackup(&mHeight, mHeight, data);
		mHeight = data;
		emit heightChanged();
	}
}

int PersonalDataModel::getSex() const{
	return mSex;
}

void PersonalDataModel::setSex(int data) {
	if(mSex!= data){
		addBackup(&mSex, mSex, data);
		mSex = data;
		emit sexChanged();
	}
}


//-------------------------------------------------------------------------------------------------------------------

bool PersonalDataModel::add(){
	auto newModel = clone(nullptr);
	bool result = newModel->save();
	newModel->deleteLater();
	return result;
}

int PersonalDataModel::save(){
	if(mPersonalDataId>0 && !mIsEdited) return true;// Avoid update for nothing
	DatabaseQuery query;

	query.begin(mPersonalDataId == 0 ? DatabaseQuery::Insert : DatabaseQuery::Update, "personal_data" );

	query.add("weight", mWeight);
	query.add("height", mHeight);
	query.add("date_time", mDateTime.toMSecsSinceEpoch());
	query.add("sex", mSex);
	query.add("birthday", mBirthday.toJulianDay());
	query.addConditionnal("personal_data_id",mPersonalDataId);

	if(mPersonalDataId == 0){
		if(!query.exec()) qCritical() << "Cannot save personal data: "  << query.mQuery.lastError().text();
		auto fieldNo = query.mQuery.record().indexOf("personal_data_id");
		while (query.mQuery.next()) {
			setPersonalDataId(query.mQuery.value(fieldNo).toLongLong());
			qDebug() << "Insert personal data: " << mPersonalDataId;
		}
		clearBackupValues();
	}else{
		if(!query.exec()) qCritical() << "Cannot update personal data: "  << query.mQuery.lastError().text();
		else {
			qDebug() << "Update personal data: " << mPersonalDataId;
			clearBackupValues();
		}
	}
	return true;
}

void PersonalDataModel::remove(){
	if(mPersonalDataId > 0){
		DatabaseQuery query;
		query.begin(DatabaseQuery::Delete, "personal_data");
		query.addConditionnal("personal_data_id",mPersonalDataId);
		if(!query.exec()){
			if(!query.exec()) qCritical() << "Cannot delete personal data  : "  << query.mQuery.lastError().text();
		}
	}
	emit removed(this);
}

PersonalDataModel *PersonalDataModel::load(QSqlQuery &query, QObject * parent) {
	auto idField = query.record().indexOf("personal_data.personal_data_id");
	auto weightField = query.record().indexOf("personal_data.weight");
	auto heightField = query.record().indexOf("personal_data.height");
	auto sexField = query.record().indexOf("personal_data.sex");
	auto dateTimeField = query.record().indexOf("personal_data.date_time");
	auto birthdayField = query.record().indexOf("personal_data.birthday");
	if( idField>= 0 && weightField >= 0 && heightField >= 0 && sexField>=0 && dateTimeField>=0){
		PersonalDataModel * model = new PersonalDataModel(parent);
	// TODO optimize
		model->setPersonalDataId(query.value(idField).toLongLong());
		model->setWeight(query.value(weightField).toDouble());
		model->setHeight(query.value(heightField).toInt());
		model->setSex(query.value(sexField).toInt());
		model->setDateTime(query.value(dateTimeField).toULongLong());
		model->setBirthday(query.value(birthdayField).toULongLong());
		model->clearBackupValues();
		return model;
	}else return nullptr;
}

void PersonalDataModel::set(QSqlQuery &query,PersonalDataModel * model) {
	auto idField = query.record().indexOf("personal_data.personal_data_id");
	auto weightField = query.record().indexOf("personal_data.weight");
	auto heightField = query.record().indexOf("personal_data.height");
	auto sexField = query.record().indexOf("personal_data.sex");
	auto dateTimeField = query.record().indexOf("personal_data.date_time");
	auto birthdayField = query.record().indexOf("personal_data.birthday");
	if( idField>= 0 && weightField >= 0 && heightField >= 0 && sexField>=0){
	// TODO optimize
		model->setPersonalDataId(query.value(idField).toLongLong());
		model->setWeight(query.value(weightField).toDouble());
		model->setHeight(query.value(heightField).toInt());
		model->setSex(query.value(sexField).toInt());
		model->setDateTime(query.value(dateTimeField).toULongLong());
		model->setBirthday(query.value(birthdayField).toULongLong());
		model->clearBackupValues();
	}
}

QList<PersonalDataModel*> PersonalDataModel::loadAll(QObject * parent){
	QList<PersonalDataModel*> models;
	QSqlQuery query;
	if(!query.exec( "SELECT * FROM personal_data ORDER BY date_time DESC")) qCritical() << "Cannot select all personal data  : "  << query.lastError().text();

	while (query.next()) {
		auto model = load(query, parent);
		models << model;
	}

	return models;
}

void PersonalDataModel::loadLast(){
	QSqlQuery query;
	if(!query.exec( "SELECT * FROM personal_data ORDER BY date_time DESC LIMIT 1")) qCritical() << "Cannot select last personal data  : "  << query.lastError().text();
	else if(query.next())
		set(query, this);
}

