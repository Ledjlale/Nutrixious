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

#ifndef STRENGTH_WORK_MODEL_H
#define STRENGTH_WORK_MODEL_H

#include <QObject>
#include <QVector>
#include <QSqlQuery>

namespace Training{
	class StrengthWorkModel;
}

namespace Description{
class StrengthWorkModel : public QObject {
	Q_OBJECT

	Q_PROPERTY(int repetitions MEMBER mRepetitions NOTIFY repetitionsChanged)
	Q_PROPERTY(int weight MEMBER mWeight NOTIFY weightChanged)
	Q_PROPERTY(int restTime MEMBER mRestTime NOTIFY restTimeChanged)

	Q_PROPERTY(bool isSaved READ getIsSaved WRITE setIsSaved NOTIFY isSavedChanged)
	Q_PROPERTY(bool isTraining MEMBER mIsTraining CONSTANT)

public:
	StrengthWorkModel();
	explicit StrengthWorkModel(QObject *parent);
	StrengthWorkModel(const StrengthWorkModel * model, QObject *parent );

	virtual StrengthWorkModel *clone(qint64 strengthId, QObject *parent)const;
	virtual Training::StrengthWorkModel * cloneTraining(qint64 programId, qint64 strengthId, QObject *parent);
	static StrengthWorkModel *load(QSqlQuery &query, QObject * parent);	// Create a model from the current query.

	int  getRepetitions() const;
	void setRepetitions(int reps);

	int  getWeight() const;
	void setWeight(int weight);

	int  getRestTime() const;
	void setRestTime(int restTime);

	qint64 getWorkId() const;
	void setWorkId(qint64 id);

	qint64 getStrengthId() const;
	void setStrengthId(qint64 id);

	qint64 getProgramId() const;
	void setProgramId(qint64 id);
	bool isProgramLinked()const;

	bool getIsSaved() const;
	void setIsSaved(bool data);

	Q_INVOKABLE void save();
signals:
	void repetitionsChanged();
	void weightChanged();
	void restTimeChanged();
	void workIdChanged();
	void strengthIdChanged();
	void programIdChanged();
	void isSavedChanged();

protected:
	qint64 mWorkId = 0;
	qint64 mStrengthId = 0;
	qint64 mProgramId = -1;
	int mRepetitions = 1;
	int mWeight = 0;
	int mRestTime = 2;
	bool mIsSaved = false;
	bool mIsTraining = false;
};
}
#endif
