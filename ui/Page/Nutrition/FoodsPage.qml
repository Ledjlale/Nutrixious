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

	ColumnLayout{
		anchors.fill: parent
		spacing: 0
		RowLayout{
			Item{
				Layout.fillWidth: true
			}
			Button{
				visible: stackView.depth == 1
				property bool scanning: stackView.currentItem.objectName == "Scanner" && stackView.currentItem.isStarted
				text: scanning ? 'X' : qsTr('Scan')
				onClicked: {
						if(!scanning)
							stackView.push(scannerComponent)
						else
							stackView.pop()
				}
			}
			Button{
				property bool editing: stackView.currentItem.objectName == "Editor"
				text: editing
						?  stackView.currentItem.foodModel.isSaved
							? 'Update'
							: 'Save'
						: 'Create'
				onClicked:{
					if(editing){
						if(stackView.currentItem.foodModel.save()) {
							stackView.pop()
							foodListModel.update()
						}
					}else {
						stackView.push(foodEditorComponent)
					}
				}
			}
		}
		StackView{
			id: stackView
			Layout.fillHeight: true
			Layout.fillWidth: true
			initialItem: ListView{
				model: FoodProxyModel{
					id: foodListModel
					Component.onCompleted: update()
				}
				delegate: FoodLineView{
					width: stackView.width
					//height: implicitHeight
					foodModel: $modelData

					MouseArea{
						anchors.fill: parent
						onClicked:{
							console.log("Click "+$modelData)
								 stackView.push(foodEditorComponent, {foodModel:$modelData})
					}
					}
				}
			}
			Component{
				id: scannerComponent
				ScannerView{
					id: scannerView
					objectName: 'Scanner'
					onScannedCodeChanged:{
						if(scannedCode != '') {
							stackView.replace(foodEditorComponent, {offCode: scannedCode})
						}
					}
					Component.onCompleted: start()
				}
			}
			Component{
				id: foodEditorComponent
				FoodEditorPage{
					id: editor
					objectName: 'Editor'
				}
			}
		}
	}
}
