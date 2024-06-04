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

#ifndef DATABASE_EXERCISE_UNIT_MODEL_H
#define DATABASE_EXERCISE_UNIT_MODEL_H

#include <QObject>
#include <QVector>
#include <QSqlQuery>

#include "src/tool/QmlModel.h"
#include "src/database/DatabaseQuery.h"

#include "../exercise/ExerciseModel.h"
#include "../exerciseSerie/ExerciseSerieModel.h"

class ExerciseUnitModel : public QmlModel{
Q_OBJECT
// DB
	Q_PROPERTY(int order READ getOrder WRITE setOrder NOTIFY orderChanged)

	DECLARE_GETSET(QString,description,Description)
	DECLARE_GETSET(bool,useDistance,UseDistance)
	DECLARE_GETSET(bool,useSpeed,UseSpeed)
	DECLARE_GETSET(bool,useWeight,UseWeight)
	DECLARE_GETSET(bool,useRepetitions,UseRepetitions)

	Q_PROPERTY(ExerciseModel *exerciseModel READ getExerciseModel WRITE setExerciseModel NOTIFY exerciseModelChanged);
	Q_PROPERTY(QVariantList series READ getVariantSeries NOTIFY seriesChanged)
	Q_PROPERTY(bool canHaveSeries READ getCanHaveSeries WRITE setCanHaveSeries NOTIFY canHaveSeriesChanged)

public:
	ExerciseUnitModel();
	explicit ExerciseUnitModel(QObject *parent);
	ExerciseUnitModel(int type, QObject *parent);
	ExerciseUnitModel(const ExerciseUnitModel * model, QObject *parent);
	virtual ~ExerciseUnitModel();
	virtual ExerciseUnitModel * clone(QObject *parent)const;

	qint64 getExerciseUnitId()const;
	virtual void setExerciseUnitId(qint64 id);

	qint64 getExerciseId()const;
	virtual void setExerciseId(qint64 id);

	ExerciseModel *getExerciseModel()const;
	virtual void setExerciseModel(ExerciseModel *model);


	Q_INVOKABLE qint64 getParentId()const;
	Q_INVOKABLE virtual void setParentId(qint64 id);

	int getOrder() const;
	void setOrder(int data);

	virtual void updateIsSaved();
	virtual void undo();

// Serie
	QVariantList getVariantSeries() const;
	QList<ExerciseSerieModel*> getSeries() const;

	virtual void addNewSerie(ExerciseSerieModel *model);	// Clone
	virtual ExerciseSerieModel* insertSerie(ExerciseSerieModel *model);
	Q_INVOKABLE void makeNew();

	Q_INVOKABLE void removeSerie(ExerciseSerieModel *model);
	void handleRemoved(ExerciseSerieModel *model);

	Q_INVOKABLE void decrementSerieOrder(ExerciseSerieModel *model);
	Q_INVOKABLE void incrementSerieOrder(ExerciseSerieModel *model);

	bool getCanHaveSeries() const;
	void setCanHaveSeries(bool data);

// ----------------------

	virtual void addQueryValues(DatabaseQuery &query){}
	Q_INVOKABLE virtual bool save();
	virtual void saveValues(DatabaseQuery &query);
	virtual void load(QSqlQuery &query);
	Q_INVOKABLE virtual void remove();

signals:
	void exerciseUnitIdChanged();
	void exerciseIdChanged();
	void exerciseModelChanged();
	void parentIdChanged();
	void orderChanged();
	void isSavedChanged();
	void isEditedChanged();
	void removed(ExerciseUnitModel * model);
	void seriesChanged();
	void canHaveSeriesChanged();


protected:
	qint64 mExerciseUnitId = 0;
	qint64 mParentId = 0;
	QPair<qint64, ExerciseModel*> mExercise = {-1,nullptr};
	QString mDescription;

	bool mUseDistance = true;
	bool mUseSpeed = true;
	bool mUseWeight = true;
	bool mUseRepetitions = true;
	int mOrder = 0;

	QList<ExerciseSerieModel*> mSeries;
	bool mCanHaveSeries = false;

	QMap<void*, QVariant> mBackupValues;

//
	QString mTablePrefix = "";
};


#endif
