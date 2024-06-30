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

Item {
	id: mainItem
	PersonalDataModel{
		id: lastDataModel
		Component.onCompleted: lastDataModel.loadLast()
	}
	ColumnLayout{
		anchors.fill: parent
		RowLayout {
			Layout.fillWidth: true
			TextField{
				Layout.fillWidth: true
				flipped: true
				title: 'Weight'
				text: lastDataModel.weight
				onEditingFinished:lastDataModel.weight = newValue
			}
			TextField{
				Layout.fillWidth: true
				flipped: true
				title: 'Height'
				text: lastDataModel.height
				onEditingFinished:lastDataModel.height = newValue
			}
			SexBox{
				Layout.fillWidth: true
				flipped: true
				title: 'Sex'
				value: lastDataModel.sex
				onAccepted:lastDataModel.sex = newValue
			}
		}
		ButtonImage{
			Layout.preferredWidth: 80
			Layout.preferredHeight: 30
			Layout.alignment: Qt.AlignCenter
			Layout.topMargin: 5
			imageSource: DefaultStyle.addFoodButton
			colorizationColor: Material.foreground
			onClicked: {
				if(lastDataModel.add())allModels.update()
			}
		}
		ListView{
			id: personalDataList
			Layout.fillHeight: true
			Layout.fillWidth: true
			spacing: 0
			model: PersonalDataProxyModel{
				id: allModels
				Component.onCompleted: update()
			}
			delegate:SwipeLayout{
				id: dataRow
				width: personalDataList.width
				isDeletable: true
				isUndoable: $modelData.isEdited
				isEditable: !$modelData.isEdited
				onDeleteClicked: $modelData.remove()
				onUndoClicked: {
					$modelData.undo()
					edit = false
				}
				
				contentItem: RowLayout{
					width: parent.width
					height: implicitHeight + 10
					TextField{
						Layout.fillWidth: true
						inputMethodHints: Qt.ImhDigitsOnly
						edit: dataRow.edit
						showTitle: false
						text: $modelData.dateTimeStr
						readOnly: false
						onEditingFinished: $modelData.dateTimeStr = newValue
					}
					TextField{
						Layout.preferredWidth: 80
						title: 'Weight'
						showTitle: index == 0
						edit: dataRow.edit
						text: $modelData.weight
						onEditingFinished:$modelData.weight = newValue
					}
					TextField{
						Layout.preferredWidth: 80
						title: 'Height'
						showTitle: index == 0
						edit: dataRow.edit
						text: $modelData.height
						onEditingFinished:$modelData.height = newValue
					}
					SexBox{
						Layout.preferredWidth: 50
						showTitle: index == 0
						edit: dataRow.edit
						value: $modelData.sex
						onAccepted:$modelData.sex = newValue
					}
					ButtonImage{
						Layout.alignment: Qt.AlignCenter
						Layout.preferredWidth: 25
						Layout.preferredHeight: 25
						Layout.rightMargin: 5
						visible: $modelData.isEdited
						imageSource: DefaultStyle.saveButton
						colorizationColor: Material.foreground
						onClicked: {
							forceActiveFocus()
							$modelData.save()
							dataRow.edit = false
						}
					}
				}
			}
		}
	}
}
