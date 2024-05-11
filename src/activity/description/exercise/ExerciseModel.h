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

#ifndef EXERCISE_MODEL_H
#define EXERCISE_MODEL_H

#include <QObject>
#include <QVector>

namespace Training{
class ExerciseModel;
}

namespace Description{
class ExerciseModel : public QObject{
Q_OBJECT
	Q_PROPERTY(QString name MEMBER mName NOTIFY nameChanged)
	Q_PROPERTY(QString description MEMBER mDescription NOTIFY descriptionChanged)

	Q_PROPERTY(bool invalidName MEMBER mInvalidName NOTIFY invalidNameChanged)
	Q_PROPERTY(bool invalidDescription MEMBER mInvalidDescription NOTIFY invalidDescriptionChanged)
	Q_PROPERTY(int type READ getType CONSTANT)
	Q_PROPERTY(int programOrder READ getProgramOrder WRITE setProgramOrder NOTIFY programOrderChanged)
public:
	explicit ExerciseModel(QObject *parent);
	ExerciseModel(const ExerciseModel * model, QObject *parent);
	virtual ExerciseModel * clone(qint64 programId, QObject *parent)const;
	virtual Training::ExerciseModel * cloneTraining(qint64, QObject *){return nullptr;}
	virtual int getType() const;	// For QML to know how to display it becausze of lacking object relationship.

	QString getName()const;
	void setName(QString name);

	QString getDescription() const;
	void setDescription(QString description);

	void setInvalidName(bool invalid);
	void setInvalidDescription(bool invalid);

	qint64 getExerciseId()const;
	virtual void setExerciseId(qint64 id);

	qint64 getProgramId() const;
	virtual void setProgramId(qint64 id);
	bool isProgramLinked()const;

	int getProgramOrder() const;
	void setProgramOrder(int data);

	Q_INVOKABLE virtual bool save();

signals:
	void exerciseIdChanged();
	void programIdChanged();
	void nameChanged();
	void descriptionChanged();
	void programOrderChanged();

	void invalidNameChanged();
	void invalidDescriptionChanged();

protected:
	qint64 mExerciseId = 0;
	qint64 mProgramId = -1;
	QString mName;
	QString mDescription;
	int mProgramOrder = -1;			// Order in program

	bool mInvalidName = true;
	bool mInvalidDescription = false;
};
}

#endif
