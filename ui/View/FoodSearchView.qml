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
import QtCore
import QtMultimedia

import App 1.0


Item {
	id: mainItem

	property string searchedCode
	
	signal foodModelFound(var foodModelSearched)

	property FoodModel foodModel: FoodModel{
		onOpenFoodFactsFound: function(results) {
			resultList.model = results
		}
	}
	property FoodProxyModel foodDatabase: FoodProxyModel{
		Component.onCompleted: update()
	}

	ColumnLayout{
		anchors.fill: parent
		RowLayout{
			Layout.preferredHeight: searchField.implicitHeight
			Layout.maximumHeight: searchField.implicitHeight
			
			TextField{
				id: searchField
				Layout.fillWidth: true
				edit: true
				placeholderText: 'Search for meal'
				onEditingFinished: text = newValue
			}
			ButtonImage{
				Layout.preferredWidth: 60
				Layout.preferredHeight: 60
				Layout.alignment: Qt.AlignCenter
				imageSource: DefaultStyle.theme == Material.Dark ? DefaultStyle.searchOFFDarkButton : DefaultStyle.searchOFFButton
				//colorizationColor: Material.foreground
				onClicked:{
					forceActiveFocus()
					foodModel.findOpenFoodFacts(searchField.text)
				}
			}
			ButtonImage{
				Layout.preferredWidth: 40
				Layout.preferredHeight: 40
				Layout.rightMargin: 5
				Layout.alignment: Qt.AlignCenter
				imageSource: DefaultStyle.searchDatabaseButton
				colorizationColor: Material.foreground
				onClicked:{
					forceActiveFocus()
					foodDatabase.name = searchField.text
					resultList.model = foodDatabase
				}
			}
		}
		Item{
			Layout.fillHeight: true
			Layout.fillWidth: true
			visible: resultList.count == 0
		}
		ListView{
			id: resultList
			Layout.fillWidth: true
			Layout.fillHeight: true
			model: mainItem.foodDatabase
			clip: true
			delegate:Item{
				height: 80
				width: resultList.width
				MouseArea{
					anchors.fill: parent
					onClicked: if(modelData) searchedCode = modelData.code
								else foodModelFound($modelData)
				}
				RowLayout{
					id: lineLayout
					anchors.fill: parent
					ColumnLayout{
						clip: true
						Text{
							id: title
							Layout.fillWidth: true
							color: Material.foreground
							text: modelData?.brands ? modelData.brands : $modelData?.brand ? $modelData.brand : ''
						}
						Text{
							id: desc
							Layout.fillWidth: true
							color: Material.foreground
							text: modelData?.generic_name ? modelData.generic_name : $modelData?.description ? $modelData.description : ''
							font.italic: true
						}
					}
					Image{
						id: imageField
						Layout.fillHeight: true
						Layout.preferredWidth: height
						source: modelData?.image_url ? modelData?.image_url : $modelData?.imageUrl
						fillMode: Image.PreserveAspectFit

						MouseArea{
							anchors.fill: parent
							onClicked:{
								imagePreview.source = imageField.source
								previewPopup.open()
							}
						}
					}
				}
				Rectangle{
					color: "white"
					anchors.bottom: parent.bottom
					anchors.left: parent.left
					anchors.right: parent.right
					height:1
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
}
// Eau
// https://ssl-api.openfoodfacts.org/api/v0/product/3257971101015.json
// Lait
// https://ssl-api.openfoodfacts.org/api/v0/product/3520836330036.json
