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
	property var programModel: programDetailsList.program
	signal newExerciseRequested()
	signal addExerciseRequested(var parameters)
	signal editExerciseRequested(var parameters)
	
	onVisibleChanged: if(visible) exercises.update()
	Connections{
		target: mainWindow.header
		enabled: mainItem.visible
		function onCreate(){
			programs.create()
			programList.positionViewAtEnd()
		}
	}
	ColumnLayout{
		anchors.fill: parent
		StackView{
			id: stackView
			Layout.fillWidth: true
			Layout.fillHeight: true
			background:Rectangle{color: Material.background}
			initialItem: ColumnLayout{
				width: stackView.width
				ListView{
					id: programList
					Layout.fillHeight: true
					Layout.fillWidth: true
					clip: true
					model:  ProgramProxyModel{
							id: programs
							onCountChanged:  programDetailsList.program = count > 0 ? programs.getAt(0) : null
							Component.onCompleted: programDetailsList.program = count > 0 ? programs.getAt(0) : null
						}
					delegate:Item{
								width: programList.width
								height: modelView.implicitHeight + 5
								Rectangle{
									anchors.fill: parent
									color: Material.background
								}
								TrainModelView{
									id: modelView
									anchors.fill: parent
									implicitWidth: programList.width
									modelData: $modelData
									onClicked: {
										programDetailsList.program = $modelData
										exercises.update()
									}
								}
					}
				}
				Rectangle{
					Layout.fillWidth: true
					Layout.preferredHeight: 1
					color: DefaultStyle.theme == Material.Dark ? 'white' : 'black'
				}
				Text{
					Layout.fillWidth: true
					horizontalAlignment: Text.AlignHCenter
					visible: !!programDetailsList.program
					text: visible ? 'Exercises for ' + programDetailsList.program.name : ''
					color: Material.foreground
					font.bold: true
					font.pixelSize: 17
				}
				RowLayout{
					visible: !!programDetailsList.program
					ComboBox{
						id: exerciseChoice
						Layout.fillWidth: true
						textRole: "displayText"
						valueRole: "$modelData"
						model: ExerciseProxyModel{
							id: exercises
							Component.onCompleted: update()
						}
					}
					ButtonImage{
						Layout.preferredWidth: 25
						Layout.preferredHeight: 25
						Layout.rightMargin: 5
						visible: !!exerciseChoice.currentValue
						imageSource: DefaultStyle.editButton
						colorizationColor: Material.foreground
						onClicked: mainItem.editExerciseRequested({exerciseModel:exerciseChoice.currentValue})
					}
					ButtonImage{
						Layout.preferredWidth: 40
						Layout.preferredHeight: 40
						Layout.rightMargin: 5
						visible: !!exerciseChoice.currentValue
						imageSource: DefaultStyle.addExerciseButton
						colorizationColor: Material.foreground
						onClicked: mainItem.addExerciseRequested({exerciseModel:exerciseChoice.currentValue})
					}
					ButtonImage{
						Layout.preferredWidth: 25
						Layout.preferredHeight: 25
						imageSource: DefaultStyle.createButton
						colorizationColor: Material.foreground
						onClicked: mainItem.newExerciseRequested()
					}
				}
				ListView{
					id: programDetailsList
					Layout.fillWidth: true
					Layout.fillHeight: true
					visible: count > 0
					clip: true
					property var program
					model: program?.exercises
					delegate:ExerciseModelView{
						width: programDetailsList.width
						exerciseModel: modelData
						programModel: programDetailsList.program
						isDeletable: true
						showWorkTime: false
						showCalories: false
						showEditButton: true
						onEditClicked: function(model){
							mainItem.addExerciseRequested( {programExerciseModel:model, exerciseModel:model.exerciseModel, editMode:1})
						}
					}
				}
		}
	}
	}
}
