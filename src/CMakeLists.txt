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



#	src/activity/base/exercise/ExerciseModel.cpp
#	src/activity/base/exercise/ExerciseListModel.cpp
#	src/activity/base/exercise/ExerciseProxyModel.cpp

#	src/activity/base/serie/SerieModel.cpp
#	src/activity/base/serie/SerieListModel.cpp
#	src/activity/base/serie/SerieProxyModel.cpp

	src/computation/Computation.cpp

	src/database/program/ProgramModel.cpp
	src/database/program/ProgramListModel.cpp
	src/database/program/ProgramProxyModel.cpp
	src/database/exercise/ExerciseModel.cpp
	src/database/exercise/ExerciseListModel.cpp
	src/database/exercise/ExerciseProxyModel.cpp
	src/database/exerciseUnit/ExerciseUnitModel.cpp
	src/database/exerciseUnit/ExerciseUnitListModel.cpp
	src/database/exerciseUnit/ExerciseUnitProxyModel.cpp
	src/database/exerciseSerie/ExerciseSerieModel.cpp
	src/database/exerciseSerie/ExerciseSerieListModel.cpp
	src/database/exerciseSerie/ExerciseSerieProxyModel.cpp

	src/database/program/serie/ProgramSerieModel.cpp
	src/database/program/exercise/ProgramExerciseModel.cpp

	src/database/training/TrainingModel.cpp
	src/database/training/TrainingListModel.cpp
	src/database/training/TrainingProxyModel.cpp
	src/database/training/serie/TrainingSerieModel.cpp
	src/database/training/exercise/TrainingExerciseModel.cpp

	src/activity/working/WorkingModel.cpp
	src/activity/working/WorkingExerciseModel.cpp
	src/activity/working/WorkingSerieModel.cpp

	#src/database/food/FoodModel.cpp
	#src/database/food/FoodListModel.cpp
	#src/database/food/FoodProxyModel.cpp

	src/database/DatabaseQuery.cpp
	src/database/DatabaseModel.cpp

	src/database/personal/PersonalDataModel.cpp
	src/database/personal/PersonalDataListModel.cpp
	src/database/personal/PersonalDataProxyModel.cpp

	src/database/food/FoodModel.cpp
	src/database/food/FoodListModel.cpp
	src/database/food/FoodProxyModel.cpp

	src/database/mealFood/MealFoodModel.cpp
	src/database/mealFood/MealFoodListModel.cpp
	src/database/mealFood/MealFoodProxyModel.cpp

	src/database/mealGroup/MealGroupModel.cpp
	src/database/mealGroup/MealGroupListModel.cpp
	src/database/mealGroup/MealGroupProxyModel.cpp

	src/database/unit/UnitModel.cpp
	src/database/unit/UnitListModel.cpp
	src/database/unit/UnitProxyModel.cpp

	src/stat/StatsModel.cpp

	src/tool/logger/Logger.cpp
	src/tool/proxyModel/SortFilterProxyModel.cpp
	src/tool/proxyModel/ProxyAbstractObject.hpp
	src/tool/proxyModel/ObjectListModel.cpp
	src/tool/proxyModel/ObjectProxyModel.cpp
	src/tool/provider/ImageProvider.cpp
	src/tool/date/DateModel.cpp
	src/tool/QmlData.cpp
	src/tool/QmlModel.cpp

)

set(_APP_SOURCE_FILES ${_APP_SOURCE_FILES} PARENT_SCOPE)
