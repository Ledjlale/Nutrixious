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

#ifndef FOOD_MODEL_H
#define FOOD_MODEL_H

#include <QObject>
#include <QNetworkReply>

class FoodModel : public QObject {
    Q_OBJECT
	Q_PROPERTY(double calories READ getCalories WRITE setCalories NOTIFY caloriesChanged)
public:
	explicit FoodModel(QObject *parent = nullptr);

	double getCalories() const;
	void setCalories(double calories);

	Q_INVOKABLE void loadFromOpenFoodFacts(const QString& code);

	void openFoodFactsDownloaded();
	void handleSslErrors (const QList<QSslError> &sslErrors);
	void handleError (QNetworkReply::NetworkError code);

signals:
	void caloriesChanged();

private:
	double mCalories = 0.0;
	QString mId;
};

#endif
