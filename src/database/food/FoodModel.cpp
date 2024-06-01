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

#include <QQmlApplicationEngine>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlField>
#include <QJsonDocument>
#include "src/database/DatabaseQuery.h"

#include "src/tool/Utils.h"


extern QQmlApplicationEngine * gEngine;

FoodModel::FoodModel() : FoodModel(nullptr){	// QML
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}
FoodModel::FoodModel(QObject *parent)
	: QmlModel{parent}
{
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	connect(this, &FoodModel::foodIdChanged, this, &FoodModel::updateIsSaved);
}

FoodModel::FoodModel(FoodModel * model, QObject *parent)
	: QmlModel{parent}
{
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	connect(this, &FoodModel::foodIdChanged, this, &FoodModel::updateIsSaved);
}

FoodModel* FoodModel::clone(QObject*parent) {
	return new FoodModel(this, parent);

}

DEFINE_GETSET(FoodModel,qint64,foodId,FoodId)
DEFINE_GETSET(FoodModel,QString,openFoodFactsCode,OpenFoodFactsCode)
DEFINE_GETSET(FoodModel,QString,brand,Brand)
DEFINE_GETSET(FoodModel,QString,imageUrl,ImageUrl)
DEFINE_GETSET(FoodModel,QString,description,Description)
DEFINE_GETSET(FoodModel,qint64,servingUnitId,ServingUnitId)
DEFINE_GETSET(FoodModel,double,servingSize,ServingSize)
DEFINE_GETSET(FoodModel,double,servingsPerContainer,ServingsPerContainer)
DEFINE_GETSET(FoodModel,double,calories,Calories)
DEFINE_GETSET(FoodModel,double,totalFat,TotalFat)
DEFINE_GETSET(FoodModel,double,saturatedFat,SaturatedFat)
DEFINE_GETSET(FoodModel,double,transFat,TransFat)
DEFINE_GETSET(FoodModel,double,polyUnsaturatedFat,PolyUnsaturatedFat)
DEFINE_GETSET(FoodModel,double,monoUnsaturatedFat,MonoUnsaturatedFat)
DEFINE_GETSET(FoodModel,double,cholesterol,Cholesterol)
DEFINE_GETSET(FoodModel,double,sodium,Sodium)
DEFINE_GETSET(FoodModel,double,totalCarbohydrate,TotalCarbohydrate)
DEFINE_GETSET(FoodModel,double,dietaryFiber,DietaryFiber)
DEFINE_GETSET(FoodModel,double,sugar,Sugar)
DEFINE_GETSET(FoodModel,double,protein,Protein)
DEFINE_GETSET(FoodModel,double,calcium,Calcium)
DEFINE_GETSET(FoodModel,double,iron,Iron)
DEFINE_GETSET(FoodModel,double,potassium,Potassium)
DEFINE_GETSET(FoodModel,double,vitaminA,VitaminA)
DEFINE_GETSET(FoodModel,double,vitaminC,VitaminC)

//-------------------------------------------------------------------------------------------------------------------

void FoodModel::updateIsSaved(){
	setIsSaved(getFoodId() > 0);
}

bool FoodModel::save(){
	if(mFoodId>0 && !mIsEdited) return true;// Avoid update for nothing
	qDebug() << "Saving food " << mBrand << mDescription;
	DatabaseQuery query;

	query.begin(mFoodId == 0 ? DatabaseQuery::Insert : DatabaseQuery::Update, "foods" );

	query.add("brand", mBrand);
	query.add("description", mDescription);
	query.add("open_food_facts_code", mOpenFoodFactsCode);
	query.add("image_url", mImageUrl);
	query.add("serving_size", mServingSize);
	query.add("servings_per_container", mServingsPerContainer);
	query.add("serving_unit_id", mServingUnitId);
	query.add("calories", mCalories);
	query.add("total_fat", mTotalFat);
	query.add("saturated_fat", mSaturatedFat);
	query.add("trans_fat", mTransFat);
	query.add("poly_unsaturated_fat", mPolyUnsaturatedFat);
	query.add("mono_unsaturated_fat", mMonoUnsaturatedFat);
	query.add("cholesterol", mCholesterol);
	query.add("sodium", mSodium);
	query.add("total_carbohydrate", mTotalCarbohydrate);
	query.add("dietary_fiber", mDietaryFiber);
	query.add("sugar", mSugar);
	query.add("protein", mProtein);
	query.add("calcium", mCalcium);
	query.add("iron", mIron);
	query.add("potassium", mPotassium);
	query.add("vitamin_a", mVitaminA);
	query.add("vitamin_c", mVitaminC);

	query.addConditionnal("food_id",mFoodId);
	if(mFoodId == 0){
		if(!query.exec()){
			qCritical() << "Cannot save food: "  << query.mQuery.lastError().text();
			return false;
		}
		auto fieldNo = query.mQuery.record().indexOf("food_id");
		while (query.mQuery.next()) {
			setFoodId(query.mQuery.value(fieldNo).toInt());
			qDebug() << "Insert food: " << mFoodId;
		}
	}else{
		if(!query.exec()) {
			qCritical() << "Cannot update food: "  << query.mQuery.lastError().text();
			return false;
		}else {
			qDebug() << "Update food: " << mFoodId;
		}
	}
	clearBackupValues();
	return true;
}

