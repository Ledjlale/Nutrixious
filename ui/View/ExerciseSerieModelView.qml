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
//import App.Training 1.0 as Training

import '../Tool/Utils.js' as Utils
Item{
	id: mainItem
	property var serieModel
	property var exerciseModel
	property bool isLive: !!serieModel?.targetSerieModel
	property bool resting: !!serieModel?.isResting
	property bool isReadOnly: isLive
	property bool showSaveButton: true
	property bool showTitle: true
	property bool trainingResultEdition: false
	property bool doSave: true
	property int leftMargin: 0

	implicitHeight: contentLayout.implicitHeight
	Rectangle{
		anchors.fill: parent
		visible: mainItem.isLive && ( serieModel?.isRunning || serieModel?.isDone)
		color: serieModel?.isResting || serieModel?.resultSerieModel?.isSaved
					? Material.color(Material.Green, Material.Shade100)
					: serieModel?.isDone
						? Material.color(Material.Blue, Material.Shade100)
						: Material.color(Material.accent, Material.Shade100)
	}
	RowLayout{
		id: contentLayout
		anchors.fill: parent
		anchors.leftMargin: mainItem.leftMargin
		property int _visibleChildren : visibleChildren.length - 1
		spacing: 0
		implicitHeight: Math.max( (saveButton.visible ? saveButton.implicitHeight: 0), fieldsList.implicitHeight, restTextField.implicitHeight, workTextField.implicitHeight)
		property int itemCount: fieldsList.count + (restTextField.visible ?  1 :0) + (workTextField.visible ? 1 :0)
		ListView{
			id: fieldsList
			Layout.fillHeight: true
			Layout.preferredWidth: contentLayout.width * ( contentLayout.itemCount - (restTextField.visible ?  1 :0)
																- (workTextField.visible ? 1 :0)) / contentLayout.itemCount
										//- (restTextField.visible ?  restTextField.implicitWidth :0)
										//- (workTextField.visible ? workTextField.implicitWidth :0)
										- (saveButton.visible ? saveButton.implicitWidth : 0)
			spacing: 0
			clip: true
			orientation: ListView.Horizontal
			model: mainItem.serieModel?.isDone
						? mainItem.serieModel?.resultSerieModel?.data
						: mainItem.serieModel?.targetSerieModel?.data || mainItem.serieModel?.data
			function updateImplicit(){
				var h = 0
				for(var i in contentItem.children){
					if( h < contentItem.children[i].implicitHeight)
						h = contentItem.children[i].implicitHeight
				}
				fieldsList.implicitHeight = h
			}
			delegate: TextField{
				width: fieldsList.width / fieldsList.count
				height: implicitHeight
				showTitle: mainItem.showTitle
				readOnly: mainItem.isReadOnly
				inputMethodHints: Qt.ImhDigitsOnly
				title: mainItem.showTitle ? modelData.name : ''
				//textColor: !mainItem.isLive || serieModel.repetitions <= serieModel.targetWork.repetitions ? Material.foreground : Material.accent
				text: modelData.value
				//text: mainItem.isLive
					//			? serieModel.targetWork.repetitions + (serieModel.isDone ? ' / ' + serieModel.repetitions : '')
						//		: serieModel.repetitions
				onEditingFinished: {
					modelData.value = newValue
				}
				onImplicitHeightChanged: fieldsList.updateImplicit()
			}
		}
		TextField{
			id: restTextField
			Layout.fillHeight: true
			Layout.preferredWidth: Math.max(implicitWidth,fieldsList.width / fieldsList.count)
			Layout.rightMargin: 10
			visible: !mainItem.trainingResultEdition
			showTitle: mainItem.showTitle || mainItem.trainingResultEdition
			readOnly:  mainItem.isReadOnly || mainItem.isLive
			inputMethodHints: Qt.ImhDigitsOnly
			title: mainItem.showTitle ? 'RestTime(s)' : ''
			textColor: !mainItem.isLive || !serieModel?.isDone || (serieModel.resultSerieModel.restTime <= serieModel.targetSerieModel.restTime) ? Material.foreground : Material.accent
			text: mainItem.isLive
							? mainItem.serieModel.targetSerieModel.restTime + (serieModel?.isDone ? ' / ' + serieModel.resultSerieModel.restTime : '')
							: mainItem.serieModel.restTime
			onEditingFinished: if(!mainItem.showSaveButton) serieModel.restTime = newValue

		}
		TextField{
			id: workTextField
			Layout.fillHeight: true
			Layout.rightMargin: 10
			Layout.preferredWidth: Math.max(implicitWidth,fieldsList.width / fieldsList.count)
			visible: serieModel?.isSaved || serieModel?.isDone || false//&& serieModel.isTraining
			showTitle: mainItem.trainingResultEdition
			readOnly:  true
			inputMethodHints: Qt.ImhDigitsOnly
			title: mainItem.showTitle ? 'WorkTime(s)' : ''
			//textColor: !mainItem.isLive || serieModel.restTime <= serieModel.targetWork.restTime ? Material.foreground : Material.accent
			text: visible
					? mainItem.isLive
						? serieModel?.isDone
							? serieModel.resultSerieModel.workTime
							: serieModel.targetSerieModel.workTime
						: serieModel?.workTime || ''
					: ''
			//onEditingFinished: if(!mainItem.showSaveButton) serieModel.restTime = newValue
		}
		Button{
			id: saveButton
			Layout.preferredWidth: implicitWidth
			visible: !mainItem.isReadOnly && mainItem.showSaveButton && serieModel?.isEdited || false
			// (repsTextField.isEdited || weightTextField.isEdited || restTextField.isEdited)
			text: 'Save'
			onClicked: {
				//if(repsTextField.isEdited) serieModel.repetitions = repsTextField.newValue
				//if(weightTextField.isEdited) serieModel.weight = weightTextField.newValue
				//if(restTextField.isEdited) serieModel.restTime = restTextField.newValue
				if(!mainItem.trainingResultEdition && mainItem.doSave) {
					serieModel.save()
				}
			}
		}
/*
		Rectangle{
			Layout.alignment: Qt.AlignCenter
			Layout.preferredHeight: 30
			Layout.preferredWidth: 30
			color: Material.primary
			visible: !!mainItem.exerciseModel && !mainItem.isReadOnly

			radius: width / 2
			Text{
				anchors.centerIn: parent
				color: 'white'
				text: '+'
			}
			MouseArea{
				anchors.fill: parent
				onClicked:{
					exerciseModel.decrementWorkOrder(mainItem.serieModel)
				}
			}
		}
		Rectangle{
			Layout.alignment: Qt.AlignCenter
			Layout.preferredHeight: 30
			Layout.preferredWidth: 30
			visible: !!mainItem.exerciseModel && !mainItem.isReadOnly
			color: Material.primary
			radius: width / 2
			Text{
				anchors.centerIn: parent
				color: 'white'
				text: '-'
			}
			MouseArea{
				anchors.fill: parent
				onClicked:{
					exerciseModel.incrementWorkOrder(mainItem.serieModel)
				}
			}
		}*/
	}
}

