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
	property bool isPicker: false

	signal back()
	signal picked(var food)



	ColumnLayout{
		anchors.fill: parent
		spacing: 0
		RowLayout{
			Button{
				visible: mainItem.isPicker || stackView.depth > 1
				text: 'Back'
				onClicked: if( stackView.depth > 1 ) stackView.pop()
				else
					mainItem.back()
			}
			Item{
				Layout.fillWidth: true
			}
			Button{
				visible: stackView.depth == 1
				property bool searching: stackView.currentItem.objectName == "Search"
				text: searching ? 'X' : qsTr('Search')
				onClicked: {
						if(!searching)
							stackView.push(searchComponent)
						else
							stackView.pop()
				}
			}
			Button{
				visible: stackView.depth == 1
				property bool scanning: stackView.currentItem.objectName == "Scanner" && stackView.currentItem.isStarted
				text: scanning ? 'X' : qsTr('Scan')
				onClicked: {
						if(!scanning)
							stackView.push(scannerComponent)
						else
							stackView.pop()
				}
			}
			Button{
				property bool editing: stackView.currentItem.objectName == "Editor"
				text: editing
						?  stackView.currentItem.foodModel.isSaved
							? mainItem.isPicker
								? 'Use'
								: 'Update'
							: mainItem.isPicker
								? 'Save & use'
								: 'Save'
						: 'Create'
				onClicked:{
					if(editing){
						if(mainItem.isPicker){
							if(stackView.currentItem.foodModel.isSaved){
								mainItem.picked(stackView.currentItem.foodModel)
								stackView.pop()
							}else{
								if(stackView.currentItem.foodModel.save() == 1){
									mainItem.picked(stackView.currentItem.foodModel)
									stackView.pop()
								}
							}
						}else if(stackView.currentItem.foodModel.save() == 1) {
							stackView.pop()
							foodListModel.update()
						}
					}else {
						stackView.push(foodEditorComponent)
					}
				}
			}
		}
		StackView{
			id: stackView
			Layout.fillHeight: true
			Layout.fillWidth: true
			initialItem: ListView{
				spacing: 5
				model: FoodProxyModel{
					id: foodListModel
					Component.onCompleted: update()
				}
				delegate: FoodLineView{
					width: stackView.width
					//height: implicitHeight
					foodModel: $modelData
					MouseArea{
						anchors.fill: parent
						onClicked:{
							$modelData.autoCompute = mainItem.isPicker && $modelData.isSaved
							stackView.push(foodEditorComponent, {foodModel:$modelData})
						}
					}
					Rectangle{
						width: parent.width
						anchors.bottom: parent.bottom
						height: 1
						color: 'gray'
					}
				}
			}
			Component{
				id: scannerComponent
				ScannerView{
					id: scannerView
					objectName: 'Scanner'
					onScannedCodeChanged:{
						if(scannedCode != '') {
							stackView.replace(foodEditorComponent, {offCode: scannedCode})
						}
					}
					Component.onCompleted: start()
				}
			}
			Component{
				id: searchComponent
				FoodSearchView{
					id: searchView
					objectName: 'Search'
					onSearchedCodeChanged:{
						if(searchedCode != '') {
							stackView.replace(foodEditorComponent, {offCode: searchedCode})
						}
					}
				}
			}
			Component{
				id: foodEditorComponent
				FoodEditorPage{
					id: editor
					objectName: 'Editor'
					onSaved: {
						if(mainItem.isPicker){
							mainItem.picked(foodModel)
							stackView.pop()
						}else {
							foodListModel.update()
							stackView.pop()
						}
					}
				}
			}
		}
	}
}
