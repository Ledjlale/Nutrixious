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
	: QmlModel{parent} {
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	connect(this, &FoodModel::idChanged, this, &FoodModel::updateIsSaved);
	mTablePrefix = "food";
}

FoodModel::FoodModel(const FoodModel * model, QObject *parent)
	: QmlModel{parent} {
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	connect(this, &FoodModel::idChanged, this, &FoodModel::updateIsSaved);
	mTablePrefix = "food";
	mId = initiBackup(model, &model->mId, model->mId, &mId).toLongLong();
	mOpenFoodFactsCode = initiBackup(model, &model->mOpenFoodFactsCode, model->mOpenFoodFactsCode, &mOpenFoodFactsCode).toString();
	mBrand = initiBackup(model, &model->mBrand, model->mBrand, &mBrand).toString();
	mImageUrl = initiBackup(model, &model->mImageUrl, model->mImageUrl, &mImageUrl).toString();
	mDescription = initiBackup(model, &model->mDescription, model->mDescription, &mDescription).toString();
	mServingUnitId = initiBackup(model, &model->mServingUnitId, model->mServingUnitId, &mServingUnitId).toLongLong();

	mServingSize = initiBackup(model, &model->mServingSize, model->mServingSize, &mServingSize).toDouble();
	mServingsPerContainer = initiBackup(model, &model->mServingsPerContainer, model->mServingsPerContainer, &mServingsPerContainer).toDouble();
	mCalories = initiBackup(model, &model->mCalories, model->mCalories, &mCalories).toDouble();
	mTotalFat = initiBackup(model, &model->mTotalFat, model->mTotalFat, &mTotalFat).toDouble();
	mSaturatedFat = initiBackup(model, &model->mSaturatedFat, model->mSaturatedFat, &mSaturatedFat).toDouble();
	mTransFat = initiBackup(model, &model->mTransFat, model->mTransFat, &mTransFat).toDouble();
	mPolyUnsaturatedFat = initiBackup(model, &model->mPolyUnsaturatedFat, model->mPolyUnsaturatedFat, &mPolyUnsaturatedFat).toDouble();
	mMonoUnsaturatedFat = initiBackup(model, &model->mMonoUnsaturatedFat, model->mMonoUnsaturatedFat, &mMonoUnsaturatedFat).toDouble();
	mCholesterol = initiBackup(model, &model->mCholesterol, model->mCholesterol, &mCholesterol).toDouble();
	mSodium = initiBackup(model, &model->mSodium, model->mSodium, &mSodium).toDouble();
	mTotalCarbohydrate = initiBackup(model, &model->mTotalCarbohydrate, model->mTotalCarbohydrate, &mTotalCarbohydrate).toDouble();
	mDietaryFiber = initiBackup(model, &model->mDietaryFiber, model->mDietaryFiber, &mDietaryFiber).toDouble();
	mSugar = initiBackup(model, &model->mSugar, model->mSugar, &mSugar).toDouble();
	mProtein = initiBackup(model, &model->mProtein, model->mProtein, &mProtein).toDouble();
	mCalcium = initiBackup(model, &model->mCalcium, model->mCalcium, &mCalcium).toDouble();
	mIron = initiBackup(model, &model->mIron, model->mIron, &mIron).toDouble();
	mPotassium = initiBackup(model, &model->mPotassium, model->mPotassium, &mPotassium).toDouble();
	mVitaminA = initiBackup(model, &model->mVitaminA, model->mVitaminA, &mVitaminA).toDouble();
	mVitaminC = initiBackup(model, &model->mVitaminC, model->mVitaminC, &mVitaminC).toDouble();

}

FoodModel* FoodModel::clone(QObject*parent) {
	auto model = new FoodModel(this, parent);
	model->clearBackupValues();
	return model;
}

void FoodModel::initRandomValues(){
	static int count = 0;
	mId = 0;
	mBrand = "Random Food "+QString::number(++count);
	mDescription = "A random food for debugging or example";
	mServingUnitId = 1;
	mServingSize = std::rand() * 40.0 / RAND_MAX;
	mCalories = std::rand() * 1000.0 / RAND_MAX;
	mTotalFat = std::rand() * 100.0 / RAND_MAX;
	mSaturatedFat = std::rand() * 50.0 / RAND_MAX;
	mTransFat = std::rand() * 50.0 / RAND_MAX;
	mPolyUnsaturatedFat = std::rand() * 50.0 / RAND_MAX;
	mMonoUnsaturatedFat = std::rand() * 50.0 / RAND_MAX;
	mCholesterol = std::rand() * 50.0 / RAND_MAX;
	mSodium = std::rand() * 50.0 / RAND_MAX;
	mTotalCarbohydrate = std::rand() * 100.0 / RAND_MAX;
	mDietaryFiber = std::rand() * 50.0 / RAND_MAX;
	mSugar = std::rand() * 50.0 / RAND_MAX;
	mProtein = std::rand() * 100.0 / RAND_MAX;
	mCalcium = std::rand() * 50.0 / RAND_MAX;
	mIron = std::rand() * 100.0 / RAND_MAX;
	mPotassium = std::rand() * 100.0 / RAND_MAX;
	mVitaminA = std::rand() * 100.0 / RAND_MAX;
	mVitaminC = std::rand() * 100.0 / RAND_MAX;
}

