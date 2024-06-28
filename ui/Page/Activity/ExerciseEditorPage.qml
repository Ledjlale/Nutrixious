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
import '../../Tool/Utils.js' as Utils

Item {
	id: mainItem
	property var exerciseModel
	signal closed()
	
	Connections{
		target: mainWindow.header
		enabled: mainItem.visible
		function onSave(){
			if(mainItem.exerciseModel.save() == 1)
				mainItem.closed()
		}
	}
	Connections{
		target: mainItem.exerciseModel
		function onIsEditedChanged(){
			mainWindow.header.updateHeaders({'showSaveButton':mainItem.exerciseModel.isEdited})
		}
	}
	
	ColumnLayout{
		anchors.fill: parent
		ColumnLayout{
			visible: !!mainItem.exerciseModel
			TextField{
				Layout.fillWidth: true
				Layout.leftMargin: 10
				edit: true
				title: 'Name'
				text: mainItem.exerciseModel?.name || ''
				onEditingFinished: {
					mainItem.exerciseModel.name = newValue
				}
			}
			RowLayout{
				TextField{
					id: metField
					Layout.fillWidth: true
					Layout.leftMargin: 10
					inputMethodHints: Qt.ImhFormattedNumbersOnly
					edit: true
					title: 'MET'
					text: mainItem.exerciseModel?.met.toFixed(2) || ''
					onEditingFinished: {
						mainItem.exerciseModel.met = newValue
						//repsField.forceActiveFocus()
					}
				}
				ComboBox{
					id: metMode
					Layout.fillWidth: true
					Layout.alignment: Qt.AlignBottom
					textRole: "text"
					valueRole: "value"
					model: [{'text':'Default', 'value':0}
						, {'text':'MET=Speed', 'value':1}]
					currentIndex: mainItem.exerciseModel ? mainItem.exerciseModel.metMode : -1
					onCurrentValueChanged:{
						if(	mainItem.exerciseModel){
							mainItem.exerciseModel.metMode = currentValue
						}
					}
				}				
			}
			Button{
				text: 'Compute from trainings'
				onClicked:{
					mainItem.exerciseModel.computeMet()
				}
			}
		}
		Item{
			Layout.fillHeight: true

		}
	}

	Dialog {
		id: error
	}

}
