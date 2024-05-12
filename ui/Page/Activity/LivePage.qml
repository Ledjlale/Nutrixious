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
import QtQuick.Controls
import QtQuick.Layouts

import App 1.0
import App.Training 1.0 as Training
import '../../Tool/Utils.js' as Utils

Item {
	id: mainItem
	property bool isRunning
	property var lastExercise
	property ProgramModel targetProgramModel
	property TrainingModel trainingModel: TrainingModel{
		id: trainingModel
		trainModel.targetProgramModel: mainItem.targetProgramModel
		onExercisesChanged: programExercises.setExercises(trainModel.exercises)
		onWorkingNextExercise: function(index){
			exercisesList.positionViewAtIndex(index,ListView.Beginning)
		}
		onFinished: {
			finishPopup.open()
			mainItem.isRunning = false
		}
	}
	onIsRunningChanged: if(isRunning)
						trainingModel.start()
					else
						trainingModel.stop()
	ColumnLayout{
		anchors.fill: parent
		RowLayout{
			id: options
			CheckBox{
				id: autoRunCheckBox
				text: 'Auto unpause'
			}
			Item{
				Layout.fillWidth: true
			}
			Button{
				visible: !trainingModel.trainModel.isSaved
				text: mainItem.isRunning ? 'Stop' : 'Run'
				onClicked: mainItem.isRunning = !mainItem.isRunning
			}
		}
		ListView{
			id: exercisesList
			property bool isRunning
			Layout.fillWidth: true
			Layout.fillHeight: true
			clip: true
			model: Training.ExerciseProxyModel{
				id: programExercises
			}
			spacing: 0
			delegate: TrainingExerciseModelView{
				width: exercisesList.width
				exerciseModel: modelData
				showRunning: true
				expandAll: true
				autoRun: autoRunCheckBox.checked
				lastExercise: mainItem.lastExercise
			}
		}
		RowLayout{
			Item{Layout.fillWidth: true}
			Button{
				Layout.bottomMargin: 15
				visible: mainItem.isRunning
				text: 'End - Good'
				onClicked: {
					mainItem.lastExercise = trainingModel.getCurrentWork()
					trainingModel.endOfCurrentWork()
				}
			}
			Button{
				Layout.bottomMargin: 15
				visible: mainItem.isRunning
				text: 'End - Fail'
				onClicked: {
					mainItem.lastExercise = trainingModel.getCurrentWork()
					trainingModel.endOfCurrentWork()
				}
			}
			Button{
				Layout.bottomMargin: 15
				visible: mainItem.isRunning
				text: 'End - All'
				onClicked: {
					trainingModel.save()
				}
			}
			Item{Layout.fillWidth: true}
		}
	}
	Dialog{
		id: finishPopup
		text: 'Program is over'
		onAccepted: trainingModel.save()
		onRejected: trainingModel.save()
	}
}
