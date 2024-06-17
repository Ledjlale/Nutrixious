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

	property FoodModel foodModel: FoodModel{
		onOpenFoodFactsFound: function(results) {
			resultList.model = results
		}
	}

	ColumnLayout{
		anchors.fill: parent
		RowLayout{
			Layout.preferredHeight: searchField.implicitHeight
			Layout.maximumHeight: searchField.implicitHeight
			TextField{
				id: searchField
				Layout.fillWidth: true
				keepEditView: true
				placeholderText: 'Search for a saved meal'
				onEditingFinished: text = newValue
			}
			Button{
				text: 'Search'
				onClicked:{
					foodModel.findOpenFoodFacts(searchField.text)
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
			delegate:Item{
				height: 80
				width: resultList.width
				MouseArea{
					anchors.fill: parent
					onClicked: searchedCode = modelData.code
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
							text: modelData.brands ? modelData.brands : ''
						}
						Text{
							id: desc
							Layout.fillWidth: true
							color: Material.foreground
							text: modelData.generic_name ? modelData.generic_name : ''
							font.italic: true
						}
					}
					Image{
						id: imageField
						Layout.fillHeight: true
						Layout.preferredWidth: height
						source: modelData.image_url
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
