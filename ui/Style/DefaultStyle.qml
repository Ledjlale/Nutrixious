pragma Singleton
import QtQuick
import QtQuick.Controls.Material


QtObject {
	property int theme: Material.Dark
	property double dp: Screen.pixelDensity

	property string emojiFont: "Noto Color Emoji"
	property string defaultFont: "Noto Sans"
	property string titleFont: "Noto Serif"

	property color mealGroupSectionColor: Material.primary //Material.color(Material.BlueGrey)

	property string deleteButton: 'image://internal/delete.svg'
	property string editButton: 'image://internal/edit.svg'
	property string caloriesComputationButton: 'image://internal/calories.svg'
	property string speedComputationButton: 'image://internal/speed.svg'
	property string saveButton: 'image://internal/save.svg'
	property string undoButton: 'image://internal/undo.svg'
	property string stopwatchButton: 'image://internal/stopwatch.svg'
	property string addFoodButton: 'image://internal/add_food.svg'
	property string addDataButton: addFoodButton
	property string addProgramButton: addFoodButton
	property string addExerciseButton: 'image://internal/add_food.svg'
	property string searchOFFButton: 'image://internal/search_off.svg'
	property string searchOFFDarkButton: 'image://internal/search_off_dark.svg'
	property string searchDatabaseButton: 'image://internal/search_database.svg'
	property string menuButton: 'image://internal/menu.svg'
	property string backButton: 'image://internal/back.svg'
	property string optionsButton: 'image://internal/options.svg'
	property string createButton: 'image://internal/create.svg'
	property string barcodeButton: 'image://internal/barcode.svg'
	property string playButton: 'image://internal/play.svg'
	property string stopButton: 'image://internal/stop.svg'
	property string startupButton: 'image://internal/startup.svg'
	property string landingButton: 'image://internal/landing.svg'
	property string resumeButton: 'image://internal/resume.svg'
	property string bodyButton: 'image://internal/body.svg'
	

}
