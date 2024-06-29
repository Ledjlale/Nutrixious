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


ColumnLayout {
	id: mainItem

	Item{// StackLayout is not used because Qt bug on layout refreshing.
		id: stackLayout
		Layout.fillWidth: true
		Layout.preferredHeight: nutritionLayout.implicitHeight
		ColumnLayout{
			id: nutritionLayout
			property var statsModel: StatsModel{
					id: nutritionTimeStats
			}
			anchors.fill: parent
			ComboBox{
				id: nutritionChoice
				Layout.fillWidth: true
				model:['Nutrition', 'Body Weight','Calories']
				onCurrentIndexChanged: {
					var points
					if( currentIndex == 1 )
						points = nutritionTimeStats.computeBodyWeights()
					else if(currentIndex == 2)
						points = nutritionTimeStats.computeNutritionCalories()
					if(points && points.length)timeView.displayPoints(points)
				}
			}
		}
	}
	TimeChart{
		id: timeView
		Layout.fillWidth: true
		Layout.fillHeight: true
	}
}
