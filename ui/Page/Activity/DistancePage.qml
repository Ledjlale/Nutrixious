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
	property DistanceModel exerciseModel: DistanceModel{}
	function save(){
		return exerciseModel.save()
	}
	ColumnLayout{
		anchors.fill: parent
		RowLayout{
			TextField{
				Layout.fillWidth: true
				title: 'Name'
				text: exerciseModel.name
				focus: true
				Material.background: exerciseModel.invalidName ? Material.color(Material.Pink, Material.Shade50) : mainItem.Material.background
				Material.foreground: exerciseModel.invalidName ? Material.accent : mainItem.Material.foreground
				onEditingFinished: {
					exerciseModel.name = newValue
					descrtiptionField.forceActiveFocus()
				}
			}
		}
		TextField{
			id: descrtiptionField
			Layout.fillWidth: true
			title: 'Description'
			text: exerciseModel.description
			onEditingFinished: {
				exerciseModel.description = newValue
				distanceField.forceActiveFocus()
			}
		}
		RowLayout{
			TextField{
				id: distanceField
				Layout.fillWidth: true
				inputMethodHints: Qt.ImhDigitsOnly
				title: 'Distance'
				text: exerciseModel.distance
				onEditingFinished: {
					exerciseModel.distance = newValue
					restTimeField.forceActiveFocus()
				}
			}
			TextField{
				id: restTimeField
				Layout.fillWidth: true
				inputMethodHints: Qt.ImhDigitsOnly
				title: 'Rest Time'
				text: exerciseModel.restTime
				onEditingFinished: exerciseModel.restTime = newValue
			}
		}

		Item{
			Layout.fillHeight: true
			Layout.fillWidth: true
		}
	}
}
