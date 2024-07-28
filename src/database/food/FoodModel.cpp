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

#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QQmlApplicationEngine>
#include <QSqlError>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSslSocket>
#include "qstandardpaths.h"
#include "src/database/DatabaseQuery.h"
#include "src/database/unit/UnitListModel.h"

#include "src/tool/Utils.h"

extern QQmlApplicationEngine * gEngine;

FoodModel::FoodModel() : FoodModel(nullptr){	// QML
	gEngine->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}
FoodModel::FoodModel(QObject *parent)
	: QmlModel{parent} {
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	connect(this, &FoodModel::idChanged, this, &FoodModel::updateIsSaved);
	//connect(this, &FoodModel::servingSizeChanged, this, &FoodModel::recomputeFromServingSize);
	mTablePrefix = "food";
}

FoodModel::FoodModel(const FoodModel * model, QObject *parent)
	: QmlModel{parent} {
	gEngine->setObjectOwnership(this, QQmlEngine::CppOwnership);// Avoid QML to destroy it when passing by Q_INVOKABLE
	connect(this, &FoodModel::idChanged, this, &FoodModel::updateIsSaved);
	//connect(this, &FoodModel::servingSizeChanged, this, &FoodModel::recomputeFromServingSize);
	mTablePrefix = "food";
	if(model){
		mId = initiBackup(model, &model->mId, model->mId, &mId).toLongLong();
		mOpenFoodFactsCode = initiBackup(model, &model->mOpenFoodFactsCode, model->mOpenFoodFactsCode, &mOpenFoodFactsCode).toString();
		mOpenFoodFactsImageUrl = initiBackup(model, &model->mOpenFoodFactsImageUrl, model->mOpenFoodFactsImageUrl, &mOpenFoodFactsImageUrl).toString();
		mBrand = initiBackup(model, &model->mBrand, model->mBrand, &mBrand).toString();
		mImageUrl = initiBackup(model, &model->mImageUrl, model->mImageUrl, &mImageUrl).toString();
		mDescription = initiBackup(model, &model->mDescription, model->mDescription, &mDescription).toString();
		mServingUnitId = initiBackup(model, &model->mServingUnitId, model->mServingUnitId, &mServingUnitId).toLongLong();
	
		mServingSize = initiBackup(model, &model->mServingSize, model->mServingSize, &mServingSize).toDouble();
		mServingsPerContainer = initiBackup(model, &model->mServingsPerContainer, model->mServingsPerContainer, &mServingsPerContainer).toDouble();
	
		mBaseUnitId = initiBackup(model, &model->mBaseUnitId, model->mBaseUnitId, &mBaseUnitId).toLongLong();
		mBaseSize = initiBackup(model, &model->mBaseSize, model->mBaseSize, &mBaseSize).toDouble();
	
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
	}else {
		mDescription = "Custom";
		mServingSize = mBaseSize;
	}
	
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
	mBaseUnitId = 1;
	mBaseSize = 100.0;
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
DEFINE_GETSET(FoodModel,QString,openFoodFactsImageUrl,OpenFoodFactsImageUrl)
DEFINE_GETSET(FoodModel,QString,brand,Brand)
DEFINE_GETSET(FoodModel,QString,imageUrl,ImageUrl)
DEFINE_GETSET(FoodModel,QString,description,Description)
DEFINE_GETSET(FoodModel,qint64,servingUnitId,ServingUnitId)
DEFINE_GETSET(FoodModel,double,servingSize,ServingSize)
DEFINE_GETSET(FoodModel,double,servingsPerContainer,ServingsPerContainer)

DEFINE_GETSET(FoodModel,double,baseSize,BaseSize)
DEFINE_GETSET(FoodModel,qint64,baseUnitId,BaseUnitId)
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

DEFINE_GETSET(FoodModel,bool,autoCompute,AutoCompute)
DEFINE_SIMPLE_GETSET(FoodModel,QDateTime,lastUsed,LastUsed)

//-------------------------------------------------------------------------------------------------------------------

void FoodModel::recomputeFromServingSize(){
	if(mAutoCompute){
		double oldServingSize = GET_OLD_VALUE_DOUBLE(ServingSize);
		if(oldServingSize>0){

			setCalories(mServingSize * GET_OLD_VALUE_DOUBLE(Calories) / oldServingSize);
			setTotalFat(mServingSize* GET_OLD_VALUE_DOUBLE(TotalFat) / oldServingSize);
			setSaturatedFat(mServingSize* GET_OLD_VALUE_DOUBLE(SaturatedFat) / oldServingSize);
			setTransFat(mServingSize* GET_OLD_VALUE_DOUBLE(TransFat) / oldServingSize);
			setPolyUnsaturatedFat(mServingSize* GET_OLD_VALUE_DOUBLE(PolyUnsaturatedFat) / oldServingSize);
			setMonoUnsaturatedFat(mServingSize* GET_OLD_VALUE_DOUBLE(MonoUnsaturatedFat) / oldServingSize);
			setCholesterol(mServingSize* GET_OLD_VALUE_DOUBLE(Cholesterol) / oldServingSize);
			setSodium(mServingSize* GET_OLD_VALUE_DOUBLE(Sodium) / oldServingSize);
			setTotalCarbohydrate(mServingSize* GET_OLD_VALUE_DOUBLE(TotalCarbohydrate) / oldServingSize);
			setDietaryFiber(mServingSize* GET_OLD_VALUE_DOUBLE(DietaryFiber) / oldServingSize);
			setSugar(mServingSize* GET_OLD_VALUE_DOUBLE(Sugar) / oldServingSize);
			setProtein(mServingSize* GET_OLD_VALUE_DOUBLE(Protein) / oldServingSize);
			setCalcium(mServingSize* GET_OLD_VALUE_DOUBLE(Calcium) / oldServingSize);
			setIron(mServingSize* GET_OLD_VALUE_DOUBLE(Iron) / oldServingSize);
			setPotassium(mServingSize* GET_OLD_VALUE_DOUBLE(Potassium) / oldServingSize);
			setVitaminA(mServingSize* GET_OLD_VALUE_DOUBLE(VitaminA) / oldServingSize);
			setVitaminC(mServingSize* GET_OLD_VALUE_DOUBLE(VitaminC) / oldServingSize);
		}
	}
}

double FoodModel::computeNutriment(double base) {
	return computeNutriment(base, mServingSize, mServingUnitId, mBaseSize, mBaseUnitId);
}

double FoodModel::computeNutriment(double base, double servingSize, qint64 servingUnitId, double baseSize, qint64 baseUnitId) {
	auto baseUnit = UnitListModel::getInstance()->findUnit(baseUnitId);
	auto targetUnit = UnitListModel::getInstance()->findUnit(servingUnitId);
	double newValue;
	double unitFactor = 1.0;
	if(baseSize<=0.0)
		baseSize = 100.0;
	if(baseSize > 0 && baseUnit && targetUnit) {
		if(targetUnit->getGramValue() > 0 && baseUnit->getGramValue() > 0)
			unitFactor = targetUnit->getGramValue() / baseUnit->getGramValue();
		else if(targetUnit->getKcalValue() > 0 && baseUnit->getKcalValue() > 0)
			unitFactor = targetUnit->getKcalValue() / baseUnit->getKcalValue();
		else if(targetUnit->getMeterValue() > 0 && baseUnit->getMeterValue() > 0)
			unitFactor = targetUnit->getMeterValue() / baseUnit->getMeterValue();
		else if(targetUnit->getMilliliterValue() > 0 && baseUnit->getMilliliterValue() > 0)
			unitFactor = targetUnit->getMilliliterValue() / baseUnit->getMilliliterValue();
		else if(targetUnit->getSecondValue() > 0 && baseUnit->getSecondValue() > 0)
			unitFactor = targetUnit->getSecondValue() / baseUnit->getSecondValue();

		newValue = base * unitFactor * servingSize / baseSize;
	}else
		newValue = base;
	return newValue;
}

void FoodModel::updateIsSaved(){
	setIsSaved(getId() > 0);
}
void FoodModel::undo(){
	DEFINE_UNDO_LONGLONG(Id)
	DEFINE_UNDO_STRING(OpenFoodFactsCode)
	DEFINE_UNDO_STRING(OpenFoodFactsImageUrl)
	DEFINE_UNDO_STRING(Brand)
	DEFINE_UNDO_STRING(ImageUrl)
	DEFINE_UNDO_STRING(Description)
	DEFINE_UNDO_LONGLONG(ServingUnitId)
	DEFINE_UNDO_DOUBLE(ServingSize)
	DEFINE_UNDO_LONGLONG(BaseUnitId)
	DEFINE_UNDO_DOUBLE(BaseSize)
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
/*
QString FoodModel::saveImage(){
	QFile image(mImageUrl);
	if(!image.exists() && !mOpenFoodFactsImageUrl.isEmpty()){

	}
}*/

QString FoodModel::generateImagepath(bool unique){
	QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/foods/img/";
	QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
	if(!dir.exists("foods/img/"))
		dir.mkpath("foods/img/");
	QFile image(path+mOpenFoodFactsCode+".png");
	if(unique){
		int count = 0;
		while(image.exists())
			image.setFileName(path+mOpenFoodFactsCode+"_"+QString::number(++count)+".png");
	}
	return image.fileName();
}

int FoodModel::save(){
	if( mImageUrl.isEmpty() && !mOpenFoodFactsImageUrl.isEmpty()){
		connect(this, &FoodModel::imageDownloaded, this, &FoodModel::save, Qt::SingleShotConnection);
		saveImage();
		return 2;
	}
	if(mId>0 && !mIsEdited) return 1;// Avoid update for nothing

	qDebug() << "Saving " << mTablePrefix  << mBrand << mDescription;
	//saveImage();
	DatabaseQuery query;

	query.begin(mId == 0 ? DatabaseQuery::Insert : DatabaseQuery::Update, mTablePrefix+"s" );

	query.add("brand", mBrand);
	query.add("description", mDescription);
	query.add("open_food_facts_code", mOpenFoodFactsCode);
	query.add("open_food_facts_image_url", mOpenFoodFactsImageUrl);
	query.add("image_url", mImageUrl);
	query.add("serving_size", mServingSize);
	query.add("servings_per_container", mServingsPerContainer);
	query.add("serving_unit_id", mServingUnitId);
	query.add("base_size", mBaseSize);
	query.add("base_unit_id", mBaseUnitId);
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
			return 0;
		}
		auto fieldNo = query.mQuery.record().indexOf(mTablePrefix+"_id");
		while (query.mQuery.next()) {
			setId(query.mQuery.value(fieldNo).toInt());
			qDebug() << "Insert " << mTablePrefix  << ": " << mId;
		}
	}else{
		if(!query.exec()) {
			qCritical() << "Cannot update " << mTablePrefix << query.mQuery.lastError().text();
			return 0;
		}else {
			qDebug() << "Update " << mTablePrefix  << ": " << mId;
		}
	}
	clearBackupValues();
	emit saved();
	return 1;
}

