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
	property var strengthModel
	property bool showAddButton: false
	property bool showSaveButton: true
	property bool showRunning: false
	property bool expandAll: false
	property bool running : false
	property bool isReadOnly: false

	implicitHeight: mainLayout.implicitHeight


	signal addClicked(ExerciseModel exerciseModel)

	function saveValues(){
		if(nameTextField.isEdited) strengthModel.name = nameTextField.newValue
		if(descriptionTextField.isEdited) strengthModel.description = descriptionTextField.newValue
	}
	ColumnLayout{
		id: mainLayout
		anchors.fill: parent
		RowLayout{
			spacing: 0
			TextField{
				id: nameTextField
				Layout.fillWidth: true
				readOnly: isReadOnly
				text: strengthModel.name
			}
			TextField{
				id: descriptionTextField
				Layout.fillWidth: true
				readOnly: isReadOnly
				text: strengthModel.description
			}
			Button{
				Layout.rightMargin: 15
				text: setList.visible ? '-' : '+'
				onClicked: setList.visible = !setList.visible
			}
			Button{
				visible: !mainItem.isReadOnly && mainItem.showSaveButton && (nameTextField.isEdited || descriptionTextField.isEdited)
				text: 'Save'
				onClicked: {
					mainItem.saveValues()
					strengthModel.save()
				}
			}
			Button{
				Layout.rightMargin: 15
				visible: mainItem.showAddButton
				text: 'Add'
				onClicked: {
					mainItem.saveValues()
					mainItem.addClicked(strengthModel)
				}
			}
		}
		ExerciseSetModelListView{
			id: setList
			Layout.fillWidth: true
			Layout.preferredHeight: implicitHeight
			visible: mainItem.expandAll
			exerciseModel: visible ? mainItem.strengthModel : null
			showSaveButton: mainItem.showSaveButton
			isReadOnly: mainItem.isReadOnly
		}
	}
}
