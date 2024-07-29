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

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QQmlFileSelector>
#include <QApplication>
#include <QStyle>
#include "src/tool/logger/Logger.h"

#include <SBarcodeScanner.h>

#include "src/database/DatabaseModel.h"



#include "src/database/program/serie/ProgramSerieModel.h"
#include "src/database/program/exercise/ProgramExerciseModel.h"
#include "src/database/program/ProgramModel.h"
#include "src/database/program/ProgramProxyModel.h"
#include "src/database/program/ProgramListModel.h"
#include "src/database/exercise/ExerciseModel.h"
#include "src/database/exercise/ExerciseListModel.h"
#include "src/database/exercise/ExerciseProxyModel.h"

#include "src/database/food/FoodModel.h"
#include "src/database/food/FoodListModel.h"
#include "src/database/food/FoodProxyModel.h"

#include "src/database/mealGroup/MealGroupModel.h"
#include "src/database/mealGroup/MealGroupListModel.h"
#include "src/database/mealGroup/MealGroupProxyModel.h"

#include "src/database/mealFood/MealFoodModel.h"
#include "src/database/mealFood/MealFoodListModel.h"
#include "src/database/mealFood/MealFoodProxyModel.h"

#include "src/database/training/serie/TrainingSerieModel.h"
#include "src/database/training/exercise/TrainingExerciseModel.h"
#include "src/database/training/TrainingModel.h"
#include "src/database/training/TrainingProxyModel.h"
#include "src/database/training/TrainingListModel.h"

#include "src/database/personal/PersonalDataModel.h"
#include "src/database/personal/PersonalDataProxyModel.h"
#include "src/database/personal/PersonalDataListModel.h"

#include "src/database/unit/UnitModel.h"
#include "src/database/unit/UnitListModel.h"
#include "src/database/unit/UnitProxyModel.h"

#include "src/activity/working/WorkingModel.h"
#include "src/activity/working/WorkingExerciseModel.h"
#include "src/activity/working/WorkingSerieModel.h"

#include "src/setting/SettingsModel.h"

#include "src/stat/StatsModel.h"


#include "src/tool/QmlData.h"
#include "src/tool/QmlModel.h"
#include "src/tool/Utils.h"

#include "src/tool/proxyModel/ObjectListModel.h"
#include "src/tool/proxyModel/ObjectProxyModel.h"
#include "src/tool/provider/ImageProvider.hpp"

#include "src/tool/date/DateModel.h"

