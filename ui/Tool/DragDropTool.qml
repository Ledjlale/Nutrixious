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

MouseArea {
	id: dragArea
	property bool held: false
	//height: content.height

	drag.target: held ? target : undefined
	drag.axis: Drag.YAxis

	onPressAndHold: held = true
	onReleased: {
		held = false
		//target.x = oldX
		//target.y = oldY
	}

	property var target
	onTargetChanged: {
		target.Drag.source = dragArea
		//target.Drag.hotSpot.x =  width / 2
		//target.Drag.hotSpot.y =  height / 2
	}
	property int oldX
	property int oldY
	onHeldChanged: {
	/*
		if(held){
			oldX = target.x
			oldY = target.y
		}else{
			target.x = oldX
			target.y = oldY
		}*/
		target.Drag.active = held
	}


/*
	Rectangle {
		id: content
		Drag.active: dragArea.held
		Drag.source: dragArea
		Drag.hotSpot.x: width / 2
		Drag.hotSpot.y: height / 2
		color: dragArea.held ? Material.color(Material.Red) : Material.background
		height: dragArea.height
		width: dragArea.width
		enabled: !dragArea.held

		//MouseArea{
			//anchors.fill: parent
			//onClicked: programExercises.setExercises($modelData.exercises)
		//}
	}
	*/
	Rectangle{
		anchors.fill: parent
		radius: 15
		color: dragArea.held ? Material.color(Material.Red) : Material.primary
	}

	/*
	DropArea {
		anchors.fill: parent
		onDropped: dragArea.DelegateModel.move(drag.source.DelegateModel.itemsIndex,
					dragArea.DelegateModel.itemsIndex, 1);
		onEntered: (drag) => {
			console.log("move")
			visualModel.items.move(
					drag.source.DelegateModel.itemsIndex,
					dragArea.DelegateModel.itemsIndex)
		}
	}
	*/
}
