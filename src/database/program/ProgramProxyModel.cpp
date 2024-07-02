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

#include "ProgramProxyModel.h"
#include "ProgramListModel.h"


ProgramProxyModel::ProgramProxyModel(QObject *parent)
	: SortFilterProxyModel{parent}
{
	auto model = new ProgramListModel(this);
	connect(this, &ProgramProxyModel::create, model, &ProgramListModel::create);
	connect(model, &ProgramListModel::mainProgramChanged, this, &ProgramProxyModel::mainProgramChanged);
	setSourceModel(model);
	sort(0);
}

QVariant ProgramProxyModel::getMainProgram()const{
	return dynamic_cast<ProgramListModel*>( sourceModel())->getMainProgram();
}

void ProgramProxyModel::update(){
	sourceModel()->deleteLater();
	auto model = new ProgramListModel(this);
	connect(this, &ProgramProxyModel::create, model, &ProgramListModel::create);
	connect(model, &ProgramListModel::mainProgramChanged, this, &ProgramProxyModel::mainProgramChanged);
	setSourceModel(model);
}

bool ProgramProxyModel::lessThan (const QModelIndex &left, const QModelIndex &right) const{
	auto l = sourceModel()->data(left);
	auto r = sourceModel()->data(right);
	auto ml = l.value<ProgramModel*>();
	auto mr = r.value<ProgramModel*>();
	return mr->getIsMain() || ml->getName() < mr->getName();
}
