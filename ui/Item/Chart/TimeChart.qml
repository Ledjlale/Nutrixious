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
import QtQuick.Effects
import QtQuick.Layouts
import QtCharts

ChartView{
	id: chartView

	antialiasing: true
	margins.left: 0
	margins.right: 0
	margins.top: 0
	margins.bottom: 0
	function displayPoints(points) {
			var minX = -1
			var maxX = -1
			var minY = -1
			var maxY = -1
			series.clear()
			for(var p in points){
				var x = points[p].x
				var y = points[p].y
				if( x > maxX) maxX = x
				if( y > maxY) maxY = y
				if( x < minX || minX == -1) minX = x
				if( y < minY || minY == -1) minY = y
				//console.log('Add : ' +x + ", "+y)
				series.append(x, y)
			}
			chartView.axisY(series).min = minY-1;
			chartView.axisY(series).max = maxY+1 ;
			//console.log(minX + " , " +minY + " / " + maxX+ " , " +maxY)
			chartView.axisX(series).min = minX;
			chartView.axisX(series).max = maxX;
		}
	function clear(){
		series.clear()
	}
	ValueAxis {
		id: vAxis
	}
	DateTimeAxis{
		id: dAxis
		format: 'yyyy-MM-dd'
		labelsAngle: -60
	}
	LineSeries{
		id: series
		axisX: dAxis
		axisY: vAxis
	}
}
