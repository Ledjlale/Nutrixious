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

#ifndef DATABASE_EXERCISE_UNIT_LIST_MODEL_H
#define DATABASE_EXERCISE_UNIT_LIST_MODEL_H

#include "src/tool/proxyModel/ProxyAbstractListModel.hpp"
#include "ExerciseUnitModel.h"

class ExerciseUnitListModel: public ProxyAbstractListModel<ExerciseUnitModel*> {
Q_OBJECT
public:
	ExerciseUnitListModel(QObject * parent = nullptr);
	ExerciseUnitListModel(QVariantList exercises, QObject * parent);

	virtual QHash<int, QByteArray> roleNames () const override;
	QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;

	QVariantList getExercises() const;

	void handleRemoved(ExerciseUnitModel *model);

};

#endif
