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

#ifndef DATABASE_UNIT_MODEL_H
#define DATABASE_UNIT_MODEL_H

#include <QObject>
#include <QVector>
#include <QVariantList>
#include <QSqlQuery>

#include "src/tool/QmlModel.h"

class UnitModel : public QmlModel{
Q_OBJECT
// DB
	DECLARE_GETSET(qint64,unitId,UnitId)
	DECLARE_GETSET(QString,name,Name)
	DECLARE_GETSET(double,gramValue,GramValue)
	DECLARE_GETSET(double,meterValue,MeterValue)
	DECLARE_GETSET(double,secondValue,SecondValue)
	DECLARE_GETSET(double,kcalValue,KcalValue)
	DECLARE_GETSET(double,milliliterValue,MilliliterValue)

public:
	UnitModel();	// QML
	UnitModel(QObject *parent);
	UnitModel(QString name, double gram, double meter, double second, double kcal, double ml, QObject *parent);


	Q_INVOKABLE virtual bool save();

	static QList<UnitModel*> buildAll(QObject * parent);
	static UnitModel *build(QSqlQuery &query, QObject * parent);



signals:

	void removed(UnitModel *model);
protected:
	qint64 mUnitId = 0;
	QString mName;
	double mGramValue = 0.0;
	double mMeterValue = 0.0;
	double mSecondValue = 0.0;
	double mKcalValue = 0.0;
	double mMilliliterValue = 0.0;
};

#endif