DEFINE_GETSET(FoodModel,qint64,id,Id)
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
	setIsSaved(getId() > 0);
}
void FoodModel::undo(){
	DEFINE_UNDO_LONGLONG(Id)
	DEFINE_UNDO_STRING(OpenFoodFactsCode)
	DEFINE_UNDO_STRING(Brand)
	DEFINE_UNDO_STRING(ImageUrl)
	DEFINE_UNDO_STRING(Description)
	DEFINE_UNDO_LONGLONG(ServingUnitId)
	DEFINE_UNDO_DOUBLE(ServingSize)
	DEFINE_UNDO_DOUBLE(ServingsPerContainer)
	DEFINE_UNDO_DOUBLE(Calories)
	DEFINE_UNDO_DOUBLE(TotalFat)
	DEFINE_UNDO_DOUBLE(SaturatedFat)
	DEFINE_UNDO_DOUBLE(TransFat)
	DEFINE_UNDO_DOUBLE(PolyUnsaturatedFat)
	DEFINE_UNDO_DOUBLE(MonoUnsaturatedFat)
	DEFINE_UNDO_DOUBLE(Cholesterol)
	DEFINE_UNDO_DOUBLE(Sodium)
	DEFINE_UNDO_DOUBLE(TotalCarbohydrate)
	DEFINE_UNDO_DOUBLE(DietaryFiber)
	DEFINE_UNDO_DOUBLE(Sugar)
	DEFINE_UNDO_DOUBLE(Protein)
	DEFINE_UNDO_DOUBLE(Calcium)
	DEFINE_UNDO_DOUBLE(Iron)
	DEFINE_UNDO_DOUBLE(Potassium)
	DEFINE_UNDO_DOUBLE(VitaminA)
	DEFINE_UNDO_DOUBLE(VitaminC)
	QmlModel::undo();
}

bool FoodModel::save(){
	if(mId>0 && !mIsEdited) return true;// Avoid update for nothing
	qDebug() << "Saving " << mTablePrefix  << mBrand << mDescription;
	DatabaseQuery query;

	query.begin(mId == 0 ? DatabaseQuery::Insert : DatabaseQuery::Update, mTablePrefix+"s" );

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
	addQueryValues(query);
	query.addConditionnal(mTablePrefix+"_id",mId);
	if(mId == 0){
		if(!query.exec()){
			qCritical() << "Cannot save " << mTablePrefix << " : " << query.mQuery.lastError().text();
			return false;
		}
		auto fieldNo = query.mQuery.record().indexOf(mTablePrefix+"_id");
		while (query.mQuery.next()) {
			setId(query.mQuery.value(fieldNo).toInt());
			qDebug() << "Insert " << mTablePrefix  << ": " << mId;
		}
	}else{
		if(!query.exec()) {
			qCritical() << "Cannot update " << mTablePrefix << query.mQuery.lastError().text();
			return false;
		}else {
			qDebug() << "Update " << mTablePrefix  << ": " << mId;
		}
	}
	clearBackupValues();
	return true;
}

void FoodModel::saveValues(DatabaseQuery &query){
}

void FoodModel::remove(){
	if(mId > 0){
		DatabaseQuery query;
		query.begin(DatabaseQuery::Delete, "mTablePrefix+s");
		query.addConditionnal(mTablePrefix+"_id",mId);
		if(!query.exec()){
			if(!query.exec()) qCritical() << "Cannot delete " << mTablePrefix << " : "  << query.mQuery.lastError().text();
		}
	}
	emit removed(this);
}

FoodModel *FoodModel::build(QSqlQuery &query, QObject * parent) {
	FoodModel * model = new FoodModel(parent);
	model->load(query);
	return model;
}

void FoodModel::load(QSqlQuery &query){
	// TODO optimize
	for(int i = 0 ; i < query.record().count() ; ++i){
		QString fieldName = query.record().field(i).name();
		if(fieldName == mTablePrefix+"_id") setId(query.value(i).toInt());
		else if(fieldName == "open_food_facts_code") setOpenFoodFactsCode(query.value(i).toString());
		else if(fieldName == "image_url") setImageUrl(query.value(i).toString());
		else if(fieldName == "brand") setBrand(query.value(i).toString());
		else if(fieldName == "description") setDescription(query.value(i).toString());
		else if(fieldName == "serving_size") setServingSize(query.value(i).toDouble());
		else if(fieldName == "servings_per_container") setServingsPerContainer(query.value(i).toDouble());
		else if(fieldName == "serving_unit_id") setServingUnitId(query.value(i).toLongLong());
		else if(fieldName == "calories") setCalories(query.value(i).toDouble());
		else if(fieldName == "total_fat") setTotalFat(query.value(i).toDouble());
		else if(fieldName == "saturated_fat") setSaturatedFat(query.value(i).toDouble());
		else if(fieldName == "trans_fat") setTransFat(query.value(i).toDouble());
		else if(fieldName == "poly_unsaturated_fat") setPolyUnsaturatedFat(query.value(i).toDouble());
		else if(fieldName == "mono_unsaturated_fat") setMonoUnsaturatedFat(query.value(i).toDouble());
		else if(fieldName == "cholesterol") setCholesterol(query.value(i).toDouble());
		else if(fieldName == "sodium") setSodium(query.value(i).toDouble());
		else if(fieldName == "total_carbohydrate") setTotalCarbohydrate(query.value(i).toDouble());
		else if(fieldName == "dietary_fiber") setDietaryFiber(query.value(i).toDouble());
		else if(fieldName == "sugar") setSugar(query.value(i).toDouble());
		else if(fieldName == "protein") setProtein(query.value(i).toDouble());
		else if(fieldName == "calcium") setCalcium(query.value(i).toDouble());
		else if(fieldName == "iron") setIron(query.value(i).toDouble());
		else if(fieldName == "potassium") setPotassium(query.value(i).toDouble());
		else if(fieldName == "vitamin_a") setVitaminA(query.value(i).toDouble());
		else if(fieldName == "vitamin_c") setVitaminC(query.value(i).toDouble());
	}
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
		setId(0);
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
