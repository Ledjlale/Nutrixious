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
import '../Tool/Utils.js' as Utils

Item{
	id: mainItem
	property var workingExerciseModel
	property var workingModel
	property bool showAddButton: false
	property bool showRunning: false
	property bool expandAll: false
	property bool isRunning : workingExerciseModel.isRunning
	property bool isResting: workingExerciseModel.isResting
	property bool isDone: workingExerciseModel.isDone
	property bool autoRun: false
	property var lastExercise
	property var exerciseModel: workingExerciseModel.targetExerciseModel
	onIsRestingChanged: if(isResting) restingPopup.pause(workingExerciseModel, workingExerciseModel.targetExerciseModel.restTime)

	implicitHeight: mainLayout.implicitHeight
	height: implicitHeight

	signal addClicked(var exerciseModel)
	Timer{
		id: workTimer
		property int count: 0
		property int maxCount: 1
		interval: 1000
		repeat: true
		onTriggered:{
			if(++count == maxCount)
			mainLineBackground.width = mainLine.width * count / maxCount

		}
	}
	Rectangle{
		id: mainLineBackground
		anchors.fill: parent
		visible: mainItem.isRunning || mainItem.isDone
		color: workingExerciseModel.isResting
					? Material.color(Material.Yellow, Material.Shade100)
					: workingExerciseModel.isSaved
						? Material.color(Material.Green, Material.Shade100)
						: workingExerciseModel.isDone
							? Material.color(Material.Blue, Material.Shade100)
							: Material.color(Material.accent, Material.Shade100)
	}
	ColumnLayout{
		id: mainLayout
		anchors.fill: parent

// Main line
		RowLayout{
			id: mainLine
			Text{
				text: exerciseModel?.name || ''
			}
			Text{
				text: ' | '
			}
			Text{
				text: exerciseModel?.description  || ''
			}
			Text{
				visible: exerciseModel?.type == 1 || exerciseModel?.type == 2
				text: ' | '
			}
			Text{
				visible: exerciseModel?.type == 1 || exerciseModel?.type == 2
				text: visible ? exerciseModel?.type == 1
								? 'D:'+exerciseModel.targetExercise.distance + (workingExerciseModel.isDone ? ' / ' + exerciseModel.distance : '')
								: 'S:'+exerciseModel.targetExercise.steps+ (workingExerciseModel.isDone ? ' / ' + exerciseModel.steps : '')
							: ''
			}
			Text{
				visible: exerciseModel?.type == 1 || exerciseModel?.type == 2
				text: ' | '
			}
			Text{
				visible: exerciseModel?.type == 1 || exerciseModel?.type == 2
				text: visible ? 'Rest Time:' +(workingExerciseModel.isDone ? exerciseModel.restTime : exerciseModel.targetExercise.restTime) +'s': ''
			}
			Text{
				visible: exerciseModel?.type == 1 || exerciseModel?.type == 2
				text: visible ? 'Work Time:' + exerciseModel.workTime +'s' : ''
			}
			Item{
				Layout.fillWidth: true
			}/*
			Button{
				Layout.rightMargin: 15
				visible: exerciseModel?.type == 3	// Display only lists like sets (aka Strength at the moment)
				text: setList.visible ? '-' : '+'
				onClicked: setList.visible = !setList.visible
			}
			Button{
				Layout.rightMargin: 15
				visible: mainItem.showAddButton
				text: 'Add'
				onClicked: mainItem.addClicked(exerciseModel)
			}
			Rectangle{
				Layout.alignment: Qt.AlignCenter
				Layout.preferredHeight: 30
				Layout.preferredWidth: 30
				visible: !!mainItem.workingModel// && !mainItem.workingExerciseModel.isDone && !mainItem.exerciseModel.isRunning
				color: Material.primary

				radius: width / 2
				Text{
					anchors.centerIn: parent
					color: 'white'
					text: '+'
				}
				MouseArea{
					anchors.fill: parent
					onClicked:{
						mainItem.workingModel.targetModel.decrementExerciseOrder(mainItem.exerciseModel)
					}
				}
			}
			Rectangle{
				Layout.alignment: Qt.AlignCenter
				Layout.preferredHeight: 30
				Layout.preferredWidth: 30
				visible: !!mainItem.workingModel// && !mainItem.workingExerciseModel.isDone && !mainItem.exerciseModel.isRunning
				color: Material.primary
				radius: width / 2
				Text{
					anchors.centerIn: parent
					color: 'white'
					text: '-'
				}
				MouseArea{
					anchors.fill: parent
					onClicked:{
						mainItem.workingModel.targetModel.incrementExerciseOrder(mainItem.exerciseModel)
					}
				}
			}*/
		}

// Details
/*
		RowLayout{
			visible: setList.visible && setList.count > 0
			Text{
				Layout.fillWidth: true
				Layout.fillHeight: true
				Layout.preferredWidth: mainLayout.width / parent.children.length
				text: 'Index'
			}
			Text{
				Layout.fillWidth: true
				Layout.fillHeight: true
				Layout.preferredWidth: mainLayout.width / parent.children.length
				text: 'Reps'
			}
			Text{
				Layout.fillWidth: true
				Layout.fillHeight: true
				Layout.preferredWidth: mainLayout.width / parent.children.length
				text: 'Weight(kg)'
			}
			Text{
				Layout.fillWidth: true
				Layout.fillHeight: true
				Layout.preferredWidth: mainLayout.width / parent.children.length
				text: 'Rest Time(s)'
			}
			Text{
				Layout.fillWidth: true
				Layout.fillHeight: true
				Layout.preferredWidth: mainLayout.width / parent.children.length
				text: 'Work Time(s)'
			}
		}
		ListView{
			id: setList
			Layout.fillWidth: true
			Layout.preferredHeight: contentHeight
			visible: mainItem.expandAll
			interactive: false
			model: visible ? workingExerciseModel.series : []
			delegate:Rectangle{
				property bool isResting: modelData.isResting

				onIsRestingChanged: if(isResting) restingPopup.pause(modelData, modelData.targetWork.restTime)
				width: setList.width
				height: 40
				Rectangle{
					anchors.fill: parent
					visible: modelData.isRunning || modelData.isDone
					color: modelData.isResting
								? Material.color(Material.Yellow, Material.Shade100)
								: modelData.isSaved
									? Material.color(Material.Green, Material.Shade100)
									: modelData.isDone
										? Material.color(Material.Blue, Material.Shade100)
										: Material.color(Material.accent, Material.Shade100)
				}
				RowLayout{
					anchors.fill: parent
					Text{
						Layout.fillWidth: true
						Layout.fillHeight: true
						Layout.preferredWidth: setList.width / parent.children.length
						text: index
					}
					Text{
						Layout.fillWidth: true
						Layout.fillHeight: true
						Layout.preferredWidth: setList.width / parent.children.length
						text: modelData.targetWork.repetitions + (modelData.isDone ? ' / ' + modelData.repetitions : '')
					}
					Text{
						Layout.fillWidth: true
						Layout.fillHeight: true
						Layout.preferredWidth: setList.width / parent.children.length
						text: modelData.targetWork.weight + (modelData.isDone ? ' / ' + modelData.weight : '')
					}
					Text{
						Layout.fillWidth: true
						Layout.fillHeight: true
						Layout.preferredWidth: setList.width / parent.children.length
						color: modelData.restTime <= modelData.targetWork.restTime ? Material.foreground : Material.accent
						text: modelData.targetWork.restTime + (modelData.isDone ? ' / ' + modelData.restTime : '')
					}
					Text{
						Layout.fillWidth: true
						Layout.fillHeight: true
						Layout.preferredWidth: setList.width / parent.children.length
						text: modelData.workTime
					}
				}
			}
		}
	}

	Dialog{
		id: restingPopup
		contentWidth: 400
		contentHeight: 200
		property int restTime
		property var target


		function pause(target, restTime){
			restingPopup.target = target
			restingPopup.restTime = restTime
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
				Component{
					id: distanceComponent
					DistanceModelView{
						exerciseModel:mainItem.lastExercise
						trainingResultEdition: true
						showAddButton: false
						showSaveButton: true
						isReadOnly: false
					}
				}
				Component{
					id: stepsComponent
					StepsModelView{
						exerciseModel: mainItem.lastExercise
						trainingResultEdition: true
						showAddButton: false
						showSaveButton: true
						isReadOnly: false
					}
				}
				Component {
					id: strengthWorkComponent
					ExerciseSerieModelView{
						exerciseModel: mainItem.lastExercise
						trainingResultEdition: true
						showSaveButton: true
						isReadOnly: false
						isLive: false
					}
				}
				sourceComponent: !mainItem.lastExercise ? null
									: mainItem.lastExercise.type == 1 ? distanceComponent
									: mainItem.lastExercise.type == 2 ? stepsComponent
									: strengthWorkComponent
				active: !!mainItem.lastExercise
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
			running: mainItem.autoRun && !!restingPopup.target && restingPopup.target.isResting
			onTriggered: {
				restingPopup.target.isResting = false
				restingPopup.close()
			}
		}
		*/
	}
}
