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
	property bool trainingResultEdition: false
	property bool doSave: true
	property int leftMargin: 0
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
							: Material.color(Material.Amber, Material.Shade100)

	}
	RowLayout{
		id: contentLayout
		anchors.fill: parent
		anchors.leftMargin: mainItem.leftMargin
		property int _visibleChildren : visibleChildren.length - 1
		spacing: 0
		TextField{
			height: implicitHeight
			Layout.preferredWidth: mainItem.width / contentLayout.visibleChildren.length
			visible: mainItem.isLive || mainItem.trainingResultEdition ? exerciseUnitModel.targetExerciseModel.useRepetitions : exerciseUnitModel.useRepetitions
			keepEditView: mainItem.keepEditView
			showTitle: mainItem.showTitle
			readOnly: mainItem.isReadOnly
			inputMethodHints: Qt.ImhDigitsOnly
			title: mainItem.showTitle ? 'Reps' : ''
			text: mainItem.isLive
						? mainItem.serieModel.isDone
							? mainItem.serieModel.resultSerieModel.repetitions
							: mainItem.serieModel.targetSerieModel.repetitions
						: mainItem.serieModel.repetitions <0 ? '' : mainItem.serieModel.repetitions
			//text: mainItem.isLive
			onEditingFinished: {
				mainItem.serieModel.repetitions = newValue
			}
			//onImplicitHeightChanged: fieldsList.updateImplicit()
		}
		TextField{
			height: implicitHeight
			Layout.preferredWidth: mainItem.width / contentLayout.visibleChildren.length
			visible: mainItem.isLive || mainItem.trainingResultEdition ? exerciseUnitModel.targetExerciseModel.useWeight :exerciseUnitModel.useWeight
			keepEditView: mainItem.keepEditView
			showTitle: mainItem.showTitle
			readOnly: mainItem.isReadOnly
			inputMethodHints: Qt.ImhDigitsOnly
			title: mainItem.showTitle ? 'Weight' : ''
			text: mainItem.isLive
						? mainItem.serieModel.isDone
							? mainItem.serieModel.resultSerieModel.weight
							: mainItem.serieModel.targetSerieModel.weight
						: mainItem.serieModel.weight <0 ? '' : mainItem.serieModel.weight
			placeholderText: 'kg'
			//text: mainItem.isLive
			onEditingFinished: {
				mainItem.serieModel.weight = newValue
			}
			//onImplicitHeightChanged: fieldsList.updateImplicit()
		}
		TextField{
			height: implicitHeight
			Layout.preferredWidth: mainItem.width / contentLayout.visibleChildren.length
			visible: mainItem.isLive || mainItem.trainingResultEdition ? exerciseUnitModel.targetExerciseModel.useDistance :exerciseUnitModel.useDistance
			keepEditView: mainItem.keepEditView
			showTitle: mainItem.showTitle
			readOnly: mainItem.isReadOnly
			inputMethodHints: Qt.ImhDigitsOnly
			title: mainItem.showTitle ? 'Distance' : ''
			text: mainItem.isLive
						? mainItem.serieModel.isDone
							? mainItem.serieModel.resultSerieModel.distance
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
			height: implicitHeight
			Layout.preferredWidth: mainItem.width / contentLayout.visibleChildren.length
			visible: mainItem.isLive || mainItem.trainingResultEdition ? exerciseUnitModel.targetExerciseModel.useSpeed :exerciseUnitModel.useSpeed
			keepEditView: mainItem.keepEditView
			showTitle: mainItem.showTitle
			readOnly: mainItem.isReadOnly
			inputMethodHints: Qt.ImhDigitsOnly
			title: mainItem.showTitle ? 'Speed' : ''
			text: mainItem.isLive
						? mainItem.serieModel.isDone
							? mainItem.serieModel.resultSerieModel.speed
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
			inputMethodHints: Qt.ImhDigitsOnly
			title: mainItem.showTitle ? 'RestTime(s)' : ''
			placeholderText: 's'
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
			Layout.preferredWidth: mainItem.width / contentLayout.visibleChildren.length

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
			Layout.preferredWidth: mainItem.width / contentLayout.visibleChildren.length
			visible: !mainItem.isReadOnly && mainItem.showSaveButton && serieModel?.isEdited || false
			text: 'Save'
			onClicked: {
				if(!mainItem.trainingResultEdition && mainItem.doSave) {
					serieModel.save()
				}
			}
		}
		Button{
			id: deleteButton
			Layout.preferredWidth: mainItem.width / contentLayout.visibleChildren.length
			visible: !mainItem.isReadOnly && mainItem.isDeletable && !!mainItem.exerciseUnitModel
			text: 'D'
			onClicked: {
				mainItem.exerciseUnitModel.removeSerie(serieModel)
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

