/*****************************************************************************
 * YouTubeAuthenticator.cpp:
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

#include "YouTubeAuthenticator.h"

#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QString>
#include <QStringList>
#include <QUrl>

YouTubeAuthenticator::YouTubeAuthenticator( const QString& username, const QString& password ) :
        m_username( username ), m_password( password )
{
    authInit();
    setPOSTData();
}

void
YouTubeAuthenticator::authInit()
{
    m_isAuthenticated = false;
    m_nick.clear();
    m_authString.clear();
    m_authError.clear();
}

const QByteArray&
YouTubeAuthenticator::getPOSTData()
{
    return m_postData;
}

QNetworkRequest
YouTubeAuthenticator::getNetworkRequest()
{
    authInit();
    QUrl url( LOGIN_URL );

    QNetworkRequest request;
    request.setHeader( QNetworkRequest::ContentTypeHeader,
                       "application/x-www-form-urlencoded" );
    request.setUrl( url );

    return request;
}

const QString&
YouTubeAuthenticator::getAuthString()
{
    return m_authString;
}


const QString&
YouTubeAuthenticator::getNick()
{
    return m_nick;
}

bool
YouTubeAuthenticator::isAuthenticated()
{
    return m_isAuthenticated;
}

void
YouTubeAuthenticator::setCredentials( const QString& username, const QString& password )
{
    m_username = username;
    m_password = password;
    setPOSTData();
}

void
YouTubeAuthenticator::setPOSTData()
{
    m_postData.clear();
    m_postData += QString("accountType=HOSTED_OR_GOOGLE&Email=%1&Passwd=%2"
                          "&service=youtube&source=VLMC").arg(m_username, m_password);
}

bool
YouTubeAuthenticator::setAuthData( QByteArray& data )
{
    QStringList lines = QString( data ).split( "\n", QString::SkipEmptyParts );

    foreach( QString line, lines )
    {
        QStringList tokenList = line.split( "=" );

        if( tokenList.at(0) == "Error" )
        {
            m_authError = tokenList.at(1);
            emit authError( m_authError );
            continue;
        }

        if( tokenList.at(0) == "Auth" )
        {
            m_authString = tokenList.at(1);
            continue;
        }

        if( tokenList.at(0) == "YouTubeUser" )
        {
            m_nick = tokenList.at(1);
        }
    }

    if( !m_authString.isEmpty() && !m_nick.isEmpty() && m_authError.isEmpty() )
        m_isAuthenticated = true;

    emit authOver();
    return m_isAuthenticated;
}

void
YouTubeAuthenticator::authenticate()
{
    QNetworkRequest request = m_auth->getNetworkRequest();

    m_currentReply = m_nam->post( request, m_auth->getPOSTData() );
    qDebug() << "Auth posted!";
    m_state = AUTH_START;

    connect( m_currentReply, SIGNAL(finished()),this,SLOT(authFinished()) );
    connect( m_currentReply, SIGNAL(error(QNetworkReply::NetworkError)),
             this, SLOT(networkError(QNetworkReply::NetworkError)) );
}

void
YouTubeAuthenticator::authFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply *>( sender() );
    QByteArray data = reply->readAll();

    qDebug() << reply << "Auth data: " << data;

    if( m_auth->setAuthData( data ) )
        m_state = AUTH_FINISH;

    disconnect( reply, SIGNAL(finished()),this,SLOT(authFinished()) );
    disconnect( reply, SIGNAL(error(QNetworkReply::NetworkError)),
             this, SLOT(networkError(QNetworkReply::NetworkError)) );

    reply->close();
    reply->deleteLater();
    m_currentReply = NULL;
}
