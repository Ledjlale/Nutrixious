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

#ifndef QML_MODEL_H
#define QML_MODEL_H

#include <QObject>
#include <QVariant>

#define DECLARE_GETSET(type, x, X) public:\
	Q_PROPERTY(type x MEMBER m##X WRITE set##X NOTIFY x##Changed)\
	type get##X() const; \
	void set##X(type data);\
	Q_SIGNAL void x##Changed();

#define DEFINE_GETSET(Class, type, x, X) type Class::get##X() const{ \
	return m##X;\
	}\
	void Class::set##X(type data){\
		if(m##X!= data){\
			addBackup(&m##X,m##X,data);\
			m##X = data;\
			emit x##Changed();\
		}\
	}

#define DEFINE_SIMPLE_GETSET(Class, type, x, X) type Class::get##X() const{ \
	return m##X;\
	}\
	void Class::set##X(type data){\
		if(m##X!= data){\
			m##X = data;\
			emit x##Changed();\
		}\
	}

#define DEFINE_UNDO_LONGLONG(X) if(mBackupValues.contains(&m##X)) set##X( mBackupValues[&m##X].toLongLong());
#define DEFINE_UNDO_STRING(X) if(mBackupValues.contains(&m##X)) set##X( mBackupValues[&m##X].toString());
#define DEFINE_UNDO_DOUBLE(X) if(mBackupValues.contains(&m##X)) set##X( mBackupValues[&m##X].toDouble());
#define DEFINE_UNDO_INT(X) if(mBackupValues.contains(&m##X)) set##X( mBackupValues[&m##X].toInt());
#define DEFINE_UNDO_BOOL(X) if(mBackupValues.contains(&m##X)) set##X( mBackupValues[&m##X].toBool());
#define DEFINE_UNDO_DATETIME(X) if(mBackupValues.contains(&m##X)) set##X( mBackupValues[&m##X].toDateTime());

#define GET_OLD_VALUE_DOUBLE(X) (mBackupValues.contains(&m##X) ? mBackupValues[&m##X].toDouble() : m##X)

class QmlModel: public QObject{
Q_OBJECT
	Q_PROPERTY(bool isSaved READ getIsSaved WRITE setIsSaved NOTIFY isSavedChanged)
	Q_PROPERTY(bool isEdited READ getIsEdited WRITE setIsEdited NOTIFY isEditedChanged)

public:
	QmlModel(QObject *parent = nullptr);

	virtual void updateIsEdited();
	virtual void updateIsSaved();
	Q_INVOKABLE virtual void undo();

	bool getIsSaved() const;
	void setIsSaved(bool data);

	bool getIsEdited() const;
	void setIsEdited(bool data);
	QVariant initiBackup(const QmlModel *fromModel, const void *from, const QVariant fromValue, void *to);
	void addBackup(void *key, QVariant keyValue, QVariant newValue);
	void clearBackupValues();

signals:
	void isSavedChanged();
	void isEditedChanged();

protected:
	QMap<const void*, QVariant> mBackupValues;

	bool mIsSaved = false;
	bool mIsEdited = false;
};

#endif
