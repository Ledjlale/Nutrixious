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
	property var selectedProgramModel
	signal back()
	onIsRunningChanged: if(isRunning){
			stackView.push(livePage, {targetProgramModel: mainItem.selectedProgramModel});
		}else {
			stackView.pop()
		}
	property bool isCurrentItem: false
	onIsCurrentItemChanged: if(isCurrentItem) {
		mainWindow.setHeaders({'showBackButton':true, 'showMenuButton':false, 'title':'Training'})
	}
	Component{
		id: livePage
		LivePage{
		}
	}
	Connections{
		target: mainWindow.header
		enabled: mainItem.visible
		function onBack(){
			mainItem.back()
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
				onCurrentValueChanged: {
					mainItem.selectedProgramModel = currentValue
				}
			}
			ButtonImage{
				Layout.alignment: Qt.AlignCenter
				Layout.preferredWidth: 40
				Layout.preferredHeight: 40
				Layout.leftMargin: 5
				Layout.rightMargin: 5
				visible: programChoice.currentIndex >= 0
				imageSource: mainItem.isRunning ? DefaultStyle.landingButton : DefaultStyle.startupButton
				onClicked: {
					forceActiveFocus()
					mainItem.isRunning = !mainItem.isRunning
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
					model: mainItem.selectedProgramModel?.exercises
					spacing: 5
					delegate:ExerciseModelView{
						width: exercisesList.width
						exerciseModel: modelData
						showWorkTime: false
						showCalories: false
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
