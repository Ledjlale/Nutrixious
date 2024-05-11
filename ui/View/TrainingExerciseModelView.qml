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
import App.Training 1.0 as Training

import '../Tool/Utils.js' as Utils

Item{
	id: mainItem
	property var exerciseModel
	property bool showAddButton: false
	property bool showRunning: false
	property bool expandAll: false
	property bool running : exerciseModel.running
	property bool resting: exerciseModel.resting
	property bool done: exerciseModel.done
	property bool autoRun: false
	property var lastExercise
	onRestingChanged: if(resting) restingPopup.pause(exerciseModel, exerciseModel.targetExercise.restTime)

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
		visible: mainItem.running || mainItem.done
		color: exerciseModel.resting
					? Material.color(Material.Green, Material.Shade100)
					: exerciseModel.done
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
				text: exerciseModel.name
			}
			Text{
				text: ' | '
			}
			Text{
				text: exerciseModel.description
			}
			Text{
				visible: exerciseModel.type == 1 || exerciseModel.type == 2
				text: ' | '
			}
			Text{
				visible: exerciseModel.type == 1 || exerciseModel.type == 2
				text: visible ? exerciseModel.type == 1
								? 'D:'+exerciseModel.targetExercise.distance + (exerciseModel.done ? ' / ' + exerciseModel.distance : '')
								: 'S:'+exerciseModel.targetExercise.steps+ (exerciseModel.done ? ' / ' + exerciseModel.steps : '')
							: ''
			}
			Text{
				visible: exerciseModel.type == 1 || exerciseModel.type == 2
				text: ' | '
			}
			Text{
				visible: exerciseModel.type == 1 || exerciseModel.type == 2
				text: visible ? 'Rest Time:' +(exerciseModel.done ? exerciseModel.restTime : exerciseModel.targetExercise.restTime) +'s': ''
			}
			Text{
				visible: exerciseModel.type == 1 || exerciseModel.type == 2
				text: visible ? 'Work Time:' + exerciseModel.workTime +'s' : ''
			}
			Item{
				Layout.fillWidth: true
			}
			Button{
				Layout.rightMargin: 15
				visible: exerciseModel.type == 3	// Display only lists like sets (aka Strength at the moment)
				text: setList.visible ? '-' : '+'
				onClicked: setList.visible = !setList.visible
			}
			Button{
				Layout.rightMargin: 15
				visible: mainItem.showAddButton
				text: 'Add'
				onClicked: mainItem.addClicked(exerciseModel)
			}
		}

// Details
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
			model: visible ? exerciseModel.sets : []
			delegate:Rectangle{
				property bool resting: modelData.resting

				onRestingChanged: if(resting) restingPopup.pause(modelData, modelData.targetWork.restTime)
				width: setList.width
				height: 40
				Rectangle{
					anchors.fill: parent
					visible: modelData.running || modelData.done
					color: modelData.resting
								? Material.color(Material.Green, Material.Shade100)
								: modelData.done
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
						text: modelData.targetWork.repetitions + (modelData.done ? ' / ' + modelData.repetitions : '')
					}
					Text{
						Layout.fillWidth: true
						Layout.fillHeight: true
						Layout.preferredWidth: setList.width / parent.children.length
						text: modelData.targetWork.weight + (modelData.done ? ' / ' + modelData.weight : '')
					}
					Text{
						Layout.fillWidth: true
						Layout.fillHeight: true
						Layout.preferredWidth: setList.width / parent.children.length
						color: modelData.restTime <= modelData.targetWork.restTime ? Material.foreground : Material.accent
						text: modelData.targetWork.restTime + (modelData.done ? ' / ' + modelData.restTime : '')
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
				text: 'Resting for max: '+restingPopup.restTime +' s'
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
						distanceModel:mainItem.lastExercise
						trainingResultEdition: true
						showAddButton: false
						showSaveButton: true
						isReadOnly: false
					}
				}
				Component{
					id: stepsComponent
					StepsModelView{
						stepsModel: mainItem.lastExercise
						trainingResultEdition: true
						showAddButton: false
						showSaveButton: true
						isReadOnly: false
					}
				}
				Component {
					id: strengthWorkComponent
					ExerciseSetModelView{
						workModel: mainItem.lastExercise
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
		onAccepted: target.resting = false
		onRejected: target.resting = false
		Timer{
			id: restTimer
			property int count: 0
			interval: 1000
			running: !!restingPopup.target && restingPopup.target.resting
			onRunningChanged: count = 0
			repeat: true
			onTriggered: ++count
		}
		Timer{
			id: autoRunTimer
			interval: restingPopup.restTime* 1000
			running: mainItem.autoRun && !!restingPopup.target && restingPopup.target.resting
			onTriggered: {
				restingPopup.target.resting = false
				restingPopup.close()
			}
		}
	}
}
