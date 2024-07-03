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
import QtQuick.Controls
import QtQuick.Layouts

import App 1.0

ApplicationWindow{
	width: 360
	height: 740
	visible: true
	ColumnLayout{
		anchors.fill: parent
		Rectangle{
			id: previewColor
			Layout.preferredHeight: 100
			Layout.preferredWidth: height
			color: Qt.rgba(red.value,green.value,blue.value,alpha.value)
			onColorChanged: DefaultStyle.mealGroupSectionColor = color
		}
		Text{
			property int r: previewColor.color.r*255
			property int g: previewColor.color.g*255
			property int b: previewColor.color.b*255
			property int a: previewColor.color.a*255
		/*
			text: parseInt((previewColor.color.r*255).toFixed(0),16) 
			+ (previewColor.color.g*255).toFixed(0)
			+(previewColor.color.b*255).toFixed(0).toString(16)
			+(previewColor.color.a*255).toFixed(0).toString(16) */
			text: (r).toString(16) + (g).toString(16) +(b).toString(16)+(a).toString(16)
		}
		RowLayout{
			Slider{
				id: red
				Layout.fillWidth: true
				from:0
				to:1
				value: 1
			}
		}
		RowLayout{
			Slider{
				id: green
				Layout.fillWidth: true
				from:0
				to:1
				value: 1
			}
		}
		RowLayout{
			Slider{
				id: blue
				Layout.fillWidth: true
				from:0
				to:1
				value: 1
			}
		}
		RowLayout{
			Slider{
				id: alpha
				Layout.fillWidth: true
				from:0
				to:1
				value: 1.0
			}
		}
	}
}
