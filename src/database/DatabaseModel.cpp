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

#include "src/activity/description/sport/StrengthModel.h"
#include "src/activity/description/sport/StrengthWorkModel.h"

#include "src/activity/description/sport/StepsModel.h"
#include "src/activity/description/sport/DistanceModel.h"
#include "src/activity/description/program/ProgramModel.h"

DatabaseModel::DatabaseModel(QObject *parent)
	: QObject{parent}
{}

// ex_ => exercises

void DatabaseModel::migrate(){
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	//db.setDatabaseName("data");
	db.setDatabaseName(":memory:");
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
// Exercise units
		if(!query.exec("CREATE TABLE ex_steps (id INTEGER PRIMARY KEY"
			", name TEXT"
			", description TEXT"
			", steps INTEGER"
			", rest_time INTEGER)")) qCritical() << "Cannot create ex_steps : " << query.lastError().text();
		if(!query.exec("CREATE TABLE ex_distance (id INTEGER PRIMARY KEY"
			", name TEXT"
			", description TEXT"
			", distance INTEGER"
			", rest_time INTEGER)")) qCritical() << "Cannot create ex_steps : " << query.lastError().text();
		if(!query.exec("CREATE TABLE ex_strength (id INTEGER PRIMARY KEY"
			", name TEXT"
			", description TEXT)")) qCritical() << "Cannot create ex_steps : " << query.lastError().text();
		if(!query.exec("CREATE TABLE ex_strength_set (id INTEGER PRIMARY KEY"
			", strength_id INTEGER NOT NULL"
			", reps INTEGER"
			", weight INTERGER"
			", rest_time INTEGER"
				   ", FOREIGN KEY (strength_id) REFERENCES ex_strength (id) ON UPDATE CASCADE ON DELETE CASCADE"
				   ")")) qCritical() << "Cannot create ex_strength_set table : " << query.lastError().text();

// Programs = copy of Exercise units
		if(!query.exec("CREATE TABLE programs (id INTEGER PRIMARY KEY"
			", name TEXT"
			", description TEXT)")) qCritical() << "Cannot create programs : " << query.lastError().text();
		if(!query.exec("CREATE TABLE pgrm_ex_steps (id INTEGER PRIMARY KEY"
			", program_id INTEGER NOT NULL"
			", name TEXT, description TEXT"
			", steps INTEGER"
			", rest_time INTEGER"
			", program_order INTEGER"
					", FOREIGN KEY (program_id) REFERENCES programs (id) ON UPDATE CASCADE ON DELETE CASCADE)")) qCritical() << "Cannot create pgrm_ex_steps : " << query.lastError().text();
		if(!query.exec("CREATE TABLE pgrm_ex_distance (id INTEGER PRIMARY KEY"
			", program_id INTEGER NOT NULL"
			", name TEXT, description TEXT"
			", distance INTEGER"
			", rest_time INTEGER"
			", program_order INTEGER"
					", FOREIGN KEY (program_id) REFERENCES programs (id) ON UPDATE CASCADE ON DELETE CASCADE)")) qCritical() << "Cannot create pgrm_ex_distance : " << query.lastError().text();
		if(!query.exec("CREATE TABLE pgrm_ex_strength (id INTEGER PRIMARY KEY"
			", program_id INTEGER NOT NULL"
			", name TEXT"
			", description TEXT"
			", program_order INTEGER"
					", FOREIGN KEY (program_id) REFERENCES programs (id) ON UPDATE CASCADE ON DELETE CASCADE)")) qCritical() << "Cannot create pgrm_ex_strength : " << query.lastError().text();
		if(!query.exec("CREATE TABLE pgrm_ex_strength_set (id INTEGER PRIMARY KEY"
			", program_id INTEGER NOT NULL"
			", strength_id INTEGER NOT NULL"
			", reps INTEGER, weight INTERGER"
			", rest_time INTEGER"
					", FOREIGN KEY (program_id) REFERENCES programs (id) ON UPDATE CASCADE ON DELETE CASCADE"
					", FOREIGN KEY (strength_id) REFERENCES pgrm_ex_strength (id) ON UPDATE CASCADE ON DELETE CASCADE"
					")")) qCritical() << "Cannot create pgrm_ex_strength_set table : " << query.lastError().text();

// Trains = copy of Program units

		if(!query.exec("CREATE TABLE trains (id INTEGER PRIMARY KEY"
			", name TEXT, description TEXT"
			", start_time INTERGER)")) qCritical() << "Cannot create trains : " << query.lastError().text();
		if(!query.exec("CREATE TABLE tr_ex_steps (id INTEGER PRIMARY KEY"
			", train_id INTEGER NOT NULL"
			", name TEXT"
			", description TEXT"
			", steps INTEGER"
			", rest_time INTEGER"
			", train_order INTEGER"
					", FOREIGN KEY (train_id) REFERENCES trains (id) ON UPDATE CASCADE ON DELETE CASCADE)")) qCritical() << "Cannot create tr_ex_steps : " << query.lastError().text();
		if(!query.exec("CREATE TABLE tr_ex_distance (id INTEGER PRIMARY KEY"
			", train_id INTEGER NOT NULL"
			", name TEXT"
			", description TEXT"
			", distance INTEGER"
			", rest_time INTEGER"
			", train_order INTEGER"
					", FOREIGN KEY (train_id) REFERENCES trains (id) ON UPDATE CASCADE ON DELETE CASCADE)")) qCritical() << "Cannot create tr_ex_distance : " << query.lastError().text();
		if(!query.exec("CREATE TABLE tr_ex_strength (id INTEGER PRIMARY KEY"
			", train_id INTEGER NOT NULL"
			", name TEXT, description TEXT"
			", train_order INTEGER"
					", FOREIGN KEY (train_id) REFERENCES trains (id) ON UPDATE CASCADE ON DELETE CASCADE)")) qCritical() << "Cannot create tr_ex_strength : " << query.lastError().text();
		if(!query.exec("CREATE TABLE tr_ex_strength_set (id INTEGER PRIMARY KEY"
			", train_id INTEGER NOT NULL"
			", strength_id INTEGER NOT NULL"
			", reps INTEGER"
			", weight INTERGER"
			", rest_time INTEGER"
			", work_time INTEGER"
					", FOREIGN KEY (train_id) REFERENCES trains (id) ON UPDATE CASCADE ON DELETE CASCADE"
					", FOREIGN KEY (strength_id) REFERENCES tr_ex_strength_set (id) ON UPDATE CASCADE ON DELETE CASCADE"
					")")) qCritical() << "Cannot create tr_ex_strength_set table : " << query.lastError().text();



		insertDefaultData();

		query.exec("CREATE TABLE version (version INTEGER)");
		query.exec("INSERT INTO version (version) VALUES(1)");

		query.exec("PRAGMA user_version=1");
	}
	query.exec("PRAGMA user_version");
	fieldNo = query.record().indexOf("user_version");
	while (query.next()) {
		version = query.value(fieldNo).toInt();
		qDebug() << "Final Database Version : " << version;
	}
}

void DatabaseModel::insertDefaultData() {
// Exercises
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

	steps->deleteLater();
	steps2->deleteLater();
	distance->deleteLater();
	distance2->deleteLater();

	for(auto i : programs)
		i->deleteLater();
	for(auto i : strengthWorks)
		i->deleteLater();
	for(auto i : strengths)
		i->deleteLater();

}

