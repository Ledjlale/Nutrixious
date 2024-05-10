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

#include "FoodModel.h"
#include <QGuiApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>

FoodModel::FoodModel(QObject *parent)
	: QObject{parent}
{}


double FoodModel::getCalories() const {
	return mCalories;
}

void FoodModel::setCalories(double calories) {
	if(mCalories != calories) {
		mCalories = calories;
		emit caloriesChanged();
	}
}

void FoodModel::loadFromOpenFoodFacts(const QString& code) {
	mId = code;
	qWarning() << code;
	static QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	QNetworkRequest request;
	QUrl url("https://ssl-api.openfoodfacts.org/api/v0/product/" +code+".json");
	if(!QSslSocket::supportsSsl()) {
		qWarning() << "Https has been requested but SSL is not supported. Fallback to http. Install manually OpenSSL libraries in your PATH.";
		url.setScheme("http");
	}
	request.setUrl(url);
	//request.setRawHeader("User-Agent", "MyOwnBrowser 1.0");

	QNetworkReply *reply = manager->get(request);
	connect(reply, &QNetworkReply::finished, this, &FoodModel::openFoodFactsDownloaded);
	connect(reply, &QNetworkReply::errorOccurred, this, &FoodModel::handleError);
	connect(reply, &QNetworkReply::sslErrors, this, &FoodModel::handleSslErrors);

	qDebug() << "Device supports OpenSSL: " << QSslSocket::supportsSsl();
}

void FoodModel::openFoodFactsDownloaded(){
	auto reply = dynamic_cast<QNetworkReply*>(sender());
	auto document = QJsonDocument::fromJson(reply->readAll());
	//qDebug() << mId << " => "<< document;
	if(document.isNull()) return;
	QMap<QString,QVariant> nutrimentsMap;
	auto rootArray = document.toVariant().toMap();
	if(rootArray.contains("product")){
		auto product = rootArray["product"].toMap();
		if(product.contains("nutriments")){
			nutrimentsMap = product["nutriments"].toMap();
		}
	}
	qDebug() << nutrimentsMap;
	setCalories(nutrimentsMap["energy-kcal_100g"].toDouble());
}

void FoodModel::handleSslErrors (const QList<QSslError> &sslErrors) {

	qDebug() << "Device supports OpenSSL: " << QSslSocket::supportsSsl();
	qDebug()<<"availableBackends:"<<QSslSocket::availableBackends();
	qDebug()<<"activeBackend:"<<QSslSocket::activeBackend();
	qDebug()<<"sslLibraryBuildVersionString():"<<QSslSocket::sslLibraryBuildVersionString();
	qDebug()<<"sslLibraryVersionString():"<<QSslSocket::sslLibraryVersionString();
	qDebug()<<"supportedFeatures():";
	for(auto v:QSslSocket::supportedFeatures())
	{
		qDebug()<<int(v);
	}
	qDebug()<<"supportedProtocols():"<<QSslSocket::supportedProtocols();
#if QT_CONFIG(ssl)
	for (const QSslError &error : sslErrors)
		qWarning() << QStringLiteral("SSL error: %1").arg(error.errorString());
#else
	Q_UNUSED(sslErrors);
#endif
}
void FoodModel::handleError (QNetworkReply::NetworkError code) {
	auto reply = dynamic_cast<QNetworkReply*>(sender());
	if (code != QNetworkReply::OperationCanceledError)
		qWarning() << QStringLiteral("Download of %1 failed: %2")
					  .arg(mId).arg(reply->errorString());

	//emit loadingFailed();
}
