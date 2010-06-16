/*****************************************************************************
 * YouTubeService.h:
 *****************************************************************************
 * Copyright (C) 2010 VideoLAN
 *
 * Authors: Rohit Yadav <rohityadav89@gmail.com>
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

#ifndef YOUTUBESERVICE_H
#define YOUTUBESERVICE_H

#include "YouTubeAuthentication.h"

#include <QObject>
#include <QString>

class YouTubeService : public QObject
{
    Q_OBJECT

    public:
        YouTubeService( const QString& username, const QString& password, const QString& devKey );
        YouTubeService();
        ~YouTubeService();

        void setDeveloperKey(const QString& devKey);
        void setCredentials( const QString& username, const QString& password );

        /* Different services */
        void authenticate();
        bool upload(const QString& file);
        void search(const QString& search);

        /* Check service states */
        bool isAuthenticated();
        bool isUploaded();

    private:
        QString                 m_devKey;
        YouTubeAuthentication*  m_ytAuth;

    signals:
        void authenticationFinished();
        void uploadFinished();
        void searchFinished();

        void error(QString message);
};

#endif // YOUTUBESERVICE_H
