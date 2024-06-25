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

	ColumnLayout{
		anchors.fill: parent
		RowLayout{
			Button{
				text: 'Back'
				visible: stackView.depth > 1
				onClicked: {
					while(stackView.depth > 1)
						stackView.pop();
				}
			}
			Item{
				Layout.fillWidth: true
			}
			Component{
				id: programEditorComponent
				ProgramEditorPage{}
			}
			Button{
				Layout.fillWidth: true
				Layout.preferredWidth: implicitWidth
				text: stackView.depth == 1 ? 'New Program'
											:  stackView.currentItem.editMode == 0
												? 'Create'
												: 'Save'
				onClicked: if(stackView.depth == 1) {
						programs.create();
					}else {
						if(stackView.currentItem.save()){
							while(stackView.depth > 1)
								stackView.pop();
							exercises.update()
						}
						//stackView.replace(programEditorComponent);
					}
			}
			Button{
				Layout.fillWidth: true
				Layout.preferredWidth: implicitWidth
				text: 'Reload'
				visible: stackView.depth == 1
				onClicked: {
					programs.update()
					exercises.update()
				}
			}
		}
		StackView{
			id: stackView
			Layout.fillWidth: true
			Layout.fillHeight: true

			initialItem: ColumnLayout{
				width: stackView.width
				ListView{
					id: programList
					Layout.fillWidth: true
					Layout.fillHeight: true
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
					color: 'black'
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
						}
					}
					Button{
						Layout.fillWidth: true
						Layout.preferredWidth: implicitWidth
						visible: !!exerciseChoice.currentValue
						text: 'Add'
						onClicked: {
							if(stackView.depth == 1) {
								stackView.push(exerciseEditorComponent,{exerciseModel:exerciseChoice.currentValue});
							}
						}
					}

					Button{
						Layout.fillWidth: true
						Layout.preferredWidth: implicitWidth
						Component{
							id: exerciseEditorComponent
							ExerciseEditorPage{
								programModel: programDetailsList.program
								showSaveButton: false
							}
						}
						text: 'New Exercise'
						visible: stackView.depth == 1
						onClicked: if(stackView.depth == 1)
										stackView.push(exerciseEditorComponent);
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
							stackView.push(exerciseEditorComponent, {programExerciseModel:model, exerciseModel:model.exerciseModel, editMode:1});
						}
					}
				}
		}
	}
	}
}
