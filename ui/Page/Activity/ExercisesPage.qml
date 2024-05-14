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
	ColumnLayout{
		anchors.fill: parent
		RowLayout{
			Button{
				text: 'Back'
				visible: exerciseCreation.currentIndex != 0
				onClicked: {
					exerciseCreation.currentIndex = 0
					while(stackView.depth > 1)
						stackView.pop();
				}
			}
			Text{
				Layout.fillWidth: true
				text: 'All default exercises'
			}
			Item{
				Layout.fillWidth: true
			}
			Button{
				text: 'Reload'
				visible: stackView.depth == 1
				onClicked: exercises.update()
			}
			ComboBox{
				id: exerciseCreation
				Layout.rightMargin: 10
				textRole: "key"

				Component{
					id: exerciseEditorComponent
					ExerciseEditorPage{
						showSaveButton: false
					}
				}
				model: [{key: 'Create'},{key:'Strength', value:3},{key:'Distance', value:1},{key:'Steps', value:2}]

				onActivated: function(index){
					if( index == 0){
						while(stackView.depth > 1)
							stackView.pop();
					}else if(stackView.depth == 1)
						stackView.push(exerciseEditorComponent, {type:model[index].value, showSaveButton:false});
					else
						stackView.replace(exerciseEditorComponent, {type:model[index].value, showSaveButton:false});
				}
				onCurrentIndexChanged: if(currentIndex == 0) exercises.update()
			}
			RoundButton{
				Layout.rightMargin: 10
				visible: exerciseCreation.currentIndex != 0
				text: '+'
				onClicked: {
					if(stackView.currentItem.save()){
						exerciseCreation.currentIndex = 0
						while(stackView.depth > 1)
							stackView.pop();
					}else{
						error.text = 'Some fields are not correctly set'
						error.open()
					}
				}
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
						id: exercises
					}
					spacing: 5
					delegate:ExerciseModelView{
						width: exercisesList.width
						exerciseModel: $modelData
						isDeletable: true
					}
				}
			}
		}
	}

	Dialog {
		id: error
	}

}
