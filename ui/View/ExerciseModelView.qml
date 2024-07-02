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

Item{
	id: mainItem
	property var workingExerciseModel
	property var exerciseModel: workingExerciseModel?.targetExerciseModel
	property var resultModel: workingExerciseModel?.resultExerciseModel
	property var programModel
	property bool showAddButton: false
	property bool showEditButton: false
	property bool showSaveButton: !isReadOnly
	property bool showRunning: false
	property bool showTitle: false
	property bool showWorkTime: true
	property bool showRestTime: true
	property bool showCalories: true
	property bool showOrderChange: false
	property bool isReadOnly: false
	property bool expandAll: false
	property bool isRunning : false
	property bool isDeletable: false
	property bool doSave: true

	implicitHeight: mainLayout.implicitHeight
	height: implicitHeight

	signal addClicked(var exerciseModel)
	signal editClicked(var exerciseModel)
	Timer{
		id: workTimer
		property int count: 0
		property int maxCount: 1
		interval: 1000
		repeat: true
		onTriggered:{
			if(++count == maxCount)
			mainLineBackground.width = mainLine.width * count / maxCount

		}
	}
	Rectangle{
		id: mainLineBackground
		anchors.fill: parent
		//anchors.left: parent.left
		//anchors.top: parent.top
		//height: mainLine.height
		//width: 0
		visible: workingExerciseModel?.isRunning || workingExerciseModel?.isDone || false
		color: workingExerciseModel?.isResting
					? Material.color(Material.Yellow, Material.Shade100)
					: resultModel?.isSaved
						? Material.color(Material.Green, Material.Shade100)
						: workingExerciseModel?.isDone
							? Material.color(Material.Blue, Material.Shade100)
							: Material.color(Material.Amber, Material.Shade100)
	}
	ColumnLayout{
		id: mainLayout
		anchors.fill: parent

// Main line
		Loader{
			id: mainLine
			Layout.fillWidth: true
			Component{
				id: exerciseComponent
				Item{
					id: exerciseItem
					property var exerciseModel: mainItem.exerciseModel
					property var programModel: mainItem.programModel
	
					implicitHeight: mainLayout.implicitHeight
					signal addClicked(var exerciseModel)
					onAddClicked: function(exerciseModel) {mainItem.addClicked(exerciseModel) }
					signal editClicked(var exerciseModel)
					onEditClicked: function(exerciseModel) {mainItem.editClicked(exerciseModel) }

					ColumnLayout{
						id: mainLayout
						anchors.fill: parent
						SwipeLayout{
							id: mainSwipeLayout
							Layout.fillWidth: true
							isDeletable: mainItem.isDeletable
							onClicked: seriesList.visible = !seriesList.visible
							onDeleteClicked: {
								console.log('Try to Delete : ' +exerciseItem.exerciseModel + " in " +exerciseItem.programModel + ' at ' +exerciseItem.exerciseModel.order)
								exerciseItem.exerciseModel.remove()
							}
							onEditClicked: {
								exerciseItem.editClicked(exerciseModel)
							}
							contentItem:RowLayout{
								id: rowlayout
								width: mainSwipeLayout.width
								spacing: 0
								TextField{
									id: nameTextField
									property bool haveName: !!exerciseModel.exerciseModel && exerciseModel.exerciseModel.name != ''
									Layout.fillWidth: true
									Layout.topMargin: 5
									//Layout.preferredWidth: mainItem.width / rowlayout.visibleChildren.length
									Layout.preferredWidth: Math.max(implicitWidth, exerciseChoice.visible ? exerciseChoice.implicitWidth : 0)
									showTitle: false
									readOnly: true
									placeholderText: 'Name'
									text: !!exerciseModel.exerciseModel
										? exerciseModel.exerciseModel.name
											? exerciseModel.exerciseModel.name
											: 'Unnamed'
										: ''
									font.weight: Font.Bold
									font.bold: true
									font.pixelSize: 20
	
									ComboBox{
										id: exerciseChoice
										anchors.fill: parent
										visible: !exerciseModel.exerciseModel
										textRole: "displayText"
										valueRole: "$modelData"
										displayText: currentIndex === -1 ? "Link" : currentText
										model: ExerciseProxyModel{
											id: exercises
										}
										onActivated:{
											exerciseModel.exerciseModel = currentValue
											exerciseModel.save()
										}
										onVisibleChanged: if(visible) exercises.update()
										Component.onCompleted: if(visible) exercises.update()
									}
	
								}
								TextField{
									id: descriptionTextField
									Layout.fillWidth: true
									Layout.topMargin: 5
									//Layout.preferredWidth: mainItem.width / rowlayout.visibleChildren.length
									visible: !readOnly || text != ''// && !!mainItem.workingExerciseModel
									showTitle: false
									readOnly: mainItem.isReadOnly
									placeholderText: 'Description'
									horizontalAlignment: Text.AlignRight
									text: mainItem.resultModel ? mainItem.resultModel.description
															: mainItem.exerciseModel ? mainItem.exerciseModel.description : ''
									edit: !readOnly && text == ''
									onEditingFinished: {
										if( mainItem.resultModel)
											resultModel.description = newValue
										else if(mainItem.exerciseModel)
											mainItem.exerciseModel.description = newValue
									}
								}
	
								Button{
									Layout.fillWidth: true
									Layout.preferredWidth: implicitWidth
									visible: !mainItem.isReadOnly && mainItem.showSaveButton && ( workingExerciseModel && mainItem.resultModel.isEdited && mainItem.resultModel.isSaved
										|| !workingExerciseModel && exerciseItem.exerciseModel.isEdited)
									text: 'Save'
									onClicked: {
										if( workingExerciseModel && workingExerciseModel.isDone)
											resultModel.save()
										else
											exerciseModel.save()
									}
								}
								Button{
									Layout.fillWidth: true
									Layout.preferredWidth: implicitWidth
									//Layout.preferredWidth: 40
									visible: mainItem.showAddButton
									text: 'Add'
									onClicked: {
										exerciseItem.addClicked(exerciseModel)
									}
								}
							}
						}
						ExerciseSerieModelListView{
							id: seriesList
							Layout.fillWidth: true
							Layout.preferredHeight: implicitHeight
							visible: mainItem.expandAll
							showWorkTime: mainItem.showWorkTime
							showRestTime: mainItem.showRestTime
							showCalories: mainItem.showCalories
							showOrderChange: mainItem.showOrderChange
							exerciseModel: visible ? exerciseItem.exerciseModel : null
							workingExerciseModel: mainItem.workingExerciseModel
							showSaveButton: mainItem.showSaveButton
							isReadOnly: mainItem.isReadOnly
							
						}
					}
				}
			}
			sourceComponent: !!exerciseModel
								? exerciseComponent
								: undefined
		}
	}
}
