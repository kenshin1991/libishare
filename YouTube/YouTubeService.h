/*****************************************************************************
 * YouTubeService.h:
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

#include "YouTubeAuthentication.h"
#include "YouTubeUpload.h"

#include "YouTubeServiceStates.h"

#include <QObject>
#include <QNetworkReply>
#include <QString>

class QAuthenticator;
class QByteArray;
class QNetworkAccessManager;
class QSslError;
class QString;

class YouTubeService : public QObject
{
    Q_OBJECT

    public:
        YouTubeService( const QString& username, const QString& password,
                        const QString& devKey );
        YouTubeService();
        ~YouTubeService();

        void setDeveloperKey( const QString& devKey );
        void setCredentials( const QString& username, const QString& password );
        void setProxyCredentials( const QString& username, const QString& password );

        /* Services */
        void authenticate();
        bool upload( const QString& fileName );
        void search( const QString& search );

    private:
        YouTubeServiceState     m_state;
        QString                 m_devKey;
        YouTubeAuthentication   m_auth;
        YouTubeUpload*          m_uploader;

        QNetworkAccessManager*  m_nam;
        QNetworkReply*          m_reply;

        QString                 m_proxyUsername;
        QString                 m_proxyPassword;

    private slots:
        void authFinished();
        //void uploadFinished();
        //void searchFinished();

        void proxyAuthRequired( QNetworkReply*, QAuthenticator * );
        void authError( QString );
        void networkError( QNetworkReply::NetworkError );
        #ifndef QT_NO_OPENSSL
        void sslErrors( QNetworkReply*,const QList<QSslError> &errors );
        #endif

    signals:
        void authOK();
};

#endif // YOUTUBESERVICE_H
