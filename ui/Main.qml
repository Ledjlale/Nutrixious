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

ApplicationWindow {
	id: mainItem
	width: 640
	height: 960
	visible: true
	title: qsTr("Nutrixious")

	color: Material.background


	ColumnLayout{
		anchors.fill: parent
		spacing: 0

		SwipeView{
			id: mainView
			Layout.fillHeight: true
			Layout.fillWidth: true
			currentIndex: 4
			OverviewPage{}
			DiaryPage{}
			FoodPage{}
			TrainingPage{}
			ExercisesPage{}
			ProgramsPage{}
			TrainingsPage{}
			StatisticsPage{}
			SettingsPage{}
		}
	}
	Item{
		id: menuPanel
		anchors.fill: parent
		property bool show: false
		visible: false

		Rectangle{
			anchors.fill: parent
			opacity: 0.5
			color: 'black'
			MouseArea{
				anchors.fill: parent
				onClicked: menuPanel.show = false
			}
		}
		Rectangle{
			id: menuLayout
			anchors.top: parent.top
			anchors.bottom: parent.bottom
			width: 2 * mainItem.width / 3
			x: -width
			color: Material.primary

			SequentialAnimation {
				running: menuPanel.show
				NumberAnimation { target: menuPanel; property: "visible"; to: 1; duration: 0 }
				NumberAnimation { target: menuLayout; property: "x"; from: -width; to:0 ;duration: 200 }
			}
			SequentialAnimation {
				running: !menuPanel.show
				NumberAnimation { target: menuLayout; property: "x"; to: -width; from: 0;duration: 200 }
				NumberAnimation { target: menuPanel; property: "visible"; to: 0; duration: 0 }
			}
			ColumnLayout{
				anchors.fill: parent
				spacing: 0
				Item{
					Layout.fillWidth: true
					Layout.preferredHeight: 150
					Rectangle{
						height: logoLayout.implicitHeight
						width: logoLayout.implicitWidth + 50
						anchors.centerIn: parent
						radius: width / 2
						color: Material.primary
						border.color: 'white'
						RowLayout{
							id: logoLayout
							anchors.centerIn: parent
							spacing: 0
							property int pixelSize: 40
							Text{
								id: logoText1
								color: 'white'
								font.pixelSize: parent.pixelSize
								text: 'Nutri'
							}
							Text{
								//color: Material.color(Material.Green, Material.Shade100)
								color: Material.color(Material.Red)//, Material.Shade100)
								//Material.foreground: Material.Orange
								font.pixelSize: parent.pixelSize*2
								font.weight: Font.Bold
								text: 'X'
							}
							Text{
								color: 'white'
								font.pixelSize: parent.pixelSize
								text: 'ious'
							}
						}
					}
				}
				Repeater{
					model:[{title: 'Overview'},{title: 'Diary'},{title: 'Food'},{title: 'Training'}, {title: 'Exercises'},{title: 'Programs'},{title: 'Trainings'},{title: 'Statistics'},{title: 'Settings'}]
					delegate: MouseArea{
						Layout.fillWidth: true
						Layout.preferredHeight: 80
						onClicked: {
							mainView.currentIndex = index
							menuPanel.show = false
						}
						ColumnLayout{
							anchors.fill: parent
							spacing: 0
							Rectangle{
								Layout.fillWidth: true
								Layout.preferredHeight: 1
								visible: index != 0
								color: 'white'
							}
							RowLayout{
								Layout.fillWidth: true
								Layout.fillHeight: true
								spacing: 40
								Text{
									Layout.leftMargin: 10
									color: 'white'
									text: 'Icon'
								}
								Text{
									Layout.fillWidth: true
									horizontalAlignment: Text.AlignHCenter
									color: 'white'
									text: modelData.title
								}
							}
						}
					}
				}
				Item{
					Layout.fillWidth: true
					Layout.fillHeight: true
				}
			}
		}
	}

	Button{
		anchors.bottom: parent.bottom
		anchors.left: parent.left
		anchors.leftMargin: 20
		anchors.bottomMargin: 20
		visible: !menuPanel.visible
		onClicked: menuPanel.show = true
		text: 'Menu'
	}
}
