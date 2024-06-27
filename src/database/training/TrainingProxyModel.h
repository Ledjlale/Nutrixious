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

#ifndef TRAINING_PROXY_MODEL_H
#define TRAINING_PROXY_MODEL_H

#include "src/tool/proxyModel/SortFilterProxyModel.hpp"
#include "src/tool/QmlModel.h"
#include "src/tool/date/DateModel.h"
#include <QDate>

class TrainingProxyModel: public SortFilterProxyModel {
Q_OBJECT
	DECLARE_GETSET(DateModel*,trainingDay,TrainingDay)
	DECLARE_GETSET(double,calories,Calories)

public:
	TrainingProxyModel(QObject * parent = nullptr);

	Q_INVOKABLE void update();
	Q_INVOKABLE void updateCalories();

	bool lessThan (const QModelIndex &left, const QModelIndex &right) const override;
	virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

	DateModel* mTrainingDay = nullptr;
	double mCalories = 0.0;
};

#endif
