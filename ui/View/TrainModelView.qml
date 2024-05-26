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
	property var trainModel

	implicitHeight: mainLine.implicitHeight

	function saveValues(){
		//if(nameTextField.isEdited) distanceModel.name = nameTextField.newValue
	}

	RowLayout{
		id: mainLine
		anchors.fill: parent
		spacing: 0
		TextField{
			id: nameTextField
			Layout.fillWidth: true
			text: trainModel ? trainModel.name : ''
			readOnly: true
		}
		TextField{
			id: descriptionTextField
			Layout.fillWidth: true
			text: trainModel ?  trainModel.description : ''
			readOnly: true
		}
		TextField{
			id: startTimeTextField
			Layout.fillWidth: true
			inputMethodHints: Qt.ImhDigitsOnly
			text: trainModel ? trainModel.startDateTimeStr: ''
			readOnly: false
			onEditingFinished: trainModel.startDateTimeStr = newValue
		}
		Button{
			visible: trainModel.isEdited
			text: 'Save'
			onClicked: {
				console.log("save")
				trainModel.save()
			}
		}
		Button{
			text: 'D'
			onClicked: {
				trainModel.remove()
			}
		}
	}
}
