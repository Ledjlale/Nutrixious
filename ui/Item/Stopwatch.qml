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
import '../Tool/Utils.js' as Utils

Item{
	id: mainItem
	property color foregroundColor: 'white'// Material.color(Material.Grey)
	function start(){
		timer.restart()
	}
	function stop(){
		timer.stop()
	}
	function pause(){
		timer.stop()
	}
	function step(){
		//steps.model.slice().unshift({count:timer.count, total:timer.total})
		steps.insert(0, {count:timer.count, total:timer.total})

		timer.count = 0
	}
	function reset(){
		stop()
		timer.count = 0
		timer.total = 0
		steps.clear()
	}


	function formatDuration (ms) {
		//ms = parseInt(ms, 10)
		//s,	m,	h,		d,		W,		M,			Y
		//1,	60,	3600,	86400,	604800,	2592000,	31104000
		var y = Math.floor(ms / 31104000000)
		var M = Math.floor(ms / 2592000000)
		var w = Math.floor(ms / 604800000)
		var d = Math.floor(ms / 86400000)
		var h = Math.floor(ms / 3600000)
		var m = Math.floor((ms - h * 3600000) / 60000)
		var s = Math.floor((ms - h * 3600000 - m * 60000) / 1000)
		ms = ms - h * 3600000 - m * 60000 - s * 1000
		return {y:y,M:M,w:w,d:d,h:h,m:m,s:s,ms:ms}
	}

	Timer{
		id: timer
		property int count: 0
		property int total: 0
		interval: 10
		repeat: true
		onTriggered: {
			timer.count += interval
			timer.total += interval
		}
	}
	Rectangle{
		anchors.top: parent.top
		anchors.left: parent.left
		anchors.bottom: parent.bottom
		width: backgroundArea.radius
		color: backgroundArea.color
	}
	Rectangle{
		id: backgroundArea
		anchors.fill: parent
		radius: 15
		color: Material.color(Material.Purple)
		opacity: 0.9
	}
	RowLayout{
		anchors.fill: parent
		ColumnLayout{
			Layout.fillWidth: true
			RowLayout{
				spacing: 30
			Layout.fillWidth: true
				TimeDisplay{
					Layout.leftMargin: 30
					Layout.preferredWidth: 60
					ms: timer.count
					foregroundColor: mainItem.foregroundColor
				}
				Item{
			Layout.fillWidth: true}
				TimeDisplay{
					Layout.rightMargin: 15
					Layout.preferredWidth: 60
					visible: steps.count != 0
					ms: timer.total
					foregroundColor: mainItem.foregroundColor
				}
			}
			RowLayout{
				Button{
					Layout.leftMargin: 15
					text: 'Reset'
					onClicked: mainItem.reset()
				}
				Button{
					text: timer.running ? 'Pause' : 'Start'
					onClicked: timer.running ? mainItem.pause() : mainItem.start()
				}
				Button{
					visible: timer.running
					text: 'Step'
					onClicked: mainItem.step()
				}
			}
		}

		ListView{
			id: listView
			Layout.fillWidth: true
			Layout.fillHeight: true
			Layout.preferredWidth: 160

			clip: true
			model:ListModel{
				id: steps
			}
			delegate: RowLayout{
				width: listView.width
				height: 20
				TimeDisplay{
					Layout.fillWidth: true
					ms: count
					foregroundColor: mainItem.foregroundColor
					flat: true
				}
				TimeDisplay{
					Layout.fillWidth: true
					ms: total
					foregroundColor: mainItem.foregroundColor
					flat: true
				}
			}
		}

	}
}
