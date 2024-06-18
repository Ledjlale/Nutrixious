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
SwipeView{
	id: mainItem
	property var serieModel
	property var exerciseUnitModel
	property bool isLive: !!serieModel?.targetSerieModel
	property bool resting: !!serieModel?.isResting
	property bool isReadOnly: isLive
	property bool isDeletable: true
	property bool showSaveButton: true
	property bool showTitle: true
	property bool showWorkTime: true
	property bool showCalories: true
	property bool showDecrementOrderChange: false
	property bool showIncrementOrderChange: false
	property bool trainingResultEdition: false
	property bool doSave: true
	property int margin: 0
	property bool keepEditView: false

	implicitHeight: contentLayout.implicitHeight
	currentIndex: 0
	Rectangle{
		width: mainItem.width
		height: mainItem.height
		//visible: mainItem.isLive && ( serieModel?.isRunning || serieModel?.isDone)
		color: mainItem.isLive && ( serieModel?.isRunning || serieModel?.isDone)
				?  serieModel?.isResting
					? Material.color(Material.Yellow, Material.Shade100)
					: serieModel?.resultSerieModel?.isSaved
						? Material.color(Material.Green, Material.Shade100)
						: serieModel?.isDone
							? Material.color(Material.Blue, Material.Shade100)
							: Material.color(Material.Amber, Material.Shade300)
				: 'transparent'
		RowLayout{
			id: contentLayout
			anchors.fill: parent
			anchors.leftMargin: mainItem.margin
			anchors.rightMargin: mainItem.margin
			spacing: 0
			TextField{
				property bool isGood: !mainItem.isLive || !serieModel?.isDone || (serieModel.resultSerieModel.repetitions >= serieModel.targetSerieModel.repetitions)
				height: implicitHeight
				Layout.preferredWidth: mainItem.width / contentLayout.visibleChildren.length
				visible: !!exerciseUnitModel && (mainItem.isLive || mainItem.trainingResultEdition ? exerciseUnitModel.targetExerciseModel.useRepetitions : exerciseUnitModel.useRepetitions)
				keepEditView: mainItem.keepEditView
				showTitle: mainItem.showTitle
				readOnly: mainItem.isReadOnly
				inputMethodHints: Qt.ImhDigitsOnly
				textColor: isGood ? Material.foreground : Material.accent
				title: mainItem.showTitle ? 'Reps' : ''
				text: mainItem.isLive
							? mainItem.serieModel.isDone
								? (isGood ? '' : mainItem.serieModel.targetSerieModel.repetitions+'/') + mainItem.serieModel.resultSerieModel.repetitions
								: mainItem.serieModel.targetSerieModel.repetitions
							: mainItem.serieModel.repetitions <0 ? '' : mainItem.serieModel.repetitions
				//text: mainItem.isLive
				onEditingFinished: {
					if(mainItem.isLive) {
						if(!mainItem.serieModel.isDone)
							mainItem.serieModel.targetSerieModel.repetitions = newValue
						mainItem.serieModel.resultSerieModel.repetitions = newValue
					}else
						mainItem.serieModel.repetitions = newValue
				}
				//onImplicitHeightChanged: fieldsList.updateImplicit()
			}
			TextField{
				property bool isGood: !mainItem.isLive || !serieModel?.isDone || (serieModel.resultSerieModel.weight >= serieModel.targetSerieModel.weight)
				height: implicitHeight
				Layout.preferredWidth: mainItem.width / contentLayout.visibleChildren.length
				visible: !!exerciseUnitModel && (mainItem.isLive || mainItem.trainingResultEdition ? exerciseUnitModel.targetExerciseModel.useWeight :exerciseUnitModel.useWeight)
				keepEditView: mainItem.keepEditView
				showTitle: mainItem.showTitle
				readOnly: mainItem.isReadOnly
				inputMethodHints: Qt.ImhDigitsOnly
				title: mainItem.showTitle ? 'Weight' : ''
				textColor: isGood ? Material.foreground : Material.accent
				text: mainItem.isLive
							? mainItem.serieModel.isDone
								?  (isGood ? '' : mainItem.serieModel.targetSerieModel.weight+'/') + Number.parseFloat(mainItem.serieModel.resultSerieModel.weight.toFixed(2))
								: Number.parseFloat(mainItem.serieModel.targetSerieModel.weight.toFixed(2))
							: mainItem.serieModel.weight <0 ? '' : Number.parseFloat(mainItem.serieModel.weight.toFixed(2))
				placeholderText: 'kg'
				//text: mainItem.isLive
				onEditingFinished: {
					if(mainItem.isLive) {
						if(!mainItem.serieModel.isDone)
							mainItem.serieModel.targetSerieModel.weight = newValue
						mainItem.serieModel.resultSerieModel.weight = newValue
					}else
						mainItem.serieModel.weight = newValue
				}
				//onImplicitHeightChanged: fieldsList.updateImplicit()
			}
			TextField{
				property bool isGood: !mainItem.isLive || !serieModel?.isDone || (serieModel.resultSerieModel.distance >= serieModel.targetSerieModel.distance)
				height: implicitHeight
				Layout.preferredWidth: mainItem.width / contentLayout.visibleChildren.length
				visible: !!exerciseUnitModel && (mainItem.isLive || mainItem.trainingResultEdition ? exerciseUnitModel.targetExerciseModel.useDistance :exerciseUnitModel.useDistance)
				keepEditView: mainItem.keepEditView
				showTitle: mainItem.showTitle
				readOnly: mainItem.isReadOnly
				inputMethodHints: Qt.ImhDigitsOnly
				textColor: isGood ? Material.foreground : Material.accent
				title: mainItem.showTitle ? 'Distance' : ''
				text: mainItem.isLive
							? mainItem.serieModel.isDone
								?  (isGood ? '' : mainItem.serieModel.targetSerieModel.distance+'/') + Number.parseFloat(mainItem.serieModel.resultSerieModel.distance.toFixed(2))
								: Number.parseFloat(mainItem.serieModel.targetSerieModel.distance.toFixed(2))
							: mainItem.serieModel.distance <0 ? '' : Number.parseFloat(mainItem.serieModel.distance.toFixed(2))
				placeholderText: 'm'
				//text: mainItem.isLive
				onEditingFinished: {
					if(mainItem.isLive) {
						if(!mainItem.serieModel.isDone)
							mainItem.serieModel.targetSerieModel.distance = newValue
						mainItem.serieModel.resultSerieModel.distance = newValue
					}else
						mainItem.serieModel.distance = newValue
				}
				//onImplicitHeightChanged: fieldsList.updateImplicit()
			}

			TextField{
				property bool isGood: !mainItem.isLive || !serieModel?.isDone || (serieModel.resultSerieModel.speed >= serieModel.targetSerieModel.speed)
				height: implicitHeight
				Layout.preferredWidth: mainItem.width / contentLayout.visibleChildren.length
				visible: !!exerciseUnitModel && (mainItem.isLive || mainItem.trainingResultEdition ? exerciseUnitModel.targetExerciseModel.useSpeed :exerciseUnitModel.useSpeed)
				keepEditView: mainItem.keepEditView
				showTitle: mainItem.showTitle
				readOnly: mainItem.isReadOnly
				inputMethodHints: Qt.ImhDigitsOnly
				textColor: isGood ? Material.foreground : Material.accent
				title: mainItem.showTitle ? 'Speed' : ''
				text: mainItem.isLive
							? mainItem.serieModel.isDone
								? (isGood ? '' : mainItem.serieModel.targetSerieModel.speed+'/') + Number.parseFloat(mainItem.serieModel.resultSerieModel.speed.toFixed(2))
								: Number.parseFloat(mainItem.serieModel.targetSerieModel.speed.toFixed(2))
							: mainItem.serieModel.speed <0 ? '' : Number.parseFloat(mainItem.serieModel.speed.toFixed(2))
				placeholderText: 'km/h'
				//text: mainItem.isLive
				onEditingFinished: {
					if(mainItem.isLive) {
						if(!mainItem.serieModel.isDone)
							mainItem.serieModel.targetSerieModel.speed = newValue
						mainItem.serieModel.resultSerieModel.speed = newValue
					}else
						mainItem.serieModel.speed = newValue
				}
				//onImplicitHeightChanged: fieldsList.updateImplicit()
			}

			TextField{
				id: restTextField
				Layout.preferredWidth: mainItem.width / contentLayout.visibleChildren.length
				Layout.fillHeight: true
				Layout.rightMargin: 10
				visible: !mainItem.trainingResultEdition
				keepEditView: mainItem.keepEditView
				showTitle: mainItem.showTitle || mainItem.trainingResultEdition
				readOnly:  mainItem.isReadOnly || mainItem.isLive
				inputMethodHints: Qt.ImhTime
				elide: Text.ElideLeft
				title: mainItem.showTitle ? 'RestTime' : ''
				placeholderText: 'hh:mm:ss'

				text: mainItem.isLive
								? mainItem.serieModel.isDone
									? mainItem.serieModel.resultSerieModel.restTimeStr
									: mainItem.serieModel.targetSerieModel.restTimeStr
								: mainItem.serieModel.restTimeStr
				onEditingFinished: {
					if(mainItem.isLive) {
						if(!mainItem.serieModel.isDone)
							mainItem.serieModel.targetSerieModel.restTimeStr = newValue
						mainItem.serieModel.resultSerieModel.restTimeStr = newValue
					}else
						mainItem.serieModel.restTimeStr = newValue
				}

			}
			TextField{
				id: workTextField
				Layout.fillHeight: true
				Layout.rightMargin: 10
				Layout.preferredWidth: mainItem.width / contentLayout.visibleChildren.length

				visible: mainItem.showWorkTime && ( !!serieModel?.isSaved || !!serieModel?.isDone)
				showTitle: mainItem.showTitle || mainItem.trainingResultEdition
				readOnly:  mainItem.isReadOnly
				inputMethodHints: Qt.ImhTime
				elide: Text.ElideLeft
				title: mainItem.showTitle ? 'WorkTime' : ''
				placeholderText: 'hh:mm:ss'
				//textColor: !mainItem.isLive || serieModel.restTime <= serieModel.targetWork.restTime ? Material.foreground : Material.accent
				text: visible
						? mainItem.isLive
							? serieModel?.isDone
								? serieModel.resultSerieModel.workTimeStr
								: serieModel.targetSerieModel.workTimeStr
							: serieModel?.workTimeStr || ''
						: ''
				onEditingFinished: {
					if(mainItem.isLive) {
						if(!mainItem.serieModel.isDone)
							mainItem.serieModel.targetSerieModel.workTimeStr = newValue
						mainItem.serieModel.resultSerieModel.workTimeStr = newValue
					}else
						mainItem.serieModel.workTimeStr = newValue
				}
			}
			TextField{
				id: calorieTextField
				Layout.fillHeight: true
				Layout.rightMargin: 10
				Layout.preferredWidth: mainItem.width / contentLayout.visibleChildren.length

				visible: mainItem.showCalories && (!!serieModel?.isSaved || !!serieModel?.isDone) ||  mainItem.trainingResultEdition
				showTitle: mainItem.showTitle || mainItem.trainingResultEdition
				readOnly:  mainItem.isReadOnly
				inputMethodHints: Qt.ImhDigitsOnly
				elide: Text.ElideRight
				title: mainItem.showTitle ? 'Calories' : ''
				placeholderText: 'kcal'
				text: visible
						? mainItem.isLive
							? serieModel?.isDone
								? Number.parseFloat(serieModel.resultSerieModel.calories.toFixed(2))
								: serieModel.targetSerieModel.calories
							: Number.parseFloat(serieModel?.calories.toFixed(2)) || ''
						: ''
				onEditingFinished: {
					if(mainItem.isLive) {
						if(!mainItem.serieModel.isDone)
							mainItem.serieModel.targetSerieModel.calories = newValue
						mainItem.serieModel.resultSerieModel.calories = newValue
					}else
						mainItem.serieModel.calories = newValue
				}
			}
			Button{
				Layout.fillWidth: true
				Layout.preferredWidth: mainItem.width / contentLayout.visibleChildren.length
				visible: !mainItem.isReadOnly && calorieTextField.visible && !mainItem.isLive && !mainItem.serieModel.isRunning
				text: 'Cal'
				onClicked:{
						serieModel.computeCalories()
				}
			}
			Rectangle{
				Layout.alignment: Qt.AlignCenter
				Layout.preferredHeight: 30
				Layout.preferredWidth: 30
				color: Material.primary
				visible: mainItem.showDecrementOrderChange && !serieModel?.isSaved && !serieModel?.isDone && !mainItem.serieModel.isRunning

				radius: width / 2
				Text{
					anchors.centerIn: parent
					color: 'white'
					text: '+'
				}
				MouseArea{
					anchors.fill: parent
					onClicked:{
						mainItem.exerciseUnitModel.decrementSerieOrder(mainItem.serieModel)
					}
				}
			}
			Rectangle{
				Layout.alignment: Qt.AlignCenter
				Layout.preferredHeight: 30
				Layout.preferredWidth: 30
				visible: mainItem.showIncrementOrderChange && !serieModel?.isSaved && !serieModel?.isDone && !mainItem.serieModel.isRunning
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
						mainItem.exerciseUnitModel.incrementSerieOrder(mainItem.serieModel)
					}
				}
			}
			Button{
				id: saveButton
				Layout.preferredWidth: 60
				visible: !mainItem.isReadOnly && mainItem.showSaveButton && serieModel?.isEdited && !mainItem.serieModel.isRunning || false
				text: 'Save'
				onClicked: {
					if(!mainItem.trainingResultEdition && mainItem.doSave) {
						serieModel.save()
					}
					mainItem.currentIndex = 0
					mainItem.keepEditView = false
				}
			}
		}
	}
	Rectangle{
		width: mainItem.width
		height: mainItem.height
		color: Material.primary
		RowLayout{
			anchors.fill: parent
			Item{Layout.fillWidth: true}
			Button{
				id: editButton
				Layout.preferredWidth: 60
				visible: !mainItem.isReadOnly && !saveButton.visible || false
				text: 'Edit'
				onClicked: {
					mainItem.currentIndex = 0
					mainItem.keepEditView = true
				}
			}


			ButtonImage{
				id: deleteButton
				Layout.preferredWidth: 30
				Layout.preferredHeight: 30
				Layout.rightMargin: 10
				visible: !mainItem.isReadOnly && mainItem.isDeletable && !!mainItem.exerciseUnitModel && !mainItem.serieModel.isRunning && !mainItem.serieModel.isDone
				imageSource: DefaultStyle.deleteButton
				colorizationColor: Material.foreground
				onClicked: {
					mainItem.exerciseUnitModel.removeSerie(serieModel)
				}
			}
		}
	}
}

