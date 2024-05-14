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

#ifndef BASE_SERIE_MODEL_H
#define BASE_SERIE_MODEL_H

#include <QObject>
#include <QVector>

#include "../exercise/ExerciseModel.h"

namespace Base{
class SerieModel : public ExerciseModel{
Q_OBJECT

	Q_PROPERTY(int order READ getOrder WRITE setOrder NOTIFY orderChanged)
public:
	SerieModel();
	SerieModel(QObject *parent);
	SerieModel(int type, QObject *parent);
	SerieModel(const SerieModel * model, QObject *parent);
	virtual SerieModel * clone( QObject *parent)const;
	//virtual Training::ExerciseModel * cloneTraining(qint64, QObject *){return nullptr;}

	virtual void setDbData(QVariant data); // Coming from DB
	virtual void buildData();

	qint64 getSerieId() const;
	void setSerieId(qint64 data);

	int getOrder() const;
	void setOrder(int data);

	virtual void updateIsSaved();
	virtual void undo();

	Q_INVOKABLE virtual bool save();
	static SerieModel *load(QSqlQuery &query, QObject * parent);
	Q_INVOKABLE virtual void remove();

signals:
	void serieIdChanged();
	void orderChanged();

	void isSavedChanged();
	void removed(SerieModel * model);

protected:
	qint64 mSerieId = 0;
	int mOrder = 0;
};
}

#endif
