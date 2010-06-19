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

#include "YouTubeServiceStates.h"

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

class YouTubeService : public QObject
{
    Q_OBJECT

    public:
        YouTubeService( const QString& devKey = "", const QString& username = "",
                        const QString& password = "" );
        ~YouTubeService();

        void setDeveloperKey( const QString& devKey );
        void setCredentials( const QString& username, const QString& password );
        void setProxyCredentials( const QString& username, const QString& password );
        void setVideoParameters( const QString& fileName, const QString& title, const QString& description,
                                 const QString& category, const QString& keywords, bool isPrivate );

        /* Services */
        void authenticate();
        bool upload();
        void search( QString& search );

    private:
        friend class           YouTubeUploader;

        QString                getDeveloperKey();
        QString                getAuthString();

    protected:
        YouTubeServiceState    m_state;
        QString                m_devKey;
        QString                m_fileName;
        YouTubeAuthenticator*  m_auth;
        YouTubeUploader*       m_uploader;
        QString                m_proxyUsername;
        QString                m_proxyPassword;

        QNetworkAccessManager* m_nam;

    private slots:
        void authFinished();
        void uploadFinished();
        //void searchFinished();

        void proxyAuthRequired( QNetworkReply*, QAuthenticator * );
        void authError( QString );
        void networkError( QNetworkReply::NetworkError );
        #ifndef QT_NO_OPENSSL
        void sslErrors( QNetworkReply*,const QList<QSslError> &errors );
        #endif

    signals:
        void authOK();
        void uploadOK( QString );
        void uploadProgress( qint64, qint64 );
        void serviceError( QString );
};

#endif // YOUTUBESERVICE_H
