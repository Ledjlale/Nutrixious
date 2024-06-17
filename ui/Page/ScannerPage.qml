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
			spacing: 15
			RoundButton{
				text: scannerView.isStarted ? 'X' : qsTr('Scan')
				onClicked: {
					debugText.text = ''
					if(scannerView.isStarted)
						scannerView.stop()
					else
						scannerView.start()
				}
			}
		}
		ScannerView{
			id: scannerView
			Layout.fillWidth: true
			Layout.fillHeight: true
			onScannedCodeChanged:{
				if(scannedCode != '')
					editor.foodModel.loadFromOpenFoodFacts(scannedCode)
					debugText.text = scannedCode
					//xmlModel.scannedCode = scannedCode
					//foodModel.request(scannedCode)
			}
			Text{
				id: debugText
				anchors.centerIn: parent
				color: Material.foreground

			}
		}
		FoodEditorPage{
			id: editor
		}
	}
}
/*
			FoodModel{
				id: foodModel

function request(scannedCode) {
	var xhr = new XMLHttpRequest();
	xhr.onreadystatechange = function() {
		if (xhr.readyState === XMLHttpRequest.HEADERS_RECEIVED) {

		} else if(xhr.readyState === XMLHttpRequest.DONE) {
		console.log(xhr.responseText.toString());
		  //var object = JSON.parse(xhr.responseText.toString());
	//print(JSON.stringify(object, null, 2));
		}
		//console.log(xhr.readyState +" / " +xhr.statusText)
	}
	var url =  "https://ssl-api.openfoodfacts.org/api/v0/product/"+scannedCode+".json?xml=1"
	console.log('Get : '+url)
	xhr.open("GET",url );
	xhr.send();
}

			}


			ListView{
				anchors.fill: parent
				model: XmlListModel {
					id: xmlModel
					property string scannedCode
					source: scannedCode ? "https://ssl-api.openfoodfacts.org/api/v0/product/"+scannedCode+".json" : ''
					//query: "/rss/channel/item"
					query:''
					XmlListModelRole { name: "nutriments"; elementName: "nutriments" }
					XmlListModelRole { name: "carbohydrates"; elementName: "carbohydrates" }

					onStatusChanged: console.log(status + " === " +count)
				}
				delegate: RowLayout{
					Text{
						text: index + " : " +nutriments + ", " +carbohydrates
					}
				}
			}

		}
	}
}
*/
// https://ssl-api.openfoodfacts.org/api/v0/product/3335880006048.json
