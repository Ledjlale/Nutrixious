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

	ui/Item/Dialog.qml
	ui/Item/ScannerOverlay.qml
	ui/Item/TextField.qml

	ui/View/ExerciseModelView.qml

	ui/View/DistanceModelView.qml
	ui/View/StepsModelView.qml
	ui/View/StrengthModelView.qml
	ui/View/ExerciseSetModelView.qml
	ui/View/ExerciseSetModelListView.qml
	ui/View/TrainingExerciseModelView.qml
	ui/View/ScannerView.qml

	ui/Page/Activity/DistancePage.qml
	ui/Page/Activity/ExercisesPage.qml
	ui/Page/Activity/ProgramsPage.qml
	ui/Page/Activity/ProgramEditorPage.qml
	ui/Page/Activity/StepsPage.qml
	ui/Page/Activity/StrengthPage.qml

	ui/Page/Activity/TrainingPage.qml
	ui/Page/Activity/TrainingsPage.qml
	ui/Page/Activity/LivePage.qml



	ui/Tool/Utils.js
)

set(_APP_QML_FILES ${_APP_QML_FILES} PARENT_SCOPE)
