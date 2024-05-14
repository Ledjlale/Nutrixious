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

#ifndef DATABASE_TRAINING_EXERCISE_MODEL_H
#define DATABASE_TRAINING_EXERCISE_MODEL_H

#include <QObject>
#include <QVector>
#include <QSqlQuery>

#include "src/tool/QmlModel.h"
#include "src/tool/QmlData.h"
#include "src/database/DatabaseQuery.h"

#include "../../exercise/ExerciseModel.h"
#include "../serie/TrainingSerieModel.h"
#include "../../program/exercise/ProgramExerciseModel.h"

class TrainingExerciseModel : public QmlModel{
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
	TrainingExerciseModel();
	explicit TrainingExerciseModel(QObject *parent);
	TrainingExerciseModel(int type, QObject *parent);
	TrainingExerciseModel(const TrainingExerciseModel * model, QObject *parent);
	TrainingExerciseModel(const ProgramExerciseModel * model, QObject *parent);
	virtual TrainingExerciseModel * clone(QObject *parent)const;
	//virtual Training::TrainingExerciseModel * cloneTraining(qint64, QObject *){return nullptr;}

	qint64 getTrainingExerciseId()const;
	virtual void setTrainingExerciseId(qint64 id);

	qint64 getExerciseId()const;
	virtual void setExerciseId(qint64 id);

	ExerciseModel *getExerciseModel()const;
	virtual void setExerciseModel(ExerciseModel *model);


	Q_INVOKABLE qint64 getTrainingId()const;
	Q_INVOKABLE virtual void setTrainingId(qint64 id);

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
	QList<TrainingSerieModel*> getSeries() const;
	Q_INVOKABLE void addSerie(TrainingSerieModel *model, bool keepId);
	Q_INVOKABLE void removeSerie(TrainingSerieModel *model);
	void handleRemoved(TrainingSerieModel *model);

	Q_INVOKABLE void decrementSerieOrder(TrainingSerieModel *model);
	Q_INVOKABLE void incrementSerieOrder(TrainingSerieModel *model);

	bool getCanHaveSeries() const;
	void setCanHaveSeries(bool data);

// ----------------------

	Q_INVOKABLE virtual bool save();
	virtual void saveValues(DatabaseQuery &query);
	static QList<TrainingExerciseModel*> loadAll(QObject * parent);
	static QList<TrainingExerciseModel*> loadAll(qint64 exerciseId, QObject * parent);
	static TrainingExerciseModel *load(QSqlQuery &query, QObject * parent);
	static void load(TrainingExerciseModel * model, QSqlQuery &query);
	Q_INVOKABLE virtual void remove();

signals:
	void trainingExerciseIdChanged();
	void exerciseIdChanged();
	void exerciseModelChanged();
	void trainingIdChanged();
	void descriptionChanged();
	void typeChanged();
	void restTimeChanged();
	void workTimeChanged();
	void dataChanged();
	void orderChanged();
	void isSavedChanged();
	void isEditedChanged();
	void removed(TrainingExerciseModel * model);
	void seriesChanged();
	void canHaveSeriesChanged();


protected:
	qint64 mTrainingExerciseId = 0;
	qint64 mTrainingId = 0;
	QPair<qint64, ExerciseModel*> mExercise = {-1,nullptr};
	QString mDescription;
	int mType = 0;
	int mWorkTime = 0;
	int mRestTime = 0;
	QList<QmlData *> mData;
	int mOrder = 0;

	QList<TrainingSerieModel*> mSeries;
	bool mCanHaveSeries = false;

	QMap<void*, QVariant> mBackupValues;
};


#endif
