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

Item{
	id: mainItem
	property var exerciseModel
	property var programModel
	property bool showAddButton: false
	property bool showSaveButton: true
	property bool showRunning: false
	property bool expandAll: false
	property bool isRunning : false
	property bool isReadOnly: false
	property bool trainingResultEdition: false

	implicitHeight: mainLine.implicitHeight


	signal addClicked(var exerciseModel)

	function saveValues(){
		if(nameTextField.isEdited) exerciseModel.name = nameTextField.newValue
		if(descriptionTextField.isEdited) exerciseModel.description = descriptionTextField.newValue
		if(distanceTextField.isEdited) exerciseModel.distance = distanceTextField.newValue
		if(restTextField.isEdited) exerciseModel.restTime = restTextField.newValue
		if(workTextField.isEdited) exerciseModel.workTime = workTextField.newValue
	}

	RowLayout{
		id: mainLine
		anchors.fill: parent
		spacing: 0
		TextField{
			id: nameTextField
			Layout.fillWidth: true
			Layout.preferredWidth: mainLayout.width / parent.visibleChanged.length
			visible: !mainItem.trainingResultEdition
			showTitle: false
			text: exerciseModel ? exerciseModel.name : ''
			readOnly: isReadOnly
		}
		TextField{
			id: descriptionTextField
			Layout.fillWidth: true
			Layout.preferredWidth: mainLayout.width / parent.visibleChanged.length
			visible: !mainItem.trainingResultEdition
			showTitle: false
			text:exerciseModel ?  exerciseModel.description : ''
			readOnly: isReadOnly
		}
		TextField{
			id: distanceTextField
			Layout.fillWidth: true
			Layout.preferredWidth: mainLayout.width / parent.visibleChanged.length
			inputMethodHints: Qt.ImhDigitsOnly
			showTitle: mainItem.trainingResultEdition
			title: 'Distance (m)'
			text: exerciseModel ? exerciseModel.distance : ''
			readOnly: isReadOnly
		}
		TextField{
			id: restTextField
			Layout.fillWidth: true
			Layout.preferredWidth: mainLayout.width / parent.visibleChanged.length
			visible: !mainItem.trainingResultEdition
			inputMethodHints: Qt.ImhDigitsOnly
			showTitle: mainItem.trainingResultEdition
			title: 'Rest Time (s)'
			text: exerciseModel ? exerciseModel.restTime : ''
			readOnly: isReadOnly
		}
		TextField{
			id: workTextField
			Layout.fillWidth: true
			Layout.preferredWidth: mainLayout.width / parent.visibleChanged.length
			visible: !mainItem.trainingResultEdition && exerciseModel && exerciseModel.isSaved && exerciseModel.workTime !== undefined
			showTitle: false
			inputMethodHints: Qt.ImhDigitsOnly
			title: 'Work Time (s)'
			text: visible ? exerciseModel.workTime : ''
			readOnly: isReadOnly
		}
		Button{
			visible: !mainItem.isReadOnly && mainItem.showSaveButton && (nameTextField.isEdited || descriptionTextField.isEdited || distanceTextField.isEdited || restTextField.isEdited || workTextField.isEdited)
			text: 'Save'
			onClicked: {
				mainItem.saveValues()
				if(!mainItem.trainingResultEdition) exerciseModel.save()
			}
		}
		Button{
			Layout.rightMargin: 15
			visible: mainItem.showAddButton
			text: 'Add'
			onClicked: {
				mainItem.saveValues()
				mainItem.addClicked(exerciseModel)
			}
		}
		Rectangle{
			Layout.alignment: Qt.AlignCenter
			Layout.preferredHeight: 30
			Layout.preferredWidth: 30
			visible: !!mainItem.programModel
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
					mainItem.programModel.decrementExerciseOrder(mainItem.exerciseModel)
				}
			}
		}
		Rectangle{
			Layout.alignment: Qt.AlignCenter
			Layout.preferredHeight: 30
			Layout.preferredWidth: 30
			visible: !!mainItem.programModel
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
					mainItem.programModel.incrementExerciseOrder(mainItem.exerciseModel)
				}
			}
		}
	}
}
