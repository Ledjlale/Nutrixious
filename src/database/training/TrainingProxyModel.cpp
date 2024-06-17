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

#include "TrainingProxyModel.h"
#include "TrainingListModel.h"


TrainingProxyModel::TrainingProxyModel(QObject *parent)
	: SortFilterProxyModel{parent}
{
	setSourceModel(new TrainingListModel());
	sort(0);
}


DateModel* TrainingProxyModel::getTrainingDay()const{
	return mTrainingDay;
}

void TrainingProxyModel::setTrainingDay(DateModel * data){
	if(mTrainingDay != data){
		if(mTrainingDay){
			disconnect(mTrainingDay);
		}
		mTrainingDay = data;
		if(mTrainingDay){
			connect(mTrainingDay, &DateModel::dateChanged, this, &TrainingProxyModel::invalidateFilter);
			connect(mTrainingDay, &DateModel::dateChanged, this, &TrainingProxyModel::updateCalories);
		}
		emit trainingDayChanged();
	}
}

DEFINE_SIMPLE_GETSET(TrainingProxyModel,double,calories,Calories)

void TrainingProxyModel::update(){
	sourceModel()->deleteLater();
	setSourceModel(new TrainingListModel());
	sort(0);
}

void TrainingProxyModel::updateCalories(){
	auto model = dynamic_cast<TrainingListModel*>(sourceModel());
	double calories = 0.0;
	if( mTrainingDay) {
		for(int i = 0 ; i < model->getCount() ; ++i){
			auto item = model->getAt(i);
			if( item->getStartDateTime().date() == mTrainingDay->getDate()){
				calories += item->getCalories();
			}
		}
	}
	setCalories(calories);
}

bool TrainingProxyModel::lessThan (const QModelIndex &left, const QModelIndex &right) const{
	auto l = sourceModel()->data(left);
	auto r = sourceModel()->data(right);
	return l.value<TrainingModel*>()->getStartDateTime() > r.value<TrainingModel*>()->getStartDateTime();
}

bool TrainingProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const{
	if(!mTrainingDay) return true;
	else{
		QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
		auto model = sourceModel()->data(index0).value<TrainingModel*>();
		return model->getStartDateTime().date() == mTrainingDay->getDate();
	}
}
