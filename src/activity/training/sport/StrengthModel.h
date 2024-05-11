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

#ifndef TRAINING_STRENGTH_MODEL_H
#define TRAINING_STRENGTH_MODEL_H

#include <QObject>
#include <QVector>
#include <QSqlQuery>

#include "src/activity/training/exercise/ExerciseModel.h"

namespace Training{
class StrengthWorkModel;
class StrengthModel : public ExerciseModel {
	Q_OBJECT
	Q_PROPERTY(QVariantList sets READ getVariantSets NOTIFY setsChanged)
	Q_PROPERTY(bool invalidSets MEMBER mInvalidSets NOTIFY invalidSetsChanged)
public:
	StrengthModel();
	explicit StrengthModel(QObject *parent );
	StrengthModel(const StrengthModel * model, QObject *parent);
	virtual ~StrengthModel();

	virtual ExerciseModel * clone(qint64 programId, QObject *parent)const;
	virtual void setTargetExercise(Description::ExerciseModel * data);

	Q_INVOKABLE virtual bool save();
	static QList<ExerciseModel*> load(QObject * parent);
	static StrengthModel *load(QSqlQuery &query, QObject * parent);	// Create a model from the current query.

	QVariantList getVariantSets() const;
	QList<StrengthWorkModel*> getSets() const;
	Q_INVOKABLE void addSet(StrengthWorkModel *model, bool keepId);
	Q_INVOKABLE void removeSet(StrengthWorkModel *model);

	void nextSet();
	virtual void startWork();
	virtual void endOfCurrentWork();
	virtual void endOfCurrentRest();
	virtual QVariant getCurrentWork();

	void setInvalidSets(bool invalid);

signals:
	void setsChanged();

	void invalidSetsChanged();

protected:
	QList<StrengthWorkModel*> mSets;
	QList<StrengthWorkModel*>::Iterator mCurrentSet;
	bool mInvalidSets = true;
};
}
#endif
