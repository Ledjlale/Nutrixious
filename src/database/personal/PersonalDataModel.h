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

#ifndef DATABASE_PERSONAL_DATA_MODEL_H
#define DATABASE_PERSONAL_DATA_MODEL_H

#include <QObject>
#include <QVector>
#include <QVariantList>
#include <QSqlQuery>
#include <QDateTime>

#include "src/tool/QmlModel.h"
#include "src/tool/date/DateModel.h"

class PersonalDataModel : public QmlModel{
Q_OBJECT
// DB
	Q_PROPERTY(double weight READ getWeight WRITE setWeight NOTIFY weightChanged)
	Q_PROPERTY(QString dateTimeStr READ getDateTimeStr WRITE setDateTimeStr NOTIFY dateTimeChanged)
	Q_PROPERTY(int sex READ getSex WRITE setSex NOTIFY sexChanged)
	Q_PROPERTY(int height READ getHeight WRITE setHeight NOTIFY heightChanged)
	Q_PROPERTY(QString birthday READ getBirthdayStr WRITE setBirthdayStr NOTIFY birthdayChanged)

public:
	PersonalDataModel();	// QML
	PersonalDataModel(QObject *parent);
	PersonalDataModel(PersonalDataModel * model, QObject *parent);
	PersonalDataModel* clone(QObject*parent);

	qint64 getPersonalDataId()const;
	virtual void setPersonalDataId(qint64 id);

	QDateTime getDateTime() const;
	void setDateTime(const time_t& data_ms);	//Description ms
	void setDateTime(const QDateTime& data);
	QString getDateTimeStr()const;
	void setDateTimeStr(QString data);

	QDate getBirthday() const;
	void setBirthday(const qint64& days);	//Description ms
	void setBirthday(const QDate& data);
	QString getBirthdayStr()const;
	void setBirthdayStr(QString data);

	double getWeight()const;
	void setWeight(double data);

	int getSex()const;
	void setSex(int data);

	int getHeight()const;
	void setHeight(int data);


	virtual void updateIsSaved();
	Q_INVOKABLE virtual bool add();
	Q_INVOKABLE virtual int save();
	Q_INVOKABLE virtual void remove();
	Q_INVOKABLE virtual void undo();

	static QList<PersonalDataModel*> loadAll(QObject * parent);
	static PersonalDataModel *load(QSqlQuery &query, QObject * parent);
	static void set(QSqlQuery &query,PersonalDataModel * model);
	Q_INVOKABLE void loadLast();
	Q_INVOKABLE void loadLastIn(DateModel * data );

signals:
	void personalDataIdChanged();
	void weightChanged();
	void dateTimeChanged();
	void sexChanged();
	void heightChanged();
	void birthdayChanged();

	void removed(PersonalDataModel *model);

protected:
	qint64 mPersonalDataId = 0;
	int mSex = 0;
	double mWeight = 90.0;
	int mHeight = 180;
	QDateTime mDateTime;
	QDate mBirthday;
};

#endif
