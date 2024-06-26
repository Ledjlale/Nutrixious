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

SwipeLayout{
	id: mainItem
	property var modelData
	
	onDeleteClicked: modelData.remove()
	
	contentItem: Rectangle{
			width: mainItem.width
			implicitHeight: descLine.implicitHeight + 5
			color: Material.background
			RowLayout{
				id: descLine
				anchors.fill: parent
				ColumnLayout{
					spacing: 0
					Text{
						Layout.fillWidth: true
						color: Material.foreground
						text: mainItem.modelData.description
					}
					Text{
						Layout.fillWidth: true
						color: Material.foreground
						text: mainItem.modelData.brand
					}
				}
				Text{
					horizontalAlignment: Text.AlignRight
					color: Material.foreground
					text: Number.parseFloat(mainItem.modelData.computeNutriment(mainItem.modelData.calories,mainItem.modelData.servingSize,mainItem.modelData.servingUnitId,mainItem.modelData.baseSize,mainItem.modelData.baseUnitId).toFixed(2))
				}
			}
	}
}
