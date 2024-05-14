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

#ifndef DATABASE_PROGRAM_MODEL_H
#define DATABASE_PROGRAM_MODEL_H

#include <QObject>
#include <QVector>
#include <QVariantList>
#include <QSqlQuery>

#include "src/tool/QmlModel.h"
#include "exercise/ProgramExerciseModel.h"

class ProgramModel : public QmlModel{
Q_OBJECT
// DB
	Q_PROPERTY(QString name MEMBER mName NOTIFY nameChanged)
	Q_PROPERTY(QString description MEMBER mDescription NOTIFY descriptionChanged)

	Q_PROPERTY(QVariantList exercises READ getVariantExercises NOTIFY exercisesChanged)

public:
	ProgramModel();	// QML
	ProgramModel(QObject *parent);


	Q_INVOKABLE qint64 getProgramId()const;
	virtual void setProgramId(qint64 id);

	QString getName() const;
	void setName(QString name);

	void setDescription(QString description);

	QVariantList getVariantExercises() const;
	const QList<ProgramExerciseModel*>& getExercises() const;
	Q_INVOKABLE ProgramExerciseModel* addExercise(ProgramExerciseModel *model, bool keepId = false);
	Q_INVOKABLE void removeExercise(ProgramExerciseModel *model);
	void clearExercises();

	Q_INVOKABLE void decrementExerciseOrder(ProgramExerciseModel *model);
	Q_INVOKABLE void incrementExerciseOrder(ProgramExerciseModel *model);

	Q_INVOKABLE void updateProgramOrder();



	Q_INVOKABLE virtual bool save();
	Q_INVOKABLE virtual void remove();
	static QList<ProgramModel*> load(QObject * parent);
	static ProgramModel *load(QSqlQuery &query, QObject * parent);

signals:
	void programIdChanged();
	void nameChanged();
	void descriptionChanged();
	void exercisesChanged();
	void removed(ProgramModel *model);

protected:
	qint64 mProgramId = 0;
	QString mName;
	QString mDescription;
	QList<ProgramExerciseModel*> mExercises;

};

#endif
