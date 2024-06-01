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

#include "DatabaseModel.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

#include "program/exercise/ProgramExerciseModel.h"
#include "program/serie/ProgramSerieModel.h"
#include "program/ProgramModel.h"
#include "exercise/ExerciseModel.h"

#include "unit/UnitModel.h"

DatabaseModel::DatabaseModel(QObject *parent)
	: QObject{parent}
{}

// ex_ => exercises

void DatabaseModel::migrate(){
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("data");
	//db.setDatabaseName(":memory:");
	if (!db.open()) {
		qCritical() << QObject::tr("Unable to establish a database connection.\n"
						"This example needs SQLite support. Please read "
						"the Qt SQL driver documentation for information how "
						"to build it.");
		return;
	}
	QSqlQuery query;
	int version = 0;
	query.exec("PRAGMA user_version");
	int fieldNo = query.record().indexOf("user_version");
	while (query.next()) {
		version = query.value(fieldNo).toInt();
		qDebug() << "Database Version detected : " << version;
	}
	if(version == 0){
		qDebug() << "Creating database";
		query.exec("PRAGMA foreign_keys=ON");

//----------------------------------------------------------------------------------------------------------------------
	if(!query.exec("CREATE TABLE exercises (exercise_id INTEGER PRIMARY KEY"
				", name TEXT"
				", met REAL"
				")")) qCritical() << "Cannot create an exercise table  : " << query.lastError().text();

// Programs : List of exercises
	if(!query.exec("CREATE TABLE programs (program_id INTEGER PRIMARY KEY"
				", name TEXT"
				", description TEXT)")) qCritical() << "Cannot create programs : " << query.lastError().text();

// Program Exercise = Exercise + program_id + exercise_order
	if(!query.exec("CREATE TABLE program_exercise_units (program_exercise_unit_id INTEGER PRIMARY KEY"
			", description TEXT"
			", exercise_order INTEGER"
			", program_id INTEGER NOT NULL"
			", exercise_id INTEGER"
			", use_distance INTEGER"
			", use_speed INTEGER"
			", use_weight INTEGER"
			", use_repetitions INTEGER"
			", FOREIGN KEY (program_id) REFERENCES programs (program_id) ON UPDATE CASCADE ON DELETE CASCADE"
			", FOREIGN KEY (exercise_id) REFERENCES exercises (exercise_id) ON UPDATE CASCADE ON DELETE SET NULL"
			")")) qCritical() << "Cannot create program exercises table : " << query.lastError().text();

// // Program Serie = Serie
	if(!query.exec("CREATE TABLE program_exercise_series (program_serie_id INTEGER PRIMARY KEY"
			", program_exercise_unit_id INTEGER NOT NULL"
			", serie_order INTEGER"
			", rest_time INTEGER"
			", work_time INTEGER"
			", distance INTEGER"
			", speed REAL"
			", weight REAL"
			", repetitions INTEGER"
			", FOREIGN KEY (program_exercise_unit_id) REFERENCES program_exercise_units (program_exercise_unit_id) ON UPDATE CASCADE ON DELETE CASCADE"

			")")) qCritical() << "Cannot create exercise series table : " << query.lastError().text();


// Training : List of exercises
	if(!query.exec("CREATE TABLE trainings (training_id INTEGER PRIMARY KEY"
				", name TEXT"
				", description TEXT"
				", start_date_time INTERGER)")) qCritical() << "Cannot create trainings: " << query.lastError().text();

// Training Exercise = Exercise + training_id + exercise_order
if(!query.exec("CREATE TABLE training_exercise_units (training_exercise_unit_id INTEGER PRIMARY KEY"
			", description TEXT"
			", exercise_order INTEGER"
			", training_id INTEGER NOT NULL"
			", exercise_id INTEGER"
			", use_distance INTEGER"
			", use_speed INTEGER"
			", use_weight INTEGER"
			", use_repetitions INTEGER"
			", FOREIGN KEY (training_id) REFERENCES trainings (training_id) ON UPDATE CASCADE ON DELETE CASCADE"
			", FOREIGN KEY (exercise_id) REFERENCES exercises (exercise_id) ON UPDATE CASCADE ON DELETE SET NULL"
			")")) qCritical() << "Cannot create training exercises table : " << query.lastError().text();

// Training Serie = Serie
	if(!query.exec("CREATE TABLE training_exercise_series (training_serie_id INTEGER PRIMARY KEY"
			", training_exercise_unit_id INTEGER NOT NULL"
			", serie_order INTEGER"
			", rest_time INTEGER"
			", work_time INTEGER"
			", distance INTEGER"
			", speed REAL"
			", weight INTEGER"
			", repetitions INTEGER"
			", FOREIGN KEY (training_exercise_unit_id) REFERENCES training_exercise_units (training_exercise_unit_id) ON UPDATE CASCADE ON DELETE CASCADE"

			")")) qCritical() << "Cannot create training series table : " << query.lastError().text();

//-----------------------------------------------------------------------------------------------------------------------

//					PERSONAL DATA

	if(!query.exec("CREATE TABLE personal_data (personal_data_id INTEGER PRIMARY KEY"
			", date_time INTEGER"
			", weight REAL"
			", sex INTEGER"
			", height INTEGER"
			")")) qCritical() << "Cannot create personal database : " << query.lastError().text();

//					EXERCISES STATS

		insertDefaultData();

		query.exec("CREATE TABLE version (version INTEGER)");
		query.exec("INSERT INTO version (version) VALUES(1)");

		query.exec("PRAGMA user_version=1");
	}else{// Migration
	/*
		if( version < 2){
			QList<QPair<qint64, QStringList>> data;
			if(!query.exec("SELECT data, program_exercise_id FROM program_exercises")) {
				qCritical() << "Cannot update database on program_exercise_id : "<< query.lastError().text();
				return;
			}
			auto dataField = query.record().indexOf("data");
			auto idField = query.record().indexOf("program_exercise_id");
			while(query.next()){
				data << QPair<qint64, QStringList>(query.value(dataField).toInt(),query.value(dataField).toString().split(","));
			}
			for(auto i : data){
				i.second << "
				query.exec("");
			}
		}*/
	}
	if(!query.exec("SELECT calories FROM training_exercise_series LIMIT 1")){
		if(!query.exec("ALTER TABLE training_exercise_series ADD COLUMN calories REAL DEFAULT 0")){
			qCritical() << "Cannot Add calories column into training_exercise_series: " << query.lastError().text();
		}else{

		}
	}
	if(!query.exec("SELECT * FROM units LIMIT 1")){
		if(!query.exec("CREATE TABLE units (unit_id INTEGER PRIMARY KEY"
			", name TEXT"
			", gram_value REAL DEFAULT 0"
			", meter_value REAL DEFAULT 0"
			", second_value REAL DEFAULT 0"
			", kcal_value REAL DEFAULT 0"
			", milliliter_value REAL DEFAULT 0"
			")"
		))
			qCritical() << "Cannot create units table: " << query.lastError().text();
		else{
			initUnitsData();
		}
	}
	if(!query.exec("SELECT * FROM foods LIMIT 1")){
		if(!query.exec("CREATE TABLE foods (food_id INTEGER PRIMARY KEY"
			", open_food_facts_code TEXT"
			", image_url TEXT"
			", brand TEXT"
			", description TEXT"
			", serving_size REAL"
			", servings_per_container REAL"
			", serving_unit_id INTEGER"
			", calories REAL"
			", total_fat REAL"
			", saturated_fat REAL"
			", trans_fat REAL"
			", poly_unsaturated_fat REAL"
			", mono_unsaturated_fat REAL"
			", cholesterol REAL"
			", sodium REAL"
			", total_carbohydrate REAL"
			", dietary_fiber REAL"
			", sugar REAL"
			", protein REAL"
			", calcium REAL"
			", iron REAL"
			", potassium REAL"
			", vitamin_a REAL"
			", vitamin_c REAL"
			", FOREIGN KEY (serving_unit_id) REFERENCES units (unit_id) ON UPDATE CASCADE ON DELETE SET NULL"
			")"
		))
			qCritical() << "Cannot create food table: " << query.lastError().text();
	}



	query.exec("PRAGMA user_version");
	fieldNo = query.record().indexOf("user_version");
	while (query.next()) {
		version = query.value(fieldNo).toInt();
		qDebug() << "Final Database Version : " << version;
	}
}

