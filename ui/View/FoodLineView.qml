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
import QtQuick.Controls.Material
import QtQuick.Layouts

import App 1.0

Item{
	id: mainItem
	property var foodModel

	implicitHeight: mainLayout.implicitHeight

	Rectangle{
		id: mainLineBackground
		anchors.fill: parent
		color: Material.background

	}
	RowLayout{
		id: mainLayout
		anchors.fill: parent
		ColumnLayout{
			Text{
				Layout.fillWidth: true
				color: Material.foreground
				elide: Text.ElideRight
				text: mainItem.foodModel.description
			}
			Text{
				Layout.fillWidth: true
				color: Material.foreground
				elide: Text.ElideRight
				text: mainItem.foodModel.brand
			}
		}
		Text{
			color: Material.foreground
			horizontalAlignment: Text.AlignRight
			text: Number.parseFloat(mainItem.foodModel.calories.toFixed(4))
		}
	}
}
