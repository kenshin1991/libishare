/*****************************************************************************
 * YouTubeAuthenticator.h:
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

#ifndef YOUTUBEAUTHENTICATOR_H
#define YOUTUBEAUTHENTICATOR_H

#include <QObject>

#define LOGIN_URL "https://www.google.com/youtube/accounts/ClientLogin"

class QByteArray;
class QNetworkRequest;
class YouTubeService;

class YouTubeAuthenticator : public QObject
{
    Q_OBJECT

    public:
        YouTubeAuthenticator( const QString& username = "", const QString& password = "" );

        void setCredentials( const QString& username, const QString& password );
        bool setAuthData( QByteArray& data );
        bool isAuthenticated();

    private:
        friend class    YouTubeService;

        QString         getAuthString();
        QNetworkRequest getNetworkRequest();
        QString         getNick();
        QByteArray      getPOSTData();

    protected:
        void            authInit();
        void            setPOSTData();

        /* Youtube Credentials */
        QString         m_username;
        QString         m_password;

        /* HTTP/S POST HEADER */
        QByteArray      m_postData;

        /* Youtube tokens */
        QString         m_authString;
        QString         m_authError;
        QString         m_nick; /* YouTube User Nickname */
        bool            m_isAuthenticated;

    signals:
        void authOK();
        void authError( QString );
};
#endif // YOUTUBEAUTHENTICATOR_H
