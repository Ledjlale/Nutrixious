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
	property var exerciseModel: ExerciseModel{
		type: 3
	}
	property var serieModel: SerieModel{
		type: 1
	}


	Component.onCompleted: Material.background = Qt.darker(Material.background, 1.02)

	function save(){
		return exerciseModel.save()
	}
	ColumnLayout{
		anchors.fill: parent
		spacing: 0
		RowLayout{
			TextField{
				Layout.fillWidth: true
				title: 'Name'
				text: exerciseModel.name
				focus: true
				Material.background: exerciseModel.invalidName ? Material.color(Material.Pink, Material.Shade50) : mainItem.Material.background
				Material.foreground: exerciseModel.invalidName ? Material.accent : mainItem.Material.foreground
				onEditingFinished: {
					exerciseModel.name = newValue
					descrtiptionField.forceActiveFocus()
				}
			}
		}
		TextField{
			id: descrtiptionField
			Layout.fillWidth: true
			title: 'Description'
			text: exerciseModel.description
			onEditingFinished: {
				exerciseModel.description = newValue
				//repsField.forceActiveFocus()
			}
		}

		RowLayout{/*
			TextField{
				id: repsField
				Layout.fillWidth: true
				inputMethodHints: Qt.ImhDigitsOnly
				title: 'Reps'
				text: serieModel.repetitions
				onEditingFinished: {
					serieModel.repetitions = newValue
					weightField.forceActiveFocus()
				}
			}
			TextField{
				id: weightField
				Layout.fillWidth: true
				inputMethodHints: Qt.ImhDigitsOnly
				title: 'Weight (kg)'
				text: serieModel.weight
				onEditingFinished: {
					serieModel.weight = newValue
					restTimeField.forceActiveFocus()
				}
			}*/
			TextField{
				id: restTimeField
				Layout.fillWidth: true
				inputMethodHints: Qt.ImhDigitsOnly
				title: 'Rest Time (s)'
				text: serieModel.restTime
				onEditingFinished: {
					serieModel.restTime = newValue
				}
			}
			TextField{
				id: workTimeField
				Layout.fillWidth: true
				inputMethodHints: Qt.ImhDigitsOnly
				visible: !!serieModel.workTime
				title: 'Work Time (s)'
				text: visible ? serieModel.workTime : ''
				onEditingFinished: serieModel.workTime = newValue
			}
		}
		ListView{
			id: fieldList
			Layout.fillWidth: true
			Layout.preferredHeight: 80
			orientation: ListView.Horizontal
			model: serieModel.data
			spacing: 0
			delegate: TextField{
				width: fieldList.width / fieldList.count
				height: fieldList.height
				inputMethodHints: Qt.ImhDigitsOnly
				title: modelData.name
				text: modelData.value
				onEditingFinished: {
					modelData.value = newValue
				}
			}
		}
		Button{
			text: 'Add Serie'
			Material.background: exerciseModel.invalidSets ? Material.color(Material.Pink, Material.Shade50) : mainItem.Material.background
			Material.foreground: exerciseModel.invalidSets ? Material.accent : mainItem.Material.foreground

			onClicked: {
				exerciseModel.addSerie(serieModel, false)
			}
		}

		ListView{
			id: serieList
			Layout.fillWidth: true
			Layout.fillHeight: true
			clip: true
			model: mainItem.exerciseModel.series
			delegate:ExerciseSerieModelView{
				serieModel: modelData
				width: serieList.width
				isReadOnly: false
				showTitle: false
				doSave: false
			}


/*
		ListView{
			id: workList
			Layout.fillWidth: true
			Layout.fillHeight: true
			spacing: 5
			clip: true
			model: exerciseModel.sets

			delegate:RowLayout{
				width: workList.width
				Rectangle{
					Layout.fillWidth: true
					Layout.fillHeight: true
					radius: 15
					color: Material.background
					Text{
						anchors.centerIn: parent
						text: modelData.repetitions
					}
				}
				Rectangle{
					Layout.fillWidth: true
					Layout.fillHeight: true
					radius: 15
					color: Material.background
					Text{
						anchors.centerIn: parent
						text: modelData.weight
					}
				}
				Rectangle{
					Layout.fillWidth: true
					Layout.fillHeight: true
					radius: 15
					color: Material.background
					Text{
						anchors.centerIn: parent
						text: modelData.restTime
					}
				}
				Button{
					Layout.alignment: Qt.AlignCenter
					text: 'Copy'
					onClicked: {
						serieModel.repetitions = modelData.repetitions
						serieModel.weight = modelData.weight
						serieModel.restTime = modelData.restTime
					}
				}
				Button{
					Layout.alignment: Qt.AlignCenter
					text: 'X'
					onClicked: {
						exerciseModel.removeSet(modelData)
					}
				}
			}
			*/
		}
	}
}
