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

#include "DatabaseQuery.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QMapIterator>

DatabaseQuery::DatabaseQuery(QObject *parent)
	: QObject{parent}
{}

void DatabaseQuery::begin(Type type, QString table){
	mType = type;
	mTable = table;
	mKeys.clear();
	mValues.clear();
}
void DatabaseQuery::add(QString key, QVariant value){
	switch(mType){
		case Insert :
			mKeys << key;
			mValues << value;
		break;
		case Update :
			mKeys << key + "=?";
			mValues << value;
		break;
		case Delete:
		break;
	}
}

void DatabaseQuery::addConditionnal(QString key, QVariant value ) {
	mCondKey = key;
	mCondValue = value;
}

QString getLastExecutedQuery(const QSqlQuery& query) {
	QString str = query.executedQuery();
	const QVariantList list = query.boundValues();
	for (qsizetype i = list.size()-1; i >=0; --i){
		str.replace(":"+QString::number(i),list.at(i).toString());
	}
	return str;
}

bool DatabaseQuery::exec(){
	mQuery.exec("PRAGMA foreign_keys=ON");
	switch(mType){
		case Insert :{
			QStringList placeHolders;
			for(auto &i : mKeys) placeHolders << "?";
			mQuery.prepare("INSERT INTO "+mTable+"("+mKeys.join(',')+") VALUES("+placeHolders.join(',')+")" +(mCondKey.isEmpty() ? "" :" RETURNING "+mCondKey));
					}
		break;
		case Update :
			mQuery.prepare("UPDATE " +mTable +" SET " +mKeys.join(',') +" WHERE "+mCondKey+"=?");
		break;
		case Delete:
			mQuery.prepare("DELETE FROM " +mTable +" WHERE "+mCondKey+"=?");
		break;
	}
	for(auto &i : mValues) mQuery.addBindValue(i);
	if(mType == Update || mType == Delete) mQuery.addBindValue(mCondValue);
	bool i =  mQuery.exec();
	return i;
}

