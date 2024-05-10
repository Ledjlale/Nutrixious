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
	property bool isLive: exerciseModel && !!exerciseModel.targetExercise
	property bool showSaveButton: !isLive
	property bool isReadOnly: false

	implicitHeight: headersLayout.implicitHeight + setList.contentHeight

	ColumnLayout{
		id: mainLayout
		anchors.fill: parent
		spacing: 0
// Details
		RowLayout{
			id: headersLayout
			visible: setList.visible && setList.count > 0
			Text{
				Layout.fillWidth: true
				Layout.fillHeight: true
				Layout.preferredWidth: mainLayout.width / parent.visibleChildren.length
				text: 'Index'
			}
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
				text: 'Weight(kg)'
			}
			Text{
				Layout.fillWidth: true
				Layout.fillHeight: true
				Layout.preferredWidth: mainLayout.width / parent.visibleChildren.length
				text: 'Rest Time(s)'
			}
			Text{
				Layout.fillWidth: true
				Layout.fillHeight: true
				Layout.preferredWidth: mainLayout.width / parent.visibleChildren.length
				visible: mainItem.isLive
				text: 'Work Time(s)'
			}
		}
		ListView{
			id: setList
			Layout.fillWidth: true
			Layout.preferredHeight: contentHeight
			interactive: false
			clip: true
			model: visible && mainItem.exerciseModel? mainItem.exerciseModel.sets : []
			delegate:ExerciseSetModelView{
				workModel: modelData
				onRestingChanged: if(mainItem.isLive && resting) restingPopup.pause(modelData, modelData.targetWork.restTime)
				width: setList.width

				/*
				Rectangle{
					anchors.fill: parent
					visible: mainItem.isLive && ( modelData.running || modelData.done)
					color: modelData.resting
								? Material.color(Material.Green, Material.Shade100)
								: modelData.done
									? Material.color(Material.Blue, Material.Shade100)
									: Material.color(Material.accent, Material.Shade100)
				}
				RowLayout{
					id: contentLayout
					anchors.fill: parent
					Text{
						Layout.fillWidth: true
						Layout.fillHeight: true
						Layout.preferredWidth: setList.width / parent.visibleChildren.length
						text: index
					}
					TextField{
						id: repsTextField
						Layout.fillWidth: true
						Layout.fillHeight: true
						Layout.preferredWidth: setList.width / parent.visibleChildren.length
						readOnly: mainItem.isReadOnly
						text: mainItem.isLive
										? modelData.targetWork.repetitions + (modelData.done ? ' / ' + modelData.repetitions : '')
										: modelData.repetitions
						onEditingFinished: if(!mainItem.showSaveButton) modelData.repetitions = newValue
					}
					TextField{
						id: weightTextField
						Layout.fillWidth: true
						Layout.fillHeight: true
						Layout.preferredWidth: setList.width / parent.visibleChildren.length
						readOnly: mainItem.isReadOnly
						text: mainItem.isLive
										? ( modelData.targetWork.weight + (modelData.done ? ' / ' + modelData.weight : ''))
										: modelData.weight

						onEditingFinished: if(!mainItem.showSaveButton) modelData.weight = newValue
					}
					TextField{
						id: restTextField
						Layout.fillWidth: true
						Layout.fillHeight: true
						Layout.preferredWidth: setList.width / parent.visibleChildren.length
						readOnly: mainItem.isLive
						textColor: !mainItem.isLive || modelData.restTime <= modelData.targetWork.restTime ? Material.foreground : Material.accent
						text: mainItem.isReadOnly
										? modelData.targetWork.restTime + (modelData.done ? ' / ' + modelData.restTime : '')
										: modelData.restTime
						onEditingFinished: if(!mainItem.showSaveButton) modelData.restTime = newValue
					}
					Text{
						Layout.fillWidth: true
						Layout.fillHeight: true
						Layout.preferredWidth: setList.width / parent.visibleChildren.length
						visible: mainItem.isLive
						text: mainItem.isLive ? modelData.workTime : ''
					}
					Button{
						visible: !mainItem.isReadOnly && mainItem.showSaveButton && (repsTextField.isEdited || weightTextField.isEdited || restTextField.isEdited)
						text: 'Save'
						onClicked: {
							if(repsTextField.isEdited) modelData.repetitions = repsTextField.newValue
							if(weightTextField.isEdited) modelData.weight = weightTextField.newValue
							if(restTextField.isEdited) modelData.restTime = restTextField.newValue
							modelData.save()
						}
					}
				}
				*/
			}
		}
	}
}
