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

import QtQuick.Controls.Material as Control
import QtQuick.Layouts

import App 1.0

Item{
	property var exerciseUnitModel
	property alias distanceEnabled: distanceSwitch.checked
	property alias speedEnabled: speedSwitch.checked
	property alias weightEnabled: weightSwitch.checked
	property alias repetitionsEnabled: repetitionsSwitch.checked
	implicitHeight: fieldsColumn.implicitHeight
	implicitWidth: fieldsColumn.implicitWidth
	ColumnLayout{
		id: fieldsColumn
		anchors.fill: parent
		Control.Switch{
			id: distanceSwitch
			text: 'Distance'
			checked: exerciseUnitModel?.useDistance
			onCheckedChanged: exerciseUnitModel.useDistance = checked
		}
		Control.Switch{
			id: speedSwitch
			text: 'Speed'
			checked: exerciseUnitModel?.useSpeed
			onCheckedChanged: exerciseUnitModel.useSpeed = checked
		}
		Control.Switch{
			id: weightSwitch
			text: 'Weight'
			checked: exerciseUnitModel?.useWeight
			onCheckedChanged: exerciseUnitModel.useWeight = checked
		}
		Control.Switch{
			id: repetitionsSwitch
			text: 'Repetitions'
			checked: exerciseUnitModel?.useRepetitions
			onCheckedChanged: exerciseUnitModel.useRepetitions = checked
		}
	}
}
