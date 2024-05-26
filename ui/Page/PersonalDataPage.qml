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
			TextField{
				Layout.fillWidth: true
				flipped: true
				title: 'Sex'
				text: lastDataModel.sex
				onEditingFinished:lastDataModel.sex = newValue
			}
		}
		Button{
			Layout.alignment: Qt.AlignCenter
			Layout.preferredWidth: 60
			text: 'Add'
			onClicked:{
				if(lastDataModel.add())allModels.update()
			}
		}
		ListView{
			id: personalDataList
			Layout.fillHeight: true
			Layout.fillWidth: true
			model: PersonalDataProxyModel{
				id: allModels
				Component.onCompleted: update()
			}
			delegate:RowLayout{
				width: personalDataList.width
				TextField{
					Layout.fillWidth: true
					inputMethodHints: Qt.ImhDigitsOnly
					text: $modelData.dateTimeStr
					readOnly: false
					onEditingFinished: $modelData.dateTimeStr = newValue
				}
				TextField{
					title: 'Weight'
					showTitle: index == 0
					text: $modelData.weight
					onEditingFinished:$modelData.weight = newValue
				}
				TextField{
					title: 'Height'
					showTitle: index == 0
					text: $modelData.height
					onEditingFinished:$modelData.height = newValue
				}
				TextField{
					title: 'Sex'
					showTitle: index == 0
					text: $modelData.sex
					onEditingFinished:$modelData.sex = newValue
				}
				Button{

					visible: $modelData.isEdited
					text: 'S'
					onClicked: $modelData.save()
				}
				Button{
					text: 'D'
					onClicked: $modelData.remove()
				}
			}
		}
	}
}
