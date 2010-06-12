/*****************************************************************************
 * YouTubeAuthentication.cpp:
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

#include "YouTubeAuthentication.h"
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkProxy>
#include <QString>
#include <QStringList>
#include <QUrl>


YouTubeAuthentication::YouTubeAuthentication( QString& username, QString& password ) :
        m_username( username ), m_password( password )
{
    setPostData();
}

void
YouTubeAuthentication::setCredentials( QString& username, QString& password )
{
    m_username = username;
    m_password = password;
    setPostData();
}

void
YouTubeAuthentication::setPostData()
{
    m_postData.clear();
    m_postData.append( QString("Email=%1&Passwd=%2&service=youtube&source=VLMC")
                              .arg(m_username, m_password) );
}

void
YouTubeAuthentication::authenticate()
{    
    QUrl url(CLIENT_LOGIN_URL);

    QNetworkRequest request;
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setUrl(url);

    QNetworkAccessManager *networkManager = new QNetworkAccessManager();
    QNetworkReply *reply = networkManager->post(request, m_postData);

    connect(reply, SIGNAL(finished()),this,SLOT(authenticationFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            SLOT(onError(QNetworkReply::NetworkError)));
}

void
YouTubeAuthentication::authenticationFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply *>(sender());
    QByteArray data = reply->readAll();

    QStringList lines = QString(data).split( "\n", QString::SkipEmptyParts );

    foreach( QString line, lines )
    {
        QStringList tokenList = line.split("=");

        if(tokenList.at(0) == "Auth")
            m_youTubeAuthString = tokenList.at(1);

        if(tokenList.at(0) == "YouTubeUser")
            m_youTubeUser = tokenList.at(1);

        if(tokenList.at(0) == "Error")
            m_youTubeError = tokenList.at(1);
    }
}

QString
YouTubeAuthentication::getYouTubeAuthString()
{
    return m_youTubeAuthString;
}


QString
YouTubeAuthentication::getYouTubeUser()
{
    return m_youTubeUser;
}


QString
YouTubeAuthentication::getYouTubeError()
{
    return m_youTubeError;
}


void
YouTubeAuthentication::onError(QNetworkReply::NetworkError e)
{
    m_error = e;
}

QNetworkReply::NetworkError
YouTubeAuthentication::networkrror()
{
    return m_error;
}
