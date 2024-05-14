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

#ifndef DATABASE_SERIE_MODEL_H
#define DATABASE_SERIE_MODEL_H

#include <QObject>
#include <QVector>
#include <QSqlQuery>

#include "src/tool/QmlModel.h"
#include "src/tool/QmlData.h"

class ProgramSerieModel : public QmlModel{
Q_OBJECT
// DB
	Q_PROPERTY(int type READ getType WRITE setType NOTIFY typeChanged)
	Q_PROPERTY(int restTime READ getRestTime WRITE setRestTime NOTIFY restTimeChanged)
	Q_PROPERTY(int workTime READ getWorkTime WRITE setWorkTime NOTIFY workTimeChanged)
	Q_PROPERTY(QVariantList data READ getQmlVariantData NOTIFY dataChanged)
	Q_PROPERTY(int order READ getOrder WRITE setOrder NOTIFY orderChanged)

public:
	ProgramSerieModel();
	ProgramSerieModel(QObject *parent);
	ProgramSerieModel(int type, QObject *parent);
	ProgramSerieModel(const ProgramSerieModel * model, QObject *parent);
	virtual ProgramSerieModel * clone( QObject *parent)const;

	qint64 getProgramSerieId() const;
	void setProgramSerieId(qint64 data);

	qint64 getProgramExerciseId() const;
	void setProgramExerciseId(qint64 data);

	int getType() const;
	void setType(int data);

	int  getRestTime() const;
	void setRestTime(int restTime);

	int  getWorkTime() const;
	void setWorkTime(int workTime);

	virtual void setDbData(QVariant data); // Coming from DB
	virtual void buildData();
	QVariantList getQmlVariantData()const;
	const QList<QmlData *>& getData()const;
	void setData(const QList<QmlData *>);

	int getOrder() const;
	void setOrder(int data);

	virtual void updateIsSaved();
	virtual void undo();

	Q_INVOKABLE virtual bool save();
	static ProgramSerieModel *load(QSqlQuery &query, QObject * parent);
	Q_INVOKABLE virtual void remove();

signals:
	void serieIdChanged();
	void programExerciseIdChanged();
	void orderChanged();
	void typeChanged();
	void restTimeChanged();
	void workTimeChanged();
	void dataChanged();

	void removed(ProgramSerieModel * model);

protected:
	qint64 mProgramSerieId = 0;
	qint64 mProgramExerciseId = 0;
	int mType = 0;
	int mWorkTime = 0;
	int mRestTime = 0;
	QList<QmlData *> mData;
	int mOrder = 0;
};

#endif
