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

#ifndef DATABASE_QUERY_H
#define DATABASE_QUERY_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QStringList>
#include <QVariantList>

class DatabaseQuery : public QObject {
	Q_OBJECT
public:
	typedef enum{
		Insert,
		Update,
		Delete
	}Type;
	explicit DatabaseQuery(QObject *parent = nullptr);

	void begin(Type type, QString table);
	void add(QString key, QVariant value);
	void addConditionnal(QString key, QVariant value = QVariant());	// Set key for RETURNING
	bool exec();


	QSqlQuery mQuery;
	QStringList mKeys;
	QVariantList mValues;
	QString mCondKey;
	QVariant mCondValue;
	QString mTable;
	Type mType;
};

#endif
