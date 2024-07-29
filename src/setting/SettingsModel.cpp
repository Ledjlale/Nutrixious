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

#include "SettingsModel.h"
#include "src/database/DatabaseModel.h"
#include <QSettings>
#include <QUrl>
#include <QDir>

SettingsModel::SettingsModel(QObject *parent)
	: QObject{parent}
{}

bool SettingsModel::getOpenFoodFactsEnabled() const{
	QSettings settings;
	return settings.value("OpenFoodFactsEnabled", false).toBool();
}

void SettingsModel::setOpenFoodFactsEnabled(bool data){
	QSettings settings;
	settings.setValue("OpenFoodFactsEnabled", data);
	emit openFoodFactsEnabledChanged();
}

bool SettingsModel::saveDatabase(QString path){
	QFileInfo fileInfo(path);
	return DatabaseModel::saveDatabase( fileInfo.absoluteFilePath());
}

bool SettingsModel::loadDatabase(QString path){
	QFileInfo fileInfo(path);
	if(fileInfo.exists())
		return DatabaseModel::loadDatabase( fileInfo.absoluteFilePath());
	else{
		qWarning() << "Cannot import database because file doesn't exist";
		return false;
	}
}

