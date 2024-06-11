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
	property string offCode
	property var foodModel: FoodModel{
			id: foodModel
		}
	signal saved()
	onOffCodeChanged: if(offCode) mainItem.foodModel.loadFromOpenFoodFacts(offCode)

	ColumnLayout{
		anchors.fill: parent
		spacing: 0

		ListView{
			id: fieldsList
			Layout.fillWidth: true
			Layout.fillHeight: true
			model:[{title:'Brand', data: 'brand', editUnits: false, inputMethodHints: Qt.ImhNone}
					, {title:'Description', data: 'description', editUnits: false, inputMethodHints: Qt.ImhNone}
					, {title:'Serving Size', data: 'servingSize', editUnits: 'servingUnitId', inputMethodHints: Qt.ImhNone}
					//, {title:'Serving per container', data: 'servingsPerContainer', editUnits: false}
					, {title:'Calories ( kcal )', data: 'calories', editUnits: false, inputMethodHints: Qt.ImhFormattedNumbersOnly}
					, {title:'Total Fat ( g )', data: 'totalFat', editUnits: false, inputMethodHints: Qt.ImhFormattedNumbersOnly}
					, {title:'Saturated Fat ( g )', data: 'saturatedFat', editUnits: false, inputMethodHints: Qt.ImhFormattedNumbersOnly}
					, {title:'Trans Fat ( g )', data: 'transFat', editUnits: false, inputMethodHints: Qt.ImhFormattedNumbersOnly}
					, {title:'Polyunsaturated Fat ( g )', data: 'polyUnsaturatedFat', editUnits: false, inputMethodHints: Qt.ImhFormattedNumbersOnly}
					, {title:'Monounsaturated Fat ( g )', data: 'monoUnsaturatedFat', editUnits: false, inputMethodHints: Qt.ImhFormattedNumbersOnly}
					, {title:'Cholesterol ( mg )', data: 'cholesterol', editUnits: false, inputMethodHints: Qt.ImhFormattedNumbersOnly}
					, {title:'Total Carbohydrates ( g )', data: 'totalCarbohydrate', editUnits: false, inputMethodHints: Qt.ImhFormattedNumbersOnly}
					, {title:'Fibers ( g )', data: 'dietaryFiber', editUnits: false, inputMethodHints: Qt.ImhFormattedNumbersOnly}
					, {title:'Sugars ( g )', data: 'sugar', editUnits: false, inputMethodHints: Qt.ImhFormattedNumbersOnly}
					, {title:'Protein ( g )', data: 'protein', editUnits: false, inputMethodHints: Qt.ImhFormattedNumbersOnly}
					, {title:'Calcium ( % )', data: 'calcium', editUnits: false, inputMethodHints: Qt.ImhFormattedNumbersOnly}
					, {title:'Iron ( % )', data: 'iron', editUnits: false, inputMethodHints: Qt.ImhFormattedNumbersOnly}
					, {title:'Potassium ( mg )', data: 'potassium', editUnits: false, inputMethodHints: Qt.ImhFormattedNumbersOnly}
					, {title:'Sodium ( mg )', data: 'sodium', editUnits: false, inputMethodHints: Qt.ImhFormattedNumbersOnly}
					, {title:'Vitamin A ( % )', data: 'vitaminA', editUnits: false, inputMethodHints: Qt.ImhFormattedNumbersOnly}
					, {title:'Vitamin C ( % )', data: 'vitaminC', editUnits: false, inputMethodHints: Qt.ImhFormattedNumbersOnly}
			]
			delegate: RowLayout{
				width: fieldsList.width
				Text{
					color: Material.foreground
					text: modelData.title
				}
				TextField{
					Layout.fillWidth: true
					horizontalAlignment: Text.AlignRight
					inputMethodHints: modelData.inputMethodHints
					property var txt: mainItem.foodModel[modelData.data] < 0 ? '' : mainItem.foodModel[modelData.data]
					text: txt && inputMethodHints ==  Qt.ImhFormattedNumbersOnly ? txt.toFixed(4) : txt
					onEditingFinished: mainItem.foodModel[modelData.data] = newValue
				}
				ComboBox{
					id: unitChoice
					visible: !!modelData.editUnits
					textRole: "displayText"
					valueRole: "id"
					model: UnitProxyModel{
						filterType: UnitProxyModel.WEIGHT | UnitProxyModel.VOLUME
						Component.onCompleted: if( unitChoice.visible) update()
					}
					onActivated:{mainItem.foodModel[modelData.editUnits] = currentValue}
					currentIndex: indexOfValue(mainItem.foodModel[modelData.editUnits])
				}
			}
		}

	}
}
