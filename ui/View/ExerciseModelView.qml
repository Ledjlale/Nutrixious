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
							: Material.color(Material.accent, Material.Shade100)

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
					property bool showAddButton: mainItem.showAddButton
					property bool showSaveButton: mainItem.showSaveButton
					property bool showRunning: false
					property bool expandAll: mainItem.expandAll
					property bool isRunning : false
					property bool isReadOnly: mainItem.isReadOnly
					property bool isDeletable: mainItem.isDeletable

					implicitHeight: mainLayout.implicitHeight
					signal addClicked(var exerciseModel)

					onAddClicked: function(exerciseModel) {mainItem.addClicked(exerciseModel) }

					ColumnLayout{
						id: mainLayout
						anchors.fill: parent
						RowLayout{
							spacing: 0
							TextField{
								id: nameTextField
								Layout.preferredWidth: mainItem.width / (3 + fieldList.count)
								showTitle: false
								readOnly: true
								text: exerciseModel.exerciseModel.name
								//onEditingFinished: {
								//		exerciseModel.exerciseModel.name = newValue
								//}
							}
							TextField{
								id: descriptionTextField
								Layout.preferredWidth: mainItem.width / (3 + fieldList.count)
								showTitle: false
								readOnly: isReadOnly
								text: exerciseModel.description ? exerciseModel.description : exerciseModel.exerciseModel.description
								onEditingFinished: {
										exerciseModel.description = newValue
								}
							}
							ListView{
								id: fieldList
								Layout.preferredWidth: mainItem.width / (3 + fieldList.count)
								Layout.preferredHeight: contentHeight
								contentHeight: contentItem.childrenRect.height
								visible: count > 0
								orientation: ListView.Horizontal
								model: workingExerciseModel?.isDone
												? resultModel?.data
												: exerciseModel?.data
								spacing: 0
								delegate: TextField{
									width: fieldList.width / fieldList.count
									height: implicitHeight
									inputMethodHints: Qt.ImhDigitsOnly
									readOnly: exerciseItem.isReadOnly
									title: modelData.name
									text: modelData.value
									onEditingFinished: {
										modelData.value = newValue
									}
								}
							}

							Button{
								Layout.rightMargin: 15
								visible: exerciseItem.exerciseModel.canHaveSeries
								text: seriesList.visible ? '-' : '+'
								onClicked: seriesList.visible = !seriesList.visible
							}
							Button{
								visible: !exerciseItem.isReadOnly && exerciseItem.showSaveButton & exerciseItem.exerciseModel.isEdited //&& (nameTextField.isEdited || descriptionTextField.isEdited)
								text: 'Save'
								onClicked: {
									exerciseModel.save()
								}
							}
							Button{
								Layout.rightMargin: 15
								visible: exerciseItem.showAddButton
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
								visible: exerciseItem.isDeletable
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
							Layout.preferredHeight: implicitHeight
							visible: exerciseItem.expandAll && exerciseItem.exerciseModel.canHaveSeries
							exerciseModel: visible ? exerciseItem.exerciseModel : null
							workingExerciseModel: mainItem.workingExerciseModel
							showSaveButton: exerciseItem.showSaveButton
							isReadOnly: exerciseItem.isReadOnly
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
