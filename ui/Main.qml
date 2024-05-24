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
	width: 360
	height: 740
	visible: true
	title: qsTr("Nutrixious")

	color: Material.background
	Material.theme: DefaultStyle.theme
	property string titleText : 'Programs'

	property bool gShowMenuButton: true


	header: ToolBar{
		id: toolBar
		z: -1
		showOptionsButton: false
		showBackButton: mainView.depth > 1
		showMenuButton: mainView.depth == 1 && gShowMenuButton
		title: menuPanel.show ? '' : mainItem.titleText
		onBack: {mainView.pop()}
		onMenu:{menuPanel.show = true}
	}


	StackView{
		id: mainView
		anchors.fill: parent
		initialItem: programsPage
		//TrainingPage{}

		//OverviewPage{}
		//DiaryPage{}


		Component{
			id: programsPage
			ProgramsPage{}
		}
		Component{
			id: trainingPage
			TrainingPage{}
		}
		Component{
			id: trainingsPage
			TrainingsPage{}
		}
		Component{
			id: statisticsPage
			StatisticsPage{}
		}
		Component{
			id: personalDataPage
			PersonalDataPage{}
		}
		Component{
			id: settingsPage
			SettingsPage{}
		}
		/*
Component{
			id: exercisesPage
			ExercisesPage{}
		}


		Component{
			id: programsPage
			ProgramsPage{}
		}


		//
		*/
		Component{
			id: foodPage
			FoodPage{}
		}
	}
	Item{
		id: menuPanel
		anchors.fill: parent
		anchors.topMargin: -toolBar.height
		property bool show: false
		visible: false

		Rectangle{
			anchors.fill: parent
			opacity: 0.5
			color: 'black'
			MouseArea{
				anchors.fill: parent
				hoverEnabled: true
				onClicked: menuPanel.show = false
			}
		}
		Rectangle{
			id: menuLayout
			anchors.top: parent.top
			anchors.bottom: parent.bottom
			width: Math.max(2 * mainItem.width / 3, logoLayout.implicitWidth + 50)
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
					id: menuItems
					//model:[{title: 'Overview'},{title: 'Diary'},{title: 'Food'},{title: 'Training'}, {title: 'Exercises'},{title: 'Programs'},{title: 'Trainings'},{title: 'Statistics'},{title: 'Settings'}]
					model:[{title: 'Food', component:foodPage}
						,{title: 'Training', component:trainingPage}
						//, {title: 'Exercises', component:exercisesPage}
						,{title: 'Programs', component:programsPage}
						,{title: 'Trainings', component:trainingsPage}
						,{title: 'Statistics', component:statisticsPage}
						,{title: 'Personal Data', component:personalDataPage}
						,{title: 'Settings', component:settingsPage}
						]
					delegate: MouseArea{
						Layout.fillWidth: true
						Layout.preferredHeight: (menuPanel.height - 150)/ menuItems.count
						onClicked: {
							//mainView.currentIndex = index
							mainView.replace(modelData.component)
							mainItem.titleText = modelData.title
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
	Item{
		height: 100
		width: Math.min(450, parent.width)
		anchors.top: parent.top
		anchors.left: parent.left
		visible: toolBar.displayStopwatch
		Stopwatch{
			anchors.fill: parent
		}
	}

}
