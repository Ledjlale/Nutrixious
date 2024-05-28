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
	property bool showSaveButton: !isReadOnly
	property bool showRunning: false
	property bool showTitle: false
	property bool showWorkTime: true
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

					ColumnLayout{
						id: mainLayout
						anchors.fill: parent
						RowLayout{
							id: rowlayout
							spacing: 0
							TextField{
								id: nameTextField
								Layout.preferredWidth: mainItem.width / rowlayout.visibleChildren.length
								showTitle: false
								readOnly: true
								text: !!exerciseModel.exerciseModel ? exerciseModel.exerciseModel.name : ''
								ComboBox{
									id: exerciseChoice
									Layout.fillWidth: true
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
								}

							}/*
							TextField{
								id: descriptionTextField
								Layout.preferredWidth: visible ? mainItem.width / rowlayout.itemCount : 0
								visible: !readOnly || text != ''
								showTitle: false
								readOnly: isReadOnly
								text: exerciseModel.description ? exerciseModel.description : exerciseModel.exerciseModel.description
								onEditingFinished: {
										exerciseModel.description = newValue
								}
							}*/

							Button{
								Layout.fillWidth: true
								Layout.preferredWidth: implicitWidth
								Layout.maximumWidth: implicitWidth
								text: seriesList.visible ? '-' : '+'
								onClicked: seriesList.visible = !seriesList.visible
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
									exerciseItem.saveValues()
									exerciseItem.addClicked(exerciseModel)
								}
							}
							/*
							Rectangle{
								Layout.alignment: Qt.AlignCenter
								Layout.preferredHeight: 30
								Layout.preferredWidth: 30
								visible: !!mainItem.programModel
								color: Material.primary
								radius: width / 2
								Text{
									anchors.centerIn: parent
									color: 'white'
									text: '+'
								}
								MouseArea{
									anchors.fill: parent
									onClicked:{
										mainItem.programModel.decrementExerciseOrder(mainItem.exerciseModel)
									}
								}
							}
							Rectangle{
								Layout.alignment: Qt.AlignCenter
								Layout.preferredHeight: 30
								Layout.preferredWidth: 30
								visible: !!mainItem.programModel
								color: Material.primary
								radius: width / 2
								Text{
									anchors.centerIn: parent
									color: 'white'
									text: '-'
								}
								MouseArea{
									anchors.fill: parent
									onClicked:{
										mainItem.programModel.incrementExerciseOrder(mainItem.exerciseModel)
									}
								}
							}*/
							Button{
								visible: mainItem.isDeletable
								text: 'D'
								onClicked: {
									console.log('Try to Delete : ' +exerciseItem.exerciseModel + " in " +exerciseItem.programModel + ' at ' +exerciseItem.exerciseModel.order)
									exerciseItem.exerciseModel.remove()
								}
							}
						}
						ExerciseSerieModelListView{
							id: seriesList
							Layout.fillWidth: true
							Layout.rightMargin: 5
							Layout.preferredHeight: implicitHeight
							visible: mainItem.expandAll
							showWorkTime: mainItem.showWorkTime
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
