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
	property var exerciseModel
	property bool showAddButton: false
	property bool showSaveButton: !isReadOnly
	property bool showRunning: false
	property bool isReadOnly: false
	property bool expandAll: false
	property bool isRunning : false

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
		anchors.left: parent.left
		anchors.top: parent.top
		height: mainLine.height
		width: 0
		color: Material.primary
	}
	ColumnLayout{
		id: mainLayout
		anchors.fill: parent

// Main line
		Loader{
			id: mainLine
			Layout.fillWidth: true
			Component{
				id: distanceComponent
				DistanceModelView{
					distanceModel: mainItem.exerciseModel
					showAddButton: mainItem.showAddButton
					showSaveButton: mainItem.showSaveButton
					isReadOnly: mainItem.isReadOnly
					onAddClicked: function(exerciseModel) {mainItem.addClicked(exerciseModel) }
				}
			}
			Component{
				id: stepsComponent
				StepsModelView{
					stepsModel: mainItem.exerciseModel
					showAddButton: mainItem.showAddButton
					showSaveButton: mainItem.showSaveButton
					isReadOnly: mainItem.isReadOnly
					onAddClicked: function(exerciseModel) {mainItem.addClicked(exerciseModel) }
				}
			}
			Component{
				id: strengthComponent
				StrengthModelView{
					strengthModel: mainItem.exerciseModel
					showAddButton: mainItem.showAddButton
					showSaveButton: mainItem.showSaveButton
					isReadOnly: mainItem.isReadOnly
					expandAll: mainItem.expandAll
					onAddClicked: function(exerciseModel) {mainItem.addClicked(exerciseModel) }
				}
			}
			sourceComponent: !!exerciseModel
								? exerciseModel.type == 1 ? distanceComponent
									: exerciseModel.type ==  2 ? stepsComponent
										: exerciseModel.type ==  3 ? strengthComponent
											: undefined
								: undefined
		}
	}
}