void FoodModel::saveValues(DatabaseQuery &query){
}

void FoodModel::remove(){
	if(mId > 0){
		DatabaseQuery query;
		query.begin(DatabaseQuery::Delete, mTablePrefix+"s");
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
		else if(fieldName == "open_food_facts_image_url") setOpenFoodFactsImageUrl(query.value(i).toString());
		else if(fieldName == "image_url") setImageUrl(query.value(i).toString());
		else if(fieldName == "brand") setBrand(query.value(i).toString());
		else if(fieldName == "description") setDescription(query.value(i).toString());
		else if(fieldName == "serving_size") setServingSize(query.value(i).toDouble());
		else if(fieldName == "servings_per_container") setServingsPerContainer(query.value(i).toDouble());
		else if(fieldName == "serving_unit_id") setServingUnitId(query.value(i).toLongLong());
		else if(fieldName == "base_size") setBaseSize(query.value(i).toDouble());
		else if(fieldName == "base_unit_id") setBaseUnitId(query.value(i).toLongLong());
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
	clearBackupValues();
}

bool FoodModel::loadFromOpenFoodFactsCode(QString code){
	QSqlQuery query;
	if(!query.exec( QStringLiteral("SELECT * FROM foods WHERE open_food_facts_code ='%1'").arg(code) )) qCritical() << "Cannot select food from OFF  : "  << query.lastError().text();
	bool haveResult = false;
	while (query.next()) {
		load(query);
		haveResult = true;
	}
	return haveResult;
}

QList<FoodModel*> FoodModel::buildAll(QObject * parent){
	QList<FoodModel*> models;
	QSqlQuery query;
	if(!query.exec( "SELECT * FROM foods ORDER BY food_id DESC")) qCritical() << "Cannot select all foods  : "  << query.lastError().text();

	while (query.next()) {
		auto model = build(query, parent);
		models << model;
	}

	if(!query.exec( "SELECT open_food_facts_code, brand, description, MAX(consumption_date_time) FROM meal_foods GROUP BY open_food_facts_code, brand, description ORDER BY consumption_date_time DESC")) qCritical() << "Cannot select all meal foods for storage order  : "  << query.lastError().text();
	
	while (query.next()) {
		qInfo() << query.value(3).toLongLong() << query.value(0).toString() << query.value(1).toString() << query.value(2).toString();
		for(int i = 0 ; i < models.size() ; ++i){
			auto item = models[i];
			if(item->getLastUsed().isValid()) continue;
			bool setLast = false;
			QString code = query.value(0).toString();
			if( !code.isEmpty()){
				if(code == item->getOpenFoodFactsCode()){
					item->setLastUsed(QDateTime::fromMSecsSinceEpoch(query.value(3).toLongLong()));
					break;
				}
			}else if(item->getOpenFoodFactsCode().isEmpty() && item->getBrand() == query.value(1).toString() && item->getDescription() == query.value(2).toString()){
				item->setLastUsed(QDateTime::fromMSecsSinceEpoch(query.value(3).toLongLong()));
				break;
			}
		}
	}
	return models;
}

QStringList getAllFields(){
	return {"product","nutriments","brands","brand_owner","ecoscore_data","generic_name","image_url","serving_quantity","serving_quantity_unit"};
}

static QNetworkAccessManager *gManager = new QNetworkAccessManager(nullptr);

void FoodModel::saveImage() {
	QNetworkRequest request;
	QUrl url(mOpenFoodFactsImageUrl);
#ifndef QT_NO_SSL
	if(!QSslSocket::supportsSsl()) {
#endif
		qWarning() << "Https has been requested but SSL is not supported. Fallback to http. Install manually OpenSSL libraries in your PATH.";
		url.setScheme("http");
#ifndef QT_NO_SSL
	}
#endif
	request.setUrl(url);
	//request.setRawHeader("User-Agent", "MyOwnBrowser 1.0");

	QNetworkReply *reply = gManager->get(request);
	connect(reply, &QNetworkReply::finished, this, &FoodModel::openFoodFactsImageDownloaded);
	connect(reply, &QNetworkReply::errorOccurred, this, &FoodModel::handleError);
#ifndef QT_NO_SSL
	connect(reply, &QNetworkReply::sslErrors, this, &FoodModel::handleSslErrors);
	qDebug() << "Device supports OpenSSL: " << QSslSocket::supportsSsl();
#endif
}


void FoodModel::findOpenFoodFacts(const QString& name){
	QNetworkRequest request;
	QUrl url("https://world.openfoodfacts.org/cgi/search.pl?search_terms="+name+"&search_simple=1&action=process&json=1&fields=code,brands,generic_name,image_url");
#ifndef QT_NO_SSL
	if(!QSslSocket::supportsSsl()) {
#endif
		qWarning() << "Https has been requested but SSL is not supported. Fallback to http. Install manually OpenSSL libraries in your PATH.";
		url.setScheme("http");
#ifndef QT_NO_SSL
	}
#endif
	request.setUrl(url);
	//request.setRawHeader("User-Agent", "MyOwnBrowser 1.0");

	QNetworkReply *reply = gManager->get(request);
	connect(reply, &QNetworkReply::finished, this, &FoodModel::openFoodFactsFoundResults);
	connect(reply, &QNetworkReply::errorOccurred, this, &FoodModel::handleError);
#ifndef QT_NO_SSL
	connect(reply, &QNetworkReply::sslErrors, this, &FoodModel::handleSslErrors);
	qDebug() << "Device supports OpenSSL: " << QSslSocket::supportsSsl();
#endif
}

void FoodModel::loadFromOpenFoodFacts(const QString& code) {
	setOpenFoodFactsCode(code);
	qInfo() << "Search for OFF code: " << code;
	if( loadFromOpenFoodFactsCode(code)) return;
	QNetworkRequest request;
	QUrl url("https://ssl-api.openfoodfacts.org/api/v0/product/" +code+".json&fields="+getAllFields().join(','));
#ifndef QT_NO_SSL
	if(!QSslSocket::supportsSsl()) {
#endif
		qWarning() << "Https has been requested but SSL is not supported. Fallback to http. Install manually OpenSSL libraries in your PATH.";
		url.setScheme("http");
#ifndef QT_NO_SSL
	}
#endif
	request.setUrl(url);
	//request.setRawHeader("User-Agent", "MyOwnBrowser 1.0");

	QNetworkReply *reply = gManager->get(request);
	connect(reply, &QNetworkReply::finished, this, &FoodModel::openFoodFactsDownloaded);
	connect(reply, &QNetworkReply::errorOccurred, this, &FoodModel::handleError);
#ifndef QT_NO_SSL
	connect(reply, &QNetworkReply::sslErrors, this, &FoodModel::handleSslErrors);
	qDebug() << "Device supports OpenSSL: " << QSslSocket::supportsSsl();
#endif
}

void FoodModel::openFoodFactsImageDownloaded(){
	auto reply = dynamic_cast<QNetworkReply*>(sender());
	auto data = reply->readAll();
	if(!data.isEmpty()){
		auto path = generateImagepath(false);
		QFile file(path);
		if (!file.open(QIODevice::WriteOnly))
			return;
		file.write(data);
		setImageUrl("file://"+path);
		emit imageDownloaded();
	}
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
		if( product.contains("image_url")) setOpenFoodFactsImageUrl(product["image_url"].toString());
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
		if(	query.exec("SELECT unit_id FROM units WHERE name = 'g'") && query.next()){
			setBaseUnitId(query.value(0).toLongLong());
		}else
			setBaseUnitId(1);
		setBaseSize(100.0);
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
#ifndef QT_NO_SSL
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
#endif

void FoodModel::openFoodFactsFoundResults(){
	auto reply = dynamic_cast<QNetworkReply*>(sender());
	auto document = QJsonDocument::fromJson(reply->readAll());
	if(document.isNull()) return;
	QVariantList results;
	auto rootArray = document.toVariant().toMap();
	if(rootArray.contains("products")){
		auto products = rootArray["products"].toList();
		for(auto productIt : products){
			auto product = productIt.toMap();
			QVariantMap result;
			if(product.contains("code")){
				result["code"] = product["code"];
			}

			if(product.contains("brands")){
				result["brands"] = product["brands"];
			}
			if(product.contains("generic_name")){
				result["generic_name"] = product["generic_name"];
			}
			if(product.contains("image_url")){
				result["image_url"] = product["image_url"];
			}
			results << result;
		}
	}
	emit openFoodFactsFound(results);
}

void FoodModel::handleError (QNetworkReply::NetworkError code) {
	auto reply = dynamic_cast<QNetworkReply*>(sender());
	if (code != QNetworkReply::OperationCanceledError)
		qWarning() << QStringLiteral("Download of %1 failed: %2")
					  .arg(mOpenFoodFactsCode).arg(reply->errorString());

	emit loadingFailed();
}

/*
 * https://openfoodfacts.github.io/openfoodfacts-server/api/ref-v2/#get-/api/v2/search
 * https://wiki.openfoodfacts.org/Open_Food_Facts_Search_API_Version_2
 * https://world.openfoodfacts.org/api/v2/search
 * https://world.openfoodfacts.org/api/v2/search?brands=orange&fields=code,brands,brand_owner,generic_name
 * https://world.openfoodfacts.org/cgi/search.pl?search_terms=banania&search_simple=1&action=process&json=1&fields=code,brands,generic_name,image_url
 * / Eau
// https://ssl-api.openfoodfacts.org/api/v0/product/3257971101015.json
// Lait
// https://ssl-api.openfoodfacts.org/api/v0/product/3520836330036.json
 * */
