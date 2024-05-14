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

Flipable {
	id: mainItem
	property var title: ''
	property var text: ''
	property var newValue
	property bool showTitle: true
	property bool isEdited: !!newValue && newValue != text
	property var inputMethodHints: Qt.ImhNone
	property string placeholderText
	property bool readOnly: false
	property color textColor: Control.Material.foreground

	signal editingFinished()

	function forceActiveFocus(){
		if(mainItem.flipped) frontItem.forceActiveFocus()
		else backItem.forceActiveFocus()
	}

	width: flipped ? backItem.implicitWidth : frontItem.implicitWidth
	implicitHeight: flipped ? backItem.implicitHeight: frontItem.implicitHeight

	property bool flipped: false

	front: MouseArea{
				id: frontItem
				anchors.fill: parent
				implicitHeight: readOnlyLayout.implicitHeight
				propagateComposedEvents: true
				preventStealing: true
				onClicked: function(mouse){
					if(!mainItem.readOnly) {
						mainItem.flipped = true
						backItem.forceActiveFocus()
					}
					mouse.accepted= false
				}
				ColumnLayout{
					id: readOnlyLayout
					anchors.fill: parent
					spacing: 0
					Text{
						Layout.leftMargin: 10
						color: mainItem.textColor
						visible: mainItem.showTitle && text != ''
						text: mainItem.title
					}
					Text{
						id: originalTextField
						Layout.fillHeight: true
						Layout.fillWidth: true
						Layout.margins: 10
						color: mainItem.textColor
						text: newValue ? newValue : mainItem.text === undefined ? '' : mainItem.text
					}
				}
			}
	back: ColumnLayout{
				id: backItem
				anchors.fill: parent
				spacing: 0
				function forceActiveFocus(){
					textField.forceActiveFocus()
				}
				Text{
					Layout.leftMargin: 10
					color: mainItem.textColor
					visible: text != ''
					font.italic: true
					font.pixelSize: textField.font.pixelSize - 2
					text: mainItem.title

				}
				Control.TextField{
					id: textField
					Layout.fillHeight: true
					Layout.fillWidth: true
					Layout.leftMargin: 10
					Layout.rightMargin: 10
					Layout.bottomMargin: 10
					inputMethodHints: mainItem.inputMethodHints
					readOnly: mainItem.readOnly
					color: mainItem.textColor
					text: mainItem.text === undefined ? '' : mainItem.text
					placeholderText: mainItem.placeholderText
					onEditingFinished:  {
						mainItem.newValue = textField.text
						mainItem.editingFinished()
						mainItem.flipped = false
					}
					onActiveFocusChanged: {
						if(!activeFocus) mainItem.flipped = false
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