void DatabaseModel::initUnitsData(){
	QList<UnitModel*> models;
// Weight
	models << new UnitModel("g",1,0,0,0,0,nullptr);
	models << new UnitModel("kg",1000,0,0,0,0,nullptr);
	models << new UnitModel("mg",0.001,0,0,0,0,nullptr);
	models << new UnitModel("oz",28.3495,0,0,0,0,nullptr);
	models << new UnitModel("ounce",28.3495,0,0,0,0,nullptr);
	models << new UnitModel("lbs",453.592,0,0,0,0,nullptr);
	models << new UnitModel("pound",453.592,0,0,0,0,nullptr);

// Volume
	models << new UnitModel("ml",0,0,0,0,1,nullptr);
	models << new UnitModel("L",0,0,0,0,1000,nullptr);
	models << new UnitModel("cl",0,0,0,0,10,nullptr);

	models << new UnitModel("teaspoon",0,0,0,0,4.92892,nullptr);
	models << new UnitModel("tablespoon",0,0,0,0,14.7868,nullptr);
	models << new UnitModel("fl oz",0,0,0,0,29.5735,nullptr);
	models << new UnitModel("cup",0,0,0,0,240,nullptr);
	models << new UnitModel("pint",0,0,0,0,473.176,nullptr);
	models << new UnitModel("quart",0,0,0,0,946.353,nullptr);
	models << new UnitModel("gal",0,0,0,0,3785.41,nullptr);


// Distance
	models << new UnitModel("m",0,1,0,0,0,nullptr);
	models << new UnitModel("cm",0,0.01,0,0,0,nullptr);
	models << new UnitModel("mm",0,0.001,0,0,0,nullptr);
	models << new UnitModel("km",0,1000,0,0,0,nullptr);

// Calories
	models << new UnitModel("kcal",0,0,0,1,0,nullptr);
	models << new UnitModel("Cal",0,0,0,1,0,nullptr);
	models << new UnitModel("cal",0,0,0,0.001,0,nullptr);

// Time
	models << new UnitModel("s",0,0,1,0,0,nullptr);
	models << new UnitModel("min",0,0,60,0,0,nullptr);
	models << new UnitModel("h",0,0,3600,0,0,nullptr);

	for(auto i : models){
		i->save();
		i->deleteLater();
	}
}
void DatabaseModel::insertDefaultData() {
/*
	QVector<ExerciseModel *> exercises;
	QVector<ProgramExerciseModel *> programExercises;
	QVector<ProgramSerieModel *> series;
	QVector<ProgramModel *> programs;

	programs << new ProgramModel();
	programs.back()->setName("Str");
	programs.back()->setDescription("Testouille");

	exercises << new ExerciseModel();
	exercises.back()->setName("Bike");
	exercises.back()->save();
	programExercises << new ProgramExerciseModel(1, nullptr);
	programExercises.back()->setExerciseModel(exercises.back());
	programExercises.back()->getData()[0]->setValue(1234);
	programs.back()->addExercise(programExercises.back());


	exercises << new ExerciseModel();
	exercises.back()->setName("Shoulder");
	exercises.back()->setDescription("Lateral elevations");
	exercises.back()->save();
	programExercises << new ProgramExerciseModel(3, nullptr);
	programExercises.back()->setExerciseModel(exercises.back());

	series.push_back(new ProgramSerieModel(1, programExercises.back()));
	series.back()->getData()[0]->setValue(12);
	series.back()->getData()[1]->setValue(15);
	series.back()->setRestTime(1);
	programExercises.back()->addSerie(series.back(), false);
	series.push_back(new ProgramSerieModel(1, programExercises.back()));
	series.back()->getData()[0]->setValue(10);
	series.back()->getData()[1]->setValue(10);
	series.back()->setRestTime(3);
	programExercises.back()->addSerie(series.back(), false);

	programs.back()->addExercise(programExercises.back());

	exercises << new ExerciseModel();
	exercises.back()->setName("Work");
	exercises.back()->setDescription("Going to Work");
	exercises.back()->save();
	programExercises << new ProgramExerciseModel(2, nullptr);
	programExercises.back()->setExerciseModel(exercises.back());
	programExercises.back()->getData()[0]->setValue(2500);
	programExercises.back()->setRestTime(0);
	programs.back()->addExercise(programExercises.back());

	programs.back()->save();


	programs << new ProgramModel();
	programs.back()->setName("Dist");
	programs.back()->setDescription("Testouille");

	programExercises << new ProgramExerciseModel(1, nullptr);
	programExercises.back()->setExerciseModel(exercises[0]);
	programExercises.back()->getData()[0]->setValue(1234);
	series.back()->setRestTime(0);
	programs.back()->addExercise(programExercises.back());

	programs.back()->save();

	programs << new ProgramModel();
	programs.back()->setName("Walk");
	programs.back()->setDescription("Testouille");

	programExercises << new ProgramExerciseModel(2, nullptr);
	programExercises.back()->setExerciseModel(exercises[2]);
	programExercises.back()->getData()[0]->setValue(2500);
	series.back()->setRestTime(0);
	programs.back()->addExercise(programExercises.back());

	programs.back()->save();





	for(auto i : programs) i->deleteLater();
	for(auto i : series) i->deleteLater();
	for(auto i : programExercises) i->deleteLater();
	for(auto i : programs) i->deleteLater();
	*/
}


