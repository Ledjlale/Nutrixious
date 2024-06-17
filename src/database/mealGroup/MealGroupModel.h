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

#ifndef DATABASE_MEAL_GROUP_MODEL_H
#define DATABASE_MEAL_GROUP_MODEL_H

#include <QObject>
#include <QVector>
#include <QVariantList>
#include <QSqlQuery>
#include <QDateTime>

#include "src/tool/QmlModel.h"

class MealGroupModel : public QmlModel{
Q_OBJECT
// DB
	DECLARE_GETSET(qint64,mealGroupId,MealGroupId)
	DECLARE_GETSET(QString,name,Name)
	DECLARE_GETSET(QTime,defaultTime,DefaultTime)
	DECLARE_GETSET(bool,isDisplayed,IsDisplayed)

	Q_PROPERTY(QString defaultTimeStr READ getDefaultTimeStr WRITE setDefaultTimeStr NOTIFY defaultTimeChanged)

public:
	MealGroupModel();	// QML
	MealGroupModel(QObject *parent);
	MealGroupModel(MealGroupModel * model, QObject *parent);
	MealGroupModel(QString name, QTime defaultTime);
	MealGroupModel* clone(QObject*parent);


	QString getDefaultTimeStr()const;
	void setDefaultTimeStr(QString data);

	Q_INVOKABLE virtual int save();
	Q_INVOKABLE virtual void remove();

	static QList<MealGroupModel*> buildAll(QObject * parent);
	static MealGroupModel *build(QSqlQuery &query, QObject * parent);

signals:
	void removed(MealGroupModel *model);

protected:
	qint64 mMealGroupId = 0;
	QString mName;
	QTime mDefaultTime;
	bool mIsDisplayed = true;
};

#endif
