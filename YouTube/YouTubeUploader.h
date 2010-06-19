/*****************************************************************************
 * YouTubeUploader.h:
 *****************************************************************************
 * Copyright (C) 2010 VideoLAN
 *
 * Authors: Rohit Yadav <rohityadav89 AT gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifndef YOUTUBEUPLOADER_H
#define YOUTUBEUPLOADER_H

#include <QObject>
#include <QString>

#define UPLOAD_URL "http://uploads.gdata.youtube.com/feeds/api/users/default/uploads"

class QNetworkRequest;
class YouTubeService;

class YouTubeUploader : public QObject
{
    public:
        YouTubeUploader( YouTubeService* service = 0, QString fileName = "" );

        void setServiceProvider( YouTubeService* service );
        QNetworkRequest getNetworkRequest();
        QByteArray      getPOSTData();

    private:
        YouTubeService* m_service;
        QString         fileName;
        QString         mimeType;
        QString         title;
        QString         category;
        QString         description;
        QString         tags;
        bool            isPrivate;
};

#endif // YOUTUBEUPLOADER_H
