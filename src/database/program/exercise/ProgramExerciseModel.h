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

#ifndef DATABASE_PROGRAM_EXERCISE_MODEL_H
#define DATABASE_PROGRAM_EXERCISE_MODEL_H

#include <QObject>
#include <QVector>
#include <QSqlQuery>

#include "src/tool/QmlModel.h"
#include "src/tool/QmlData.h"
#include "src/database/DatabaseQuery.h"

#include "../../exercise/ExerciseModel.h"
#include "../serie/ProgramSerieModel.h"

class ProgramExerciseModel : public QmlModel{
Q_OBJECT
// DB
	Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY descriptionChanged)
	Q_PROPERTY(int type READ getType WRITE setType NOTIFY typeChanged)
	Q_PROPERTY(int restTime READ getRestTime WRITE setRestTime NOTIFY restTimeChanged)
	Q_PROPERTY(int workTime READ getWorkTime WRITE setWorkTime NOTIFY workTimeChanged)
	Q_PROPERTY(QVariantList data READ getQmlVariantData NOTIFY dataChanged)
	Q_PROPERTY(int order READ getOrder WRITE setOrder NOTIFY orderChanged)

	Q_PROPERTY(ExerciseModel *exerciseModel READ getExerciseModel WRITE setExerciseModel NOTIFY exerciseModelChanged);
	Q_PROPERTY(QVariantList series READ getVariantSeries NOTIFY seriesChanged)
	Q_PROPERTY(bool canHaveSeries READ getCanHaveSeries WRITE setCanHaveSeries NOTIFY canHaveSeriesChanged)

public:
	ProgramExerciseModel();
	explicit ProgramExerciseModel(QObject *parent);
	ProgramExerciseModel(int type, QObject *parent);
	ProgramExerciseModel(const ProgramExerciseModel * model, QObject *parent);
	virtual ProgramExerciseModel * clone(QObject *parent)const;
	//virtual Training::ProgramExerciseModel * cloneTraining(qint64, QObject *){return nullptr;}

	qint64 getProgramExerciseId()const;
	virtual void setProgramExerciseId(qint64 id);

	qint64 getExerciseId()const;
	virtual void setExerciseId(qint64 id);

	ExerciseModel *getExerciseModel()const;
	virtual void setExerciseModel(ExerciseModel *model);


	Q_INVOKABLE qint64 getProgramId()const;
	Q_INVOKABLE virtual void setProgramId(qint64 id);

	QString getDescription() const;
	void setDescription(QString description);

	virtual int getType() const;
	virtual void setType(int data);

	int  getRestTime() const;
	void setRestTime(int restTime);

	int  getWorkTime() const;
	void setWorkTime(int workTime);

	virtual void setDbData(QVariant data); // Coming from DB
	QVariantList getQmlVariantData()const;
	const QList<QmlData *>& getData()const;
	void setData(const QList<QmlData *>);
	virtual void buildData();

	int getOrder() const;
	void setOrder(int data);

	virtual void updateIsSaved();
	virtual void undo();

// Serie
	QVariantList getVariantSeries() const;
	QList<ProgramSerieModel*> getSeries() const;
	Q_INVOKABLE void addSerie(ProgramSerieModel *model, bool keepId);
	Q_INVOKABLE void removeSerie(ProgramSerieModel *model);
	void handleRemoved(ProgramSerieModel *model);

	Q_INVOKABLE void decrementSerieOrder(ProgramSerieModel *model);
	Q_INVOKABLE void incrementSerieOrder(ProgramSerieModel *model);

	bool getCanHaveSeries() const;
	void setCanHaveSeries(bool data);

// ----------------------

	Q_INVOKABLE virtual bool save();
	virtual void saveValues(DatabaseQuery &query);
	static QList<ProgramExerciseModel*> loadAll(QObject * parent);
	static ProgramExerciseModel *load(QSqlQuery &query, QObject * parent);
	static void load(ProgramExerciseModel * model, QSqlQuery &query);
	Q_INVOKABLE virtual void remove();

signals:
	void programExerciseIdChanged();
	void exerciseIdChanged();
	void exerciseModelChanged();
	void programIdChanged();
	void descriptionChanged();
	void typeChanged();
	void restTimeChanged();
	void workTimeChanged();
	void dataChanged();
	void orderChanged();
	void isSavedChanged();
	void isEditedChanged();
	void removed(ProgramExerciseModel * model);
	void seriesChanged();
	void canHaveSeriesChanged();


protected:
	qint64 mProgramExerciseId = 0;
	qint64 mProgramId = 0;
	QPair<qint64, ExerciseModel*> mExercise = {-1,nullptr};
	QString mDescription;
	int mType = 0;
	int mWorkTime = 0;
	int mRestTime = 60;
	QList<QmlData *> mData;
	int mOrder = 0;

	QList<ProgramSerieModel*> mSeries;
	bool mCanHaveSeries = false;

	QMap<void*, QVariant> mBackupValues;
};


#endif
