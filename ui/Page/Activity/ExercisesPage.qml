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
	ColumnLayout{
		anchors.fill: parent
		RowLayout{
			ComboBox{
				id: exerciseChoice
				Layout.fillWidth: true
				textRole: "displayText"
				valueRole: "$modelData"
				model: ExerciseProxyModel{
					id: exercises
				}
				Component.onCompleted: {
					exercises.update()
					currentIndex = 0
				}
				onCurrentValueChanged:{
					//chartView.exerciseType = currentValue.type
					mainItem.exerciseModel = currentValue
				}
			}
			Button{
				Layout.fillWidth: true
				Layout.preferredWidth: implicitWidth
				Layout.minimumWidth: implicitWidth
				Layout.maximumWidth: implicitWidth
				visible: !!exerciseChoice.currentValue
				text: 'D'
				onClicked: {
					exerciseChoice.currentValue.remove()

				}
			}
			Button{
				text: 'Reload'
				//visible: stackView.depth == 1
				onClicked: exercises.update()
			}
		}
		ColumnLayout{
			visible: !!mainItem.exerciseModel
			Text{
				Layout.fillWidth: true
				horizontalAlignment: Text.AlignHCenter
				text: 'Exercise description'
				color: Material.foreground
			}
			TextField{
				Layout.fillWidth: true
				Layout.leftMargin: 10
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
					title: 'MET'
					text: mainItem.exerciseModel?.met.toFixed(2) || ''
					onEditingFinished: {
						mainItem.exerciseModel.met = newValue
						//repsField.forceActiveFocus()
					}
				}
				Button{
					text: 'Compute from trainings'
					onClicked:{
						mainItem.exerciseModel.computeMet()
					}
				}
			}
			Button{
				visible: mainItem.exerciseModel?.isEdited || false
				text: 'Save'
				onClicked:{
					mainItem.exerciseModel.save()
					exercises.update()
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
