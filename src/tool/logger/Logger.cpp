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

#include "Logger.h"


#include <QStandardPaths>
#include <QMutex>
#include <QFile>
#include <QFileInfo>
#include <QDir>

QFile Logger::gFile;
int Logger::gFileCount = 0;
QMutex Logger::gFileLocker;
const int Logger::gMaxFileSize;
QtMessageHandler Logger::gOriginalHandler = nullptr;

Logger::Logger(QObject * parent) : QObject(parent){
}

void Logger::init(){
	gOriginalHandler = qInstallMessageHandler(logToFile);
}

void Logger::logToFile(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
	QString message = qFormatLogMessage(type, context, msg);
	QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

	gFileLocker.lock();
	if(!gFile.exists()){
		QFileInfo file0(path+"/nutrixious_0.log");
		QFileInfo file1(path+"/nutrixious_1.log");
		bool removeCurrentFile = false;
		if(!file0.exists()) {
			gFileCount = 0;
		}else if(!file1.exists()) {
			if( file0.size() > gMaxFileSize){
				gFileCount = 1;
			}else {
				gFileCount = 0;
			}
		}else if(file0.lastModified() > file1.lastModified()){
			if(file0.size() > gMaxFileSize){
				gFileCount = 1;
				removeCurrentFile = true;
			}else{
				gFileCount = 0;
			}
		}else {
			if(file1.size() > gMaxFileSize){
				gFileCount = 0;
				removeCurrentFile = true;
			}else{
				gFileCount = 1;
			}
		}
		gFile.setFileName(path+"/nutrixious_"+QString::number(gFileCount)+".log");
		if(removeCurrentFile) gFile.remove();
		gFile.open(QIODeviceBase::WriteOnly | QIODeviceBase::Append);
	}else{
		if(gFile.size() > gMaxFileSize){
			gFile.close();
			gFileCount = (gFileCount +1)%2;
			gFile.setFileName(path+"/nutrixious_"+QString::number(gFileCount)+".log");
			gFile.remove();
			gFile.open(QIODeviceBase::WriteOnly | QIODeviceBase::Append);
		}
	}
	gFile.write(qPrintable(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss") + ": " +message+"\n"));
	gFile.flush();

	gFileLocker.unlock();
	if (gOriginalHandler)
		(*gOriginalHandler)(type, context, msg);
}

QString Logger::getRawLogs(){
	QDir path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
	auto files = path.entryInfoList(QStringList("nutrixious*.log"), QDir::Files | QDir::NoSymLinks | QDir::Readable, QDir::Time | QDir::Reversed);
	QString result;
	for(auto fileInfo : files){
		QFile file(fileInfo.filePath());
		if (file.open(QIODevice::ReadOnly)) {
			QByteArray arr = file.readAll();
			result += QString::fromLatin1(arr);
			file.close();
		}
	}
	return result;
}
QStringList Logger::getLogs(){
	return getRawLogs().split("\n");
}

void Logger::cleanLogs(){
	QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

	gFileLocker.lock();
	gFile.close();
	for(int i = 0 ; i < 2 ; ++i){
		QFile file(path+"/nutrixious_"+QString::number(i)+".log");
		if(file.exists()) file.remove();
	}
	gFileCount = 0;
	gFile.setFileName(path+"/nutrixious_"+QString::number(gFileCount)+".log");
	gFile.open(QIODeviceBase::WriteOnly | QIODeviceBase::Append);
	gFileLocker.unlock();
}
