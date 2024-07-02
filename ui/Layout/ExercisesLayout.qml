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

StackView{
	id: mainItem
	property var lastHeaders: {'showBackButton': false,
			'showMenuButton': true,
			'showCreateButton':true,
			'title': 'Offline exercises'
		}
	property bool isHeaderOwner: true
	/*
	currentIndex: 0
	onCurrentIndexChanged: {
		lastHeaders = {'showBackButton':currentIndex > 0,
			'showMenuButton':currentIndex==0,
			'title' : currentIndex == 0 ? 'Offline exercises' : 'Exercise'
		}
		mainWindow.setHeaders(lastHeaders)
	}*/
	Component.onCompleted:{
		mainWindow.setHeaders(lastHeaders)
	}
	Connections{
		target: mainWindow
		enabled: mainItem.isHeaderOwner
		function onGBack(){
			mainWindow.setHeaders(lastHeaders)
			mainItem.pop()
		}
	}
	initialItem: ProgramsPage{
		id: programsPage
		onNewExerciseRequested:{
			mainItem.push(exerciseUnitEditorComponent);
			mainWindow.setHeaders({'showBackButton': true,
				'showMenuButton': false,
				'showSaveButton':true,
				'title': 'Create Exercise'
			});
		}
		onAddExerciseRequested: function(parameters){
			mainItem.push(exerciseUnitEditorComponent, parameters);
			mainWindow.setHeaders({'showBackButton': true,
				'showMenuButton': false,
				'showSaveButton':true,
				'title': 'Add Exercise'
			});
		}
		onEditExerciseRequested: function(parameters){
			mainItem.push(exerciseEditorComponent, parameters);
			mainWindow.setHeaders({'showBackButton': true,
				'showMenuButton': false,
				'title': 'Exercise Description'
			});
		}
		/*
		onExerciseRequested: currentIndex = 1
		onSetHeaders: function(headers) {
			isHeaderOwner = !headers
			mainWindow.setHeaders(headers ? headers : lastHeaders)
		}*/
	}
	Component{
		id: exerciseUnitEditorComponent
		ExerciseUnitEditorPage{
			programModel: programsPage.programModel
			mainProgramModel: programsPage.mainProgramModel
			showSaveButton: false
			onClosed: {
				mainWindow.setHeaders(lastHeaders)
				mainItem.pop()
			}
		}
	}
	Component{
		id: exerciseEditorComponent
		ExerciseEditorPage{
			onClosed: {
				mainWindow.setHeaders(lastHeaders)
				mainItem.pop()
			}
		}
	}
}
