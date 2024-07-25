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
import QtQuick.Layouts

import App 1.0

Control.ToolBar{
	id: mainItem
	property bool showBackButton : true
	property bool showMenuButton: true
	property bool showOptionsButton: true
	property bool showSaveButton: false
	property bool showBarcodeButton: false
	property bool showCreateButton: false
	property bool showRecipeButton: false
	property bool showBodyButton: false
	property bool showOkButton: false
	property alias title: titleLabel.text
	property bool displayStopwatch: false
	property bool displayBody: false
	signal back()
	signal menu()
	signal options()
	signal save()
	signal create()
	signal barcode()
	signal recipe()
	signal ok()
	
	
	function setHeaders(headers){
		showBackButton = headers.showBackButton != undefined ? headers.showBackButton : false
		showBodyButton = headers.showBodyButton != undefined ? headers.showBodyButton : false
		showMenuButton = headers.showMenuButton != undefined  ? headers.showMenuButton : true
		showOptionsButton = headers.showOptionsButton != undefined  ? headers.showOptionsButton : false
		showSaveButton = headers.showSaveButton != undefined  ? headers.showSaveButton : false
		showBarcodeButton = headers.showBarcodeButton != undefined  ? headers.showBarcodeButton : false
		showCreateButton = headers.showCreateButton != undefined  ? headers.showCreateButton : false
		showRecipeButton = headers.showRecipeButton != undefined  ? headers.showRecipeButton : false
		showOkButton = headers.showOkButton != undefined  ? headers.showOkButton : false
		if( headers.title != undefined) title = headers.title
	}
	
	function updateHeaders(headers){
		if(headers.showBackButton != undefined) showBackButton = headers.showBackButton
		if(headers.showBodyButton != undefined) showBodyButton = headers.showBodyButton
		if(headers.showMenuButton != undefined) showMenuButton = headers.showMenuButton
		if(headers.showOptionsButton != undefined) showOptionsButton = headers.showOptionsButton
		if(headers.showSaveButton != undefined) showSaveButton = headers.showSaveButton
		if(headers.showBarcodeButton != undefined) showBarcodeButton = headers.showBarcodeButton
		if(headers.showCreateButton != undefined) showCreateButton = headers.showCreateButton
		if(headers.showRecipeButton != undefined) showRecipeButton = headers.showRecipeButton
		if(headers.showOkButton != undefined) showOkButton = headers.showOkButton
	}
	
	RowLayout {
		anchors.fill: parent
		ToolButtonImage{
			Layout.preferredHeight: 20
			Layout.preferredWidth: 40
			Layout.leftMargin: 5
			visible: mainItem.showMenuButton
			imageSource: DefaultStyle.menuButton
			colorizationColor: 'white'
			onClicked: mainItem.menu()
		}
		
		ToolButtonImage{
			Layout.preferredHeight: 20
			Layout.preferredWidth: 40
			Layout.leftMargin: 5
			visible: mainItem.showBackButton
			imageSource: DefaultStyle.backButton
			colorizationColor: 'white'
			onClicked: mainItem.back()
		}
		 
		Control.Label {
			id: titleLabel
			Layout.leftMargin: 15
			Layout.rightMargin: 5
			visible: !!text
			elide: Control.Label.ElideRight
			horizontalAlignment: Qt.AlignLeft
			verticalAlignment: Qt.AlignVCenter
			Layout.fillWidth: true
			color: 'white'
			font.pixelSize: 18
			font.bold: true
		}
		ToolButtonImage{
			Layout.preferredHeight: 25
			Layout.preferredWidth: 25
			Layout.rightMargin: 5
			visible: mainItem.showBodyButton
			imageSource: DefaultStyle.bodyButton
			colorizationColor: mainItem.displayBody ? Control.Material.accent : 'white'
			onClicked:  mainItem.displayBody = !mainItem.displayBody
		}
		ToolButtonImage{
			Layout.preferredHeight: 25
			Layout.preferredWidth: 25
			Layout.rightMargin: 5
			imageSource: DefaultStyle.stopwatchButton
			colorizationColor: mainItem.displayStopwatch ? Control.Material.accent : 'white'
			onClicked:  mainItem.displayStopwatch = !mainItem.displayStopwatch
		}
		
		ToolButtonImage{
			Layout.preferredWidth: 25
			Layout.preferredHeight: 25
			Layout.leftMargin: 5
			Layout.rightMargin: 5
			visible: SettingsCpp.openFoodFactsEnabled && mainItem.showBarcodeButton
			imageSource: DefaultStyle.barcodeButton
			colorizationColor: 'white'
			onClicked:{
				mainItem.barcode()
			}
		}
		
		ToolButtonImage{
			Layout.preferredWidth: 25
			Layout.preferredHeight: 25
			Layout.leftMargin: 5
			Layout.rightMargin: 5
			visible: mainItem.showRecipeButton
			imageSource: DefaultStyle.recipeButton
			colorizationColor: 'white'
			onClicked:{
				mainItem.recipe()
			}
		}
		
		ToolButtonImage{
			Layout.preferredWidth: 25
			Layout.preferredHeight: 25
			Layout.leftMargin: 5
			Layout.rightMargin: 5
			visible: mainItem.showCreateButton
			imageSource: DefaultStyle.createButton
			colorizationColor: 'white'
			onClicked:{
				mainItem.create()
			}
		}
		
		ToolButtonImage {
			Layout.preferredHeight: 25
			Layout.preferredWidth: 25
			Layout.leftMargin: 5
			Layout.rightMargin: 5
			visible: mainItem.showSaveButton
			imageSource: DefaultStyle.saveButton
			colorizationColor: 'white'
			onClicked: mainItem.save()
		}
		
		ToolButtonImage {
			Layout.preferredHeight: 25
			Layout.preferredWidth: 25
			Layout.leftMargin: 5
			Layout.rightMargin: 5
			visible: mainItem.showOkButton
			imageSource: DefaultStyle.okButton
			colorizationColor: 'white'
			onClicked: mainItem.ok()
		}
		
		ToolButtonImage{
			Layout.preferredHeight: 25
			Layout.preferredWidth: 25
			Layout.leftMargin: 5
			Layout.rightMargin: 5
			visible: mainItem.showOptionsButton
			imageSource: DefaultStyle.optionsButton
			colorizationColor: 'white'
			onClicked: mainItem.options()
		}
	}
}
