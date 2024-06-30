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

import App 1.0

Flipable {
	id: mainItem
	property var title: 'Sex'
	property var value
	property var newValue
	property bool showTitle: true
	property bool edit: false
	property var inputMethodHints: Qt.ImhNone
	property color textColor: Control.Material.foreground
	property int elide: Text.ElideRight
	property alias font: originalTextField.font
	property int horizontalAlignment: Text.AlignLeft

	signal accepted()

	implicitWidth: flipped ? backItem.implicitWidth : frontItem.implicitWidth
	implicitHeight: flipped ? backItem.implicitHeight : frontItem.implicitHeight

	property bool flipped: edit
	property bool showEdit : flipped
	Binding on showEdit{
			when: mainItem.edit
			value: true
	}
	
	front: Item{
				id: frontItem
				anchors.fill: parent
				implicitHeight: readOnlyLayout.implicitHeight
				implicitWidth: readOnlyLayout.implicitWidth
				ColumnLayout{
					id: readOnlyLayout
					anchors.fill: parent
					spacing: 0
					//implicitHeight: originalTextField.contentHeight
					Text{
						Layout.fillWidth: true
						color: mainItem.textColor
						visible: mainItem.showTitle && text != ''
						text: mainItem.title
					}
					Text{
						id: originalTextField
						Layout.fillWidth: true
						//Layout.margins: 5
						horizontalAlignment: mainItem.horizontalAlignment
						elide: mainItem.elide
						color: mainItem.textColor
						text: mainItem.value == 0 ? 'Male' : 'Female'
					}
				}
			}
	back: ColumnLayout{
				id: backItem
				anchors.fill: parent
				Text{
					id: title
					Layout.fillWidth: true
					color: mainItem.textColor
					visible: text != ''
					font.italic: true
					font.pixelSize: textField.font.pixelSize - 2
					text: mainItem.title
				}

				Control.ComboBox{
					id: textField
					Layout.preferredHeight: implicitHeight
					Layout.fillWidth: true
					Layout.topMargin: 5
					Layout.bottomMargin: 10

					currentIndex: mainItem.value
					model:['Male','Female']
					
					onActivated:  {
						mainItem.newValue = currentIndex
						mainItem.accepted()
					}
				}
			}

	transform: Rotation {
		id: rotation
		origin.x: mainItem.width/2
		origin.y: mainItem.height/2
		axis.x: 0; axis.y: 1; axis.z: 0     // set axis.y to 1 to rotate around y-axis
		angle: 0    // the default angle
	}

	states: State {
		name: "back"
		PropertyChanges { target: rotation; angle: 180 }
		when: mainItem.flipped
	}

	transitions: Transition {
		NumberAnimation { target: rotation; property: "angle"; duration: 100 }
	}

}
