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


Item{
	id: mainItem
	property var recipeModel: RecipeModel{
	}
	
	property bool isCurrentItem: false
	
	signal back()
	signal saved()
	signal foodEditorRequested(var params)
	
	function updateHeaders(){
		mainWindow.header.setHeaders( {'showBackButton': true,
			'showMenuButton': false,
			'showSaveButton': true,
			'title': 'Recipe Editor'
		})
	}
	
	onVisibleChanged: if(visible){
		updateHeaders();
	}
	Component.onCompleted: updateHeaders()
		/*
	Connections{
		target: mainItem.foodModel
		function onSaved(){
			mainItem.saved()
		}
		function onLoadingFailed(){
			errorText.text = "Cannot save. If the food have an Open Food Facts image URL, the image could'nt be download. Restart saving."
			errorPopup.open()
		}
	}*/
	/*
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
	}*/
	Connections{
		target: mainWindow.header
		enabled: mainItem.isCurrentItem && stackView.depth == 1
		property bool editing: stackView.currentItem.objectName == "Editor"
		property bool scanning: stackView.currentItem.objectName == "Scanner" && stackView.currentItem.isStarted
		function onOk(){
			console.log("Ok for Recipe Editor")
		}
		function onCreate(){
			//stackView.push(foodEditorComponent)
			//mainWindow.header.updateHeaders({'showCreateButton': false, 'showSaveButton': true})
		}
		function onRecipe(){
			//mainItem.createRecipeRequest()
			//stackView.push(recipeEditorComponent)
			//mainWindow.header.updateHeaders({'showCreateButton': false, 'showSaveButton': true})
		}
		function onBarcode(){
			//if(!scanning)
				//stackView.push(scannerComponent)
			//else
				//stackView.pop()
		}
		function onSave(){
		/*
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
				
			}*/
		}
		function onBack(){
			if(stackView.depth > 1) {
				stackView.pop()
				mainItem.updateHeaders()
			}else
				mainItem.back()
		}
	}
	StackView{
		id: stackView
		anchors.fill: parent
		Component{
			id: foodsComponent
			FoodsPage{
				isCurrentItem: true
				isPicker: true
				onPicked: (ingredient) =>{
					mainItem.recipeModel.addIngredient(ingredient)
					stackView.pop()
				}
				onBack: {
					stackView.pop()
				}
			}
		}
		//property Component foodsComponent: Qt.createComponent("App", "FoodsPage", Component.Asynchronous, stackView)
		//property Component foodsComponent: Qt.createComponent("FoodsPage.qml")
		initialItem: ColumnLayout{
			width: stackView.width
			height: stackView.height
			TextField{
				id: brandField
				Layout.fillWidth: true
				horizontalAlignment: Text.AlignRight
//				inputMethodHints: modelData.inputMethodHints
				edit: true
				showTitle: true
				title: 'Brand'
				text: mainItem.recipeModel.brand
				onEditingFinished: mainItem.recipeModel.brand = newValue
			}
			TextField{
				id: descriptionField
				Layout.fillWidth: true
				horizontalAlignment: Text.AlignRight
//				inputMethodHints: modelData.inputMethodHints
				edit: true
				showTitle: true
				title: 'Description'
				text: mainItem.recipeModel.description
				onEditingFinished: mainItem.recipeModel.description = newValue
			}
			ListView{
				id: ingredientsList
				Layout.fillWidth: true
				Layout.fillHeight: true
				model: RecipeIngredientProxyModel{
					recipeModel: mainItem.recipeModel
				}
				delegate:Item{
					width: ingredientsList.width
					height: ingredient.implicitHeight
					RowLayout{
						id: ingredient
						anchors.fill: parent
						Text{
							Layout.fillWidth: true
							color: Material.foreground
							text: $modelData.foodModel.brand
						}
						Text{
							color: Material.foreground
							text: $modelData.ingredientSize +" / " + $modelData.ingredientUnitId
						}
					}
				}
				footer:Component{
					Item{
						width: ingredientsList.width
						height: 30
						ButtonImage{
							width: 80
							height: 30
							anchors.centerIn: parent
							imageSource: DefaultStyle.addFoodButton
							colorizationColor: Material.foreground
							onClicked: {
								//stackView.push(searchComponent, {width: stackView.width, height:stackView.height , isPicker:true})
								stackView.push(foodsComponent, {width: stackView.width, height:stackView.height , isPicker:true, isRecipeTarget: true})
							}
						}
					}
				}
			}
		}
	}
}
