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
	property TrainingListModel trainings: TrainingListModel{
	}
	PersonalDataModel{
		id: lastDataModel
	}
	property alias currentDay: dayBar.currentDay
	property int totalRefresh: 1
	signal exerciseRequested()
	signal setHeaders(var headers)
	signal mealGroupRequested()
	signal foodPickerRequested(var mealGroup)
	signal foodEditorRequested(var params)
	
	function updateTrainings(){
		trainings.update()
		trains.updateCalories()
	}
	function updateMealGroups(){
		mealGroups.update()
	}
	onTotalRefreshChanged: mainItem.meals.updateTotals()
	property bool isCurrentItem: false
	onIsCurrentItemChanged: if(isCurrentItem) {
		mainWindow.header.setHeaders( {'showBackButton': false,
			'showMenuButton': true,
			'showBodyButton': true,
			'title': 'Diary'
		})
	}
	StackView{
		id: stackView
		anchors.fill: parent
		onDepthChanged:
			if(depth > 1) mainItem.setHeaders({'showBackButton':true, 'showMenuButton':false})
			else mainItem.setHeaders(null)
		initialItem:
		ColumnLayout{
			width: stackView.width
			height: stackView.height
			DayNavigationBar{
				id: dayBar
				Layout.fillWidth : true
				onNext: updateFromDate()
				onPrevious: updateFromDate()
				Component.onCompleted: updateFromDate()
				function updateFromDate(){
					lastDataModel.loadLastIn(dayBar.currentDay)
					mealFoods.updateFromDate(currentDay)
					// This day need to have data
					if(!mainWindow.header.displayBody && dayBar.currentDay.isToday && !lastDataModel.isSameDay(currentDay))
						mainWindow.header.displayBody = true
				}
			}
//---------------------------------		OVERVIEW
			RowLayout{
				visible: mainWindow.header.displayBody
				RowLayout{
					width: parent.width
					height: implicitHeight + 10
					property bool editable: dayBar.currentDay.isToday
					TextField{
						Layout.fillWidth: true
						visible: lastDataModel.isSaved
						inputMethodHints: Qt.ImhDigitsOnly
						title: ' '
						text: lastDataModel.dateTimeStr
						readOnly: false
						onEditingFinished: lastDataModel.dateTimeStr = newValue
					}
					TextField{
						Layout.fillWidth: true
						title: 'Weight'
						edit: parent.editable
						text: lastDataModel.weight
						onEditingFinished:lastDataModel.weight = newValue
					}
					TextField{
						Layout.fillWidth: true
						title: 'Height'
						edit: parent.editable
						text: lastDataModel.height
						onEditingFinished:lastDataModel.height = newValue
					}
					SexBox{
						Layout.fillWidth: true
						edit: parent.editable
						value: lastDataModel.sex
						onAccepted:lastDataModel.sex = newValue
					}
					ButtonImage{
						Layout.alignment: Qt.AlignCenter
						Layout.preferredWidth: 25
						Layout.preferredHeight: 25
						Layout.rightMargin: 5
						visible: parent.editable
						imageSource: DefaultStyle.addDataButton
						colorizationColor: Material.foreground
						onClicked: {
							forceActiveFocus()
							lastDataModel.add()
							lastDataModel.loadLastIn(dayBar.currentDay)
						}
					}
				}
			}
//-------------------------------------------------------------
			RowLayout {
				spacing: 0
				ColumnLayout{
					id: caloriesLayout
					property real total: mainItem.meals.totalCalories
					property real target: mainItem.meals.targetCalories + trains.calories
					property real weight: 1000 * (caloriesLayout.total - caloriesLayout.target) / 7500
					spacing: 0
					RowLayout{
						Text{
							Layout.fillWidth : true
							color: Material.foreground
							text: 'Calories'
						}
						Text{
							Layout.fillWidth : true
							color: caloriesLayout.weight > 0 ? Material.accent : Material.color(Material.LightGreen)
							text: Number.parseFloat(caloriesLayout.weight.toFixed(2)) + 'g'
						}
					}
					Text{
						text: Number.parseFloat(caloriesLayout.total.toFixed(2)) + ' / ' +Number.parseFloat(caloriesLayout.target.toFixed(2))
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
			Flickable{
				Layout.fillWidth: true
				Layout.fillHeight: true
				contentWidth: width
				contentHeight: mainColumnLayout.implicitHeight
				clip: true
				ColumnLayout{
					id: mainColumnLayout
					width: parent.width
					ListView{
						id: mealGroupList
						Layout.fillWidth: true
						
						implicitHeight: contentHeight
						clip: true
						model: mainItem.mealGroups
						interactive: false
						delegate:
							ColumnLayout{
								width: mealGroupList.width
								height: sectionItem.implicitHeight +15+ mealFoodsList.contentHeight + addButton.height + 4*spacing
								spacing: 5
								SwipeLayout{
									id: sectionItem
									property var model: $modelData
									Layout.fillWidth: true
									Layout.preferredHeight: sectionRow.implicitHeight + 15
									isDeletable: false
									isEditable: true
									onEditClicked:{
										mainItem.mealGroupRequested()
									}
									background:Rectangle{
										color: Material.color(Material.BlueGrey)
									}
									contentItem: RowLayout{
										id: sectionRow
										width: mealGroupList.width
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
											text: mainItem.totalRefresh ? Number.parseFloat(mainItem.meals.calories($modelData.mealGroupId).toFixed(2)) : ''
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
									interactive: false
									model: MealFoodProxyModel{
										meals: mainItem.meals
										mealGroupId: $modelData.mealGroupId
									}
									delegate:	ColumnLayout{
										spacing:4
										width: mealFoodsList.width
										height: mealView.implicitHeight+5
										MealView{
											id: mealView
											Layout.fillWidth: true
											Layout.preferredHeight: implicitHeight
											visible: $modelData.mealGroupId == sectionItem.model.mealGroupId
											modelData: $modelData
											isEditable: false
											onClicked: {
												$modelData.autoCompute = true
												mainItem.foodEditorRequested( $modelData)
												//stackView.push(foodEditorComponent, {foodModel:$modelData})
												//mainItem.setHeaders({'showBackButton':true, 'showSaveButton':true, 'showMenuButton':false})
											}
										}
										Rectangle{
											Layout.fillWidth: true
											Layout.leftMargin: 5
											Layout.rightMargin: 5
											height: 1
											opacity: 0.5
											color: Material.foreground
										}
									}
								}
								ButtonImage{
									id: addButton
									Layout.preferredWidth: 80
									Layout.preferredHeight: 30
									Layout.alignment: Qt.AlignCenter
									Layout.topMargin: 5
									imageSource: DefaultStyle.addFoodButton
									colorizationColor: Material.foreground
									onClicked: {
										mainItem.foodPickerRequested( $modelData)
										//stackView.push(foodPickerComponent, {mealGroup:$modelData})
										//mainItem.setHeaders({'showBackButton':true, 'showMenuButton':false, 'showBarcodeButton':true, 'showCreateButton':true})
									}
								}
							}
						}
						Rectangle{
							Layout.fillWidth: true
							height: 80
							color: Material.primary
							Text{
								anchors.centerIn: parent
								color: 'white'
								text: 'Exercises'
							}
							MouseArea{
								anchors.fill: parent
								onClicked: {
									mainItem.setHeaders(null)
									mainItem.exerciseRequested()
								}
							}
						}
						ListView{
							id: trainingList
							Layout.fillWidth: true
							implicitHeight: contentHeight
							clip: true
							interactive: false
							model: TrainingProxyModel{
								id: trains
								trainingDay: dayBar.currentDay
								sourceModel: mainItem.trainings
								Component.onCompleted: trains.updateCalories()
							}
							delegate:
								Item{
									width: trainingList.width
									height: trainView.implicitHeight
									//anchors.fill: parent
									//propagateComposedEvents: true
									//preventStealing: true
									TrainModelView{
										id: trainView
										width: parent.width
										modelData: $modelData
										displayDate: false
										//onClicked: trainDetailsList.program = $modelData
									}
								}
								
							footer:Item{
								height: 50
								width: trainingList.width
							}
						}
					}
			}
		}
		
	}
}

