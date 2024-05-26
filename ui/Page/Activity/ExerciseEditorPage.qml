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
	property bool showSaveButton: true
	property alias exerciseModel: programExerciseModelId.exerciseModel
	property var programExerciseModel: ProgramExerciseModel {
		id: programExerciseModelId
		Component.onCompleted: addSerie()
	}
	property var serieModel: ProgramSerieModel{
		id: serieModelId
	}
	property var programModel

	Component.onCompleted: Material.background = Qt.darker(Material.background, 1.02)

	function save(){
		mainItem.programModel.addNewExercise(mainItem.programExerciseModel)
		return mainItem.programModel.save()
	}
	ColumnLayout{
		anchors.fill: parent
		spacing: 0
//---------------------------------------------------------------------------------
//							EXERCISE
		RowLayout{
			ColumnLayout{
				Text{
					Layout.fillWidth: true
					horizontalAlignment: Text.AlignHCenter
					text: 'Exercise description'
					color: Material.foreground
				}
				TextField{
					Layout.fillWidth: true
					Layout.leftMargin: 10
					title: 'Name'
					text: exerciseModel.name
					focus: true
					//Material.background: exerciseModel.invalidName ? Material.color(Material.Pink, Material.Shade50) : mainItem.Material.background
					//Material.foreground: exerciseModel.invalidName ? Material.accent : mainItem.Material.foreground
					onEditingFinished: {
						exerciseModel.name = newValue
						descrtiptionField.forceActiveFocus()
					}
				}
				TextField{
					id: descrtiptionField
					Layout.fillWidth: true
					Layout.leftMargin: 10
					title: 'Description'
					text: programExerciseModelId.description
					onEditingFinished: {
						programExerciseModelId.description = newValue
						metField.forceActiveFocus()
					}
				}
				TextField{
					id: metField
					Layout.fillWidth: true
					Layout.leftMargin: 10
					inputMethodHints: Qt.ImhDigitsOnly
					title: 'MET'
					text: exerciseModel.met
					onEditingFinished: {
						exerciseModel.met = newValue
						//repsField.forceActiveFocus()
					}
				}
			}
			SerieFieldPicker{
				//Layout.fillHeight: true
				Layout.preferredHeight: implicitHeight
				Layout.preferredWidth: implicitWidth
				exerciseUnitModel: mainItem.programExerciseModel
			}
		}
		Rectangle{
			Layout.fillWidth: true
			Layout.preferredHeight: 2
			color: 'black'
		}
		RowLayout{
			Layout.leftMargin: 10
			Text{
				Layout.fillWidth: true
				horizontalAlignment: Text.AlignHCenter
				text: 'Definition'
				color: Material.foreground
			}/*
			SerieFieldPicker{

			}*/
		}
/*
		ListView{
			id: exerciseFieldList
			Layout.fillWidth: true
			Layout.leftMargin: 10
			Layout.preferredHeight: !!model ? model.length * 40 : 0

			visible: mainItem.type > 0
			orientation: ListView.Horizontal
			model: ExerciseUnitProxyModel{
				exercises: mainItem.programExerciseModel.exercises
			}
			spacing: 0
			delegate: TextField{
				width: exerciseFieldList.width / exerciseFieldList.count
				height: exerciseFieldList.height
				inputMethodHints: Qt.ImhDigitsOnly
				title: modelData.name
				text: modelData.value
				onEditingFinished: {
					modelData.value = newValue
				}
			}
		}
*/
//---------------------------------------------------------------------------------
//							SERIE
		Button{
			Layout.fillWidth: true
			Layout.preferredWidth: implicitWidth
			Layout.alignment: Qt.AlignCenter
			text: 'Add Serie'
			//Material.background: programExerciseModelId.invalidSets ? Material.color(Material.Pink, Material.Shade50) : mainItem.Material.background
			//Material.foreground: programExerciseModelId.invalidSets ? Material.accent : mainItem.Material.foreground

			onClicked: {
				programExerciseModelId.addSerie()
			}
		}

		ListView{
			id: serieList
			Layout.fillWidth: true
			Layout.fillHeight: true
			Layout.leftMargin: 10
			Layout.rightMargin: 10
			Layout.topMargin: 10
			clip: true
			model: programExerciseModelId.series
			delegate:ExerciseSerieModelView{
				serieModel: modelData
				exerciseUnitModel: mainItem.programExerciseModel
				width: serieList.width
				isReadOnly: false
				showTitle: index == 0
				doSave: false
				showSaveButton: mainItem.showSaveButton
				keepEditView: true
			}
		}
//---------------------------------------------------------------------------------

	}
}
