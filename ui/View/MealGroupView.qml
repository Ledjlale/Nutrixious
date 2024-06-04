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

Item{
	id: mainItem
	property var mealGroup
	implicitHeight: mainRow.implicitHeight
	RowLayout{
		id: mainRow
		anchors.fill: parent
		TextField{
			Layout.fillWidth: true
			text: mainItem.mealGroup.name
			placeholderText: 'Group Name'
			keepEditView: text == ''
			onEditingFinished: mainItem.mealGroup.name = newValue
		}
		TextField{
			inputMethodHints: Qt.ImhTime
			text: mainItem.mealGroup.defaultTimeStr
			keepEditView: text == ''
			placeholderText: 'hh:mm:ss'
			onEditingFinished: mainItem.mealGroup.defaultTimeStr = newValue
		}
		Switch{
			checked: mainItem.mealGroup.isDisplayed
			onCheckedChanged: mainItem.mealGroup.isDisplayed = checked
		}
		Button{
			visible: mainItem.mealGroup.isEdited
			text: 'Save'
			onClicked: mainItem.mealGroup.save()
		}
		Button{
			visible: mainItem.mealGroup.isSaved
			text: 'D'
			onClicked: mainItem.mealGroup.remove()
		}
	}
}
