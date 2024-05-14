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

class QmlModel: public QObject{
Q_OBJECT
	Q_PROPERTY(bool isSaved READ getIsSaved WRITE setIsSaved NOTIFY isSavedChanged)
	Q_PROPERTY(bool isEdited READ getIsEdited WRITE setIsEdited NOTIFY isEditedChanged)

public:
	QmlModel(QObject *parent = nullptr);

	virtual void updateIsEdited();
	virtual void updateIsSaved();
	virtual void undo();

	bool getIsSaved() const;
	void setIsSaved(bool data);

	bool getIsEdited() const;
	void setIsEdited(bool data);
	void addBackup(void *key, QVariant keyValue, QVariant newValue);
	void clearBackupValues();

signals:
	void isSavedChanged();
	void isEditedChanged();

protected:
	QMap<void*, QVariant> mBackupValues;

	bool mIsSaved = false;
	bool mIsEdited = false;
};

#endif
