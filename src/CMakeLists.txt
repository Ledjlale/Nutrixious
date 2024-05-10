#[[
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
 ]]

 list(APPEND _APP_SOURCE_FILES

	src/activity/description/sport/StrengthModel.cpp
	src/activity/description/sport/StrengthWorkModel.cpp
	src/activity/description/sport/DistanceModel.cpp
	src/activity/description/sport/StepsModel.cpp

	src/activity/description/exercise/ExerciseModel.cpp
	src/activity/description/exercise/ExerciseListModel.cpp
	src/activity/description/exercise/ExerciseProxyModel.cpp

	src/activity/description/program/ProgramModel.cpp
	src/activity/description/program/ProgramListModel.cpp
	src/activity/description/program/ProgramProxyModel.cpp

	src/activity/training/sport/StrengthModel.cpp
	src/activity/training/sport/StrengthWorkModel.cpp
	src/activity/training/sport/DistanceModel.cpp
	src/activity/training/sport/StepsModel.cpp

	src/activity/training/exercise/ExerciseModel.cpp
	src/activity/training/exercise/ExerciseListModel.cpp
	src/activity/training/exercise/ExerciseProxyModel.cpp

	src/activity/training/program/ProgramModel.cpp
	src/activity/training/program/ProgramListModel.cpp
	src/activity/training/program/ProgramProxyModel.cpp

	src/activity/training/TrainingModel.cpp



	src/database/DatabaseQuery.cpp
	src/database/DatabaseModel.cpp
	src/food/FoodModel.cpp

	src/tool/proxyModel/SortFilterProxyModel.cpp
	src/tool/proxyModel/ProxyAbstractObject.hpp

)

set(_APP_SOURCE_FILES ${_APP_SOURCE_FILES} PARENT_SCOPE)
