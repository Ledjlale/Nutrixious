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

#ifndef BASE_EXERCISE_PROXY_MODEL_H
#define BASE_EXERCISE_PROXY_MODEL_H

#include "src/tool/proxyModel/SortFilterProxyModel.hpp"

namespace Base{
class ExerciseProxyModel: public SortFilterProxyModel {
Q_OBJECT
	Q_PROPERTY(QVariantList exercises READ getExercises WRITE setExercises NOTIFY exercisesChanged)
public:
	ExerciseProxyModel(QObject * parent = nullptr);

	Q_INVOKABLE void update();

	Q_INVOKABLE QVariantList getExercises() const;
	Q_INVOKABLE void setExercises(QVariantList exercises);

signals:
	void exercisesChanged();

};
}

#endif