void registerTypes(){
	qmlRegisterType<SBarcodeScanner>("com.scythestudio.scodes", 1, 0, "SBarcodeScanner");
	//qmlRegisterType<FoodModel>("App", 1, 0, "FoodModel");

	qmlRegisterUncreatableType<QmlData>("App", 1, 0, "QmlData", "");
	qmlRegisterUncreatableType<QmlModel>("App", 1, 0, "QmlModel", "");

	qmlRegisterType<ObjectListModel>("App", 1, 0, "ObjectListModel");
	qmlRegisterType<ObjectProxyModel>("App", 1, 0, "ObjectProxyModel");

	qmlRegisterType<ExerciseModel>("App", 1, 0, "ExerciseModel");
	qRegisterMetaType<ExerciseModel*>("ExerciseModel*");
	qmlRegisterType<ExerciseListModel>("App", 1, 0, "ExerciseListModel");
	qmlRegisterType<ExerciseProxyModel>("App", 1, 0, "ExerciseProxyModel");
	qmlRegisterType<ProgramModel>("App", 1, 0, "ProgramModel");
	qRegisterMetaType<ProgramModel*>("ProgramModel*");
	qmlRegisterType<ProgramListModel>("App", 1, 0, "ProgramListModel");
	qmlRegisterType<ProgramProxyModel>("App", 1, 0, "ProgramProxyModel");
	qmlRegisterType<ProgramExerciseModel>("App", 1, 0, "ProgramExerciseModel");
	qRegisterMetaType<ProgramExerciseModel*>("ProgramExerciseModel*");
	qmlRegisterType<ProgramSerieModel>("App", 1, 0, "ProgramSerieModel");
	qRegisterMetaType<ProgramSerieModel*>("ProgramSerieModel*");

	qmlRegisterType<TrainingModel>("App", 1, 0, "TrainingModel");
	qRegisterMetaType<TrainingModel*>("TrainingModel*");
	qmlRegisterType<TrainingListModel>("App", 1, 0, "TrainingListModel");
	qmlRegisterType<TrainingProxyModel>("App", 1, 0, "TrainingProxyModel");
	qmlRegisterType<TrainingExerciseModel>("App", 1, 0, "TrainingExerciseModel");
	qRegisterMetaType<TrainingExerciseModel*>("TrainingExerciseModel*");
	qmlRegisterType<TrainingSerieModel>("App", 1, 0, "TrainingSerieModel");
	qRegisterMetaType<TrainingSerieModel*>("TrainingSerieModel*");


	qmlRegisterType<WorkingModel>("App", 1, 0, "WorkingModel");
	qRegisterMetaType<WorkingModel*>("WorkingModel*");
	qmlRegisterType<WorkingExerciseModel>("App", 1, 0, "WorkingExerciseModel");
	qRegisterMetaType<WorkingExerciseModel*>("WorkingExerciseModel*");
	qmlRegisterType<WorkingSerieModel>("App", 1, 0, "WorkingSerieModel");
	qRegisterMetaType<WorkingSerieModel*>("WorkingSerieModel*");

	qmlRegisterType<PersonalDataModel>("App", 1, 0, "PersonalDataModel");
	qRegisterMetaType<PersonalDataModel*>("PersonalDataModel*");
	qmlRegisterType<PersonalDataListModel>("App", 1, 0, "PersonalDataListModel");
	qmlRegisterType<PersonalDataProxyModel>("App", 1, 0, "PersonalDataProxyModel");

	qmlRegisterType<FoodModel>("App", 1, 0, "FoodModel");
	qRegisterMetaType<FoodModel*>("FoodModel*");
	qmlRegisterType<FoodListModel>("App", 1, 0, "FoodListModel");
	qmlRegisterType<FoodProxyModel>("App", 1, 0, "FoodProxyModel");

	qmlRegisterType<MealFoodModel>("App", 1, 0, "MealFoodModel");
	qRegisterMetaType<MealFoodModel*>("MealFoodModel*");
	qmlRegisterType<MealFoodListModel>("App", 1, 0, "MealFoodListModel");
	qmlRegisterType<MealFoodProxyModel>("App", 1, 0, "MealFoodProxyModel");

	qmlRegisterType<MealGroupModel>("App", 1, 0, "MealGroupModel");
	qRegisterMetaType<MealGroupModel*>("MealGroupModel*");
	qmlRegisterType<MealGroupListModel>("App", 1, 0, "MealGroupListModel");
	qmlRegisterType<MealGroupProxyModel>("App", 1, 0, "MealGroupProxyModel");



	qmlRegisterType<UnitModel>("App", 1, 0, "UnitModel");
	qRegisterMetaType<UnitModel*>("UnitModel*");
	qmlRegisterType<UnitListModel>("App", 1, 0, "UnitListModel");
	qmlRegisterType<UnitProxyModel>("App", 1, 0, "UnitProxyModel");

	//qRegisterMetaType<FoodModel*>("FoodModel*");
	//qmlRegisterType<FoodListModel>("App", 1, 0, "FoodListModel");
	//qmlRegisterType<FoodProxyModel>("App", 1, 0, "FoodProxyModel");

	qmlRegisterType<StatsModel>("App", 1, 0, "StatsModel");
	qmlRegisterType<DateModel>("App", 1, 0, "DateModel");

	qmlRegisterSingletonType<Logger>("App", 1,0, "Logger",
								[](QQmlEngine *engine, QJSEngine *) -> QObject * { return new Logger(engine); });
	qmlRegisterSingletonType<Logger>("App", 1,0, "UtilsCpp",
								[](QQmlEngine *engine, QJSEngine *) -> QObject * { return new Utils(engine); });
	qmlRegisterSingletonType<SettingsModel>("App", 1,0, "SettingsCpp",
								[](QQmlEngine *engine, QJSEngine *) -> QObject * { return new SettingsModel(engine); });
}

static QQmlApplicationEngine * gEngine = nullptr;


int main(int argc, char *argv[]) {
	int result = -1;
	Logger::init();
	{
		QApplication app(argc, argv);	// Not QGuiApplication because of Qt Chart that will crash on QApplication::style()
		// Ignore vertical sync. This way, we avoid blinking on resizes(and other refresh steps like layouts etc.).
		auto ignoreVSync = QSurfaceFormat::defaultFormat();
		ignoreVSync.setSwapInterval(0);
		QSurfaceFormat::setDefaultFormat(ignoreVSync);
		DatabaseModel::migrate();
		app.setWindowIcon(QIcon(":/asset/icons/nutrixious_logo.svg"));
	
		QStringList selectors("custom");
		gEngine = new QQmlApplicationEngine(&app);
		auto selector = new QQmlFileSelector(gEngine, gEngine);
		selector->setExtraSelectors(selectors);
		gEngine->addImportPath(":/");
		gEngine->addImageProvider(ImageProvider::ProviderId, new ImageProvider());
		gEngine->rootContext()->setContextProperty("applicationUrl", APPLICATION_URL);
		gEngine->rootContext()->setContextProperty("applicationVersionName", APPLICATION_VERSION_NAME);
		gEngine->rootContext()->setContextProperty("applicationVersion", APPLICATION_VERSION);
		const QUrl url(u"qrc:/App/ui/Main.qml"_qs);
		QObject::connect(
			gEngine,
			&QQmlApplicationEngine::objectCreationFailed,
			&app,
			[]() { QCoreApplication::exit(-1); },
			Qt::QueuedConnection);
		QObject::connect(gEngine, &QQmlApplicationEngine::quit,&app, QCoreApplication::quit);
		QQuickStyle::setStyle("Material");
		registerTypes();
		gEngine->load(url);
	#ifdef QT_DEBUG
		const QUrl colorPicker(u"qrc:/App/ui/Tool/ColorPicker.qml"_qs);
		
		//gEngine->load(colorPicker);
	#endif
		result = app.exec();
		qInfo() << "Deleting QML engine before exit";
		delete  gEngine;// If not, there will be a deadlock from QPixmapReader
	}
	qInfo() << "Exiting application with code " << result;
	return result;
}
