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
import QtQuick.Effects

Item {
	id: mainItem
	property rect captureRect
	Item {
		id: captureZoneCorners
		width: parent.width * mainItem.captureRect.width
		height: parent.height * mainItem.captureRect.height
		x: parent.width * mainItem.captureRect.x
		y: parent.height * mainItem.captureRect.y

		Rectangle {
			id: topLeftCornerH
			anchors {
				top: parent.top
				left: parent.left
			}
			width: 20
			height: 5
			color: Material.accent
			radius: height / 2
		}
		Rectangle {
			id: topLeftCornerV
			anchors {
				top: parent.top
				left: parent.left
			}
			width: 5
			height: 20
			color: Material.accent
			radius: width / 2
		}

		// ----------------------
		Rectangle {
			id: bottomLeftCornerH
			anchors {
				bottom: parent.bottom
				left: parent.left
			}
			width: 20
			height: 5
			color: Material.accent
			radius: height / 2
		}

		Rectangle {
			id: bottomLeftCornerV
			anchors {
				bottom: parent.bottom
				left: parent.left
			}
			width: 5
			height: 20
			color: Material.accent
			radius: width / 2
		}

		// ----------------------
		Rectangle {
			id: topRightCornerH
			anchors {
				top: parent.top
				right: parent.right
			}
			width: 20
			height: 5
			color: Material.accent
			radius: height / 2
		}

		Rectangle {
			id: topRightCornerV
			anchors {
				top: parent.top
				right: parent.right
			}
			width: 5
			height: 20
			color: Material.accent
			radius: width / 2
		}

		// ----------------------
		Rectangle {
			id: bottomRightCornerH
			anchors {
				bottom: parent.bottom
				right: parent.right
			}
			width: 20
			height: 5
			color: Material.accent
			radius: height / 2
		}

		Rectangle {
			id: bottomRightCornerV
			anchors {
				bottom: parent.bottom
				right: parent.right
			}
			width: 5
			height: 20
			color: Material.accent
			radius: width / 2
		}

		Rectangle {
			id: scanIndicator
			anchors {
				horizontalCenter: parent.horizontalCenter
			}
			width: parent.width - 20
			height: 1
			color: Material.accent

			SequentialAnimation {
				id: scanIndicatorAnimation
				loops: Animation.Infinite
				PropertyAnimation {
					id: toTopAnimation
					target: scanIndicator
					property: "y"
					duration: 2000
					to: topLeftCornerH.y+10
				}
				PropertyAnimation {
					id: toBottomAnimation
					target: scanIndicator
					property: "y"
					duration: 2000
					to: bottomLeftCornerH.y-10
				}
			}
		}
		MultiEffect{
			id: effect
			source: scanIndicator
			anchors.fill: scanIndicator
			shadowColor: Material.background
			shadowEnabled: true
		}

		/*
		RectangularGlow {
			id: effect
			anchors.centerIn: scanIndicator
			width: scanIndicator.width / 2
			height: scanIndicator.height
			glowRadius: 50
			spread: 0.2
			color: Material.accent
			cornerRadius: glowRadius
		}*/
	}

	Text {
		id: scanCapsuleText
		anchors {
			verticalCenter: captureZoneCorners.bottom
			horizontalCenter: captureZoneCorners.horizontalCenter
		}
		text: qsTr("Scan barcode")
		color: Material.accent
	}
	onVisibleChanged: if(visible) scanIndicatorAnimation.start()
	/*
	onCaptureRectChanged: {
		scanIndicatorAnimation.start()
	}*/
}
