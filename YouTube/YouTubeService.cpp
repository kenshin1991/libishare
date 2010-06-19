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

#include "YouTubeAuthenticator.h"
#include "YouTubeService.h"
#include "YouTubeUploader.h"

#include <QByteArray>
#include <QMessageBox>
#include <QtNetwork>

#include <QDebug>

YouTubeService::YouTubeService( const QString& devKey, const QString& username, const QString& password ) :
        m_devKey( devKey )
{
    m_auth = new YouTubeAuthenticator( username, password );

    /* Tell world on successful authentication */
    connect( m_auth, SIGNAL(authOK()), this, SIGNAL(authOK()) );

    /* On authentication error, m_auth will send the error token */
    connect( m_auth, SIGNAL(authError(QString)), this, SLOT(authError(QString)) );

    m_nam = new QNetworkAccessManager();

    /* In case the proxy asks for credentials, handle it */
    connect( m_nam, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)),
            this, SLOT(proxyAuthRequired(QNetworkReply*,QAuthenticator*)) );

    /* If SSL is available, handle SSL errors for better security */
    #ifndef QT_NO_OPENSSL
    connect( m_nam, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
            this, SLOT(sslErrors(QNetworkReply*,QList<QSslError>)) );
    #endif
}

YouTubeService::~YouTubeService()
{
    delete m_auth;
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
    m_auth->setCredentials( username, password );
}

void
YouTubeService::setProxyCredentials(const QString &username, const QString &password)
{
    m_proxyUsername = username;
    m_proxyPassword = password;
}

QString
YouTubeService::getAuthString()
{
    return m_auth->getAuthString();
}

QString
YouTubeService::getDeveloperKey()
{
    return m_devKey;
}

void
YouTubeService::authenticate()
{
    QByteArray devKeyBA;
    devKeyBA.append( "key=" + m_devKey );

    QNetworkRequest request = m_auth->getNetworkRequest();
    
    if( !m_devKey.isEmpty() )
        request.setRawHeader( "X-GData-Key", devKeyBA );

    m_reply = m_nam->post( request, m_auth->getPOSTData() );

    connect( m_reply, SIGNAL(finished()),this,SLOT(authFinished()) );
    connect( m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
            SLOT(networkError(QNetworkReply::NetworkError)) );
}

void
YouTubeService::authFinished()
{
    QByteArray data = m_reply->readAll();
    qDebug() << data;

    /* Disconnect local mappings, just in case authenticate is called again */
    disconnect( m_reply, SIGNAL(finished()),this,SLOT(authFinished()) );
    disconnect( m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this,
            SLOT(networkError(QNetworkReply::NetworkError)) );

    if( m_auth->setAuthData( data ) )
        m_state = YouTubeServiceStates::Ok;

    m_reply->deleteLater();
}

bool
YouTubeService::upload(const QString& fileName)
{
    if( m_auth->isAuthenticated() )
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

void
YouTubeService::authError(QString e)
{
    qDebug() << e;

    if( e == "BadAuthentication" )
        m_state = YouTubeServiceStates::BadAuthentication;
    else
    if( e == "CaptchaRequired")
        m_state = YouTubeServiceStates::CaptchaRequired;
    else
    if( e == "ServiceUnavailable")
        m_state = YouTubeServiceStates::ServiceUnavailable;
    else
        m_state = YouTubeServiceStates::UnknownError;

}

void
YouTubeService::networkError(QNetworkReply::NetworkError e)
{
    m_state = YouTubeServiceStates::NetworkError;
}

void
YouTubeService::proxyAuthRequired( QNetworkReply*, QAuthenticator *authenticator )
{
    m_state = YouTubeServiceStates::ConnectionError;

    /* */
    if( !m_proxyUsername.isEmpty() && !m_proxyPassword.isEmpty() )
    {
        authenticator->setUser( m_proxyUsername );
        authenticator->setPassword( m_proxyPassword );
        return;
    }

    /* TODO: Exec a dialog take in credentials */
}

#ifndef QT_NO_OPENSSL
void
YouTubeService::sslErrors( QNetworkReply*, const QList<QSslError> &errors )
{
    m_state = YouTubeServiceStates::SSLError;

    QString errorString;
    foreach (const QSslError &error, errors)
    {
        if (!errorString.isEmpty())
            errorString += ", ";
        errorString += error.errorString();
    }

    /* Prompt for insecure connection */
    if ( QMessageBox::warning(NULL, tr("YouTube Authentication"),
                             tr("Connection may be insecure, do you want to continue?"
                                "One or more SSL errors has occurred: %1").arg(errorString),
                             QMessageBox::Ignore | QMessageBox::Abort) == QMessageBox::Ignore )
    {
        m_reply->ignoreSslErrors();
    }
}
#endif
