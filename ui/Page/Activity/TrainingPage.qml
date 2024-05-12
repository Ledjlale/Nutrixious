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
import '../../Tool/Utils.js' as Utils

Item {
	id: mainItem
	property bool isRunning : false
	property ProgramModel selectedProgramModel
	//onSelectedProgramModelChanged: programExercises.setExercises(selectedProgramModel.exercises)
	onIsRunningChanged: if(isRunning){
			//stackView.push(livePage, {exercises: programExercises.getExercises()});
			stackView.push(livePage, {targetProgramModel: mainItem.selectedProgramModel});
		}else {
			stackView.pop()
		}
	Component{
		id: livePage
		LivePage{
		}
	}
	ColumnLayout{
		anchors.fill: parent
		Layout.fillWidth: true
		Layout.preferredHeight: 60
		RowLayout{
			ComboBox{
				id: programChoice
				Layout.fillWidth: true
				enabled: !mainItem.isRunning
				textRole: "displayText"
				valueRole: "$modelData"
				model: ProgramProxyModel{
					id: programs
				}
				onCurrentValueChanged: mainItem.selectedProgramModel = currentValue
			}
			Button{
				visible: programChoice.currentIndex >= 0
				text: mainItem.isRunning
						? stackView.depth == 2 && stackView.currentItem.trainingModel.trainModel.isSaved
							? 'Back'
							: 'Cancel'
						: 'Begin'
				onClicked: mainItem.isRunning = !mainItem.isRunning
			}
			Button{
				text: 'Reload'
				visible: stackView.depth == 1
				onClicked: programs.update()
			}
		}
		StackView{
			id: stackView
			Layout.fillHeight: true
			Layout.fillWidth: true
			initialItem: Item{
				height: stackView.height
				width: stackView.width
				ListView{
					id: exercisesList
					anchors.fill: parent
					clip: true
					model: ExerciseProxyModel{
						id: programExercises
						exercises: !!mainItem.selectedProgramModel ? mainItem.selectedProgramModel.exercises : []
					}
					spacing: 5
					delegate:ExerciseModelView{
						width: exercisesList.width
						exerciseModel: $modelData

						isReadOnly: true
					}
				}
			}
		}
	}

	Dialog {
		id: error
	}

}
