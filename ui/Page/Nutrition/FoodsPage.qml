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
	property bool isEdition: false

	signal back()
	signal picked(var food)
	
	property bool isCurrentItem: false
	onIsCurrentItemChanged: if(isCurrentItem) {
		console.log("FoodsPage visible")
		mainWindow.setHeaders({'showBackButton':true, 'showMenuButton':false, 'showBarcodeButton':true, 'showCreateButton':true, 'title':'Foods'})
	}
	
	function editFoodModel(foodModelToEdit){
		foodModelToEdit.autoCompute = mainItem.isPicker && foodModelToEdit.isSaved
		stackView.push(foodEditorComponent, {foodModel:foodModelToEdit})
	}

	Connections{
		target: mainWindow.header
		enabled: mainItem.isCurrentItem
		property bool editing: stackView.currentItem.objectName == "Editor"
		property bool scanning: stackView.currentItem.objectName == "Scanner" && stackView.currentItem.isStarted
		function onCreate(){
				stackView.push(foodEditorComponent)
				mainWindow.header.updateHeaders({'showCreateButton': false, 'showSaveButton': true})
		}
		function onBarcode(){
			if(!scanning)
				stackView.push(scannerComponent)
			else
				stackView.pop()
		}
		function onSave(){
			console.log("FoodsPage onSave button")
			if(mainItem.isPicker){
				if(stackView.currentItem.foodModel.isSaved){
					mainItem.picked(stackView.currentItem.foodModel)
					stackView.pop()
					mainItem.back()
				}else{
					var result = stackView.currentItem.foodModel.save()
					if( result == 1){
					}else if(result == 2){
						console.log("waiting for download")
					}else {
						console.log("couldn't save")
					}
				}
			}else if(stackView.currentItem.foodModel.save() == 1) {
				
			}
		}
		function onBack(){
			if(stackView.depth > 1) {
				stackView.pop()
				if(mainItem.isEdition)
					mainItem.back()
			}else
				mainItem.back()
		}
	}

	ColumnLayout{
		anchors.fill: parent
		spacing: 0
		StackView{
			id: stackView
			Layout.fillHeight: true
			Layout.fillWidth: true
			initialItem: searchComponent
			
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
							stackView.push(foodEditorComponent, {offCode: searchedCode})
						}
					}
					onFoodModelFound: function(foodModelSearched){
						foodModelSearched.autoCompute = mainItem.isPicker && foodModelSearched.isSaved
						stackView.push(foodEditorComponent, {foodModel:foodModelSearched})
					}
				}
			}
			Component{
				id: foodEditorComponent
				FoodEditorPage{
					id: editor
					objectName: 'Editor'
					onSaved:{
						if(mainItem.isPicker){
							mainItem.picked(foodModel)
							stackView.pop()
							mainItem.back()
						}else
							stackView.pop()
						
					}
				}
			}
		}
	}
}
