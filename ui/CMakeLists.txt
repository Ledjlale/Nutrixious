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

 list(APPEND _APP_QML_FILES
	ui/Main.qml
	ui/Page/NutritionPage.qml
	ui/Page/OverviewPage.qml
	ui/Page/DiaryPage.qml
	ui/Page/SettingsPage.qml
	ui/Page/FoodPage.qml
	ui/Page/ActivitiesPage.qml
	ui/Page/StatisticsPage.qml
	ui/Page/AboutPage.qml

	ui/Item/Button.qml
	ui/Item/ButtonImage.qml
	ui/Item/DatePicker.qml
	ui/Item/DayNavigationBar.qml
	ui/Item/Dialog.qml
#	ui/Item/MouseArea.qml
	ui/Item/ScannerOverlay.qml
	ui/Item/SexBox.qml
	ui/Item/TextInput.qml
	ui/Item/TextField.qml
	ui/Item/SerieFieldPicker.qml
	ui/Item/Stopwatch.qml
	ui/Item/TimeDisplay.qml
	ui/Item/ToolBar.qml
	ui/Item/ToolButtonImage.qml

	ui/Item/Chart/TimeChart.qml

	ui/Item/TimePicker/TimeComponentLabel.qml
	ui/Item/TimePicker/TimePicker.qml
	ui/Item/TimePicker/TimePickerDialog.qml
	ui/Item/TimePicker/TimePickerLabel.qml

	ui/Layout/SwipeLayout.qml
	ui/Layout/DiaryLayout.qml
	ui/Layout/ExercisesLayout.qml

	ui/View/ExerciseModelView.qml
	ui/View/FoodModelView.qml
	ui/View/FoodLineView.qml

	ui/View/ExerciseSerieModelView.qml
	ui/View/ExerciseSerieModelListView.qml
	ui/View/TrainModelView.qml
	ui/View/FoodSearchView.qml
	ui/View/ScannerView.qml
	ui/View/MealView.qml
	ui/View/MealGroupView.qml

	ui/Page/Activity/ExerciseEditorPage.qml
	ui/Page/Activity/ExerciseUnitEditorPage.qml
	ui/Page/Activity/ProgramsPage.qml
	ui/Page/Activity/ProgramEditorPage.qml

	ui/Page/Activity/TrainingPage.qml
	ui/Page/Activity/LivePage.qml
	
	ui/Page/History/TrainingsPage.qml
	ui/Page/History/PersonalDataPage.qml
	ui/Page/History/ExerciseStatsPage.qml
	ui/Page/History/NutritionStatsPage.qml

	ui/Page/Nutrition/NutritionPage.qml
	ui/Page/Nutrition/FoodEditorPage.qml
	ui/Page/Nutrition/FoodsPage.qml
	ui/Page/Nutrition/MealGroupsPage.qml


	ui/Tool/ColorPicker.qml
	ui/Tool/DragDropTool.qml
	ui/Tool/Utils.js
)


list(APPEND _APP_QML_SINGLETONS
	ui/Style/DefaultStyle.qml
)

set(_APP_QML_FILES ${_APP_QML_FILES} PARENT_SCOPE)
set(_APP_QML_SINGLETONS ${_APP_QML_SINGLETONS} PARENT_SCOPE)
