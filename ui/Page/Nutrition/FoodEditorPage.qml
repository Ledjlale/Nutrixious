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
	ColumnLayout{
		anchors.fill: parent
		spacing: 0
		Text{
			Layout.fillWidth: true
			horizontalAlignment: Text.AlignHCenter
			color: Material.foreground
			text: qsTr('Create your meals')
		}
		RowLayout{
			spacing: 15
			TextField{
				Layout.fillWidth: true
				Layout.leftMargin: 5
				placeholderText: 'Search for a saved meal'
			}

		}

		FoodModel{
			id: foodModel
			onServingUnitChanged: console.log("Change !!! " +servingUnit)
		}
		ListView{
			id: fieldsList
			Layout.fillWidth: true
			Layout.fillHeight: true
			model:[{title:'Brand', data: 'brand', editUnits: false}
					, {title:'Description', data: 'description', editUnits: false}
					, {title:'Serving Size', data: 'servingSize', editUnits: 'servingUnit'}
					, {title:'Serving per container', data: 'servingsPerContainer', editUnits: false}
					, {title:'Calories ( kcal )', data: 'calories', editUnits: false}
					, {title:'Total Fat ( g )', data: 'totalFat', editUnits: false}
					, {title:'Saturated Fat ( g )', data: 'saturatedFat', editUnits: false}
					, {title:'Trans Fat ( g )', data: 'transFat', editUnits: false}
					, {title:'Polyunsaturated Fat ( g )', data: 'polyUnsaturatedFat', editUnits: false}
					, {title:'Monounsaturated Fat ( g )', data: 'monoUnsaturatedFat', editUnits: false}
					, {title:'Cholesterol ( mg )', data: 'cholesterol', editUnits: false}
					, {title:'Sodium ( mg )', data: 'sodium', editUnits: false}
					, {title:'Total Carbohydrates ( g )', data: 'totalCarbohydrate', editUnits: false}
					, {title:'Dietary Fibers ( g )', data: 'dietaryFiber', editUnits: false}
					, {title:'Sugars ( g )', data: 'sugar', editUnits: false}
					, {title:'Protein ( g )', data: 'protein', editUnits: false}
					, {title:'Calcium ( % )', data: 'calcium', editUnits: false}
					, {title:'Iron ( % )', data: 'iron', editUnits: false}
					, {title:'Potassium ( mg )', data: 'potassium', editUnits: false}
					, {title:'Vitamin A ( % )', data: 'vitaminA', editUnits: false}
					, {title:'Vitamin C ( % )', data: 'vitaminC', editUnits: false}
			]
			delegate: RowLayout{
				width: fieldsList.width
				Text{
					text: modelData.title
				}
				TextField{
					Layout.fillWidth: true
					horizontalAlignment: Text.AlignRight
					text: foodModel[modelData.data]
					onEditingFinished: foodModel[modelData.data] = newValue
				}
				ComboBox{
					visible: !!modelData.editUnits
					displayText: currentText
					onDisplayTextChanged: console.log(displayText)
					model: visible  ? ['kg','g'] : []
					onActivated:{foodModel[modelData.editUnits] = currentText}

					Component.onCompleted: if(visible) currentIndex = find(foodModel[modelData.editUnits])
				}
			}
		}

	}
}
