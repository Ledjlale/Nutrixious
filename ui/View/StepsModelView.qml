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
	property var stepsModel
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
		if(nameTextField.isEdited) stepsModel.name = nameTextField.newValue
		if(descriptionTextField.isEdited) stepsModel.description = descriptionTextField.newValue
		if(stepsTextField.isEdited) stepsModel.steps = stepsTextField.newValue
		if(restTextField.isEdited) stepsModel.restTime = restTextField.newValue
		if(workTextField.isEdited) stepsModel.workTime = workTextField.newValue
	}

	RowLayout{
		id: mainLine
		anchors.fill: parent
		spacing: 0
		TextField{
			id: nameTextField
			Layout.fillWidth: true
			visible: !mainItem.trainingResultEdition
			readOnly: isReadOnly
			text: visible ? stepsModel.name : ''
		}
		TextField{
			id: descriptionTextField
			Layout.fillWidth: true
			visible: !mainItem.trainingResultEdition
			readOnly: isReadOnly
			text: visible ? stepsModel.description : ''
		}
		TextField{
			id: stepsTextField
			Layout.fillWidth: true
			readOnly: isReadOnly
			inputMethodHints: Qt.ImhDigitsOnly
			title: mainItem.trainingResultEdition ? 'Steps' : ''
			text: visible ? stepsModel.steps : ''
		}
		TextField{
			id: restTextField
			Layout.fillWidth: true
			visible: !mainItem.trainingResultEdition
			readOnly: isReadOnly
			inputMethodHints: Qt.ImhDigitsOnly
			text: visible ? stepsModel.restTime : ''
		}
		TextField{
			id: workTextField
			Layout.fillWidth: true
			visible: !mainItem.trainingResultEdition && stepsModel && stepsModel.isSaved && stepsModel.workTime !== undefined
			inputMethodHints: Qt.ImhDigitsOnly
			text: visible ? stepsModel.workTime : ''
			readOnly: isReadOnly
		}
		Button{
			visible: !mainItem.isReadOnly && mainItem.showSaveButton && (nameTextField.isEdited || descriptionTextField.isEdited || stepsTextField.isEdited || restTextField.isEdited || workTextField.isEdited)
			text: 'Save'
			onClicked: {
				mainItem.saveValues()
				if(!mainItem.trainingResultEdition) stepsModel.save()
			}
		}
		Button{
			Layout.rightMargin: 15
			visible: mainItem.showAddButton
			text: 'Add'
			onClicked: {
				mainItem.saveValues()
				mainItem.addClicked(stepsModel)
			}
		}
	}
}
