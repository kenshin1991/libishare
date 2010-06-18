/*****************************************************************************
 * YouTubeAuthentication.cpp:
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

#include "YouTubeAuthentication.h"

#include <QByteArray>
#include <QNetworkRequest>
#include <QString>
#include <QStringList>
#include <QUrl>

#include <QDebug>

YouTubeAuthentication::YouTubeAuthentication( const QString& username, const QString& password ) :
        m_username( username ), m_password( password )
{
    YouTubeAuthentication();
}

YouTubeAuthentication::YouTubeAuthentication()
{
}

YouTubeAuthentication::~YouTubeAuthentication()
{
    m_username = "";
    m_password = "";
    m_authString = "";
}

void
YouTubeAuthentication::authInit()
{
    m_authentication = false;
    m_nick.clear();
    m_authString.clear();
    m_youTubeError.clear();
}

void
YouTubeAuthentication::setCredentials( const QString& username, const QString& password )
{
    m_username = username;
    m_password = password;
    setPOSTData();
}

void
YouTubeAuthentication::setPOSTData()
{
    m_postData.clear();
    m_postData.append( QString("accountType=HOSTED_OR_GOOGLE"
                               "&Email=%1&Passwd=%2"
                               "&service=youtube&source=VLMC")
                              .arg(m_username, m_password) );
}

QByteArray
YouTubeAuthentication::getPOSTData()
{
    return m_postData;
}

QNetworkRequest
YouTubeAuthentication::getNetworkRequest()
{
    authInit();
    QUrl url( LOGIN_URL );

    QNetworkRequest request;
    request.setHeader( QNetworkRequest::ContentTypeHeader,
                       "application/x-www-form-urlencoded" );
    request.setUrl( url );
}

const QString
YouTubeAuthentication::getAuthUrl()
{
    return LOGIN_URL;
}

QString
YouTubeAuthentication::getAuthString()
{
    return m_youTubeAuthString;
}


QString
YouTubeAuthentication::getNick()
{
    return m_youTubeUser;
}

bool
YouTubeAuthentication::isAuthenticated()
{
    return m_authentication;
}

void
YouTubeAuthentication::authenticationFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply *>(sender());
    QByteArray data = reply->readAll();

    qDebug() << data;

    QStringList lines = QString(data).split( "\n", QString::SkipEmptyParts );

    foreach( QString line, lines )
    {
        QStringList tokenList = line.split("=");

        if(tokenList.at(0) == "Auth")
            m_youTubeAuthString = tokenList.at(1);

        if(tokenList.at(0) == "YouTubeUser")
            m_youTubeUser = tokenList.at(1);

        if(tokenList.at(0) == "Error")
        {
            m_youTubeError = tokenList.at(1);
            emit error(m_youTubeError);
        }
    }

    if( !m_youTubeAuthString.isEmpty() &&
        !m_youTubeUser.isEmpty() &&
        m_youTubeError.isEmpty() )
    {
        m_authentication = true;
        emit finished();
    }

    reply->deleteLater();
}

