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

#ifndef DATABASE_FOOD_PROXY_MODEL_H
#define DATABASE_FOOD_PROXY_MODEL_H

#include "src/tool/proxyModel/SortFilterProxyModel.hpp"
#include "src/tool/QmlModel.h"

class FoodProxyModel: public SortFilterProxyModel {
Q_OBJECT
	DECLARE_GETSET(QString,name,Name)
public:
	FoodProxyModel(QObject * parent = nullptr);
	
	virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
	virtual bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;

signals:
	void update();

protected:
	QString mName;
};

#endif
