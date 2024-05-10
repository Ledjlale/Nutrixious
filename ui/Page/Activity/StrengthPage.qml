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
	property StrengthModel exerciseModel: StrengthModel{}
	property StrengthWorkModel workModel: StrengthWorkModel{}


	Component.onCompleted: Material.background = Qt.darker(Material.background, 1.02)

	function save(){
		return exerciseModel.save()
	}
	ColumnLayout{
		anchors.fill: parent
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
				repsField.forceActiveFocus()
			}
		}

		RowLayout{
			TextField{
				id: repsField
				Layout.fillWidth: true
				inputMethodHints: Qt.ImhDigitsOnly
				title: 'Reps'
				text: workModel.repetitions
				onEditingFinished: {
					workModel.repetitions = newValue
					weightField.forceActiveFocus()
				}
			}
			TextField{
				id: weightField
				Layout.fillWidth: true
				inputMethodHints: Qt.ImhDigitsOnly
				title: 'Weight (kg)'
				text: workModel.weight
				onEditingFinished: {
					workModel.weight = newValue
					restTimeField.forceActiveFocus()
				}
			}
			TextField{
				id: restTimeField
				Layout.fillWidth: true
				inputMethodHints: Qt.ImhDigitsOnly
				title: 'Rest Time (s)'
				text: workModel.restTime
				onEditingFinished: {
					workModel.restTime = newValue
				}
			}
		}
		Button{
			text: 'Add'
			Material.background: exerciseModel.invalidSets ? Material.color(Material.Pink, Material.Shade50) : mainItem.Material.background
			Material.foreground: exerciseModel.invalidSets ? Material.accent : mainItem.Material.foreground

			onClicked: {
				exerciseModel.addSet(workModel)
			}
		}
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
						workModel.repetitions = modelData.repetitions
						workModel.weight = modelData.weight
						workModel.restTime = modelData.restTime
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
		}
	}
}
