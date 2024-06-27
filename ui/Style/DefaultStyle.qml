pragma Singleton
import QtQuick
import QtQuick.Controls.Material


QtObject {
	property int theme: Material.Dark
	//property color primary: Material.primary
	property color primary: "#00BCD4"
	property color primary_dark: "#0097A7"
	property color primary_light: "#B2EBF2"
	property color accent: "#FF5722"
	property color primary_text: "#212121"
	property color secondary_text: "#757575"
	property color icons: "#FFFFFF"
	property color divider: "#BDBDBD"
	property double dp: Screen.pixelDensity


	property string emojiFont: "Noto Color Emoji"
	property string defaultFont: "Noto Sans"
	property string titleFont: "Noto Serif"

	property color numericPadPressedButtonColor: "#EEF7F8"

	property color groupCallButtonColor: "#EEF7F8"

	property string deleteButton: 'image://internal/delete.svg'
	property string editButton: 'image://internal/edit.svg'
	property string caloriesComputationButton: 'image://internal/calories.svg'
	property string speedComputationButton: 'image://internal/speed.svg'
	property string saveButton: 'image://internal/save.svg'
	property string undoButton: 'image://internal/undo.svg'
	property string stopwatchButton: 'image://internal/stopwatch.svg'
	property string addFoodButton: 'image://internal/add_food.svg'
	property string searchOFFButton: 'image://internal/search_off.svg'
	property string searchOFFDarkButton: 'image://internal/search_off_dark.svg'
	property string searchDatabaseButton: 'image://internal/search_database.svg'
	property string menuButton: 'image://internal/menu.svg'
	property string backButton: 'image://internal/back.svg'
	property string optionsButton: 'image://internal/options.svg'
	property string createButton: 'image://internal/create.svg'
	property string barcodeButton: 'image://internal/barcode.svg'

}
