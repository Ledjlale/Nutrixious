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

#ifndef IMAGE_PROVIDER_H_
#define IMAGE_PROVIDER_H_

#include <QQuickAsyncImageProvider>

// =============================================================================
class ImageAsyncImageResponse : public QQuickImageResponse {
public:
	ImageAsyncImageResponse(const QString &id, const QSize &requestedSize);

	QQuickTextureFactory *textureFactory() const override;

	void imageGrabbed(QImage image);

	QImage mImage;
	QString mPath;
};

class ImageProvider : public QQuickAsyncImageProvider {
public:
	virtual QQuickImageResponse *requestImageResponse(const QString &id, const QSize &requestedSize) override;

	static const QString ProviderId;
};

#endif
