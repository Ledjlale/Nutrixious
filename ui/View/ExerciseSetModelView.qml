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
Rectangle{
	id: mainItem
	property var workModel
	property var exerciseModel
	property bool isLive: false
	property bool resting: !!workModel.isResting
	property bool isReadOnly: isLive
	property bool showSaveButton: true
	property bool showTitle: true
	property bool trainingResultEdition: false
	property bool doSave: true

	implicitHeight: contentLayout.implicitHeight
	Rectangle{
		anchors.fill: parent
		visible: mainItem.isLive && ( workModel.isRunning || workModel.isDone)
		color: workModel.isResting
					? Material.color(Material.Green, Material.Shade100)
					: workModel.isDone
						? Material.color(Material.Blue, Material.Shade100)
						: Material.color(Material.accent, Material.Shade100)
	}
	RowLayout{
		id: contentLayout
		anchors.fill: parent
		property int _visibleChildren : visibleChildren.length - 1
		spacing: 0
		TextField{
			id: repsTextField
			Layout.fillWidth: true
			Layout.fillHeight: true
			Layout.preferredWidth: mainItem.width / parent._visibleChildren
			showTitle: mainItem.trainingResultEdition
			readOnly: mainItem.isReadOnly
			inputMethodHints: Qt.ImhDigitsOnly
			title: mainItem.showTitle ? 'Reps' : ''
			textColor: !mainItem.isLive || workModel.repetitions <= workModel.targetWork.repetitions ? Material.foreground : Material.accent
			text: mainItem.isLive
							? workModel.targetWork.repetitions + (workModel.isDone ? ' / ' + workModel.repetitions : '')
							: workModel.repetitions
			onEditingFinished: if(!mainItem.showSaveButton) workModel.repetitions = newValue
		}
		TextField{
			id: weightTextField
			Layout.fillWidth: true
			Layout.fillHeight: true
			Layout.preferredWidth: mainItem.width / parent._visibleChildren
			showTitle: mainItem.trainingResultEdition
			readOnly: mainItem.isReadOnly
			inputMethodHints: Qt.ImhDigitsOnly
			title: mainItem.showTitle ? 'Weight(kg)' : ''
			textColor: !mainItem.isLive || workModel.weight <= workModel.targetWork.weight ? Material.foreground : Material.accent
			text: mainItem.isLive
							? ( workModel.targetWork.weight + (workModel.isDone ? ' / ' + workModel.weight : ''))
							: workModel.weight

			onEditingFinished: if(!mainItem.showSaveButton) workModel.weight = newValue
		}
		TextField{
			id: restTextField
			Layout.fillWidth: true
			Layout.fillHeight: true
			Layout.preferredWidth: mainItem.width / parent._visibleChildren
			visible: !mainItem.trainingResultEdition
			showTitle: mainItem.trainingResultEdition
			readOnly:  mainItem.isReadOnly || mainItem.isLive
			inputMethodHints: Qt.ImhDigitsOnly
			title: mainItem.showTitle ? 'RestTime(s)' : ''
			textColor: !mainItem.isLive || workModel.restTime <= workModel.targetWork.restTime ? Material.foreground : Material.accent
			text: mainItem.isLive
							? workModel.targetWork.restTime + (workModel.isDone ? ' / ' + workModel.restTime : '')
							: workModel.restTime
			onEditingFinished: if(!mainItem.showSaveButton) workModel.restTime = newValue
		}
		Text{
			Layout.fillWidth: true
			Layout.fillHeight: true
			Layout.preferredWidth: mainItem.width / parent._visibleChildren
			visible: (mainItem.isLive || workModel.isSaved) && workModel.isTraining
			text: visible ? workModel.workTime : ''
		}
		Button{
			visible: !mainItem.isReadOnly && mainItem.showSaveButton && (repsTextField.isEdited || weightTextField.isEdited || restTextField.isEdited)
			text: 'Save'
			onClicked: {
				if(repsTextField.isEdited) workModel.repetitions = repsTextField.newValue
				if(weightTextField.isEdited) workModel.weight = weightTextField.newValue
				if(restTextField.isEdited) workModel.restTime = restTextField.newValue
				if(!mainItem.trainingResultEdition && mainItem.doSave) {
					workModel.save()
				}
			}
		}

		Rectangle{
			Layout.alignment: Qt.AlignCenter
			Layout.preferredHeight: 30
			Layout.preferredWidth: 30
			color: Material.primary
			visible: !!mainItem.exerciseModel && !mainItem.isReadOnly

			radius: width / 2
			Text{
				anchors.centerIn: parent
				color: 'white'
				text: '+'
			}
			MouseArea{
				anchors.fill: parent
				onClicked:{
					exerciseModel.decrementWorkOrder(mainItem.workModel)
				}
			}
		}
		Rectangle{
			Layout.alignment: Qt.AlignCenter
			Layout.preferredHeight: 30
			Layout.preferredWidth: 30
			visible: !!mainItem.exerciseModel && !mainItem.isReadOnly
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
					exerciseModel.incrementWorkOrder(mainItem.workModel)
				}
			}
		}
	}
}

