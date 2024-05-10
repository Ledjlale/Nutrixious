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
import QtCore
import QtMultimedia

import App 1.0

import com.scythestudio.scodes 1.0

Item {
	id: mainItem

	readonly property bool isStarted: barcodeLoader.active
	property string scannedCode

	function start(){
		if(permission.status !== Qt.PermissionStatus.Granted)
			permission.request()
		else
			barcodeLoader.active = true
	}
	function stop(){
		barcodeLoader.active = false
	}
	CameraPermission{
		id: permission
		onStatusChanged: if(status === Qt.PermissionStatus.Granted)
			barcodeLoader.active = true
	}
// Embed SBarcodeScanner into Loader to reset internal capture
	Loader{
		id: barcodeLoader

		active: false
		property int targetWidth: videoOutput.width
		property int targetHeight: videoOutput.height
		property int minSize : Math.min(targetWidth, targetHeight) /2

		property rect captureRect: Qt.rect( (targetWidth - minSize)/ 2 / targetWidth, (targetHeight - minSize)/ 2 / targetHeight, minSize / targetWidth , minSize / targetHeight)

		sourceComponent:Component{
			SBarcodeScanner {
				id: barcodeScanner

				forwardVideoSink: videoOutput.videoSink
				captureRect: barcodeLoader.captureRect

				onCapturedChanged: function (captured) {
					mainItem.scannedCode = captured
					console.log('Captured: ' +captured)
					barcodeLoader.active = false
				}
				Component.onCompleted: mainItem.scannedCode = ''
			}
		}
	}

	VideoOutput {
		id: videoOutput
		anchors.fill: parent
		visible: barcodeLoader.status === Loader.Ready
		/*
		onVisibleChanged: if(visible)
								autoTimer.restart()
							else
								autoTimer.stop()
								*/
// Debug
		Timer{
			id: autoTimer
			interval: 2000
			onTriggered:  {
						mainItem.scannedCode = '3520836330036'
						console.log('Captured auto: 3520836330036 (lait)')
						barcodeLoader.active = false
					}
		}
		ScannerOverlay {
			id: scannerOverlay
			anchors.fill: parent
			captureRect: barcodeLoader.captureRect
		}
		RoundButton{
			anchors.bottom: parent.bottom
			anchors.horizontalCenter: parent.horizontalCenter
			onClicked: {
				barcodeLoader.active = false
			}
		}
	}
}
// Eau
// https://ssl-api.openfoodfacts.org/api/v0/product/3257971101015.json
// Lait
// https://ssl-api.openfoodfacts.org/api/v0/product/3520836330036.json
