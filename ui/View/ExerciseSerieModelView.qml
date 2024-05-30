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
	property var exerciseUnitModel
	property bool isLive: !!serieModel?.targetSerieModel
	property bool resting: !!serieModel?.isResting
	property bool isReadOnly: isLive
	property bool isDeletable: true
	property bool showSaveButton: true
	property bool showTitle: true
	property bool showWorkTime: true
	property bool showCalories: true
	property bool showOrderChange: false
	property bool trainingResultEdition: false
	property bool doSave: true
	property int margin: 0
	property bool keepEditView: false

	implicitHeight: contentLayout.implicitHeight
	Rectangle{
		anchors.fill: parent
		visible: mainItem.isLive && ( serieModel?.isRunning || serieModel?.isDone)
		color: serieModel?.isResting
					? Material.color(Material.Yellow, Material.Shade100)
					: serieModel?.resultSerieModel?.isSaved
						? Material.color(Material.Green, Material.Shade100)
						: serieModel?.isDone
							? Material.color(Material.Blue, Material.Shade100)
							: Material.color(Material.Amber, Material.Shade300)

	}
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
							?  (isGood ? '' : mainItem.serieModel.targetSerieModel.weight+'/') + mainItem.serieModel.resultSerieModel.weight
							: mainItem.serieModel.targetSerieModel.weight
						: mainItem.serieModel.weight <0 ? '' : mainItem.serieModel.weight
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
							?  (isGood ? '' : mainItem.serieModel.targetSerieModel.distance+'/') + mainItem.serieModel.resultSerieModel.distance
							: mainItem.serieModel.targetSerieModel.distance
						: mainItem.serieModel.distance <0 ? '' : mainItem.serieModel.distance
			placeholderText: 'm'
			//text: mainItem.isLive
			onEditingFinished: {
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
							? (isGood ? '' : mainItem.serieModel.targetSerieModel.speed+'/') + mainItem.serieModel.resultSerieModel.speed
							: mainItem.serieModel.targetSerieModel.speed
						: mainItem.serieModel.speed <0 ? '' : mainItem.serieModel.speed
			placeholderText: 'km/h'
			//text: mainItem.isLive
			onEditingFinished: {
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
			onEditingFinished: mainItem.serieModel.workTimeStr = newValue
			//onEditingFinished: if(!mainItem.showSaveButton) serieModel.restTime = newValue
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
							? serieModel.resultSerieModel.calories
							: serieModel.targetSerieModel.calories
						: serieModel?.calories || ''
					: ''
			onEditingFinished: mainItem.serieModel.calories = newValue
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
			visible: mainItem.showOrderChange && !serieModel?.isSaved && !serieModel?.isDone && !mainItem.serieModel.isRunning

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
			visible: mainItem.showOrderChange && !serieModel?.isSaved && !serieModel?.isDone && !mainItem.serieModel.isRunning
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
			Layout.fillWidth: true
			Layout.preferredWidth: mainItem.width / contentLayout.visibleChildren.length
			visible: !mainItem.isReadOnly && mainItem.showSaveButton && serieModel?.isEdited && !mainItem.serieModel.isRunning || false
			text: 'Save'
			onClicked: {
				if(!mainItem.trainingResultEdition && mainItem.doSave) {
					serieModel.save()
				}
			}
		}
		Button{
			id: deleteButton
			Layout.fillWidth: true
			Layout.preferredWidth: mainItem.width / contentLayout.visibleChildren.length
			visible: !mainItem.isReadOnly && mainItem.isDeletable && !!mainItem.exerciseUnitModel && !mainItem.serieModel.isRunning && !mainItem.serieModel.isDone
			text: 'D'
			onClicked: {
				mainItem.exerciseUnitModel.removeSerie(serieModel)
			}
		}


	}
}

