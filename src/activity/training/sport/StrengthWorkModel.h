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

#ifndef TRAINING_STRENGTH_WORK_MODEL_H
#define TRAINING_STRENGTH_WORK_MODEL_H

#include <QObject>
#include <QVector>
#include <QSqlQuery>
#include <QDateTime>

#include "src/activity/description/sport/StrengthWorkModel.h"

namespace Training{
class StrengthWorkModel : public QObject {
	Q_OBJECT
	Q_PROPERTY(Description::StrengthWorkModel *targetWork MEMBER mTargetWork NOTIFY targetWorkChanged)
	Q_PROPERTY(int repetitions MEMBER mRepetitions NOTIFY repetitionsChanged)
	Q_PROPERTY(int weight MEMBER mWeight NOTIFY weightChanged)
	Q_PROPERTY(int workTime MEMBER mWorkTime NOTIFY workTimeChanged)
	Q_PROPERTY(int restTime MEMBER mRestTime NOTIFY restTimeChanged)

	Q_PROPERTY(bool running READ getIsRunning WRITE setIsRunning NOTIFY isRunningChanged)
	Q_PROPERTY(bool resting READ getIsResting WRITE setIsResting NOTIFY isRestingChanged)
	Q_PROPERTY(bool done READ getIsDone WRITE setIsDone NOTIFY isDoneChanged)

public:
	explicit StrengthWorkModel(QObject *parent = nullptr);
	StrengthWorkModel(const StrengthWorkModel * model, QObject *parent = nullptr);

	virtual StrengthWorkModel * clone()const;
	static StrengthWorkModel *load(QSqlQuery &query);	// Create a model from the current query.

	void setTargetWork(Description::StrengthWorkModel * data);

	int  getRepetitions() const;
	void setRepetitions(int reps);

	int  getWeight() const;
	void setWeight(int weight);

	int  getWorkTime() const;
	void setWorkTime(int workTime);

	int  getRestTime() const;
	void setRestTime(int restTime);

	bool getIsRunning() const;
	void setIsRunning(bool data);

	bool getIsResting() const;
	void setIsResting(bool data);

	bool getIsDone() const;
	void setIsDone(bool data);

	qint64 getId() const;
	void setId(qint64 id);

	void startWork();
	virtual void endOfCurrentWork();
	virtual void endOfCurrentRest();

	void save(qint64 parentId);
	void saveProgram(qint64 programId, qint64 parentId);

signals:
	void targetWorkChanged();
	void repetitionsChanged();
	void weightChanged();
	void workTimeChanged();
	void restTimeChanged();
	void idChanged();
	void isRunningChanged();
	void isRestingChanged();
	void isDoneChanged();
	void finished();

protected:
	Description::StrengthWorkModel *mTargetWork;
	qint64 mDbId = 0;
	int mRepetitions = 1;
	int mWeight = 0;
	int mWorkTime = 0;
	int mRestTime = 60;

	bool mIsRunning = false;
	bool mIsResting = false;
	bool mIsDone = false;
	QDateTime mStart;
};
}
#endif