/*
void DatabaseModel::insertDefaultData() {
	QVector<Base::ExerciseModel *> exercises;
	QVector<Base::SerieModel *> series;
	exercises.push_back(new Base::ExerciseModel(3,nullptr));
	exercises.back()->setName("Shoulder");
	exercises.back()->setDescription("Lateral elevations");

	series.push_back(new Base::SerieModel(1, exercises.back()));
	series.back()->getData()[0]->setValue(12);
	series.back()->getData()[1]->setValue(15);
	series.back()->setRestTime(1);
	exercises.back()->addSerie(series.back(), false);

	series.push_back(new Base::SerieModel(1, exercises.back()));
	series.back()->getData()[0]->setValue(10);
	series.back()->getData()[1]->setValue(10);
	series.back()->setRestTime(2);
	exercises.back()->addSerie(series.back(), false);

	series.push_back(new Base::SerieModel(1, exercises.back()));
	series.back()->getData()[0]->setValue(8);
	series.back()->getData()[1]->setValue(8);
	series.back()->setRestTime(3);
	exercises.back()->addSerie(series.back(), false);

	series.push_back(new Base::SerieModel(1, exercises.back()));
	series.back()->getData()[0]->setValue(8);
	series.back()->getData()[1]->setValue(5);
	series.back()->setRestTime(2);
	exercises.back()->addSerie(series.back(), false);

	exercises.back()->save();

	//---------------------------------------------------------------------

	exercises.push_back(new Base::ExerciseModel(3,nullptr));
	exercises.back()->setName("Pectorals");
	exercises.back()->setDescription("Bench press");

	series.push_back(new Base::SerieModel(1, exercises.back()));
	series.back()->getData()[0]->setValue(10);
	series.back()->getData()[1]->setValue(60);
	series.back()->setRestTime(2);
	exercises.back()->addSerie(series.back(), false);

	series.push_back(new Base::SerieModel(1, exercises.back()));
	series.back()->getData()[0]->setValue(10);
	series.back()->getData()[1]->setValue(60);
	series.back()->setRestTime(1);
	exercises.back()->addSerie(series.back(), false);

	series.push_back(new Base::SerieModel(1, exercises.back()));
	series.back()->getData()[0]->setValue(8);
	series.back()->getData()[1]->setValue(50);
	series.back()->setRestTime(2);
	exercises.back()->addSerie(series.back(), false);

	series.push_back(new Base::SerieModel(1, exercises.back()));
	series.back()->getData()[0]->setValue(6);
	series.back()->getData()[1]->setValue(50);
	series.back()->setRestTime(4);
	exercises.back()->addSerie(series.back(), false);

	series.push_back(new Base::SerieModel(1, exercises.back()));
	series.back()->getData()[0]->setValue(6);
	series.back()->getData()[1]->setValue(40);
	series.back()->setRestTime(3);
	exercises.back()->addSerie(series.back(), false);

	exercises.back()->save();

//---------------------------------------------------------------------

	exercises.push_back(new Base::ExerciseModel(3,nullptr));
	exercises.back()->setName("Biceps");
	exercises.back()->setDescription("Curls");

	series.push_back(new Base::SerieModel(1, exercises.back()));
	series.back()->getData()[0]->setValue(12);
	series.back()->getData()[1]->setValue(60);
	series.back()->setRestTime(1);
	exercises.back()->addSerie(series.back(), false);

	series.push_back(new Base::SerieModel(1, exercises.back()));
	series.back()->getData()[0]->setValue(12);
	series.back()->getData()[1]->setValue(55);
	series.back()->setRestTime(1);
	exercises.back()->addSerie(series.back(), false);

	series.push_back(new Base::SerieModel(1, exercises.back()));
	series.back()->getData()[0]->setValue(10);
	series.back()->getData()[1]->setValue(50);
	series.back()->setRestTime(2);
	exercises.back()->addSerie(series.back(), false);

	series.push_back(new Base::SerieModel(1, exercises.back()));
	series.back()->getData()[0]->setValue(8);
	series.back()->getData()[1]->setValue(45);
	series.back()->setRestTime(2);
	exercises.back()->addSerie(series.back(), false);

	exercises.back()->save();

//---------------------------------------------------------------------

	exercises.push_back(new Base::ExerciseModel(3,nullptr));
	exercises.back()->setName("Triciceps");
	exercises.back()->setDescription("Push-ups");

	series.push_back(new Base::SerieModel(1, exercises.back()));
	series.back()->getData()[0]->setValue(15);
	series.back()->getData()[1]->setValue(80);
	series.back()->setRestTime(3);
	exercises.back()->addSerie(series.back(), false);

	series.push_back(new Base::SerieModel(1, exercises.back()));
	series.back()->getData()[0]->setValue(15);
	series.back()->getData()[1]->setValue(80);
	series.back()->setRestTime(1);
	exercises.back()->addSerie(series.back(), false);

	series.push_back(new Base::SerieModel(1, exercises.back()));
	series.back()->getData()[0]->setValue(10);
	series.back()->getData()[1]->setValue(80);
	series.back()->setRestTime(3);
	exercises.back()->addSerie(series.back(), false);

	series.push_back(new Base::SerieModel(1, exercises.back()));
	series.back()->getData()[0]->setValue(10);
	series.back()->getData()[1]->setValue(80);
	series.back()->setRestTime(1);
	exercises.back()->addSerie(series.back(), false);

	exercises.back()->save();

	//---------------------------------------------------------------------

	exercises.push_back(new Base::ExerciseModel(3,nullptr));
	exercises.back()->setName("Abs");
	exercises.back()->setDescription("Crunchs");

	for(int i = 0 ; i < 5 ; ++i) {
		series.push_back(new Base::SerieModel(1, exercises.back()));
		series.back()->getData()[0]->setValue(20);
		series.back()->getData()[1]->setValue(80);
		series.back()->setRestTime(i);
		exercises.back()->addSerie(series.back(), false);
	}

	exercises.back()->save();

	//---------------------------------------------------------------------

	exercises.push_back(new Base::ExerciseModel(3,nullptr));
	exercises.back()->setName("Abs");
	exercises.back()->setDescription("Lateral Crunchs");

	for(int i = 0 ; i < 3 ; ++i) {
		series.push_back(new Base::SerieModel(1, exercises.back()));
		series.back()->getData()[0]->setValue(20);
		series.back()->getData()[1]->setValue(80);
		series.back()->setRestTime(i);
		exercises.back()->addSerie(series.back(), false);
	}

	exercises.back()->save();

	//---------------------------------------------------------------------
	exercises.push_back(new Base::ExerciseModel(1,nullptr));
	exercises.back()->setName("Exo Distance");
	exercises.back()->setDescription("Dummy Distance");
	exercises.back()->setRestTime(1);
	exercises.back()->save();

	exercises.push_back(new Base::ExerciseModel(1,nullptr));
	exercises.back()->setName("Exo2 Distance");
	exercises.back()->setDescription("Dummy Distance");

	exercises.back()->setRestTime(1);
	exercises.back()->save();


	for(auto i : exercises)
		i->deleteLater();
}

*/
void DatabaseModel::insertDefaultData_old() {
// Exercises
/*
	QVector<Description::StrengthModel *> strengths;
	//---------------------------------------------------------------------
	strengths.push_back(new Description::StrengthModel(nullptr));
	strengths.back()->setName("Shoulder");
	strengths.back()->setDescription("Lateral elevations");

	QVector<Description::StrengthWorkModel *> strengthWorks;

	strengthWorks.push_back(new Description::StrengthWorkModel(nullptr));
	strengthWorks.back()->setRepetitions(12);
	strengthWorks.back()->setWeight(15);
	strengthWorks.back()->setRestTime(1);
	strengths.back()->addSet(strengthWorks.back());

	strengthWorks.push_back(new Description::StrengthWorkModel(nullptr));
	strengthWorks.back()->setRepetitions(10);
	strengthWorks.back()->setWeight(10);
	strengthWorks.back()->setRestTime(2);
	strengths.back()->addSet(strengthWorks.back());

	strengthWorks.push_back(new Description::StrengthWorkModel(nullptr));
	strengthWorks.back()->setRepetitions(8);
	strengthWorks.back()->setWeight(8);
	strengthWorks.back()->setRestTime(3);
	strengths.back()->addSet(strengthWorks.back());

	strengthWorks.push_back(new Description::StrengthWorkModel(nullptr));
	strengthWorks.back()->setRepetitions(8);
	strengthWorks.back()->setWeight(5);
	strengthWorks.back()->setRestTime(2);
	strengths.back()->addSet(strengthWorks.back());

	strengths.back()->save();

	//---------------------------------------------------------------------

	strengths.push_back(new Description::StrengthModel(nullptr));
	strengths.back()->setName("Pectorals");
	strengths.back()->setDescription("Bench press");

	strengthWorks.push_back(new Description::StrengthWorkModel(nullptr));
	strengthWorks.back()->setRepetitions(10);
	strengthWorks.back()->setWeight(60);
	strengthWorks.back()->setRestTime(2);
	strengths.back()->addSet(strengthWorks.back());

	strengthWorks.push_back(new Description::StrengthWorkModel(nullptr));
	strengthWorks.back()->setRepetitions(10);
	strengthWorks.back()->setWeight(60);
	strengthWorks.back()->setRestTime(1);
	strengths.back()->addSet(strengthWorks.back());

	strengthWorks.push_back(new Description::StrengthWorkModel(nullptr));
	strengthWorks.back()->setRepetitions(8);
	strengthWorks.back()->setWeight(50);
	strengthWorks.back()->setRestTime(2);
	strengths.back()->addSet(strengthWorks.back());

	strengthWorks.push_back(new Description::StrengthWorkModel(nullptr));
	strengthWorks.back()->setRepetitions(6);
	strengthWorks.back()->setWeight(50);
	strengthWorks.back()->setRestTime(4);
	strengths.back()->addSet(strengthWorks.back());

	strengthWorks.push_back(new Description::StrengthWorkModel(nullptr));
	strengthWorks.back()->setRepetitions(6);
	strengthWorks.back()->setWeight(40);
	strengthWorks.back()->setRestTime(3);
	strengths.back()->addSet(strengthWorks.back());

	strengths.back()->save();

//---------------------------------------------------------------------

	strengths.push_back(new Description::StrengthModel(nullptr));
	strengths.back()->setName("Biceps");
	strengths.back()->setDescription("Curls");

	strengthWorks.push_back(new Description::StrengthWorkModel(nullptr));
	strengthWorks.back()->setRepetitions(12);
	strengthWorks.back()->setWeight(60);
	strengthWorks.back()->setRestTime(1);
	strengths.back()->addSet(strengthWorks.back());

	strengthWorks.push_back(new Description::StrengthWorkModel(nullptr));
	strengthWorks.back()->setRepetitions(12);
	strengthWorks.back()->setWeight(55);
	strengthWorks.back()->setRestTime(1);
	strengths.back()->addSet(strengthWorks.back());

	strengthWorks.push_back(new Description::StrengthWorkModel(nullptr));
	strengthWorks.back()->setRepetitions(10);
	strengthWorks.back()->setWeight(50);
	strengthWorks.back()->setRestTime(2);
	strengths.back()->addSet(strengthWorks.back());

	strengthWorks.push_back(new Description::StrengthWorkModel(nullptr));
	strengthWorks.back()->setRepetitions(8);
	strengthWorks.back()->setWeight(45);
	strengthWorks.back()->setRestTime(2);
	strengths.back()->addSet(strengthWorks.back());

	strengths.back()->save();

//---------------------------------------------------------------------

	strengths.push_back(new Description::StrengthModel(nullptr));
	strengths.back()->setName("Triciceps");
	strengths.back()->setDescription("Push-ups");

	strengthWorks.push_back(new Description::StrengthWorkModel(nullptr));
	strengthWorks.back()->setRepetitions(15);
	strengthWorks.back()->setWeight(80);
	strengthWorks.back()->setRestTime(3);
	strengths.back()->addSet(strengthWorks.back());

	strengthWorks.push_back(new Description::StrengthWorkModel(nullptr));
	strengthWorks.back()->setRepetitions(15);
	strengthWorks.back()->setWeight(80);
	strengthWorks.back()->setRestTime(1);
	strengths.back()->addSet(strengthWorks.back());

	strengthWorks.push_back(new Description::StrengthWorkModel(nullptr));
	strengthWorks.back()->setRepetitions(10);
	strengthWorks.back()->setWeight(80);
	strengthWorks.back()->setRestTime(3);
	strengths.back()->addSet(strengthWorks.back());

	strengthWorks.push_back(new Description::StrengthWorkModel(nullptr));
	strengthWorks.back()->setRepetitions(10);
	strengthWorks.back()->setWeight(80);
	strengthWorks.back()->setRestTime(1);
	strengths.back()->addSet(strengthWorks.back());

	strengths.back()->save();

	//---------------------------------------------------------------------

	strengths.push_back(new Description::StrengthModel(nullptr));
	strengths.back()->setName("Abs");
	strengths.back()->setDescription("Crunchs");

	for(int i = 0 ; i < 5 ; ++i) {
		strengthWorks.push_back(new Description::StrengthWorkModel(nullptr));
		strengthWorks.back()->setRepetitions(20);
		strengthWorks.back()->setWeight(80);
		strengthWorks.back()->setRestTime(i);
		strengths.back()->addSet(strengthWorks.back());
	}

	strengths.back()->save();

	//---------------------------------------------------------------------

	strengths.push_back(new Description::StrengthModel(nullptr));
	strengths.back()->setName("Abs");
	strengths.back()->setDescription("Lateral Crunchs");

	for(int i = 0 ; i < 3 ; ++i) {
		strengthWorks.push_back(new Description::StrengthWorkModel(strengths.back()));
		strengthWorks.back()->setRepetitions(20);
		strengthWorks.back()->setWeight(80);
		strengthWorks.back()->setRestTime(i);
		strengths.back()->addSet(strengthWorks.back());
	}

	strengths.back()->save();

	//---------------------------------------------------------------------



	Description::DistanceModel * distance = new Description::DistanceModel(nullptr);
	distance->setName("Exo Distance");
	distance->setDescription("Dummy Distance");
	distance->setRestTime(1);
	distance->save();
	Description::DistanceModel * distance2 = new Description::DistanceModel(nullptr);
	distance2->setName("Exo Distance2");
	distance2->setDescription("Dummy Distance2");
	distance2->setDistance(12332);
	distance2->setRestTime(2);
	distance2->save();

	Description::StepsModel * steps = new Description::StepsModel(nullptr);
	steps->setName("Exo Steps");
	steps->setDescription("Dummy Steps");
	steps->setRestTime(1);
	steps->save();
	Description::StepsModel * steps2 = new Description::StepsModel(nullptr);
	steps2->setName("Exo Steps2");
	steps2->setSteps(6666);
	steps2->setRestTime(2);
	steps2->save();

// Programs
	QVector<Description::ProgramModel *> programs;
	programs.push_back(new Description::ProgramModel(nullptr));
	programs.back()->setName("Easy 1");
	programs.back()->setDescription("Day 1");
	for(auto i : strengths)
		programs.back()->addExercise(i);
	programs.back()->save();

	programs.push_back(new Description::ProgramModel(nullptr));
	programs.back()->setDescription("Dummy Program");
	programs.back()->addExercise(strengths[3]);
	programs.back()->addExercise(distance);
	programs.back()->addExercise(steps);
	programs.back()->addExercise(distance2);
	programs.back()->addExercise(steps2);
	programs.back()->addExercise(strengths[1]);
	programs.back()->save();

	programs.push_back(new Description::ProgramModel(nullptr));
	programs.back()->setName("Program Strength");
	programs.back()->setDescription("Dummy program");
	programs.back()->addExercise(strengths[2]);
	programs.back()->addExercise(strengths[3]);
	programs.back()->save();

	programs.push_back(new Description::ProgramModel(nullptr));
	programs.back()->setName("Program Distance");
	programs.back()->setDescription("Dummy program");
	programs.back()->addExercise(distance);
	programs.back()->addExercise(distance2);
	programs.back()->save();

	programs.push_back(new Description::ProgramModel(nullptr));
	programs.back()->setName("Program Steps");
	programs.back()->setDescription("Dummy program");
	programs.back()->addExercise(steps);
	programs.back()->addExercise(steps2);
	programs.back()->save();

	QVector<Training::TrainModel*> trains;

	for(int j = 0 ; j < 10 ; ++j) {
		for(auto i : programs) {
			trains.push_back(new Training::TrainModel(nullptr));
			trains.back()->setTargetProgramModel(i);
			trains.back()->setStartDateTime(QDateTime::currentDateTime().addDays(j-10));
			trains.back()->fillRandomValues();
			trains.back()->save();
		}
	}




	steps->deleteLater();
	steps2->deleteLater();
	distance->deleteLater();
	distance2->deleteLater();
	for(auto i : trains)
		i->deleteLater();
	for(auto i : programs)
		i->deleteLater();
	for(auto i : strengthWorks)
		i->deleteLater();
	for(auto i : strengths)
		i->deleteLater();
*/
}

