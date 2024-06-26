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
	property bool showPreviousButton : true
	property bool showNextButton: true
	signal previous()
	signal next()

	property var currentDay: DateModel{}

	onNext:{ currentDay.nextDay()
	}
	onPrevious:{ currentDay.previousDay()
	}
	RowLayout {
		 anchors.fill: parent
		 Control.ToolButton {
			visible: mainItem.showPreviousButton
			text: qsTr("<")
			onClicked: mainItem.previous()
		 }
		 Control.Label {
			id: titleLabel
			horizontalAlignment: Qt.AlignHCenter
			verticalAlignment: Qt.AlignVCenter
			Layout.fillWidth: true
			text: currentDay.date
		}
		 Control.ToolButton {
			visible: mainItem.showNextButton
			text: qsTr(">")
			onClicked: mainItem.next()
		 }
	}
}
