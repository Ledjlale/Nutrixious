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

Control.ToolBar{
	id: mainItem
	property bool showBackButton : true
	property bool showMenuButton: true
	property bool showOptionsButton: true
	property bool showSaveButton: false
	property alias title: titleLabel.text
	property bool displayStopwatch: false
	signal back()
	signal menu()
	signal options()
	signal save()
	RowLayout {
		 anchors.fill: parent
		 Control.ToolButton {
			visible: mainItem.showMenuButton
			text: qsTr("=")
			onClicked: mainItem.menu()
		 }
		 Control.ToolButton {
			visible: mainItem.showBackButton
			text: qsTr("‹")
			onClicked: mainItem.back()
		 }
		 Control.Label {
			id: titleLabel
			visible: !!text
			elide: Control.Label.ElideRight
			horizontalAlignment: Qt.AlignLeft
			verticalAlignment: Qt.AlignVCenter
			Layout.fillWidth: true
		}
		Control.ToolButton {
			text: qsTr("T")
			onClicked: mainItem.displayStopwatch = !mainItem.displayStopwatch
		}

		Control.ToolButton {
			visible: mainItem.showOptionsButton
			text: qsTr("⋮")
			onClicked: mainItem.options()
		}
		Control.ToolButton {
			visible: mainItem.showSaveButton
			text: qsTr("S")
			onClicked: mainItem.save()
		}
	}
}
