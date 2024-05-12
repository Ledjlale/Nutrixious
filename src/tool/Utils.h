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

#ifndef UTILS_H
#define UTILS_H

#include <QList>

class Utils{
public:
	template<class Model>
	static Model* add(Model * model, qint64 parentId, QObject * parent, QList<Model*> &list){
		int order = model->getOrder();
		Model * insertedModel = nullptr;
		if(order < 0) {
			list.push_back(model->clone(parentId, parent));
			insertedModel = list.back();
			insertedModel->setOrder(list.size());
		}else{
			if( list.size() == 0){
				list.push_back(model->clone(parentId, parent));
			insertedModel = list.back();
		}else {
			auto it = list.begin();
			while(it != list.end() && (*it)->getOrder() <= order)
				++it;
			insertedModel = *list.insert(it, model->clone(parentId, parent));
		}
	}
	return insertedModel;
	}

	template<class Model>
	static bool decrementOrder(Model *model, QList<Model*> &list) {
		for(auto it = list.begin() ; it != list.end() ; ++it){
			if(*it == model){
				if(it == list.begin()) return false;
				else{
					auto itModel = it;
					(*--it)->setOrder((*it)->getOrder()+1);
					(*itModel)->setOrder((*itModel)->getOrder()-1);
					qSwap(*it, *itModel);
					return true;
				}
			}
		}
		return false;
	}
	template<class Model>
	static bool incrementOrder(Model *model, QList<Model*> &list) {
		for(auto it = list.begin() ; it != list.end() ; ++it){
			if(*it == model){
				if(it == list.end()) return false;
				else{
					auto itModel = it;
					(*++it)->setOrder((*it)->getOrder()-1);
					(*itModel)->setOrder((*itModel)->getOrder()+1);
					qSwap(*it, *itModel);
					return true;
				}
			}
		}
		return false;
	}
};

#endif
