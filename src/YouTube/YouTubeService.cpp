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

#include <QAuthenticator>
#include <QByteArray>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QSslError>

#include <QDebug>

YouTubeService::YouTubeService( const QString& devKey, const QString& username, const QString& password )
{
    m_devKey = devKey;
    m_auth = new YouTubeAuthenticator( username, password );

    /* Tell world on successful authentication */
    connect( m_auth, SIGNAL(authOK()), this, SIGNAL(authOK()) );

    /* On authentication error, m_auth will send the error token */
    connect( m_auth, SIGNAL(authError(QString)), this, SLOT(authError(QString)) );

    /* This is a temporary pointer to track current QNetworkReply in progress */
    m_currentReply = NULL;

    m_nam = new QNetworkAccessManager();

    /* In case the proxy asks for credentials, handle it */
    connect( m_nam, SIGNAL((authenticationRequired(QNetworkReply*,QAuthenticator*)),
            this, SLOT(proxyAuthRequired(QNetworkReply*,QAuthenticator*)) );

    /* If SSL is available, handle SSL errors for better security */
    #ifndef QT_NO_OPENSSL
    connect( m_nam, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
            this, SLOT(sslErrors(QNetworkReply*,QList<QSslError>)) );
    #endif

    m_ioDevice     = NULL;
    m_currentReply = NULL;
}

YouTubeService::~YouTubeService()
{
    if( m_currentReply )
    {
        m_currentReply->abort();
        m_currentReply->deleteLater();
    }

    delete m_nam;
    delete m_auth;
    delete m_uploader;
}

void
YouTubeService::setCredentials( const QString& username, const QString& password )
{
    m_auth->setCredentials( username, password );
}

void
YouTubeService::setDeveloperKey( const QString& devKey )
{
    m_devKey = devKey;
}

void
YouTubeService::setProxyCredentials(const QString &username, const QString &password)
{
    m_proxyUsername = username;
    m_proxyPassword = password;
}

void
YouTubeService::setVideoParameters( const QString& fileName, const YouTubeVideoData& data )
{
    m_fileName = fileName;
    m_uploader = new YouTubeUploader( this, fileName );
    m_uploader->setVideoData( data );
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

const VideoData&
YouTubeService::getVideoData()
{
    return m_uploader->getVideoData();
}

void
YouTubeService::authenticate()
{
    QNetworkRequest request = m_auth->getNetworkRequest();

    m_currentReply = m_nam->post( request, m_auth->getPOSTData() );
    qDebug() << "Auth posted!";

    connect( m_currentReply, SIGNAL(finished()),this,SLOT(authFinished()) );
    connect( m_currentReply, SIGNAL(error(QNetworkReply::NetworkError)),
             this, SLOT(networkError(QNetworkReply::NetworkError)) );
}

void
YouTubeService::authFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply *>( sender() );
    QByteArray data = reply->readAll();

    qDebug() << "Auth data: " << data;

    if( m_auth->setAuthData( data ) )
        m_status = Ok;

    disconnect( reply, SIGNAL(finished()),this,SLOT(authFinished()) );
    disconnect( reply, SIGNAL(error(QNetworkReply::NetworkError)),
             this, SLOT(networkError(QNetworkReply::NetworkError)) );

    reply->close();
    reply->deleteLater();
    m_currentReply = NULL;
}

bool
YouTubeService::upload()
{
    if( m_auth->isAuthenticated() )
    {
        /* Upload Stuff here :) */
        m_ioDevice = new UploaderIODevice( static_cast<QObject*>(this), m_fileName,
                                                       m_uploader->getMimeHead(),
                                                       m_uploader->getMimeTail() );

        QNetworkRequest request = m_uploader->getNetworkRequest();
        request.setHeader( QNetworkRequest::ContentLengthHeader, m_ioDevice->size() );
        request.setRawHeader( "Connection", "close" );

        if( m_ioDevice->openFile() )
        {
            m_currentReply = m_nam->post( request, m_ioDevice );

            connect( m_currentReply, SIGNAL(finished()), this, SLOT(uploadFinished()) );
            connect( m_currentReply, SIGNAL(uploadProgress(qint64,qint64)),
                     this, SIGNAL(uploadProgress(qint64,qint64)) );
            connect( m_currentReply, SIGNAL(error(QNetworkReply::NetworkError)),
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

    /* TODO: Handle XML response */
    /* FIXME: check upload status of video, fix it as in AuthOK */
    emit uploadOK( QString( data ) );

    disconnect( reply, SIGNAL(finished()), this, SLOT(uploadFinished()) );
    disconnect( reply, SIGNAL(uploadProgress(qint64,qint64)),
             this, SIGNAL(uploadProgress(qint64,qint64)) );
    disconnect( reply, SIGNAL(error(QNetworkReply::NetworkError)),
             this, SLOT(networkError(QNetworkReply::NetworkError)) );

    reply->close();
    reply->deleteLater();

    if( m_ioDevice )
        delete m_ioDevice;

    m_ioDevice     = NULL;
    m_currentReply = NULL;

}

void
YouTubeService::search( QString& search )
{
}

bool
YouTubeService::abort()
{
    if( m_currentReply )
    {
        qDebug() << "in abort()";
        m_currentReply->abort();
        m_currentReply->deleteLater();
        m_currentReply = NULL;
        return true;
    }
    return false;
}

void
YouTubeService::authError( QString e )
{
    qDebug() << "[AUTH ERROR]: " << e;

    if( e == "BadAuthentication" )
        m_status = BadAuthentication;
    else
    if( e == "CaptchaRequired")
        m_status = CaptchaRequired;
    else
    if( e == "ServiceUnavailable")
        m_status = ServiceUnavailable;
    else
        m_status = UnknownError;

    emit error( e );

}

void
YouTubeService::networkError( QNetworkReply::NetworkError e )
{
    QNetworkReply *reply = static_cast<QNetworkReply *>( sender() );

    qDebug() << "[NETWORK ERROR]: " << e;
    m_status = NetworkError;
    emit error( QString().setNum( e ) );

    //reply->abort();
    //reply->deleteLater();

    //if( m_ioDevice )
     //   delete m_ioDevice;

    //m_ioDevice = NULL;
    //m_currentReply = NULL;
}

void
YouTubeService::proxyAuthRequired( QNetworkReply*, QAuthenticator *authenticator )
{
    m_status = ConnectionError;

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
    m_status = SSLError;

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
