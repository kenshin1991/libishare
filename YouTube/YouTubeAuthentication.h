/*****************************************************************************
 * YouTubeAuthentication.h:
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

#ifndef YOUTUBEAUTHENTICATION_H
#define YOUTUBEAUTHENTICATION_H

#include "YouTubeServiceStates.h"
#include <QObject>

class YouTubeAuthentication : public QObject
{
    Q_OBJECT

    public:
        YouTubeAuthentication( const QString& username, const QString& password );
        YouTubeAuthentication();
        ~YouTubeAuthentication();

        void setCredentials( const QString& username, const QString& password );

        const QString getAuthUrl();        
        QByteArray    getPOSTData();
        QString       getAuthString();
        QString       getNick();
        bool          isAuthenticated();

        const QString LOGIN_URL = "https://www.google.com/youtube/accounts/ClientLogin";

    private:
        void authInit();
        void setPOSTData();

        /* Youtube Credentials */
        QString        m_username;
        QString        m_password;

        /* HTTP/S POST HEADER */
        QByteArray     m_postData;

        /* Youtube tokens */
        QString        m_authString;
        QString        m_nick; /* YouTube User Nickname */
        bool           m_authentication;

    signals:
        void finished();
        void error(QString message);

    private slots:
        void authenticationFinished();
        void onError(QNetworkReply::NetworkError e);
};
#endif // YOUTUBEAUTHENTICATION_H
