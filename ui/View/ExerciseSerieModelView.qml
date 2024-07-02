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
SwipeLayout{
	id: mainItem
	property var modelData
	property var exerciseUnitModel
	property bool isLive: !!modelData?.targetSerieModel
	property bool resting: !!modelData?.isResting
	property bool isReadOnly: isLive
	property bool showSaveButton: true
	property bool showTitle: true
	property bool showWorkTime: true
	property bool showRestTime: true
	property bool showCalories: true
	property bool showDecrementOrderChange: false
	property bool showIncrementOrderChange: false
	property bool trainingResultEdition: false
	property bool doSave: true
	property int margin: 0

	//implicitHeight: contentLayout.implicitHeight
	
	isEditable: !mainItem.isReadOnly
	isCaloriesComputable: !mainItem.isReadOnly && calorieTextField.visible && !mainItem.isLive && !mainItem.modelData.isRunning || false
	isSpeedComputable: !mainItem.isReadOnly && !mainItem.isLive && !mainItem.modelData.isRunning && speedField.visible && distanceField.visible && mainItem.modelData.isSpeedComputable  || false
	
	onDeleteClicked: mainItem.exerciseUnitModel.removeSerie(modelData) 
	onCaloriesComputationClicked: modelData.computeCalories()
	onSpeedComputationClicked: modelData.computeSpeed()
	
	background: Rectangle{
		id: background
		width: parent.width
		height: parent.height
		property var foregroundColor: Material.foreground
		//visible: mainItem.isLive && ( modelData?.isRunning || modelData?.isDone)
		property var shade: Material.theme == Material.Dark ? Material.Shade600 : Material.Shade300
		color: mainItem.isLive && ( modelData?.isRunning || modelData?.isDone)
				?  modelData?.isResting
					? Material.color(Material.Yellow, shade)
					: modelData?.resultSerieModel?.isSaved
						? Material.color(Material.Green, shade)
						: modelData?.isDone
							? Material.color(Material.Blue, shade)
							: Material.color(Material.Amber, shade)
				: Material.background
		}
	contentItem: RowLayout{
		id: contentLayout
		height: parent.height
		width: parent.width
		spacing: 0
		onImplicitWidthChanged: console.log(width + " / " +implicitWidth)
		TextField{
			property bool isGood: !mainItem.isLive || !modelData?.isDone || (modelData.resultSerieModel.repetitions >= modelData.targetSerieModel.repetitions)
			Layout.preferredHeight: implicitHeight
			Layout.fillWidth: true
			Layout.preferredWidth: 0
			//Layout.preferredWidth: mainItem.width / contentLayout.visibleChildren.length
			visible: !!exerciseUnitModel && (mainItem.isLive || mainItem.trainingResultEdition ? exerciseUnitModel.targetExerciseModel.useRepetitions : exerciseUnitModel.useRepetitions)
			edit: mainItem.edit
			showTitle: mainItem.showTitle
			readOnly: mainItem.isReadOnly
			inputMethodHints: Qt.ImhDigitsOnly
			textColor: isGood ? background.foregroundColor : Material.accent
			title: mainItem.showTitle ? 'Reps' : ''
			text: mainItem.isLive
						? mainItem.modelData.isDone
							? (isGood ? '' : mainItem.modelData.targetSerieModel.repetitions+'/') + mainItem.modelData.resultSerieModel.repetitions
							: mainItem.modelData.targetSerieModel.repetitions
						: mainItem.modelData.repetitions <0 ? '' : mainItem.modelData.repetitions
			//text: mainItem.isLive
			onEditingFinished: {
				if(mainItem.isLive) {
					if(!mainItem.modelData.isDone)
						mainItem.modelData.targetSerieModel.repetitions = newValue
					mainItem.modelData.resultSerieModel.repetitions = newValue
				}else
					mainItem.modelData.repetitions = newValue
			}
			//onImplicitHeightChanged: fieldsList.updateImplicit()
		}
		TextField{
			property bool isGood: !mainItem.isLive || !modelData?.isDone || (modelData.resultSerieModel.weight >= modelData.targetSerieModel.weight)
			Layout.preferredHeight: implicitHeight
			Layout.fillWidth: true
			Layout.preferredWidth: 0
			//Layout.preferredWidth: mainItem.width / contentLayout.visibleChildren.length
			visible: !!exerciseUnitModel && (mainItem.isLive || mainItem.trainingResultEdition ? exerciseUnitModel.targetExerciseModel.useWeight :exerciseUnitModel.useWeight)
			edit: mainItem.edit
			showTitle: mainItem.showTitle
			readOnly: mainItem.isReadOnly
			inputMethodHints: Qt.ImhDigitsOnly
			title: mainItem.showTitle ? 'Weight' : ''
			textColor: isGood ? background.foregroundColor : Material.accent
			text: mainItem.isLive
						? mainItem.modelData.isDone
							?  (isGood ? '' : mainItem.modelData.targetSerieModel.weight+'/') + Number.parseFloat(mainItem.modelData.resultSerieModel.weight.toFixed(2))
							: Number.parseFloat(mainItem.modelData.targetSerieModel.weight.toFixed(2))
						: mainItem.modelData.weight <0 ? '' : Number.parseFloat(mainItem.modelData.weight.toFixed(2))
			placeholderText: 'kg'
			//text: mainItem.isLive
			onEditingFinished: {
				if(mainItem.isLive) {
					if(!mainItem.modelData.isDone)
						mainItem.modelData.targetSerieModel.weight = newValue
					mainItem.modelData.resultSerieModel.weight = newValue
				}else
					mainItem.modelData.weight = newValue
			}
			//onImplicitHeightChanged: fieldsList.updateImplicit()
		}
		TextField{
			id: distanceField
			property bool isGood: !mainItem.isLive || !modelData?.isDone || (modelData.resultSerieModel.distance >= modelData.targetSerieModel.distance)
			Layout.preferredHeight: implicitHeight
			Layout.fillWidth: true
			Layout.preferredWidth: 0
			//Layout.preferredWidth: mainItem.width / contentLayout.visibleChildren.length
			visible: !!exerciseUnitModel && (mainItem.isLive || mainItem.trainingResultEdition ? exerciseUnitModel.targetExerciseModel.useDistance :exerciseUnitModel.useDistance)
			edit: mainItem.edit
			showTitle: mainItem.showTitle
			readOnly: mainItem.isReadOnly
			inputMethodHints: Qt.ImhDigitsOnly
			textColor: isGood ? background.foregroundColor : Material.accent
			title: mainItem.showTitle ? 'Distance' : ''
			text: mainItem.isLive
						? mainItem.modelData.isDone
							?  (isGood ? '' : mainItem.modelData.targetSerieModel.distance+'/') + Number.parseFloat(mainItem.modelData.resultSerieModel.distance.toFixed(2))
							: Number.parseFloat(mainItem.modelData.targetSerieModel.distance.toFixed(2))
						: mainItem.modelData.distance <0 ? '' : Number.parseFloat(mainItem.modelData.distance.toFixed(2))
			placeholderText: 'm'
			//text: mainItem.isLive
			onEditingFinished: {
				if(mainItem.isLive) {
					if(!mainItem.modelData.isDone)
						mainItem.modelData.targetSerieModel.distance = newValue
					mainItem.modelData.resultSerieModel.distance = newValue
				}else
					mainItem.modelData.distance = newValue
			}
			//onImplicitHeightChanged: fieldsList.updateImplicit()
		}

		TextField{
			id: speedField
			property bool isGood: !mainItem.isLive || !modelData?.isDone || (modelData.resultSerieModel.speed >= modelData.targetSerieModel.speed)
			Layout.preferredHeight: implicitHeight
			Layout.fillWidth: true
			Layout.preferredWidth: 0
			//Layout.preferredWidth: mainItem.width / contentLayout.visibleChildren.length
			visible: !!exerciseUnitModel && (mainItem.isLive || mainItem.trainingResultEdition ? exerciseUnitModel.targetExerciseModel.useSpeed :exerciseUnitModel.useSpeed)
			edit: mainItem.edit
			showTitle: mainItem.showTitle
			readOnly: mainItem.isReadOnly
			inputMethodHints: Qt.ImhDigitsOnly
			textColor: isGood ? background.foregroundColor : Material.accent
			title: mainItem.showTitle ? 'Speed' : ''
			text: mainItem.isLive
						? mainItem.modelData.isDone
							? (isGood ? '' : mainItem.modelData.targetSerieModel.speed+'/') + Number.parseFloat(mainItem.modelData.resultSerieModel.speed.toFixed(2))
							: Number.parseFloat(mainItem.modelData.targetSerieModel.speed.toFixed(2))
						: mainItem.modelData.speed <0 ? '' : Number.parseFloat(mainItem.modelData.speed.toFixed(2))
			placeholderText: 'km/h'
			//text: mainItem.isLive
			onEditingFinished: {
				if(mainItem.isLive) {
					if(!mainItem.modelData.isDone)
						mainItem.modelData.targetSerieModel.speed = newValue
					mainItem.modelData.resultSerieModel.speed = newValue
				}else
					mainItem.modelData.speed = newValue
			}
			//onImplicitHeightChanged: fieldsList.updateImplicit()
		}

		TextField{
			id: restTextField
			Layout.fillWidth: true
			//Layout.preferredWidth: mainItem.width / contentLayout.visibleChildren.length
			Layout.fillHeight: true
			Layout.preferredWidth: 0
			visible: mainItem.showRestTime && !mainItem.trainingResultEdition
			edit: mainItem.edit
			showTitle: mainItem.showTitle || mainItem.trainingResultEdition
			readOnly:  mainItem.isReadOnly || mainItem.isLive
			inputMethodHints: Qt.ImhTime
			elide: Text.ElideLeft
			title: mainItem.showTitle ? 'RestTime' : ''
			placeholderText: 'hh:mm:ss'
			textColor: background.foregroundColor
			text: mainItem.isLive
							? mainItem.modelData.isDone
								? mainItem.modelData.resultSerieModel.restTimeStr
								: mainItem.modelData.targetSerieModel.restTimeStr
							: mainItem.modelData.restTimeStr
			onEditingFinished: {
				if(mainItem.isLive) {
					if(!mainItem.modelData.isDone)
						mainItem.modelData.targetSerieModel.restTimeStr = newValue
					mainItem.modelData.resultSerieModel.restTimeStr = newValue
				}else
					mainItem.modelData.restTimeStr = newValue
			}
		}
		TextField{
			id: workTextField
			Layout.fillHeight: true
			Layout.fillWidth: true
			Layout.preferredWidth: 0
			//Layout.preferredWidth: mainItem.width / contentLayout.visibleChildren.length
			visible: mainItem.showWorkTime && ( !!modelData?.isSaved || !!modelData?.isDone)
			showTitle: mainItem.showTitle || mainItem.trainingResultEdition
			readOnly:  mainItem.isReadOnly
			inputMethodHints: Qt.ImhTime
			elide: Text.ElideLeft
			edit: mainItem.edit
			title: mainItem.showTitle ? 'WorkTime' : ''
			placeholderText: 'hh:mm:ss'
			textColor: background.foregroundColor
			//textColor: !mainItem.isLive || modelData.restTime <= modelData.targetWork.restTime ? Material.foreground : Material.accent
			text: visible
					? mainItem.isLive
						? modelData?.isDone
							? modelData.resultSerieModel.workTimeStr
							: modelData.targetSerieModel.workTimeStr
						: modelData?.workTimeStr || ''
					: ''
			onEditingFinished: {
				if(mainItem.isLive) {
					if(!mainItem.modelData.isDone)
						mainItem.modelData.targetSerieModel.workTimeStr = newValue
					mainItem.modelData.resultSerieModel.workTimeStr = newValue
				}else
					mainItem.modelData.workTimeStr = newValue
			}
		}
		TextField{
			id: calorieTextField
			Layout.fillHeight: true
			Layout.fillWidth: true
			Layout.preferredWidth: 0
			//Layout.preferredWidth: mainItem.width / contentLayout.visibleChildren.length
			visible: mainItem.showCalories && (!!modelData?.isSaved || !!modelData?.isDone) ||  mainItem.trainingResultEdition
			showTitle: mainItem.showTitle || mainItem.trainingResultEdition
			readOnly:  mainItem.isReadOnly
			inputMethodHints: Qt.ImhDigitsOnly
			elide: Text.ElideRight
			edit: mainItem.edit
			title: mainItem.showTitle ? 'Calories' : ''
			placeholderText: 'kcal'
			textColor: background.foregroundColor
			text: visible
					? mainItem.isLive
						? modelData?.isDone
							? Number.parseFloat(modelData.resultSerieModel.calories.toFixed(2))
							: modelData.targetSerieModel.calories
						: Number.parseFloat(modelData?.calories.toFixed(2)) || ''
					: ''
			onEditingFinished: {
				if(mainItem.isLive) {
					if(!mainItem.modelData.isDone)
						mainItem.modelData.targetSerieModel.calories = newValue
					mainItem.modelData.resultSerieModel.calories = newValue
				}else
					mainItem.modelData.calories = newValue
			}
		}/*
		Rectangle{
			Layout.alignment: Qt.AlignCenter
			Layout.preferredHeight: 30
			Layout.preferredWidth: 30
			color: Material.primary
			visible: mainItem.showDecrementOrderChange && !modelData?.isSaved && !modelData?.isDone && !mainItem.modelData.isRunning

			radius: width / 2
			Text{
				anchors.centerIn: parent
				color: 'white'
				text: '+'
			}
			MouseArea{
				anchors.fill: parent
				onClicked:{
					mainItem.exerciseUnitModel.decrementSerieOrder(mainItem.modelData)
				}
			}
		}
		Rectangle{
			Layout.alignment: Qt.AlignCenter
			Layout.preferredHeight: 30
			Layout.preferredWidth: 30
			visible: mainItem.showIncrementOrderChange && !modelData?.isSaved && !modelData?.isDone && !mainItem.modelData.isRunning
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
					mainItem.exerciseUnitModel.incrementSerieOrder(mainItem.modelData)
				}
			}
		}*/
		
		ButtonImage{
			id: saveButton
			Layout.alignment: Qt.AlignCenter
			Layout.preferredWidth: 25
			Layout.preferredHeight: 25
			visible: !mainItem.isReadOnly && mainItem.showSaveButton && modelData?.isEdited && !mainItem.modelData.isRunning || false
			imageSource: DefaultStyle.saveButton
			colorizationColor: background.foregroundColor
			onClicked: {
				if(!mainItem.trainingResultEdition && mainItem.doSave) {
					modelData.save()
				}
				mainItem.edit = false
			}
		}
	}
}

