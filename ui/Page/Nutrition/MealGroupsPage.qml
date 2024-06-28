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
import QtCore
import QtMultimedia

import QtQml.XmlListModel

import App 1.0


Item {
	id: mainItem
	onVisibleChanged: if(visible) mealGroups.update()
	ColumnLayout{
		anchors.fill: parent
		spacing: 0
		ListView{
			id: mealGroupList
			Layout.fillWidth: true
			Layout.fillHeight: true
			clip: true
			model: MealGroupProxyModel{
				id: mealGroups
				showAll: true
				Component.onCompleted: update()
			}
			delegate: MealGroupView{
				width: mealGroupList.width
				mealGroup: $modelData
			}
			footer:	Item{
				width: mealGroupList.width
				height: 40
				ButtonImage{
					id: addButton
					anchors.centerIn: parent
					width: 80
					height: 30
					//Layout.preferredWidth: 80
					//Layout.preferredHeight: 30
					//Layout.alignment: Qt.AlignCenter
					//Layout.topMargin: 5
					imageSource: DefaultStyle.addFoodButton
					colorizationColor: Material.foreground
					onClicked: {
						mealGroups.addNewMealGroup()
						mealGroupList.positionViewAtEnd()
					}
				}
			}
		}
	}
}
