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

#ifndef BASE_EXERCISE_MODEL_H
#define BASE_EXERCISE_MODEL_H

#include <QObject>
#include <QVector>
#include <QSqlQuery>

#include "src/tool/QmlData.h"
#include "src/database/DatabaseQuery.h"
namespace Base{
class SerieModel;

class ExerciseModel : public QObject{
Q_OBJECT
	Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
	Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY descriptionChanged)
	Q_PROPERTY(int type READ getType WRITE setType NOTIFY typeChanged)
	Q_PROPERTY(int restTime READ getRestTime WRITE setRestTime NOTIFY restTimeChanged)
	Q_PROPERTY(int workTime READ getWorkTime WRITE setWorkTime NOTIFY workTimeChanged)
	Q_PROPERTY(QVariantList data READ getQmlVariantData NOTIFY dataChanged)
	Q_PROPERTY(QVariantList series READ getVariantSeries NOTIFY seriesChanged)
	Q_PROPERTY(bool canHaveSeries READ getCanHaveSeries WRITE setCanHaveSeries NOTIFY canHaveSeriesChanged)


	Q_PROPERTY(bool invalidName MEMBER mInvalidName NOTIFY invalidNameChanged)
	Q_PROPERTY(bool invalidDescription MEMBER mInvalidDescription NOTIFY invalidDescriptionChanged)

	Q_PROPERTY(bool isSaved READ getIsSaved WRITE setIsSaved NOTIFY isSavedChanged)
	Q_PROPERTY(bool isEdited READ getIsEdited WRITE setIsEdited NOTIFY isEditedChanged)
public:
	ExerciseModel();
	explicit ExerciseModel(QObject *parent);
	ExerciseModel(int type, QObject *parent);
	ExerciseModel(const ExerciseModel * model, QObject *parent);
	virtual ExerciseModel * clone(QObject *parent)const;
	//virtual Training::ExerciseModel * cloneTraining(qint64, QObject *){return nullptr;}

	qint64 getExerciseId()const;
	virtual void setExerciseId(qint64 id);

	QString getName()const;
	void setName(QString name);

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


	void setInvalidName(bool invalid);
	void setInvalidDescription(bool invalid);

	//int getOrder() const;
	//void setOrder(int data);

	bool getIsSaved() const;
	void setIsSaved(bool data);
	virtual void updateIsSaved();

	bool getIsEdited() const;
	void setIsEdited(bool data);
	void addBackup(void *key, QVariant keyValue, QVariant newValue);
	virtual void updateIsEdited();
	void clearBackupValues();
	virtual void undo();

// Serie
	QVariantList getVariantSeries() const;
	QList<SerieModel*> getSeries() const;
	Q_INVOKABLE void addSerie(SerieModel *model);	// Clone
	void addSerie(SerieModel *model, bool keepId);
	Q_INVOKABLE void removeSerie(SerieModel *model);
	void handleRemoved(SerieModel *model);

	Q_INVOKABLE void decrementSerieOrder(SerieModel *model);
	Q_INVOKABLE void incrementSerieOrder(SerieModel *model);

	bool getCanHaveSeries() const;
	void setCanHaveSeries(bool data);

// ----------------------

	Q_INVOKABLE virtual bool save();
	virtual void saveValues(DatabaseQuery &query);
	static QList<ExerciseModel*> loadAll(QObject * parent);
	static ExerciseModel *load(QSqlQuery &query, QObject * parent);
	static void load(ExerciseModel * model, QSqlQuery &query);
	Q_INVOKABLE virtual void remove();

signals:
	void exerciseIdChanged();
	void nameChanged();
	void descriptionChanged();
	void typeChanged();
	void restTimeChanged();
	void workTimeChanged();
	void dataChanged();
	void isSavedChanged();
	void isEditedChanged();
	void removed(ExerciseModel * model);
	void seriesChanged();
	void canHaveSeriesChanged();

	void invalidNameChanged();
	void invalidDescriptionChanged();

protected:
	qint64 mExerciseId = 0;
	QString mName;
	QString mDescription;
	int mType = 0;
	int mWorkTime = 0;
	int mRestTime = 0;
	QList<QmlData *> mData;

	bool mInvalidName = true;
	bool mInvalidDescription = false;
	bool mIsSaved = false;
	bool mIsEdited = false;
	QList<SerieModel*> mSeries;
	bool mCanHaveSeries = false;

	QMap<void*, QVariant> mBackupValues;

	QString mDbTable = "exercises";
};

/*
class StrengthModel : public QObject{
	Q_PROPERTY(int repetition READ getRepetition WRITE setRepetition NOTIFY repetitionChanged)
	Q_PROPERTY(int weight READ getWeight WRITE setWeight NOTIFY weightChanged)
public:
	StrengthModel(ExerciseModel * parent) {
		mParent = parent;
		connect(mParent, &ExerciseModel);
	}

	int getWeight()const{
		return mParent->getData()[1].toInt();
	}
	void setWeight(int data){
		mParent->setData(1, data);
	}
signals:
	void repetitionChanged();
	void weightChanged();
protected:
	ExerciseModel *mParent;
};*/

}

#endif
