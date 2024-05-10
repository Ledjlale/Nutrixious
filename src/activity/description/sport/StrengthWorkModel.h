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

public:
	explicit StrengthWorkModel(QObject *parent = nullptr);
	StrengthWorkModel(const StrengthWorkModel * model, QObject *parent = nullptr);

	virtual StrengthWorkModel *clone(qint64 strengthId)const;
	virtual Training::StrengthWorkModel * cloneTraining(qint64 programId, qint64 strengthId);
	static StrengthWorkModel *load(QSqlQuery &query);	// Create a model from the current query.

	int  getRepetitions() const;
	void setRepetitions(int reps);

	int  getWeight() const;
	void setWeight(int weight);

	int  getRestTime() const;
	void setRestTime(int restTime);

	qint64 getId() const;
	void setId(qint64 id);

	qint64 getStrengthId() const;
	void setStrengthId(qint64 id);

	qint64 getProgramId() const;
	void setProgramId(qint64 id);
	bool isProgramLinked()const;

	Q_INVOKABLE void save();
signals:
	void repetitionsChanged();
	void weightChanged();
	void restTimeChanged();
	void idChanged();
	void strengthIdChanged();
	void programIdChanged();

protected:
	qint64 mDbId = 0;
	qint64 mStrengthId = 0;
	qint64 mProgramId = -1;
	int mRepetitions = 1;
	int mWeight = 0;
	int mRestTime = 2;
};
}
#endif
