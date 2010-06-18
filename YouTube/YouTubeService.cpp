/*****************************************************************************
 * YouTubeService.cpp:
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

#include "YouTubeService.h"

#include <QByteArray>
#include <QtNetwork>
#include <QMessageBox>

YouTubeService::YouTubeService( const QString& username, const QString& password, const QString& devKey ) :
        m_devKey( devKey )
{
    m_auth.setCredentials( username, password );

    YouTubeService();
}

YouTubeService::YouTubeService()
{
    m_nam = new QNetworkAccessManager();

    connect( m_nam, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)),
            this, SLOT(proxyAuthRequired(QNetworkReply*,QAuthenticator*)) );

#ifndef QT_NO_OPENSSL
    connect( m_nam, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
            this, SLOT(sslErrors(QNetworkReply*,QList<QSslError>)) );
#endif

}

YouTubeService::~YouTubeService()
{
    delete m_nam;
}

void
YouTubeService::setDeveloperKey( const QString& devKey )
{
    m_devKey = devKey;
}

void
YouTubeService::setCredentials( const QString& username, const QString& password )
{
    m_auth.setCredentials( username, password );
}


/* Different services */
void
YouTubeService::authenticate()
{
    QByteArray devKeyBA;
    devKeyBA.append( "key=" + m_devKey );

    QNetworkRequest request = m_auth.getNetworkRequest();
    request.setRawHeader( "X-GData-Key", devKeyBA );

    m_reply = m_nam->post( request, m_auth.getPOSTData() );

    connect( m_reply, SIGNAL(finished()),this,SLOT(authFinished()) );
    connect( m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
            SLOT(networkError(QNetworkReply::NetworkError)) );
}

void
YouTubeService::authFinished()
{
    QByteArray data = m_reply->readAll();
    qDebug() << data;

    m_auth.setAuthData( data );

    m_reply->deleteLater();
}

bool
YouTubeService::upload(const QString& file)
{
    if( isAuthenticated() )
    {
        /* Upload Stuff here :) */


        return true;
    }
    return false;
}

void
YouTubeService::search(const QString& search)
{
}

/* Check service states */
bool
YouTubeService::isAuthenticated()
{
    return m_auth.isAuthenticated();
}

bool
YouTubeService::isUploaded()
{
    return true;
}

void
YouTubeService::networkError(QNetworkReply::NetworkError e)
{
}

void
YouTubeService::proxyAuthRequired( QNetworkReply*, QAuthenticator *authenticator )
{
    /* Get username and password from VLMC's settings */
    //authenticator->setUser();
    //authenticator->setPassword();
}

#ifndef QT_NO_OPENSSL
void
YouTubeService::sslErrors( QNetworkReply*, const QList<QSslError> &errors )
{
    QString errorString;
    foreach (const QSslError &error, errors)
    {
        if (!errorString.isEmpty())
            errorString += ", ";
        errorString += error.errorString();
    }

    if ( QMessageBox::warning(NULL, tr("YouTube Authentication"),
                             tr("One or more SSL errors has occurred: %1").arg(errorString),
                             QMessageBox::Ignore | QMessageBox::Abort) == QMessageBox::Ignore )
    {
        m_reply->ignoreSslErrors();
    }
}
#endif
