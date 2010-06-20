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
#include "UploaderIODevice.h"

#include <QByteArray>
#include <QMessageBox>
#include <QtNetwork>

#include <QDebug>

YouTubeService::YouTubeService( const QString& devKey, const QString& username, const QString& password )
{
    m_devKey = devKey;
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
    delete m_nam;
    delete m_auth;
    delete m_uploader;
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

void
YouTubeService::setVideoParameters( const QString& fileName, const QString& title,
                                    const QString& description, const QString& category,
                                    const QString& keywords, bool isPrivate )
{
    m_fileName = fileName;
    m_uploader = new YouTubeUploader( this, fileName );
    m_uploader->setVideoParameters(title, description, category, keywords, isPrivate);
}

const QString&
YouTubeService::getAuthString()
{
    return m_auth->getAuthString();
}

const QString&
YouTubeService::getDeveloperKey()
{
    return m_devKey;
}

void
YouTubeService::authenticate()
{
    QNetworkRequest request = m_auth->getNetworkRequest();

    QNetworkReply* authReply = m_nam->post( request, m_auth->getPOSTData() );

    connect( authReply, SIGNAL(finished()),this,SLOT(authFinished()) );
    connect( authReply, SIGNAL(error(QNetworkReply::NetworkError)),
             this, SLOT(networkError(QNetworkReply::NetworkError)) );
}

void
YouTubeService::authFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply *>( sender() );
    QByteArray data = reply->readAll();

    if( m_auth->setAuthData( data ) )
        m_state = YouTubeServiceStates::Ok;

    reply->deleteLater();
}

bool
YouTubeService::upload()
{
    if( m_auth->isAuthenticated() )
    {
        /* Upload Stuff here :) */
        UploaderIODevice* data = new UploaderIODevice( static_cast<QObject*>(this), m_fileName,
                                                       m_uploader->getMimeHead(),
                                                       m_uploader->getMimeTail() );

        QNetworkRequest request = m_uploader->getNetworkRequest();
        request.setHeader( QNetworkRequest::ContentLengthHeader, data->size() );
        request.setRawHeader( "Connection", "close" );

        if( data->openFile() )
        {
            QNetworkReply* upReply = m_nam->post( request, data );

            connect( upReply, SIGNAL(finished()), this, SLOT(uploadFinished()) );
            connect( upReply, SIGNAL(uploadProgress(qint64,qint64)),
                     this, SIGNAL(uploadProgress(qint64,qint64)) );
            connect( upReply, SIGNAL(error(QNetworkReply::NetworkError)),
                     this, SLOT(networkError(QNetworkReply::NetworkError)) );

            return true;
        }
    }
    return false;
}

void
YouTubeService::uploadFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply *>( sender() );
    QByteArray data = reply->readAll();

    qDebug() << data;

    /* TODO: Handle XML response */
    emit uploadOK( QString( data ) );

    reply->deleteLater();
}

void
YouTubeService::search( QString& search )
{
}

void
YouTubeService::authError( QString e )
{
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

    emit serviceError( e );

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

    /* TODO: Make a small QDialog to take in usr:passwd */
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
YouTubeService::sslErrors( QNetworkReply* reply, const QList<QSslError> &errors )
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
        reply->ignoreSslErrors();
    }
}
#endif
