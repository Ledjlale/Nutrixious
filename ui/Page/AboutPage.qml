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
import QtCore
import QtMultimedia

import QtQml.XmlListModel

import App 1.0


Item {
	id: mainItem
	property var lastHeaders: {'showBackButton': false,
			'showMenuButton': true,
			'title': 'About'
		}
	Component.onCompleted:{
		mainWindow.setHeaders(lastHeaders)
	}
	StackView{
		id: stackView
		anchors.fill: parent
		initialItem: Item{
			ColumnLayout{
				anchors.centerIn: parent
				spacing: 120
				ColumnLayout{
					spacing: 0
					Text{
						Layout.alignment: Qt.AlignCenter
						color: Material.foreground
						font.weight: 400
						font.pixelSize: 30
						text: applicationVersionName
					}
					Text{
						Layout.alignment: Qt.AlignCenter
						color: Material.foreground
						text: ('Version %1').arg(applicationVersion)
					}
				}

				Text{
					Layout.alignment: Qt.AlignCenter
					color: Material.foreground
					linkColor: Material.accent
					textFormat: Text.StyledText
					text: '<a href="'+applicationUrl+'">'+applicationUrl+'</a>'
					onLinkActivated: (link)=> Qt.openUrlExternally(link)
				}
				Button{
					text: "Logs"
					onClicked: stackView.push(logView)
				}
			}
		}
	}
	Component{
		id: logView
		ColumnLayout{
			RowLayout{
				Button{
					text: 'Back'
					onClicked: stackView.pop()
				}
				Button{
					text: 'Copy'
					onClicked: {
						textEdit.text = Logger.getRawLogs()
						textEdit.selectAll()
						textEdit.copy()
					}
				}
				TextEdit{
					id: textEdit
					visible: false
				}
				Item{
					Layout.fillWidth: true
				}
				Button{
					text: 'Clean'
					onClicked: {
						Logger.cleanLogs()
						logs.model = Logger.getLogs()
					}
				}
			}
			ListView{
				id: logs
				Layout.fillWidth: true
				Layout.fillHeight: true
				clip: true
				model: Logger.getLogs()
				delegate:Text{
					color: Material.foreground
					font.pixelSize: 10
					text: modelData
				}
			}
		}
	}
}
