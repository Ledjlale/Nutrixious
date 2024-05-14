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

RowLayout{
	id: mainItem
	property int ms
	property color foregroundColor: Material.color(Material.foreground)
	property bool flat: false
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
	spacing: 0
	property var format: formatDuration(mainItem.ms)
	Text{
		Layout.fillHeight: true
		verticalAlignment: Text.AlignBottom
		font.pixelSize: mainItem.flat ? 20 : 30
		color: mainItem.foregroundColor
		visible: parent.format.h>0
		text: parent.format.h+ ':'
	}
	Text{
		Layout.fillHeight: true
		verticalAlignment: Text.AlignBottom
		font.pixelSize: mainItem.flat ? 20 : 30
		color: mainItem.foregroundColor
		visible: parent.format.m>0
		text: parent.format.m+':'
	}
	Text{
		Layout.fillHeight: true
		verticalAlignment: Text.AlignBottom
		font.pixelSize: mainItem.flat ? 20 : 30
		color: mainItem.foregroundColor
		text: parent.format.s+ (mainItem.flat ? '.' : ' ')
	}
	Text{
		Layout.fillHeight: true
		verticalAlignment: Text.AlignBottom
		Layout.alignment: Qt.AlignBottom
		font.pixelSize: 20
		color: mainItem.foregroundColor
		text:Math.floor(parent.format.ms/10)
	}
}

