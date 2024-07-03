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

StackLayout{
	id: mainItem
	property var lastHeaders: {'showBackButton': false,
			'showMenuButton': true,
			'showBodyButton': true,
			'title': 'Diary'
		}
	property bool isHeaderOwner: true
	currentIndex: 0
	/*
	onCurrentIndexChanged: {
		if(mainItem.visible) {
			lastHeaders = {'showBackButton':currentIndex > 0,
				'showMenuButton':currentIndex==0,
				'showBodyButton': currentIndex==0,
				'title' : currentIndex == 0 ? 'Diary'
					: currentIndex == 1 ? 'Training'
					: currentIndex == 2 ? 'Meal Groups'
					: 'Foods'
			}
			mainWindow.setHeaders(lastHeaders)
		}
	}*/
	Component.onCompleted:{
		//mainWindow.setHeaders(lastHeaders)
	}
	/*
	Connections{
		target: mainWindow
		enabled: mainItem.isHeaderOwner && mainItem.visible
		function onGBack(){
			if(mainItem.currentIndex>0) {
				if(mainItem.currentIndex == 2)
					diaryPage.updateMealGroups()
				else
					diaryPage.updateTrainings()
				mainItem.currentIndex= 0
			}
		}
	}*/
	DiaryPage{
		id: diaryPage
		isCurrentItem: StackLayout.isCurrentItem
		onExerciseRequested: currentIndex = 1
		onMealGroupRequested: currentIndex = 2
		onSetHeaders: function(headers) {
			isHeaderOwner = !headers
			mainWindow.setHeaders(headers ? headers : lastHeaders)
		}
		onFoodPickerRequested: function(mealGroup){
			foodPage.mealGroup = mealGroup
			foodPage.isEdition = false
			currentIndex = 3
		}
		onFoodEditorRequested: function(foodModel) {
			currentIndex = 3
			foodPage.isEdition = true
			foodPage.editFoodModel(foodModel)
		}
	}
	TrainingPage{
		isCurrentItem: StackLayout.isCurrentItem
		onBack: mainItem.currentIndex = 0
	}
	MealGroupsPage{
		isCurrentItem: StackLayout.isCurrentItem
		onBack: mainItem.currentIndex = 0
	}
	
	FoodsPage{
		id: foodPage
		property var mealGroup
		isPicker: true
		isCurrentItem: StackLayout.isCurrentItem

		onBack: mainItem.currentIndex = 0
		onPicked: function(food){
			if(foodPage.isEdition){
				console.log("Save edited food")
				food.save()
			}else{
				console.log("Add food to model")
				diaryPage.meals.addFoodModel(food, mealGroup, diaryPage.currentDay)
			}
			mainItem.currentIndex = 0
			++diaryPage.totalRefresh
		}
					
	}
}
