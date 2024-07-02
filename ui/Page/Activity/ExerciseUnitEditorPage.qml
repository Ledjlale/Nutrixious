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
	property int editMode: 0	// 0=New, 1=Edit
	property bool showSaveButton: true
	property alias exerciseModel: programExerciseModelId.exerciseModel
	property var programExerciseModel: ProgramExerciseModel {
		id: programExerciseModelId
		Component.onCompleted: updateFromLastProgram()
	}
	property var serieModel: ProgramSerieModel{
		id: serieModelId
	}
	property var programModel
	property var mainProgramModel
	signal closed()
	Component.onCompleted: Material.background = Qt.darker(Material.background, 1.02)

	function save(){
		if(editMode == 0){
			if(mainProgramModel && !mainItem.programModel.isMain){// Add to main group
				mainItem.programExerciseModel = mainItem.mainProgramModel.addNewExercise(mainItem.programExerciseModel)
				mainItem.mainProgramModel.save()
			}
			mainItem.programModel.addNewExercise(mainItem.programExerciseModel)
			return mainItem.programModel.save()
		}else if(editMode == 1){
			return mainItem.programExerciseModel.save()
		}
	}
	Connections{
		target: mainWindow.header
		enabled: mainItem.visible
		function onSave(){
			if(mainItem.save() == 1)
				mainItem.closed()
		}
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
					placeholderText: 'Name'
					title: 'Main name'
					text: exerciseModel.name
					focus: true
					readOnly: exerciseModel.isSaved
					edit: true
					//Material.background: exerciseModel.invalidName ? Material.color(Material.Pink, Material.Shade50) : mainItem.Material.background
					//Material.foreground: exerciseModel.invalidName ? Material.accent : mainItem.Material.foreground
					onEditingFinished: {
						mainItem.exerciseModel.name = newValue
						descrtiptionField.forceActiveFocus()
					}
				}
				TextField{
					id: descrtiptionField
					Layout.fillWidth: true
					Layout.leftMargin: 10
					title: 'Sub description'
					placeholderText: 'Description'
					edit: true
					text: mainItem.programExerciseModel.description
					onEditingFinished: {
						mainItem.programExerciseModel.description = newValue
						metField.forceActiveFocus()
					}
				}
				TextField{
					id: metField
					Layout.fillWidth: true
					Layout.leftMargin: 10
					inputMethodHints: Qt.ImhDigitsOnly
					edit: true
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
		ButtonImage{
			Layout.preferredWidth: 60
			Layout.preferredHeight: 40
			Layout.rightMargin: 10
			Layout.alignment: Qt.AlignRight
			
			imageSource: DefaultStyle.addExerciseButton
			colorizationColor: Material.foreground
			onClicked: {
				forceActiveFocus();
				mainItem.programExerciseModel.addSerie()
			}
		}

		ListView{
			id: serieList
			Layout.fillWidth: true
			Layout.fillHeight: true
			Layout.topMargin: 10
			clip: true
			model: mainItem.programExerciseModel.series
			delegate: Item{
					width: serieList.width
					height: serieView.implicitHeight + 20
					ExerciseSerieModelView{
						id: serieView
						width: parent.width
						height: parent.height
						modelData: model.modelData
						exerciseUnitModel: mainItem.programExerciseModel
						isReadOnly: false
						isDeletable: serieList.count > 1
						showTitle: index == 0
						showCalories: false
						doSave: false
						showSaveButton: mainItem.showSaveButton
						showWorkTime: false
						edit: true
					}
			}
		}
//---------------------------------------------------------------------------------

	}
}
