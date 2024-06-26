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

SwipeDelegate{
	id: mainItem
	property var modelData
	property bool isDeletable: true
	property bool isEditable: true

	//required property int index
	property bool edit: false
	
	signal deleteClicked(var modelData)

	implicitHeight: contentItem.implicitHeight

	swipe.enabled: isDeletable || isEditable

	onClicked: function(mouse) {
		if(mainItem.edit)
			mainItem.edit = false
	}
	swipe.right: Rectangle {
			color: editButton.pressed  || deleteButton.pressed ? Qt.darker(Material.primary, 1.1) : Material.primary

			anchors.right: parent.right
			width: choiceLayout.width + 5
			height: parent.height
			RowLayout{
				id: choiceLayout
				height: parent.height
				anchors.right: parent.right
				ButtonImage{
					id: editButton
					Layout.preferredWidth: 25
					Layout.preferredHeight: 25
					Layout.rightMargin: 10
					visible: mainItem.isEditable
					imageSource: DefaultStyle.editButton
					colorizationColor: Material.foreground
					onClicked: {
						swipe.close()
						mainItem.edit = true
					}
				}
				ButtonImage{
					id: deleteButton
					Layout.preferredWidth: 25
					Layout.preferredHeight: 25
					Layout.rightMargin: 10
					visible: mainItem.isDeletable
					imageSource: DefaultStyle.deleteButton
					colorizationColor: Material.foreground
					onClicked: {
						swipe.close()
						mainItem.deleteClicked(mainItem.modelData)
					}
				}
			}
	}
}
