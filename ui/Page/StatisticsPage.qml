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
	ColumnLayout{
		anchors.fill: parent
		RowLayout{
		/*
			Button{
				text: 'Back'
				visible: exerciseCreation.currentIndex != 0
				onClicked: {
					exerciseCreation.currentIndex = 0
					while(stackView.depth > 1)
						stackView.pop();
				}
			}*/
			Text{
				color: Material.foreground
				text: 'Exercises'
			}
			ComboBox{
				id: exerciseChoice
				Layout.fillWidth: true
				textRole: "displayText"
				valueRole: "$modelData"
				model: ExerciseProxyModel{
					id: exercises
				}
				Component.onCompleted: exercises.update()
				//onCurrentValueChanged: programExercises.setExercises(currentValue.exercises)
				onCurrentValueChanged:{
					//chartView.exerciseType = currentValue.type
					stats.setExercise(currentValue)
				}
			}
			ComboBox{
				id: modeChoice
				Layout.fillWidth: true
				textRole: "text"
				valueRole: "value"
				model: stats.availableSerieModes
				//onCurrentValueChanged: programExercises.setExercises(currentValue.exercises)
				onCurrentValueChanged:{
					//chartView.exerciseType = currentValue.type
					//stats.setExercise(currentValue)
					stats.compute(currentValue)

				}
			}
			Button{
				text: 'Reload'
				//visible: stackView.depth == 1
				onClicked: exercises.update()
			}
		}
		ChartView{
			id: chartView
			property int exerciseType: 1
			property var statsModel: StatsModel{
				id: stats
				function compute(mode){
					var points = stats.computeOnSerie(mode)
					var minX = -1
					var maxX = -1
					var minY = -1
					var maxY = -1
					series.clear()
					for(var p in points){
						var x = points[p].x
						var y = points[p].y
						if( x > maxX) maxX = x
						if( y > maxY) maxY = y
						if( x < minX || minX == -1) minX = x
						if( y < minY || minY == -1) minY = y
						console.log('Add : ' +x + ", "+y)
						series.append(x, y)
					}
					chartView.axisY(series).min = minY-1;
					chartView.axisY(series).max = maxY+1 ;
					console.log(minX + " , " +minY + " / " + maxX+ " , " +maxY)
					chartView.axisX(series).min = minX;
					chartView.axisX(series).max = maxX;
				}
				onExerciseModelChanged: {compute(StatsModel.WEIGHT)
				}

			}
			Layout.fillWidth: true
			Layout.fillHeight: true
			title: chartView.exerciseType == 3 ? 'Strength' : chartView.exerciseType==2 ? 'Steps' : chartView.exerciseType==1 ? 'Distance' : ''
			antialiasing: true
			ValueAxis {
				id: vAxis
			}
			DateTimeAxis{
				id: dAxis
				format: 'yyyy-MM-dd'
				labelsAngle: -60
			}
			LineSeries{
				id: series
				name: chartView.exerciseType == 3 ? 'Weights (kg)' : chartView.exerciseType==2 ? 'Nb of Steps' : chartView.exerciseType==1 ? 'Distance (m)' : ''
				axisX: dAxis
				axisY: vAxis
			}
		}

		/*
		ChartView{
			property StatsModel stats
		}*/
		/*
		ChartView{
			property var statsModel: StatsModel{
				id: stats
				onExerciseModelChanged: loader.active = true
			}
			Layout.fillWidth: true
			Layout.fillHeight: true
			title: 'test'
			antialiasing: true
			LineSeries{
				name: "Line"
				Loader{
					id: loader
					active: false
					sourceComponent:
					Repeater{
						id: data
						model: stats.computeWeights()
						XYPoint{
							Component.onCompleted: console.log(modelData)
						}
					}
				}
			}
		}
		*/
		/*
		ChartView{
			Layout.fillWidth: true
			Layout.fillHeight: true
			title: 'test'
			antialiasing: true
			LineSeries{
				name: "Line"
				XYPoint { x: 0; y: 0 }
				XYPoint { x: 1.1; y: 2.1 }
				XYPoint { x: 1.9; y: 3.3 }
				XYPoint { x: 2.1; y: 2.1 }
				XYPoint { x: 2.9; y: 4.9 }
				XYPoint { x: 3.4; y: 3.0 }
				XYPoint { x: 4.1; y: 3.3 }
			}
		}*/
	}
}
