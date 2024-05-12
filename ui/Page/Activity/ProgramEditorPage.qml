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
import '../../Tool/Utils.js' as Utils

Item {
	id: mainItem
	property ProgramModel programModel: ProgramModel{}


	Component.onCompleted: Material.background = Qt.darker(Material.background, 1.02)

	function save(){
		return mainItem.programModel.save()
	}
	ColumnLayout{
		anchors.fill: parent
		RowLayout{
			TextField{
				Layout.fillWidth: true
				title: 'Name'
				text: mainItem.programModel.name
				focus: true
				Material.background: mainItem.programModel.invalidName ? Material.color(Material.Pink, Material.Shade50) : mainItem.Material.background
				Material.foreground: mainItem.programModel.invalidName ? Material.accent : mainItem.Material.foreground
				onEditingFinished: {
					mainItem.programModel.name = newValue
					descrtiptionField.forceActiveFocus()
				}
			}
		}
		TextField{
			id: descrtiptionField
			Layout.fillWidth: true
			title: 'Description'
			text: mainItem.programModel.description
			onEditingFinished: {
				mainItem.programModel.description = newValue
			}
		}
		Item{
			Layout.fillWidth: true
			Layout.fillHeight: true
			ListView{
				id: exercisesList
				anchors.fill: parent
				clip: true
				model: ExerciseProxyModel{
					id: exercises
					Component.onCompleted: exercises.update()
				}
				delegate: ExerciseModelView{
					width: exercisesList.width
					exerciseModel: $modelData
					showAddButton: true
					showSaveButton: false
					onAddClicked: mainItem.programModel.addExercise($modelData)
				}
			}
		}
		ListView{
			id: workList
			Layout.fillWidth: true
			Layout.fillHeight: true
			visible: count > 0
			spacing: 5
			clip: true
			model: mainItem.programModel.exercises

			delegate:ExerciseModelView{
				width: workList.width
				exerciseModel: modelData
				showSaveButton: false
				showAddButton: false
			}
		}
	}
}