void FoodModel::remove(){
	if(mFoodId > 0){
		DatabaseQuery query;
		query.begin(DatabaseQuery::Delete, "foods");
		query.addConditionnal("food_id",mFoodId);
		if(!query.exec()){
			if(!query.exec()) qCritical() << "Cannot delete food  : "  << query.mQuery.lastError().text();
		}
	}
	emit removed(this);
}

FoodModel *FoodModel::build(QSqlQuery &query, QObject * parent) {
	auto idField = query.record().indexOf("foods.food_id");

	if( idField>= 0){
		FoodModel * model = new FoodModel(parent);
	// TODO optimize
		for(int i = 0 ; i < query.record().count() ; ++i){
			QString fieldName = query.record().field(i).name();
			if(fieldName == "food_id") model->setFoodId(query.value(i).toInt());
			else if(fieldName == "open_food_facts_code") model->setOpenFoodFactsCode(query.value(i).toString());
			else if(fieldName == "image_url") model->setImageUrl(query.value(i).toString());
			else if(fieldName == "brand") model->setBrand(query.value(i).toString());
			else if(fieldName == "description") model->setDescription(query.value(i).toString());
			else if(fieldName == "serving_size") model->setServingSize(query.value(i).toDouble());
			else if(fieldName == "servings_per_container") model->setServingsPerContainer(query.value(i).toDouble());
			else if(fieldName == "serving_unit_id") model->setServingUnitId(query.value(i).toLongLong());
			else if(fieldName == "calories") model->setCalories(query.value(i).toDouble());
			else if(fieldName == "total_fat") model->setTotalFat(query.value(i).toDouble());
			else if(fieldName == "saturated_fat") model->setSaturatedFat(query.value(i).toDouble());
			else if(fieldName == "trans_fat") model->setTransFat(query.value(i).toDouble());
			else if(fieldName == "poly_unsaturated_fat") model->setPolyUnsaturatedFat(query.value(i).toDouble());
			else if(fieldName == "mono_unsaturated_fat") model->setMonoUnsaturatedFat(query.value(i).toDouble());
			else if(fieldName == "cholesterol") model->setCholesterol(query.value(i).toDouble());
			else if(fieldName == "sodium") model->setSodium(query.value(i).toDouble());
			else if(fieldName == "total_carbohydrate") model->setTotalCarbohydrate(query.value(i).toDouble());
			else if(fieldName == "dietary_fiber") model->setDietaryFiber(query.value(i).toDouble());
			else if(fieldName == "sugar") model->setSugar(query.value(i).toDouble());
			else if(fieldName == "protein") model->setProtein(query.value(i).toDouble());
			else if(fieldName == "calcium") model->setCalcium(query.value(i).toDouble());
			else if(fieldName == "iron") model->setIron(query.value(i).toDouble());
			else if(fieldName == "potassium") model->setPotassium(query.value(i).toDouble());
			else if(fieldName == "vitamin_a") model->setVitaminA(query.value(i).toDouble());
			else if(fieldName == "vitamin_c") model->setVitaminC(query.value(i).toDouble());
		}
		return model;
	}else return nullptr;
}

QList<FoodModel*> FoodModel::buildAll(QObject * parent){
	QList<FoodModel*> models;
	QSqlQuery query;
	if(!query.exec( "SELECT * FROM foods ORDER BY food_id DESC")) qCritical() << "Cannot select all foods  : "  << query.lastError().text();

	while (query.next()) {
		auto model = build(query, parent);
		models << model;
	}

	return models;
}



