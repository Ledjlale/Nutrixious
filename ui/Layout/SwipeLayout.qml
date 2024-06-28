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
	property bool isDeletable: true
	property bool isEditable: true
	property bool isSavable: false
	property bool isUndoable: false
	property bool isCaloriesComputable: false
	property bool isSpeedComputable: false

	//required property int index
	property bool edit: false
	property bool autoEditFlip: true
	
	property var color: 'white'
	
	signal caloriesComputationClicked()
	signal speedComputationClicked()
	signal deleteClicked()
	signal editClicked()
	signal undoClicked()
	signal saveClicked()

	implicitHeight: contentItem.implicitHeight
	leftInset: 0
	rightInset: 0
	topInset: 0
	bottomInset: 0
	padding: 0
	spacing: 0
	
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
					id: caloriesComputeButton
					Layout.preferredWidth: 25
					Layout.preferredHeight: 25
					Layout.rightMargin: 10
					Layout.alignment: Qt.AlignCenter
					visible: mainItem.isCaloriesComputable
					imageSource: DefaultStyle.caloriesComputationButton
					colorizationColor: mainItem.color
					onClicked: {
						swipe.close()
						mainItem.caloriesComputationClicked()
					}
				}
				
				ButtonImage{
					id: speedComputationButton
					Layout.preferredWidth: 25
					Layout.preferredHeight: 25
					Layout.rightMargin: 10
					Layout.alignment: Qt.AlignCenter
					visible: mainItem.isSpeedComputable
					imageSource: DefaultStyle.speedComputationButton
					colorizationColor: mainItem.color
					onClicked: {
						swipe.close()
						mainItem.speedComputationClicked()
					}
				}
				ButtonImage{
					id: saveButton
					Layout.preferredWidth: 25
					Layout.preferredHeight: 25
					Layout.rightMargin: 10
					Layout.alignment: Qt.AlignCenter
					visible: mainItem.isSavable
					imageSource: DefaultStyle.saveButton
					colorizationColor: mainItem.color
					onClicked: {
						swipe.close()
						mainItem.saveClicked()
					}
				}
				ButtonImage{
					id: undoButton
					Layout.preferredWidth: 25
					Layout.preferredHeight: 25
					Layout.rightMargin: 10
					Layout.alignment: Qt.AlignCenter
					visible: mainItem.isUndoable
					imageSource: DefaultStyle.undoButton
					colorizationColor: mainItem.color
					onClicked: {
						swipe.close()
						mainItem.undoClicked()
					}
				}
				
				ButtonImage{
					id: editButton
					Layout.preferredWidth: 25
					Layout.preferredHeight: 25
					Layout.rightMargin: 10
					visible: mainItem.isEditable
					imageSource: DefaultStyle.editButton
					colorizationColor: mainItem.color
					onClicked: {
						if(autoEditFlip){
							swipe.close()
							mainItem.edit = true
						}
						mainItem.editClicked()
					}
				}
				ButtonImage{
					id: deleteButton
					Layout.preferredWidth: 25
					Layout.preferredHeight: 25
					Layout.rightMargin: 10
					Layout.alignment: Qt.AlignCenter
					visible: mainItem.isDeletable
					imageSource: DefaultStyle.deleteButton
					colorizationColor: mainItem.color
					onClicked: {
						swipe.close()
						mainItem.deleteClicked()
					}
				}
			}
	}
}
