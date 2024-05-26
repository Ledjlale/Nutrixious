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

#ifndef DATABASE_EXERCISE_MODEL_H
#define DATABASE_EXERCISE_MODEL_H

#include <QObject>
#include <QVector>
#include <QVariantList>
#include <QSqlQuery>

#include "src/tool/QmlModel.h"

class ExerciseModel : public QmlModel{
Q_OBJECT
// DB
	Q_PROPERTY(QString name MEMBER mName NOTIFY nameChanged)
	Q_PROPERTY(double met MEMBER mMet NOTIFY metChanged)

public:
	ExerciseModel();	// QML
	ExerciseModel(QObject *parent);
	ExerciseModel(ExerciseModel * model, QObject *parent);
	ExerciseModel* clone(QObject*parent);

	qint64 getExerciseId()const;
	virtual void setExerciseId(qint64 id);

	QString getName() const;
	void setName(QString name);

	double getMet() const;
	void setMet(double data);


	Q_INVOKABLE virtual bool save();
	Q_INVOKABLE virtual void remove();

	static QList<ExerciseModel*> buildAll(QObject * parent);
	static ExerciseModel *build(QSqlQuery &query, QObject * parent);

signals:
	void exerciseIdChanged();
	void nameChanged();
	void metChanged();
	void removed(ExerciseModel *model);

protected:
	qint64 mExerciseId = 0;
	QString mName;
	double mMet = 1.0;
};

#endif
