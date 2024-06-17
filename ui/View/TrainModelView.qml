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

SwipeView{
	id: mainItem
	property var trainModel
	property bool isDeletable: true
	property bool displayDate: true

	implicitHeight: mainLine.implicitHeight

	function saveValues(){
		//if(nameTextField.isEdited) distanceModel.name = nameTextField.newValue
	}
	signal clicked()
	currentIndex: 0
	interactive: mainItem.isDeletable
	RowLayout{
		id: mainLine
		width: mainItem.width
		height: mainItem.height
		//anchors.fill: parent
		spacing: 0
		TextField{
			id: nameTextField
			Layout.fillWidth: true
			property int bestWidth: Math.min(mainLine.width / mainLine.visibleChildren.length, descriptionTextField.flipped ? 40 : implicitWidth)
			Layout.preferredWidth: bestWidth
			Layout.minimumWidth : descriptionTextField.flipped ? 0 : bestWidth
			font.bold: true
			font.pixelSize: 16
			text: trainModel ? trainModel.name : ''
			keepEditView: text == ''
			placeholderText: 'Name'
			onEditingFinished: trainModel.name = newValue
			onClicked: mainItem.clicked()
		}
		TextField{
			id: descriptionTextField
			Layout.fillWidth: true
			text: trainModel ?  trainModel.description : ''
			keepEditView: text == ''
			placeholderText: 'Description'
			onEditingFinished: {
				trainModel.description = newValue

				}

			onClicked: mainItem.clicked()
		}
		TextField{
			id: startTimeTextField
			Layout.fillWidth: true
			visible: trainModel.startDateTimeStr !== undefined
			inputMethodHints: Qt.ImhDigitsOnly
			text: visible && trainModel ? mainItem.displayDate ? trainModel.startDateTimeStr : trainModel.startTimeStr : ''
			readOnly: !mainItem.displayDate
			onEditingFinished: mainItem.displayDate ? trainModel.startDateTimeStr = newValue : trainModel.startTimeStr = newValue
		}
		Text{
			visible: text != ''
			color: Material.foreground
			text: trainModel.calories ? Number.parseFloat(trainModel.calories.toFixed(2)) : ''
		}

		Button{
			visible: trainModel.isEdited
			text: 'Save'
			onClicked: {
				trainModel.save()
			}
		}

	}
	Rectangle{
		width: mainItem.width
		height: mainItem.height
		color: Material.accent
		RowLayout{
		anchors.fill: parent
			Item{
				Layout.fillWidth: true
			}
			Button{
				visible: mainItem.isDeletable
				text: 'D'
				onClicked: {
					trainModel.remove()
				}
			}
		}
	}
}
