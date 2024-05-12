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
import QtQuick.Controls
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
			Text{
				Layout.fillWidth: true
				text: 'All default programs'
			}
			Component{
				id: programEditorComponent
				ProgramEditorPage{}
			}
			Button{
				text: stackView.depth == 1 ? 'New' : 'Create'
				onClicked: if(stackView.depth == 1)
						stackView.push(programEditorComponent);
					else {
						if(stackView.currentItem.save()){
							while(stackView.depth > 1)
								stackView.pop();
							programs.update()
						}
						//stackView.replace(programEditorComponent);
					}
			}
			Button{
				text: 'Reload'
				visible: stackView.depth == 1
				onClicked: programs.update()
			}
		}
		StackView{
			id: stackView
			Layout.fillWidth: true
			Layout.fillHeight: true



			initialItem: ColumnLayout{
				ListView{
					id: programList
					Layout.fillWidth: true
					Layout.fillHeight: true
					clip: true
					model:  ProgramProxyModel{
							id: programs
						}
					delegate:Rectangle{
						width: programList.width
						height: 40
						RowLayout{
							Item{
								Layout.fillWidth: true
							}
							Text{
								text: $modelData.name
							}
							Text{
								text: ' | '
							}
							Text{
								text: $modelData.description
							}
							Item{
								Layout.fillWidth: true
							}
						}
						MouseArea{
							anchors.fill: parent
							onClicked: {
								programExercises.program = $modelData
								//programExercises.setExercises($modelData.exercises)
							}
						}
					}
				}
				Rectangle{
					Layout.fillWidth: true
					Layout.preferredHeight: 1
					color: 'black'
				}
				ListView{
					id: programDetailsList
					Layout.fillWidth: true
					Layout.fillHeight: true
					visible: count > 0
					clip: true
					model: ExerciseProxyModel{
						id: programExercises
						property var program
						exercises: !!program ? program.exercises : []
					}
					delegate:ExerciseModelView{
						width: programDetailsList.width
						exerciseModel: $modelData
						programModel: programExercises.program
					}
				}



/*
				DelegateModel {
					id: visualModel
					model: ExerciseProxyModel{
						id: programExercises
						property var program
					}
					delegate: DragDropTool{
								id: dragRopTool
								width: programDetailsList.width
								height: exerciseModelView.implicitHeight
								ExerciseModelView{
									id: exerciseModelView
									width: programDetailsList.width
									exerciseModel: $modelData
								}
								onHeldChanged: {
									if(!held){
										programExercises.program.updateProgramOrder()
										programDetailsList.interactive = true
									}else{
										programDetailsList.interactive = false
									}
								}
							}
				}
*/
/*
				DelegateModel {
					id: visualModel
					model: ExerciseProxyModel{
						id: programExercises
						property var program
					}
					delegate: RowLayout{
								width: programDetailsList.width
								height: exerciseModelView.implicitHeight
								ExerciseModelView{
									id: exerciseModelView
									Layout.fillWidth: true
									exerciseModel: $modelData
									DropArea {
										anchors.fill: parent
										onDropped: dragRopTool.DelegateModel.move(drag.source.DelegateModel.itemsIndex,
													dragRopTool.DelegateModel.itemsIndex, 1);
										onEntered: (drag) => {
											console.log("move")
											visualModel.items.move(
													drag.source.DelegateModel.itemsIndex,
													dragRopTool.DelegateModel.itemsIndex)
										}
									}
								}
								DragDropTool{
									id: dragRopTool
									Layout.preferredWidth: 40
									Layout.fillHeight: true
									target: parent
								}

							}
				}

				ListView{
					id: programDetailsList
					Layout.fillWidth: true
					Layout.fillHeight: true

					visible: count > 0
					clip: true
					model:visualModel
				}
				*/
			}
		}
	}
}
