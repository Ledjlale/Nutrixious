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

#ifndef DATABASE_FOOD_MODEL_H
#define DATABASE_FOOD_MODEL_H

#include <QObject>
#include <QVector>
#include <QVariantList>
#include <QSqlQuery>

#include "src/tool/QmlModel.h"

class FoodModel : public QmlModel{
Q_OBJECT
// DB
	Q_PROPERTY(QString name MEMBER mName NOTIFY nameChanged)
	Q_PROPERTY(QString description MEMBER mDescription NOTIFY descriptionChanged)
	Q_PROPERTY(QString imageUrl MEMBER mImageUrl NOTIFY imageUrlChanged)

	Q_PROPERTY(double calories MEMBER mCalories NOTIFY caloriesChanged)


public:
	FoodModel();	// QML
	FoodModel(QObject *parent);
	FoodModel(FoodModel * model, QObject *parent);
	FoodModel* clone(QObject*parent);

	qint64 getFoodId()const;
	virtual void setFoodId(qint64 id);

	QString getName() const;
	void setName(QString name);

	QString getDescription() const;
	void setDescription(QString description);


	Q_INVOKABLE virtual bool save();
	Q_INVOKABLE virtual void remove();

	static QList<FoodModel*> buildAll(QObject * parent);
	static FoodModel *build(QSqlQuery &query, QObject * parent);

signals:
	void exerciseIdChanged();
	void nameChanged();
	void descriptionChanged();
	void removed(FoodModel *model);

protected:
	qint64 mExerciseId = 0;
	QString mName;
	QString mDescription;
};

#endif
