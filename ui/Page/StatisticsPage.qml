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
import QtCharts

import App 1.0

import '../Tool/Utils.js' as Utils

Item {
	id: mainItem
	property var lastHeaders: {'showBackButton': false,
			'showMenuButton': true,
			'title': 'History'
		}
	Component.onCompleted:{
		mainWindow.setHeaders(lastHeaders)
	}
	ColumnLayout{
		anchors.fill: parent
		TabBar{
			id: bar
			Layout.fillWidth: true
			currentIndex: 0
			TabButton{
				width: Math.max(100, implicitWidth)
				text: 'Personal data'
			}
			TabButton{
				width: Math.max(100, implicitWidth)
				text: 'Trainings'
			}
			TabButton{
				width: Math.max(100,implicitWidth)
				text: 'Exercises'
			}
			TabButton{
				width: Math.max(100,implicitWidth)
				text: 'Nutrition'
			}
			onCurrentIndexChanged: {
				switch(currentIndex){
				case 1 : stackView.replace(trainingsComponent); break;
				case 2 : stackView.replace(exerciseComponent); break;
				case 3 : stackView.replace(nutritionComponent); break;
				default:{
					stackView.replace(personalDataComponent);
				}
				}
			}
		}
		StackView{
			id: stackView
			Layout.fillWidth: true
			Layout.fillHeight: true
			initialItem: personalDataComponent
			Component{
				id: personalDataComponent
				PersonalDataPage{}
			}
			Component{
				id: trainingsComponent
				TrainingsPage{}
			}
			Component{
				id: exerciseComponent
				ExerciseStatsPage{}
			}
			Component{
				id: nutritionComponent
				NutritionStatsPage{}
			}
		}
	}
}
