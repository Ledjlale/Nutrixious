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

Item {
	id: mainItem
	property MealGroupProxyModel mealGroups: MealGroupProxyModel{
		Component.onCompleted: update()
	}
	property MealFoodListModel meals: MealFoodListModel{
				id: mealFoods
	}
	Connections{
		target: mainWindow
		function onGSave(){
			stackView.currentItem.foodModel.save()
			stackView.pop()
			gShowBackButton = false
			gShowSaveButton = false
			gShowMenuButton = true
		}
		function onGBack(){
			stackView.currentItem.foodModel.undo()
			stackView.pop()
			gShowBackButton = false
			gShowSaveButton = false
			gShowMenuButton = true
		}
	}
	StackView{
		id: stackView
		anchors.fill: parent
		initialItem:
		ColumnLayout{
			width: stackView.width
			height: stackView.height
			DayNavigationBar{
				id: dayBar
				Layout.fillWidth : true
				onNext: mealFoods.updateFromDate(currentDay)
				onPrevious: mealFoods.updateFromDate(currentDay)
				Component.onCompleted: mealFoods.updateFromDate(currentDay)
			}
//---------------------------------		OVERVIEW
			RowLayout {
				spacing: 0
				ColumnLayout{
					spacing: 0
					Text{
						Layout.fillWidth : true
						text: 'Calories'
						color: Material.foreground
					}
					Text{
						text: Number.parseFloat(mainItem.meals.totalCalories.toFixed(2)) + ' / ' +Number.parseFloat(mainItem.meals.targetCalories.toFixed(2))
						color: Material.foreground
					}
				}
				ColumnLayout{
					spacing: 0
					Text{
						Layout.fillWidth : true
						text: 'Carbohydreats'
						color: Material.foreground
					}
					Text{
						text: Number.parseFloat(mainItem.meals.totalCarboRatio.toFixed(2)) + ' / ' +Number.parseFloat(mainItem.meals.targetCarboRatio.toFixed(2))
						color: Material.foreground
					}
				}
				ColumnLayout{
					spacing: 0
					Text{
						Layout.fillWidth : true
						text: 'Fat'
						color: Material.foreground
					}
					Text{
						text: Number.parseFloat(mainItem.meals.totalFatRatio.toFixed(2)) + ' / ' +Number.parseFloat(mainItem.meals.targetFatRatio.toFixed(2))
						color: Material.foreground
					}
				}
				ColumnLayout{
					spacing: 0
					Text{
						Layout.fillWidth : true
						text: 'Protein'
						color: Material.foreground
					}
					Text{
						text: Number.parseFloat(mainItem.meals.totalProteinRatio.toFixed(2)) + ' / ' +Number.parseFloat(mainItem.meals.targetProteinRatio.toFixed(2))
						color: Material.foreground
					}
				}


			}
//----------------------------------------------------------------------------------------
			ListView{
				id: mealGroupList
				Layout.fillWidth: true
				Layout.fillHeight: true
				clip: true
				model: mainItem.mealGroups
				delegate:
					ColumnLayout{
						width: mealGroupList.width
						height: sectionItem.height + mealFoodsList.contentHeight + addButton.height
						spacing: 0
						Rectangle{
							id: sectionItem
							property var model: $modelData
							width: mealGroupList.width
							height: sectionRow.implicitHeight + 10
							color: Material.color(Material.BlueGrey)
							RowLayout{
								id: sectionRow
								anchors.fill: parent
								anchors.margins: 5
								Text{
									id: mealGroupTitle
									Layout.fillWidth: true
									color: 'white'
									text:  $modelData.name
									//Component.onCompleted: text = $modelData.name
								}
								Text{
									id: totalCalories
									color: 'white'
									text: Number.parseFloat(mainItem.meals.calories($modelData.mealGroupId).toFixed(2))
									Connections{
										target: mainItem.meals
										function onCountChanged(){
											totalCalories.text = Number.parseFloat(mainItem.meals.calories($modelData.mealGroupId).toFixed(2))
										}
										function onCaloriesChanged(){
											totalCalories.text = Number.parseFloat(mainItem.meals.calories($modelData.mealGroupId).toFixed(2))
										}

									}
								}
							}
						}
						ListView{
							id: mealFoodsList
							Layout.fillWidth: true
							Layout.fillHeight: true
							model: MealFoodProxyModel{
								meals: mainItem.meals
								mealGroupId: $modelData.mealGroupId
							}
							delegate:	MealView{
								id: mealView
								width: mealFoodsList.width
								height: implicitHeight
								visible: $modelData.mealGroupId == sectionItem.model.mealGroupId
								mealFoodModel: $modelData
								MouseArea{
									anchors.fill: parent
									onClicked: {
										gShowBackButton = true
										gShowSaveButton = true
										gShowMenuButton = false
										$modelData.autoCompute = true
										stackView.push(foodEditorComponent, {foodModel:$modelData})
									}
								}
							}
						}
						Button{
							id: addButton
							Layout.fillWidth: true
							text: 'Add'
							onClicked: {
								stackView.push(foodPickerComponent, {mealGroup:$modelData})
							}
						}
					}
			}
		}
		Component{
			id: foodPickerComponent
			FoodsPage{
				width: stackView.width
				height: stackView.height
				property var mealGroup
				isPicker: true

				onBack: stackView.pop()
				onPicked: function(food){
					mainItem.meals.addFoodModel(food, mealGroup)
					stackView.pop()
				}
			}
		}
		Component{
			id: foodEditorComponent
			FoodEditorPage{
				onSaved: stackView.pop()
			}
		}
	}
}

