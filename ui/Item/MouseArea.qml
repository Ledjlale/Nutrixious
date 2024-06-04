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

import QtQuick as Quick
import QtQuick.Controls.Material as Control
import QtQuick.Effects
import QtQuick.Layouts

Control.Button {
	id: mainItem

	spacing: 0
	hoverEnabled: true

	leftInset: 0
	rightInset: 0
	topInset: 0
	bottomInset: 0

	leftPadding:0
	rightPadding:0

	//onClicked: console.log("click")
	//onDoubleClicked: console.log("doubleClick")
	//onPressAndHold: console.log("holdPress")
	Quick.MouseArea {
		anchors.fill: parent
		hoverEnabled: parent.hoverEnabled
		cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
		acceptedButtons: Qt.NoButton
	}

	background: Quick.Item {
		anchors.fill: parent
			Quick.MouseArea {
				anchors.fill: parent
				hoverEnabled: mainItem.hoverEnabled
				cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
			}
	}
}
