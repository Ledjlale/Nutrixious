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
import QtQuick.Controls.Material as Control
import QtQuick.Layouts

Rectangle{
	id: mainItem
	property alias title: titleText.text
	property var text: ''
	property var newValue
	property bool isEdited: !!newValue && newValue != text
	property alias inputMethodHints: textField.inputMethodHints
	property alias placeholderText: textField.placeholderText
	property alias readOnly: textField.readOnly
	property color textColor: Control.Material.foreground
	//property alias focus: textField.focus

	signal editingFinished()

	function forceActiveFocus(){
		textField.forceActiveFocus()
	}

	radius: 15
	width: mainLayout.implicitWidth
	height: mainLayout.implicitHeight

	color: Control.Material.background
	onEditingFinished: {
		newValue = textField.text
	}
	ColumnLayout{
		id: mainLayout
		anchors.fill: parent
		spacing: 0
		Text{
			Layout.leftMargin: 10
			id: titleText
			color: mainItem.textColor
			visible: text != ''
		}
		Control.TextField{
			id: textField
			Layout.fillHeight: true
			Layout.fillWidth: true
			Layout.margins: 10
			color: mainItem.textColor
			text: mainItem.text
			onEditingFinished: mainItem.editingFinished()
		}
	}
}
