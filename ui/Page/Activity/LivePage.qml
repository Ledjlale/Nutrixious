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
import '../../Tool/Utils.js' as Utils

Item {
	id: mainItem
	property bool isRunning
	property var lastExercise
	property var targetProgramModel
	onTargetProgramModelChanged: workingModel.loadFromProgram(targetProgramModel)
	property var workingModel: WorkingModel{
		//onExercisesChanged: programExercises.setExercises(trainModel.exercises)
		onWorkingNextExercise: function(index){
			exercisesList.positionViewAtIndex(index,ListView.Beginning)
		}
		property bool isResting: currentWork?.isResting || false
		onIsRestingChanged: if(isResting) restingPopup.pause(currentWork)

		onFinished: {
			finishPopup.open()
			mainItem.isRunning = false
		}
	}
	onIsRunningChanged: if(isRunning) {
						workingModel.start()
						gShowMenuButton = false
					}else {
						workingModel.stop()
						gShowMenuButton = true
					}
	ColumnLayout{
		anchors.fill: parent
		RowLayout{
			id: options
			CheckBox{
				id: autoRunCheckBox
				text: 'Auto unpause'
			}

			ComboBox{
				id: exerciseChoice
				Layout.fillWidth: true
				textRole: "displayText"
				valueRole: "$modelData"
				model: ExerciseProxyModel{
					id: exercises
				}
				Component.onCompleted: exercises.update()
			}
			Button{
				visible: !workingModel.targetModel.isSaved
				text: 'Add Exo'
				onClicked: {
						workingModel.targetModel.addExercise(exerciseChoice.currentValue, false)
				}
			}

			Button{
				visible: !workingModel.targetModel.isSaved
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
			model: workingModel.exercises
			spacing: 0
			delegate: ExerciseModelView{
				width: exercisesList.width
				workingExerciseModel: modelData
				///workingModel: mainItem.workingModel
				showRunning: true
				expandAll: true
				//autoRun: autoRunCheckBox.checked
				//lastExercise: mainItem.lastExercise



			}
		}
		RowLayout{
			Item{Layout.fillWidth: true}
			Button{
				Layout.bottomMargin: 15
				visible: mainItem.isRunning
				text: 'End - Good'
				onClicked: {
					mainItem.lastExercise = workingModel.getCurrentWork()
					workingModel.endOfCurrentWork()
				}
			}
			Button{
				Layout.bottomMargin: 15
				visible: mainItem.isRunning
				text: 'End - Fail'
				onClicked: {
					mainItem.lastExercise = workingModel.getCurrentWork()
					workingModel.endOfCurrentWork()
				}
			}
			Rectangle{
				id: timerArea
				Layout.preferredHeight: 30
				Layout.preferredWidth: 140
				Layout.bottomMargin: 15
				color: Material.primary
				radius: 15
				property var currentWork: workingModel.currentWork
				visible: !!currentWork && !currentWork.isResting && !currentWork.isDone
				Text{
					anchors.centerIn: parent
					text: timer.count + ' s'
					color: 'white'
					Timer{
						id: timer
						property int count:0
						interval: 1000
						repeat: true
						running: timerArea.visible
						onRunningChanged: count = 0
						onTriggered: {++count}
					}
				}
			}

			//Button{
			//	Layout.bottomMargin: 15
			//	visible: mainItem.isRunning
			//	text: 'End - All'
			//	onClicked: {
			//		trainingModel.save()
			//	}
			//}
			Item{Layout.fillWidth: true}
		}
	}

	Dialog{
		id: restingPopup
		contentWidth: 400
		contentHeight: 200
		property int restTime
		property var target


		function pause(target){
			restingPopup.target = target
			console.log(target)
			restingPopup.restTime = target.targetExerciseModel ? target.targetExerciseModel.restTime : target.targetSerieModel.restTime
			open()
		}

		ColumnLayout{
			id: view
				anchors.fill: parent
			Text{
				Layout.fillWidth: true
				text: 'isResting for max: '+restingPopup.restTime +' s'
			}
			Text{
				Layout.fillWidth: true
				property int diff: restingPopup.restTime - restTimer.count
				text: diff + " s left"
				color: diff >= 0 ? Material.foreground : Material.accent
			}
			Loader{
				Layout.fillWidth: true
				Layout.fillHeight: true

				Component {
					id: serieComponent
					ExerciseSerieModelView{
						serieModel: restingPopup.target.resultSerieModel
						trainingResultEdition: true
						showSaveButton: false
						isReadOnly: false
						isLive: false
						Component.onCompleted: console.log("Serie:"+exerciseModel)
					}
				}
				Component {
					id: exerciseComponent
					ExerciseModelView{
						exerciseModel: restingPopup.target.resultExerciseModel
						showSaveButton: false
						isReadOnly: false
						Component.onCompleted: console.log("Ex:"+exerciseModel)
					}
				}
				sourceComponent: !restingPopup.target ? null
									: restingPopup.target.resultSerieModel
										? serieComponent
										: exerciseComponent
				active: restingPopup.visible && !!restingPopup.target
			}
		}
		onAccepted: target.isResting = false
		onRejected: target.isResting = false
		Timer{
			id: restTimer
			property int count: 0
			interval: 1000
			running: !!restingPopup.target && restingPopup.target.isResting
			onRunningChanged: count = 0
			repeat: true
			onTriggered: ++count
		}
		Timer{
			id: autoRunTimer
			interval: restingPopup.restTime* 1000
			running: mainItem.autoRun && !!restingPopup.target && restingPopup.target.isResting || false
			onTriggered: {
				restingPopup.target.isResting = false
				restingPopup.close()
			}
		}
	}
	Dialog{
		id: finishPopup
		text: 'Program is over'
		onAccepted: workingModel.save()
		onRejected: workingModel.save()
	}
}
