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
import QtQuick.Dialogs
import QtCore

import App 1.0

Item {
	id: mainItem
	property var lastHeaders: {'showBackButton': false,
			'showMenuButton': true,
			'title': 'Settings'
		}
	Component.onCompleted:{
		mainWindow.setHeaders(lastHeaders)
	}
	ColumnLayout{
		anchors.fill: parent
		spacing: 15
		Item{
			Layout.fillHeight: true
		}
		Switch {
			id: oFFSwitch
			text: qsTr('Open Food Facts for search and barcode')
			checked: SettingsCpp.openFoodFactsEnabled
			onCheckedChanged: if(SettingsCpp.openFoodFactsEnabled != checked) SettingsCpp.openFoodFactsEnabled = checked
		}
		Switch {
			id: darkThemeSwitch
			text: qsTr("Dark Mode")
			checked: DefaultStyle.theme == Material.Dark
			onCheckedChanged: DefaultStyle.theme= darkThemeSwitch.checked ? Material.Dark : Material.Light
		}
		RowLayout{
			Button{
				text: 'Export Database'
				onClicked: saveDialog.open()
				FileDialog {
					id: saveDialog
					fileMode: FileDialog.SaveFile
					currentFolder: StandardPaths.standardLocations(StandardPaths.DocumentsLocation)[0]
					onAccepted: {
						if(!SettingsCpp.saveDatabase(selectedFile))
							mainErrorPopup.text = "Error on saving. Database couldn't be exported. Check logs for more details";
						else
							mainErrorPopup.text = "Database saved.";
						mainErrorPopup.open()
					}
				}
			}
			Button{
				text: 'Import Database (erase old data!)'
				onClicked: loadDialog.open()
				FileDialog {
					id: loadDialog
					currentFolder: StandardPaths.standardLocations(StandardPaths.DocumentsLocation)[0]
					onAccepted: {
						if(!SettingsCpp.loadDatabase(selectedFile))
							mainErrorPopup.text = "Error on loading. Database couldn't be restored. Check logs for more details";
						else
							mainErrorPopup.text = "Database restored.";	
						mainErrorPopup.open()
					}
				}
			}
		}
		Item{
			Layout.fillHeight: true
		}
	}
}
