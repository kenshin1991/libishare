/*****************************************************************************
 * YouTubeService.h: YouTube Service APIs
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

#ifndef YOUTUBESERVICE_H
#define YOUTUBESERVICE_H

#include "AbstractSharingService.h"
#include "YouTubeCommon.h"

#include <QObject>
#include <QNetworkReply>
#include <QString>

class QAuthenticator;
class QByteArray;
class QNetworkAccessManager;
class QSslError;
class QString;
class YouTubeAuthenticator;
class YouTubeUploader;

class YouTubeService : public AbstractSharingService
{
    Q_OBJECT

    friend class YouTubeAuthenticator;
    friend class YouTubeUploader;

    public:
        YouTubeService( const QString& devKey = "", 
                        const QString& username = "",
                        const QString& password = "" );
        ~YouTubeService();

        /* Service Interfaces */
        void authenticate();            // Authenticate the service
        bool upload();                  // Upload video
        void search( QString& search ); // Search for a video
        bool abort();                   // Abort on going service

        const VideoData& getVideoData();

        void setCredentials( const QString& username, const QString& password );
        void setDeveloperKey( const QString& devKey );
        void setProxyCredentials( const QString& username, const QString& password );
        void setVideoParameters( const QString& fileName, const VideoData& data );

    private:

        const QString&         getDeveloperKey();
        const QString&         getAuthString();

        QString                m_devKey;
        QString                m_username;
        QString                m_password;

        QString                m_fileName;
        VideoData              m_videoData;

        YouTubeAuthenticator*  m_auth;
        YouTubeUploader*       m_uploader;

        YouTubeServiceState    m_state;
        YouTubeError           m_error;
        QString                m_proxyUsername;
        QString                m_proxyPassword;

    private slots:
        void authError( QString );
        void networkError( QNetworkReply::NetworkError );
        void proxyAuthRequired( QNetworkReply*, QAuthenticator * );
        #ifndef QT_NO_OPENSSL
        void sslErrors( QNetworkReply*, const QList<QSslError> &errors );
        #endif
};

#endif // YOUTUBESERVICE_H
