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

#ifndef PROGRAM_SERIE_MODEL_H
#define PROGRAM_SERIE_MODEL_H

#include <QObject>
#include <QVector>

#include "src/activity/base/serie/SerieModel.h"

namespace Program{
class SerieModel : public Base::SerieModel{
Q_OBJECT
public:
	SerieModel();
	SerieModel(QObject *parent);
	SerieModel(int type, QObject *parent);
	SerieModel(const SerieModel * model, QObject *parent);
	virtual SerieModel * clone( QObject *parent)const;

};
}

#endif
