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
	property var title: ''
	property alias text: originalTextField.text
	property var newValue
	property bool showTitle: true
	property bool edit: false
	property var inputMethodHints: Qt.ImhNone
	property string placeholderText
	property bool readOnly: false
	property color textColor: Control.Material.foreground
	property int elide: Text.ElideRight
	property alias font: originalTextField.font
	property int horizontalAlignment: Text.AlignLeft

	signal editingFinished()

	implicitWidth: flipped ? backItem.implicitWidth : frontItem.implicitWidth
	implicitHeight: flipped ? backItem.implicitHeight: frontItem.implicitHeight

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
					Text{
						Layout.fillWidth: true
						color: mainItem.textColor
						visible: mainItem.showTitle && text != ''
						text: mainItem.title
					}
					Text{
						id: originalTextField
						Layout.fillHeight: true
						Layout.fillWidth: true
						Layout.margins: 10
						horizontalAlignment: mainItem.horizontalAlignment
						elide: mainItem.elide
						color: mainItem.textColor
						text: mainItem.text
						wrapMode: TextEdit.WordWrap
					}
				}
			}
	back: ColumnLayout{
				id: backItem
				anchors.fill: parent
				spacing: 5
				property var qtBug: title.implicitHeight + textField.implicitHeight + 5
				onQtBugChanged: implicitHeight = qtBug
				implicitHeight: qtBug
				onImplicitHeightChanged: if( implicitHeight != qtBug) implicitHeight = qtBug
				Text{
					id: title
					Layout.fillWidth: true
					color: mainItem.textColor
					visible: text != ''
					font.italic: true
					font.pixelSize: textField.font.pixelSize - 2
					text: mainItem.title
				}

				Control.TextField{
					id: textField
					Layout.preferredHeight: implicitHeight
					Layout.fillWidth: true
					//Layout.leftMargin: 10
					//Layout.rightMargin: 10
					Layout.bottomMargin: 100
					Layout.topMargin: 5

					horizontalAlignment: mainItem.horizontalAlignment
					wrapMode: TextEdit.WordWrap
					inputMethodHints: mainItem.inputMethodHints
					readOnly: mainItem.readOnly
					color: mainItem.textColor
					text: mainItem.text === undefined ? '' : mainItem.text
					placeholderText: mainItem.placeholderText

					onEditingFinished:  {
						mainItem.newValue = textField.text
						mainItem.editingFinished()
					}
					Keys.onReturnPressed: function (event){
						console.log('Return:'+event.modifiers)
						if (event.modifiers == Qt.NoModifier || event.modifiers & Qt.KeypadModifier) {
							editingFinished();
							event.accepted = true;
						}else if(event.modifiers & Qt.ControlModifier) {
							insert(cursorPosition, "\n")
						}else {
							event.accepted = false;
						}
					}
					Keys.onEnterPressed: function (event){
						console.log('Enter:'+event.modifiers)
						if (event.modifiers == Qt.NoModifier || event.modifiers & Qt.KeypadModifier) {
							editingFinished();
							event.accepted = true;
						}else if(event.modifiers & Qt.ControlModifier ) {
							insert(cursorPosition, "\n")
						}else {
							event.accepted = false;
						}
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
