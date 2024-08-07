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

SwipeLayout{
	id: mainItem
	property var modelData
	property bool displayDate: false
	signal saved()
	
	onDeleteClicked: modelData.remove()
	isUndoable:  mainItem.modelData.isEdited
	isEditable: !mainItem.modelData.isEdited && !mainItem.modelData.isMain
	isDeletable: !mainItem.modelData.isMain
	onUndoClicked: {
		mainItem.modelData.undo()
	}
	
	contentItem: RowLayout{
		id: mainLine
		width: mainItem.width
		//height: mainItem.height
		//anchors.fill: parent
		spacing: 0
		TextField{
			id: nameTextField
			Layout.fillWidth: true
			//property int bestWidth: Math.min(mainLine.width / mainLine.visibleChildren.length, descriptionTextField.flipped ? 40 : implicitWidth)
			//Layout.preferredWidth: bestWidth
			//Layout.minimumWidth : descriptionTextField.flipped ? 0 : bestWidth
			font.bold: true
			font.pixelSize: 16
			text: mainItem.modelData ? mainItem.modelData.name : ''
			edit: text == '' || mainItem.edit
			placeholderText: 'Name'
			onEditingFinished: mainItem.modelData.name = newValue
			//onClicked: mainItem.clicked()
		}
		TextField{
			id: descriptionTextField
			Layout.fillWidth: true
			horizontalAlignment: Text.AlignRight
			text: mainItem.modelData ?  mainItem.modelData.description : ''
			edit: text == '' || mainItem.edit
			placeholderText: 'Description'
			onEditingFinished: {
				mainItem.modelData.description = newValue

				}

			//onClicked: mainItem.clicked()
		}
		TextField{
			id: startTimeTextField
			Layout.fillWidth: true
			Layout.leftMargin: 5
			visible: mainItem.modelData.startDateTimeStr !== undefined
			inputMethodHints: Qt.ImhDigitsOnly
			text: visible && mainItem.modelData ? mainItem.displayDate ? mainItem.modelData.startDateTimeStr : mainItem.modelData.startTimeStr : ''
			readOnly: !mainItem.displayDate
			onEditingFinished: mainItem.displayDate ? mainItem.modelData.startDateTimeStr = newValue : mainItem.modelData.startTimeStr = newValue
		}
		Text{
			Layout.rightMargin: 5
			visible: text != ''
			color: Material.foreground
			text: mainItem.modelData.calories ? Number.parseFloat(mainItem.modelData.calories.toFixed(2)) : ''
		}
		ButtonImage{
			Layout.alignment: Qt.AlignCenter
			Layout.preferredWidth: 25
			Layout.preferredHeight: 25
			Layout.rightMargin: 5
			visible: mainItem.modelData.isEdited
			imageSource: DefaultStyle.saveButton
			colorizationColor: Material.foreground
			onClicked: {
				forceActiveFocus()
				mainItem.modelData.save()
				mainItem.saved()
			}
		}

	}
}
