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
	Connections{
		target: mainItem.foodModel
		function onSaved(){
			mainItem.saved()
		}
		function onLoadingFailed(){
			errorText.text = "Cannot save. If the food have an Open Food Facts image URL, the image could'nt be download. Restart saving."
			errorPopup.open()
		}
	}
	ColumnLayout{
		anchors.fill: parent
		spacing: 0

		ListView{
			id: fieldsList
			Layout.fillWidth: true
			Layout.fillHeight: true
			model:[{title:'Brand', data: 'brand', editUnits: false, inputMethodHints: Qt.ImhNone, isComputable:false, showImage:true}
					, {title:'Description', data: 'description', editUnits: false, inputMethodHints: Qt.ImhNone, isComputable:false}
					, {title:'Serving Size', data: 'servingSize', editUnits: 'servingUnitId', inputMethodHints: Qt.ImhFormattedNumbersOnly, isComputable:false}
					//, {title:'Serving per container', data: 'servingsPerContainer', editUnits: false}

					, {title:'Base size', data: 'baseSize', editUnits: 'baseUnitId', inputMethodHints: Qt.ImhFormattedNumbersOnly, isComputable:false}
					, {title:'Calories ( kcal )', data: 'calories', editUnits: false, inputMethodHints: Qt.ImhFormattedNumbersOnly, isComputable:true}
					, {title:'Total Fat ( g )', data: 'totalFat', editUnits: false, inputMethodHints: Qt.ImhFormattedNumbersOnly, isComputable:true}
					, {title:'Saturated Fat ( g )', data: 'saturatedFat', editUnits: false, inputMethodHints: Qt.ImhFormattedNumbersOnly, isComputable:true}
					, {title:'Trans Fat ( g )', data: 'transFat', editUnits: false, inputMethodHints: Qt.ImhFormattedNumbersOnly, isComputable:true}
					, {title:'Polyunsaturated Fat ( g )', data: 'polyUnsaturatedFat', editUnits: false, inputMethodHints: Qt.ImhFormattedNumbersOnly, isComputable:true}
					, {title:'Monounsaturated Fat ( g )', data: 'monoUnsaturatedFat', editUnits: false, inputMethodHints: Qt.ImhFormattedNumbersOnly, isComputable:true}
					, {title:'Cholesterol ( mg )', data: 'cholesterol', editUnits: false, inputMethodHints: Qt.ImhFormattedNumbersOnly, isComputable:true}
					, {title:'Total Carbohydrates ( g )', data: 'totalCarbohydrate', editUnits: false, inputMethodHints: Qt.ImhFormattedNumbersOnly, isComputable:true}
					, {title:'Fibers ( g )', data: 'dietaryFiber', editUnits: false, inputMethodHints: Qt.ImhFormattedNumbersOnly, isComputable:true}
					, {title:'Sugars ( g )', data: 'sugar', editUnits: false, inputMethodHints: Qt.ImhFormattedNumbersOnly, isComputable:true}
					, {title:'Protein ( g )', data: 'protein', editUnits: false, inputMethodHints: Qt.ImhFormattedNumbersOnly, isComputable:true}
					, {title:'Calcium ( % )', data: 'calcium', editUnits: false, inputMethodHints: Qt.ImhFormattedNumbersOnly, isComputable:true}
					, {title:'Iron ( % )', data: 'iron', editUnits: false, inputMethodHints: Qt.ImhFormattedNumbersOnly, isComputable:true}
					, {title:'Potassium ( mg )', data: 'potassium', editUnits: false, inputMethodHints: Qt.ImhFormattedNumbersOnly, isComputable:true}
					, {title:'Sodium ( mg )', data: 'sodium', editUnits: false, inputMethodHints: Qt.ImhFormattedNumbersOnly, isComputable:true}
					, {title:'Vitamin A ( % )', data: 'vitaminA', editUnits: false, inputMethodHints: Qt.ImhFormattedNumbersOnly, isComputable:true}
					, {title:'Vitamin C ( % )', data: 'vitaminC', editUnits: false, inputMethodHints: Qt.ImhFormattedNumbersOnly, isComputable:true}
			]
			delegate: RowLayout{
				width: fieldsList.width
				spacing: 1
				Text{
					id: titleField
					Layout.fillWidth: true
					color: Material.foreground
					text: modelData.title
				}
				Image{
					id: imageField
					Layout.fillWidth: true
					Layout.preferredHeight: dataField.height
					visible: !!modelData.showImage && url != ''
					fillMode: Image.PreserveAspectFit
					property string url: mainItem.foodModel.imageUrl != ''  ? mainItem.foodModel.imageUrl : mainItem.foodModel.openFoodFactsImageUrl
					source: !visible ? '' : url

					MouseArea{
						anchors.fill: parent
						onClicked:{
							imagePreview.source = imageField.source
							previewPopup.open()
						}
					}
				}

				Text{
					visible: !!modelData.isComputable
					color: Material.accent
					text: visible ? mainItem.foodModel.computeNutriment(dataField.text, mainItem.foodModel.servingSize, mainItem.foodModel.servingUnitId, mainItem.foodModel.baseSize, mainItem.foodModel.baseUnitId).toFixed(2) : ''
				}
				TextField{
					id: dataField
					Layout.preferredWidth:  inputMethodHints ==  Qt.ImhFormattedNumbersOnly ? 120 : 240
					horizontalAlignment: Text.AlignRight
					inputMethodHints: modelData.inputMethodHints
					edit: true
					property var txt: mainItem.foodModel[modelData.data] < 0 ? '' : mainItem.foodModel[modelData.data]
					text: txt && inputMethodHints ==  Qt.ImhFormattedNumbersOnly ? Number.parseFloat(txt.toFixed(4)) : txt
					onEditingFinished: mainItem.foodModel[modelData.data] = newValue
				}
				ComboBox{
					id: unitChoice
					visible: !!modelData.editUnits
					textRole: "displayText"
					valueRole: "id"
					model: UnitProxyModel{
						id: unitProxyModel
						filterType: UnitProxyModel.WEIGHT | UnitProxyModel.VOLUME
					}
					onActivated:{mainItem.foodModel[modelData.editUnits] = currentValue}
					currentIndex: unitChoice.count && indexOfValue(mainItem.foodModel[modelData.editUnits])
				}
			}
		}

	}
	Popup{
		id: previewPopup
		height: mainWindow.height
		width: mainWindow.width
		y: -mapToGlobal(0,0).y
		Rectangle{
			color: Material.background
			anchors.fill: parent
			Image{
				id: imagePreview
				anchors.fill: parent
				fillMode: Image.PreserveAspectFit
			}
			MouseArea{
				anchors.fill: parent
				onClicked: previewPopup.close()
			}
		}
	}
	Popup{
		id: errorPopup
		height: mainWindow.height
		width: mainWindow.width
		y: -mapToGlobal(0,0).y
		Rectangle{
			color: Material.background
			anchors.fill: parent
			Text{
				id: errorText
				anchors.centerIn: parent
			}
			MouseArea{
				anchors.fill: parent
				onClicked: errorPopup.close()
			}
		}
	}
}
