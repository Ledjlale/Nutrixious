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

SwipeView{
	id: mainItem
	property var mealFoodModel
	signal clicked()
	implicitHeight: descLine.implicitHeight
	//implicitHeight: mainLayout.implicitHeight // don't work

	ColumnLayout{
		id: mainLayout
		height: mainItem.height
		width: mainItem.width
		spacing: 0
		implicitHeight: descLine.implicitHeight
		Rectangle{
			Layout.fillWidth: true
			Layout.fillHeight: true
			color: Material.background
			RowLayout{
				id: descLine
				anchors.fill: parent
				ColumnLayout{
					Text{
						Layout.fillWidth: true
						color: Material.foreground
						text: mealFoodModel.description
					}
					Text{
						Layout.fillWidth: true
						color: Material.foreground
						text: mealFoodModel.brand
					}
				}
				Text{
					horizontalAlignment: Text.AlignRight
					color: Material.foreground
					text: Number.parseFloat(mealFoodModel.computeNutriment(mealFoodModel.calories,mealFoodModel.servingSize,mealFoodModel.servingUnitId,mealFoodModel.baseSize,mealFoodModel.baseUnitId).toFixed(2))
				}
			}
			MouseArea{
				anchors.fill: parent
				onClicked: {
					mainItem.clicked()
				}
			}
		}
	}
	Rectangle{
		width: mainItem.width
		height: mainItem.height
		color: Material.accent
		RowLayout{
		anchors.fill: parent
			Item{
				Layout.fillWidth: true
			}
			Button{
				text: 'D'
				onClicked: {
					mealFoodModel.remove()
				}
			}
		}
	}
}