void FoodModel::loadFromOpenFoodFacts(const QString& code) {
	setOpenFoodFactsCode(code);
	qWarning() << code;
	static QNetworkAccessManager *manager = new QNetworkAccessManager(nullptr);
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

// https://wiki.openfoodfacts.org/API_Fields
void FoodModel::openFoodFactsDownloaded(){
	auto reply = dynamic_cast<QNetworkReply*>(sender());
	auto document = QJsonDocument::fromJson(reply->readAll());
	//qDebug() << mId << " => "<< document;
	if(document.isNull()) return;
	QMap<QString,QVariant> nutrimentsMap;
	auto rootArray = document.toVariant().toMap();
	if(rootArray.contains("product")){
		setFoodId(0);
		auto product = rootArray["product"].toMap();
		if(product.contains("nutriments")){
			nutrimentsMap = product["nutriments"].toMap();
		}
		if( product.contains("brands") ) setBrand(product["brands"].toString());
		if( product.contains("brand_owner") && getBrand().isEmpty()) setBrand(product["brand_owner"].toString());

		if(product.contains("ecoscore_data")){
			auto ecoscore = product["ecoscore_data"].toMap();
			if(ecoscore.contains("agribalyse")){
				auto agribalyse = ecoscore["agribalyse"].toMap();
				if(agribalyse.contains("name_en")) setDescription(agribalyse["name_en"].toString());
			}
		}
		if( product.contains("generic_name") && getDescription().isEmpty()) setDescription(product["generic_name"].toString());
		if( product.contains("image_url")) setImageUrl(product["image_url"].toString());
		if( product.contains("serving_quantity")) setServingSize(product["serving_quantity"].toDouble());
		else setServingSize(1);
		//if( product.contains("")) setServingsPerContainer(product[""].toDouble());

		QString unit = "g";
		if( product.contains("serving_quantity_unit")) unit = product["serving_quantity_unit"].toString();
		QSqlQuery query;
		query.prepare("SELECT unit_id FROM units WHERE name = ?");
		query.addBindValue(unit);
		if(query.exec() && query.next()){
			setServingUnitId(query.value(0).toLongLong());
		}


		if( nutrimentsMap.contains("energy-kcal_100g")) setCalories(nutrimentsMap["energy-kcal_100g"].toDouble() );
		if( nutrimentsMap.contains("fat_100g")) setTotalFat(nutrimentsMap["fat_100g"].toDouble());
		if( nutrimentsMap.contains("saturated-fat_100g")) setSaturatedFat(nutrimentsMap["saturated-fat_100g"].toDouble());
		if( nutrimentsMap.contains("trans-fat_100g")) setTransFat(nutrimentsMap["trans-fat_100g"].toDouble());
		if( nutrimentsMap.contains("polyunsaturated-fat_100g")) setPolyUnsaturatedFat(nutrimentsMap["polyunsaturated-fat_100g"].toDouble());
		if( nutrimentsMap.contains("monounsaturated-fat_100g")) setMonoUnsaturatedFat(nutrimentsMap["monounsaturated-fat_100g"].toDouble());
		if( nutrimentsMap.contains("cholesterol_100g")) setCholesterol(nutrimentsMap["cholesterol_100g"].toDouble());
		if( nutrimentsMap.contains("sodium_100g")) setSodium(nutrimentsMap["sodium_100g"].toDouble());
		if( nutrimentsMap.contains("carbohydrates_100g")) setTotalCarbohydrate(nutrimentsMap["carbohydrates_100g"].toDouble());
		if( nutrimentsMap.contains("fiber_100g")) setDietaryFiber(nutrimentsMap["fiber_100g"].toDouble());
		if( nutrimentsMap.contains("sugars_100g")) setSugar(nutrimentsMap["sugars_100g"].toDouble());
		if( nutrimentsMap.contains("proteins_100g")) setProtein(nutrimentsMap["proteins_100g"].toDouble());
		if( nutrimentsMap.contains("calcium_100g")) setCalcium(nutrimentsMap["calcium_100g"].toDouble());
		if( nutrimentsMap.contains("iron_100g")) setIron(nutrimentsMap["iron_100g"].toDouble());
		if( nutrimentsMap.contains("potassium_100g")) setPotassium(nutrimentsMap["potassium_100g"].toDouble());
		if( nutrimentsMap.contains("vitamin-a_100g")) setVitaminA(nutrimentsMap["vitamin-a_100g"].toDouble());
		if( nutrimentsMap.contains("vitamin-c_100g")) setVitaminC(nutrimentsMap["vitamin-c_100g"].toDouble());
	}
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
					  .arg(mOpenFoodFactsCode).arg(reply->errorString());

	//emit loadingFailed();
}
