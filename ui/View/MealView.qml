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
	property var mealFoodModel
	property string title

	implicitHeight: descLine.implicitHeight +  mainLine.implicitHeight + addButton.implicitHeight
	//implicitHeight: mainLayout.implicitHeight // don't work
	ColumnLayout{
		id: mainLayout
		anchors.fill: parent
		spacing: 0
		implicitHeight: descLine.implicitHeight +  mainLine.implicitHeight + addButton.implicitHeight
		onImplicitHeightChanged: console.log( descLine.implicitHeight +'/'+  mainLine.implicitHeight +'/'+ addButton.implicitHeight)
		Rectangle{
			id: mainLineBackground
			Layout.fillWidth: true
			Layout.fillHeight: true
			color: Material.background
			RowLayout{
				id: mainLine
				anchors.fill: parent
				Text{
					Layout.fillWidth: true
					horizontalAlignment: Text.AlignLeft
					text: mainItem.title
				}
				Text{
					horizontalAlignment: Text.AlignRight
					text: '95 kcal'
				}
			}
		}
		Rectangle{
			Layout.fillWidth: true
			Layout.fillHeight: true
			RowLayout{
				id: descLine
				anchors.fill: parent
				ColumnLayout{
					Text{
						text:'Description'
					}
					Text{
						text: mealFoodModel.brand
					}
				}
				Text{
					horizontalAlignment: Text.AlignRight
					text: '0 kcal'
				}
			}
		}
		Button{
			id: addButton
			text: 'Add'
		}
	}
}
