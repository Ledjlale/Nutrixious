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

#include "Utils.h"
#include <QCoreApplication>

Utils::Utils(QObject * parent) : QObject(parent){
}

void Utils::sendMousePressEvent(QQuickItem* object, int button, int buttons, int modifiers, double x, double y) {
	QPointF pos(x,y);
	//pos = sender->mapToItem(object, pos);
	QMouseEvent * event = new QMouseEvent(QEvent::MouseButtonPress, pos, object->mapToGlobal(pos), (Qt::MouseButton)button, (Qt::MouseButtons)buttons, (Qt::KeyboardModifiers)modifiers);
	QCoreApplication::postEvent(object, event);
}

void Utils::sendMouseReleaseEvent(QQuickItem* object, int button, int buttons, int modifiers, double x, double y) {
	QPointF pos(x,y);
	QMouseEvent * event = new QMouseEvent(QEvent::MouseButtonRelease, pos, object->mapToGlobal(pos), (Qt::MouseButton)button, (Qt::MouseButtons)buttons, (Qt::KeyboardModifiers)modifiers);
	QCoreApplication::postEvent(object, event);
}
void Utils::sendMouseClickEvent( QQuickItem* object, int button, int buttons, int modifiers, double x, double y) {
	sendMousePressEvent(object, button, buttons, modifiers, x, y);
	sendMouseReleaseEvent(object, button, buttons, modifiers, x, y);
}

/*
void Utils::sendMousePressEvent(QQuickItem* sender, QQuickItem* object, int button, int buttons, int modifiers, double x, double y) {
	QPointF pos(x,y);
	pos = sender->mapToItem(object, pos);
	QMouseEvent * event = new QMouseEvent(QEvent::MouseButtonPress, pos, object->mapToGlobal(pos), (Qt::MouseButton)button, (Qt::MouseButtons)buttons, (Qt::KeyboardModifiers)modifiers);
	QCoreApplication::postEvent(object, event);
}

void Utils::sendMouseReleaseEvent(QQuickItem* sender, QQuickItem* object, int button, int buttons, int modifiers, double x, double y) {
	QPointF pos(x,y);
	pos = sender->mapToItem(object, pos);
	QMouseEvent * event = new QMouseEvent(QEvent::MouseButtonRelease, pos, object->mapToGlobal(pos), (Qt::MouseButton)button, (Qt::MouseButtons)buttons, (Qt::KeyboardModifiers)modifiers);
	QCoreApplication::postEvent(object, event);
}
void Utils::sendMouseClickEvent(QQuickItem* sender, QQuickItem* object, int button, int buttons, int modifiers, double x, double y) {
	sendMousePressEvent(sender, object, button, buttons, modifiers, x, y);
	sendMouseReleaseEvent(sender, object, button, buttons, modifiers, x, y);
}
*/

/*
void Utils::sendMouseEvent(QObject* object, QMouseEvent * mouseEvent) {
	QMouseEvent * event = new QMouseEvent(mouseEvent->type(), mouseEvent->position(), mouseEvent->globalPosition(), mouseEvent->button(), mouseEvent->buttons(), mouseEvent->modifiers());

	//QMouseEvent(QEvent::Type type, const QPointF &localPos, const QPointF &globalPos, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers, const QPointingDevice *device = QPointingDevice::primaryPointingDevice())
	QCoreApplication::postEvent(object, event);
}
*/
	/*
	Q_INVOKABLE static void sendMouseEvent(QObject* object, QPointF pos, int button){
		Qt::MouseButton type = Qt::MouseButton(button);
		auto down = new QMouseEvent(QMouseEvent::Type::MouseButtonPress, pos, type, type, Qt::KeyboardModifier::NoModifier);
		auto up = new QMouseEvent(QMouseEvent::Type::MouseButtonRelease, pos, type, type, Qt::KeyboardModifier::NoModifier);
		QCoreApplication::postEvent(object, down);
		QCoreApplication::postEvent(object, up);
	}*/
