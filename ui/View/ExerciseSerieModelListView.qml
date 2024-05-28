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

import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

import App 1.0
//import App.Training 1.0 as Training

import '../Tool/Utils.js' as Utils

Item{
	id: mainItem
	property var workingExerciseModel
	property var exerciseModel: workingExerciseModel?.targetExerciseModel
	property bool isLive: !!workingExerciseModel
	property bool showSaveButton: !isLive
	property bool showWorkTime: true
	property bool showCalories: true
	property bool showOrderChange: false
	property bool isReadOnly: false
	implicitHeight: setList.contentHeight
	ListView{
		id: setList
		anchors.fill: parent
		interactive: false
		clip: true
		model: visible
					? mainItem.workingExerciseModel
						? mainItem.workingExerciseModel.series
						: mainItem.exerciseModel
							? mainItem.exerciseModel.series
							: []
					: []
		delegate:ExerciseSerieModelView{
			exerciseUnitModel: mainItem.workingExerciseModel ? mainItem.workingExerciseModel : mainItem.exerciseModel
			serieModel: modelData
			width: setList.width
			showWorkTime: mainItem.showWorkTime
			showCalories: mainItem.showCalories
			showOrderChange: mainItem.showOrderChange
			isReadOnly: mainItem.isReadOnly
			isDeletable: setList.count > 1
			showTitle: index == 0
		}
	}
}
