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
	property bool isReadOnly: false

	//implicitHeight: headersLayout.implicitHeight + setList.contentHeight
	implicitHeight: setList.contentHeight
/*
	ColumnLayout{
		id: mainLayout
		anchors.fill: parent
		spacing: 0
// Details

		RowLayout{
			id: headersLayout
			visible: setList.visible && setList.count > 0
			spacing: 0
			Text{
				Layout.fillWidth: true
				Layout.fillHeight: true
				Layout.preferredWidth: mainLayout.width / parent.visibleChildren.length
				text: 'Reps'
			}
			Text{
				Layout.fillWidth: true
				Layout.fillHeight: true
				Layout.preferredWidth: mainLayout.width / parent.visibleChildren.length
				text: 'Weight (kg)'
			}
			Text{
				Layout.fillWidth: true
				Layout.fillHeight: true
				Layout.preferredWidth: mainLayout.width / parent.visibleChildren.length
				text: 'Rest Time (s)'
			}
			Text{
				Layout.fillWidth: true
				Layout.fillHeight: true
				Layout.preferredWidth: mainLayout.width / parent.visibleChildren.length
				visible: mainItem.isLive || (mainItem.exerciseModel && mainItem.exerciseModel.isSaved )//&& mainItem.exerciseModel.isTraining)
				text: 'Work Time (s)'
			}
			Item{// Actions
				Layout.fillWidth: true
				Layout.fillHeight: true
				Layout.preferredWidth: mainLayout.width / parent.visibleChildren.length
			}
		}
		*/
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
				exerciseModel: mainItem.workingExerciseModel ? mainItem.workingExerciseModel : mainItem.exerciseModel
				serieModel: modelData
				//onRestingChanged: if(mainItem.isLive && resting) restingPopup.pause(modelData, modelData.targetWork.restTime)
				width: setList.width
				isReadOnly: mainItem.isReadOnly
				showTitle: index == 0
			}
		}
	//}
}
