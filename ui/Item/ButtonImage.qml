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
import QtQuick.Layouts
import QtQuick.Controls as Control
import QtQuick.Effects


Loader {
	id: mainItem
	active: visible
	property var imageSource
	property var fillMode: Image.PreserveAspectFit
	property color colorizationColor
	
	property int imageWidth: width
	property int imageHeight: height
	property bool useColor: colorizationColor != undefined
	signal clicked()
	asynchronous: true	// If not true, colorization may be not correctly updated in some cases.
	sourceComponent: Item {
		Image {
			id: image
			visible: !effect2.effectEnabled
			source: mainItem.imageSource ? mainItem.imageSource : ""
			fillMode: mainItem.fillMode
			sourceSize.width: width
			sourceSize.height: height
			width: mainItem.imageWidth
			height: mainItem.imageHeight
			anchors.centerIn: parent
		}
		MultiEffect {
			id: effect
			visible: false
			anchors.fill: image
			source: image
			maskSource: image
			brightness: effect2.effectEnabled ? 1.0 : 0.0
		}

		MultiEffect {
			id: effect2
			visible: mainItem.useColor
			property bool effectEnabled: mainItem.useColor
			anchors.fill: effect
			source: effect
			maskSource: effect
			colorizationColor: effectEnabled && mainItem.colorizationColor ?  mainItem.colorizationColor : 'black'
			colorization: effectEnabled ? 1.0: 0.0
		}
		MouseArea{
			anchors.fill: parent
			onClicked: mainItem.clicked()
		}
	}
}
