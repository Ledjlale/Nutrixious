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
		onIsRestingChanged: if(isResting) restingPopup.pause(currentExercise, currentWork)

		onFinished: {
			finishPopup.open()
			mainItem.isRunning = false
		}
	}
	onIsRunningChanged: if(isRunning) {
						if(workingModel.start())
							gShowMenuButton = false
						else
							isRunning = false
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
			ButtonImage{
				Layout.alignment: Qt.AlignCenter
				Layout.preferredWidth: 30
				Layout.preferredHeight: 30
				Layout.rightMargin: 5
				visible: !workingModel.targetModel.isSaved && exerciseChoice.currentIndex >= 0
				imageSource: DefaultStyle.addExerciseButton
				colorizationColor: Material.foreground
				onClicked: {
					forceActiveFocus()
					workingModel.addExercise(exerciseChoice.currentValue)
				}
			}
			ButtonImage{
				Layout.alignment: Qt.AlignCenter
				Layout.preferredWidth: 30
				Layout.preferredHeight: 30
				Layout.rightMargin: 5
				visible: !workingModel.targetModel.isSaved && exercisesList.count > 0
				imageSource: mainItem.isRunning ? DefaultStyle.stopButton : DefaultStyle.playButton
				onClicked: {
					forceActiveFocus()
					mainItem.isRunning = !mainItem.isRunning
				}
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
				showOrderChange: true
				//autoRun: autoRunCheckBox.checked
				//lastExercise: mainItem.lastExercise



			}
		}
		Flipable{
			id: restingPopup
			property int restTime
			property var target
			property var exercise
			property bool flipped: target?.isResting || false
			Layout.fillWidth: true
			Layout.preferredHeight: flipped ? backItem.implicitHeight + 10: frontItem.implicitHeight

			function pause(exercise, target){
			restingPopup.target = target
			restingPopup.exercise = exercise
			restingPopup.restTime = target.targetExerciseModel ? target.targetExerciseModel.restTime : target.targetSerieModel.restTime
			//open()
			//flippableItem.flipped = true
		}

			front: RowLayout{
				id: frontItem
				anchors.fill: parent
				Item{Layout.fillWidth: true}
				Button{
					Layout.preferredWidth: 80
					Layout.bottomMargin: 15
					visible: mainItem.isRunning
					text: 'End'
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
						id: elapsedTimeText
						anchors.centerIn: parent
						text: '0 s'
						color: 'white'
						Timer{
							id: timer
							interval: 1000
							repeat: true
							running: timerArea.visible
							onTriggered: {
								elapsedTimeText.text = timerArea.currentWork.getElapsedWorkTime() + ' s'
							}
						}
					}
				}
				Item{Layout.fillWidth: true}
			}
			back:ColumnLayout{
				id: backItem
				anchors.fill: parent
				spacing: 0
				Timer{
					id: restTimer
					property int count: 0
					property int diff: restingPopup.restTime - count
					interval: 1000
					running: !!restingPopup.target && restingPopup.target.isResting
					onRunningChanged: count = 0
					repeat: true
					onTriggered:{
						++count
						if( autoRunCheckBox.checked && diff <= 0 ) {
							restingPopup.target.isResting = false
							restingPopup.close()
						}
					}
				}
				RowLayout{
					spacing: 0
					Text{
						Layout.alignment: Qt.AlignCenter
						Layout.fillWidth: true
						horizontalAlignment: Text.AlignHCenter
						text: 'Resting : '+restTimer.diff + " s " +(restTimer.diff > 0 ? 'left' : '')
						color: restTimer.diff >= 0 ? Material.foreground : Material.accent
						font.weight: Font.Bold
					}
					
					ButtonImage{
						Layout.alignment: Qt.AlignCenter
						Layout.preferredWidth: 60
						Layout.preferredHeight: 60
						Layout.rightMargin: 5
						imageSource: DefaultStyle.resumeButton
						colorizationColor: Material.foreground
						onClicked: {
							forceActiveFocus()
							restingPopup.target.isResting = false
						}
					}
				}
				Loader{
					Layout.fillWidth: true
					Layout.fillHeight: true
					Layout.leftMargin: 5
					Layout.rightMargin: 5
					Layout.bottomMargin: 5

					Component {
						id: serieComponent
						ExerciseSerieModelView{
							modelData: restingPopup.target.resultSerieModel
							exerciseUnitModel: restingPopup.exercise
							trainingResultEdition: true
							showCalories: true
							showSaveButton: false
							edit: true
							isReadOnly: false
							isLive: false
							isDeletable: false
							Component.onCompleted: restingPopup.target.resultSerieModel.computeCalories()
						}
					}
					Component {
						id: exerciseComponent
						ExerciseModelView{
							exerciseModel: restingPopup.target.resultExerciseModel
							showSaveButton: false
							isReadOnly: false
						}
					}
					sourceComponent: !restingPopup.target ? null
										: restingPopup.target.resultSerieModel
											? serieComponent
											: exerciseComponent
					active: restingPopup.flipped && !!restingPopup.target
				}
			}

			transform: Rotation {
				id: rotation
				origin.x: restingPopup.width/2
				origin.y: restingPopup.height/2
				axis.x: 0; axis.y: 1; axis.z: 0     // set axis.y to 1 to rotate around y-axis
				angle: 0    // the default angle
			}

			states: State {
				name: "back"
				PropertyChanges { target: rotation; angle: 180 }
				when: restingPopup.flipped
			}

			transitions: Transition {
				NumberAnimation { target: rotation; property: "angle"; duration: 100 }
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
