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

#include <SBarcodeScanner.h>

#include "src/database/DatabaseModel.h"
#include "src/food/FoodModel.h"
#include "src/activity/description/sport/DistanceModel.h"
#include "src/activity/description/sport/StepsModel.h"
#include "src/activity/description/sport/StrengthModel.h"
#include "src/activity/description/sport/StrengthWorkModel.h"

#include "src/activity/description/exercise/ExerciseProxyModel.h"
#include "src/activity/description/exercise/ExerciseListModel.h"
#include "src/activity/description/exercise/ExerciseModel.h"

#include "src/activity/description/program/ProgramProxyModel.h"
#include "src/activity/description/program/ProgramListModel.h"
#include "src/activity/description/program/ProgramModel.h"

#include "src/activity/training/sport/DistanceModel.h"
#include "src/activity/training/sport/StepsModel.h"
#include "src/activity/training/sport/StrengthModel.h"
#include "src/activity/training/sport/StrengthWorkModel.h"

#include "src/activity/training/exercise/ExerciseProxyModel.h"
#include "src/activity/training/exercise/ExerciseListModel.h"
#include "src/activity/training/exercise/ExerciseModel.h"

#include "src/activity/training/TrainingModel.h"

#include "src/activity/training/train/TrainProxyModel.h"
#include "src/activity/training/train/TrainListModel.h"
#include "src/activity/training/train/TrainModel.h"


void registerTypes(){
	qmlRegisterType<SBarcodeScanner>("com.scythestudio.scodes", 1, 0, "SBarcodeScanner");
	qmlRegisterType<FoodModel>("App", 1, 0, "FoodModel");

	qmlRegisterType<Description::DistanceModel>("App", 1, 0, "DistanceModel");
	qmlRegisterType<Description::StepsModel>("App", 1, 0, "StepsModel");
	qmlRegisterType<Description::StrengthModel>("App", 1, 0, "StrengthModel");
	qmlRegisterType<Description::StrengthWorkModel>("App", 1, 0, "StrengthWorkModel");
	qRegisterMetaType<Description::StrengthWorkModel*>("StrengthWorkModel*");

	qmlRegisterType<Description::ExerciseModel>("App", 1, 0, "ExerciseModel");
	qmlRegisterType<Description::ExerciseListModel>("App", 1, 0, "ExerciseListModel");
	qmlRegisterType<Description::ExerciseProxyModel>("App", 1, 0, "ExerciseProxyModel");

	qmlRegisterType<Description::ProgramModel>("App", 1, 0, "ProgramModel");
	qmlRegisterType<Description::ProgramListModel>("App", 1, 0, "ProgramListModel");
	qmlRegisterType<Description::ProgramProxyModel>("App", 1, 0, "ProgramProxyModel");

	qmlRegisterType<Training::TrainingModel>("App", 1, 0, "TrainingModel");

	qmlRegisterType<Training::DistanceModel>("App.Training", 1, 0, "DistanceModel");
	qmlRegisterType<Training::StepsModel>("App.Training", 1, 0, "StepsModel");
	qmlRegisterType<Training::StrengthModel>("App.Training", 1, 0, "StrengthModel");
	qmlRegisterType<Training::StrengthWorkModel>("App.Training", 1, 0, "StrengthWorkModel");

	qmlRegisterType<Training::ExerciseModel>("App.Training", 1, 0, "ExerciseModel");
	qmlRegisterType<Training::ExerciseListModel>("App.Training", 1, 0, "ExerciseListModel");
	qmlRegisterType<Training::ExerciseProxyModel>("App.Training", 1, 0, "ExerciseProxyModel");

	qmlRegisterType<Training::TrainModel>("App.Training", 1, 0, "TrainModel");
	qmlRegisterType<Training::TrainListModel>("App.Training", 1, 0, "TrainListModel");
	qmlRegisterType<Training::TrainProxyModel>("App.Training", 1, 0, "TrainProxyModel");

}

static QQmlApplicationEngine * gEngine = nullptr;

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
	// Ignore vertical sync. This way, we avoid blinking on resizes(and other refresh steps like layouts etc.).
	auto ignoreVSync = QSurfaceFormat::defaultFormat();
	ignoreVSync.setSwapInterval(0);
	QSurfaceFormat::setDefaultFormat(ignoreVSync);
	DatabaseModel::migrate();

	QStringList selectors("custom");
	gEngine = new QQmlApplicationEngine();
	auto selector = new QQmlFileSelector(gEngine, gEngine);
	selector->setExtraSelectors(selectors);
	gEngine->addImportPath(":/");
	const QUrl url(u"qrc:/App/ui/Main.qml"_qs);
    QObject::connect(
		gEngine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
		Qt::QueuedConnection);
	QQuickStyle::setStyle("Material");
	registerTypes();
	gEngine->load(url);
	int result = app.exec();
	gEngine->deleteLater();
	return result;
}
