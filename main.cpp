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



/*
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

*/

/*
#include "src/activity/base/exercise/ExerciseProxyModel.h"
#include "src/activity/base/exercise/ExerciseListModel.h"
#include "src/activity/base/exercise/ExerciseModel.h"

#include "src/activity/base/serie/SerieProxyModel.h"
#include "src/activity/base/serie/SerieListModel.h"
#include "src/activity/base/serie/SerieModel.h"
*/
#include "src/database/program/serie/ProgramSerieModel.h"
#include "src/database/program/exercise/ProgramExerciseModel.h"
#include "src/database/program/ProgramModel.h"
#include "src/database/program/ProgramProxyModel.h"
#include "src/database/program/ProgramListModel.h"
#include "src/database/exercise/ExerciseModel.h"
#include "src/database/exercise/ExerciseListModel.h"
#include "src/database/exercise/ExerciseProxyModel.h"

#include "src/database/training/serie/TrainingSerieModel.h"
#include "src/database/training/exercise/TrainingExerciseModel.h"
#include "src/database/training/TrainingModel.h"
#include "src/database/training/TrainingProxyModel.h"
#include "src/database/training/TrainingListModel.h"

#include "src/database/personal/PersonalDataModel.h"
#include "src/database/personal/PersonalDataProxyModel.h"
#include "src/database/personal/PersonalDataListModel.h"


#include "src/activity/working/WorkingModel.h"
#include "src/activity/working/WorkingExerciseModel.h"
#include "src/activity/working/WorkingSerieModel.h"

#include "src/stat/StatsModel.h"


#include "src/tool/QmlData.h"
#include "src/tool/QmlModel.h"

void registerTypes(){
	qmlRegisterType<SBarcodeScanner>("com.scythestudio.scodes", 1, 0, "SBarcodeScanner");
	qmlRegisterType<FoodModel>("App", 1, 0, "FoodModel");

	qmlRegisterUncreatableType<QmlData>("App", 1, 0, "QmlData", "");
	qmlRegisterUncreatableType<QmlModel>("App", 1, 0, "QmlModel", "");

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

	qmlRegisterType<StatsModel>("App", 1, 0, "StatsModel");

	/*
	qmlRegisterType<Base::ExerciseModel>("App", 1, 0, "ExerciseModel");
	qmlRegisterType<Base::ExerciseListModel>("App", 1, 0, "ExerciseListModel");
	qmlRegisterType<Base::ExerciseProxyModel>("App", 1, 0, "ExerciseProxyModel");

	qmlRegisterType<Base::SerieModel>("App", 1, 0, "SerieModel");
	qRegisterMetaType<Base::SerieModel*>("SerieModel*");
	qmlRegisterType<Base::SerieListModel>("App", 1, 0, "SerieListModel");
	qmlRegisterType<Base::SerieProxyModel>("App", 1, 0, "SerieProxyModel");
*/


/*
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


	*/
}

static QQmlApplicationEngine * gEngine = nullptr;

#include <QApplication>
#include <QStyle>

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);	// Not QGuiApplication because of Qt Chart that will crash on QApplication::style()
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
