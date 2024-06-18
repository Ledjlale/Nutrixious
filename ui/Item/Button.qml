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
import QtQuick.Effects
import QtQuick.Layouts

Control.Button {
	id: mainItem
	property int capitalization
	property color color: Control.Material.color(Control.Material.Grey)
	property color disabledColor: Control.Material.primary
	property color borderColor: "transparent"
	property color pressedColor: Control.Material.primary
	property bool inversedColors: false
	property int textSize: 11
	property int textWeight: 400
	property int radius: 48
	property color textColor: Control.Material.foreground
	property bool underline: false
	property bool shadowEnabled: false
	property var contentImageColor
	spacing: 0
	hoverEnabled: true

	leftInset: 0
	rightInset: 0
	topInset: 0
	bottomInset: 0

	leftPadding:2
	rightPadding:2
	implicitWidth: Math.max(textMetrics.width + 4, 40)

	width: implicitWidth
	MouseArea {
		anchors.fill: parent
		hoverEnabled: true
		cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
		acceptedButtons: Qt.NoButton
	}
	TextMetrics {
		id: textMetrics
		elide: Qt.ElideNone
		text: mainItem.text
	}


	background: Item {
		Rectangle {
			anchors.fill: parent
			id: buttonBackground
			color: Control.Material.buttonColor(Control.Material.theme, Control.Material.background,
						Control.Material.accent, Control.enabled, Control.flat, Control.highlighted, Control.checked)
			/*
			color: mainItem.enabled
				? inversedColors
					? mainItem.pressed
						? Control.Material.background
						: 'white'
					: mainItem.pressed
						? mainItem.pressedColor
						: mainItem.color
				: mainItem.disabledColor
				*/
			radius: mainItem.radius
			border.color: inversedColors ? mainItem.color : mainItem.borderColor

			MouseArea {
				anchors.fill: parent
				hoverEnabled: true
				cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
			}
		}
		MultiEffect {
			enabled: mainItem.shadowEnabled
			anchors.fill: buttonBackground
			source: buttonBackground
			shadowEnabled: mainItem.shadowEnabled
			shadowColor: Control.Material.background
			shadowBlur: 1
			shadowOpacity: 0.1
		}
	}

	component ButtonText: Text {
		horizontalAlignment: Text.AlignHCenter
		verticalAlignment: Text.AlignVCenter
		width: mainItem.text != undefined ? mainItem.width : 0
		//height: mainItem.height
		wrapMode: Text.WrapAnywhere
		// Layout.fillWidth: true
		// Layout.fillHeight: true
		text: mainItem.text

		maximumLineCount: 1
		color: inversedColors ? mainItem.color : mainItem.textColor
		font {
			pixelSize: mainItem.textSize
			weight: mainItem.textWeight
			//family: DefaultStyle.defaultFont
			capitalization: mainItem.capitalization
			underline: mainItem.underline
		}
	}
/*
	component ButtonImage: EffectImage {
		// Layout.fillWidth: true
		// Layout.fillHeight: true
		imageSource: mainItem.icon.source
		imageWidth: mainItem.icon.width
		imageHeight: mainItem.icon.height
		colorizationColor: mainItem.contentImageColor
	}*/

	contentItem: /*StackLayout {
		id: stacklayout
		currentIndex: mainItem.text.length != 0 && mainItem.icon.source.toString().length != 0
			? 0
			: mainItem.text.length != 0
				? 1
				: mainItem.icon.source.toString().length != 0
					? 2
					: 3

		//width: mainItem.width
		RowLayout {
			spacing: mainItem.spacing
			ButtonImage{
				Layout.preferredWidth: mainItem.icon.width
				Layout.preferredHeight: mainItem.icon.height
			}
			ButtonText{}
		}*/
		ButtonText {
		}/*
		ButtonImage{}
		Item {
			Layout.fillWidth: true
			Layout.fillHeight: true
		}
	}*/
}
