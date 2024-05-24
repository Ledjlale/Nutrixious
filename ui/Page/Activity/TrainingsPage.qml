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
				color: Material.foreground
				text: 'All training done'
			}
			Button{
				text: 'Reload'
				visible: stackView.depth == 1
				onClicked: trains.update()
			}
		}
		StackView{
			id: stackView
			Layout.fillWidth: true
			Layout.fillHeight: true
			initialItem: ColumnLayout{
				ListView{
					id: trainList
					Layout.fillWidth: true
					Layout.fillHeight: true
					clip: true
					model: TrainingProxyModel{
						id: trains
					}
					delegate:
						MouseArea{
							width: trainList.width
							height: trainView.implicitHeight
							//anchors.fill: parent
							propagateComposedEvents: true
							preventStealing: true
							onClicked: function(mouse){
								trainDetailsList.program = $modelData
								mouse.accepted = false
							}
							TrainModelView{
								id: trainView
								width: parent.width
								trainModel: $modelData
							}
						}
				}
				Rectangle{
					Layout.fillWidth: true
					Layout.preferredHeight: 1
					color: 'black'
				}
				ListView{
					id: trainDetailsList
					Layout.fillWidth: true
					Layout.fillHeight: true
					visible: count > 0
					clip: true
					property var program
					model: program?.exercises
					delegate: ExerciseModelView{
						width: trainDetailsList.width
						exerciseModel: modelData
						programModel: trainDetailsList.program
						showTitle: index == 0
						showWorkTime: true
					}
				}
			}
		}
	}
}
