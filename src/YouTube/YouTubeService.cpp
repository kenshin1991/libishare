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

#include "AbstractSharingService.h"
#include "YouTubeAuthenticator.h"
#include "YouTubeService.h"
#include "YouTubeUploader.h"

#include <QAuthenticator>
#include <QByteArray>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QSslError>

#include <QDebug>

YouTubeService::YouTubeService( const QString& devKey,
                                const QString& username, const QString& password )
{
    m_devKey   = devKey;
    m_username = username;
    m_password = password;

    m_auth     = NULL;
    m_uploader = NULL;
}

YouTubeService::~YouTubeService()
{
    if( m_auth )
        delete m_auth;

    if( m_uploader )
        delete m_uploader;
}

void
YouTubeService::authenticate()
{
    if( !m_auth )
        m_auth = new YouTubeAuthenticator( this, m_username, m_password );
    else
        m_auth->setCredentials( m_username, m_password );

    /* Tell world on successful authentication */
    connect( m_auth, SIGNAL(authOver()), this, SIGNAL(authOver()) );

    /* On authentication error, m_auth will send the error token */
    connect( m_auth, SIGNAL(authError(QString)), this, SLOT(authError(QString)) );

    m_auth->authenticate();
}

bool
YouTubeService::upload()
{
    if( m_auth->isAuthenticated() )
    {
        if( !m_uploader )
            m_uploader = new YouTubeUploader( this, m_fileName );
        else
            m_uploader->setVideoFile( m_fileName );

        m_uploader->setVideoData( m_videoData );

        /* Tell world on successful uploading */
        connect( m_uploader, SIGNAL(uploadOver(QString)),
                 this, SIGNAL(uploadOver(QString)) );

        /* Connect upload progress */
        connect( m_uploader, SIGNAL(uploadProgress(qint64,qint64)),
                 this, SIGNAL(uploadProgress(qint64,qint64)) );

        return m_uploader->upload();
    }
    return false;
}

void
YouTubeService::search( QString& search )
{
}

bool
YouTubeService::abort()
{
    /* Check States and abort corresponding service */
    return false;
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
YouTubeService::setCredentials( const QString& username, const QString& password )
{
    m_username = username;
    m_password = password;
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
YouTubeService::setVideoParameters( const QString& fileName, const VideoData& data )
{
    m_fileName  = fileName;
    m_videoData = data;
}

void
YouTubeService::authError( QString e )
{
    qDebug() << "[YT SERVICE]: [AUTH ERROR]: " << e;

    if( e == "BadAuthentication" )
        m_error = BadAuthentication;
    else
    if( e == "CaptchaRequired")
        m_error = CaptchaRequired;
    else
    if( e == "ServiceUnavailable")
        m_error = ServiceUnavailable;
    else
        m_error = UnknownError;

    emit error( e );

}

void
YouTubeService::networkError( QNetworkReply::NetworkError e )
{
    // Temporary error
    qDebug() << "[NETWORK ERROR]: " << e;

    switch( e )
    {
        case QNetworkReply::ContentOperationNotPermittedError:
        default: return;
    }

    qDebug() << "[YT SERVICE]: DANGER!!!";

    QNetworkReply *reply = static_cast<QNetworkReply *>( sender() );

    m_error = NetworkError;
    emit error( QString().setNum( e ) );

    disconnect( reply, SIGNAL(error(QNetworkReply::NetworkError)),
             this, SLOT(networkError(QNetworkReply::NetworkError)) );

    //if( m_state == AUTH_START );
        //disconnect( reply, SIGNAL(finished()),this,SLOT(authFinished()) );

    if( m_state == UploadStart )
    {
        disconnect( reply, SIGNAL(finished()), this, SLOT(uploadFinished()) );
        disconnect( reply, SIGNAL(uploadProgress(qint64,qint64)),
                 this, SIGNAL(uploadProgress(qint64,qint64)) );
    }

    reply->close();
    reply->deleteLater();
}

void
YouTubeService::proxyAuthRequired( QNetworkReply*, QAuthenticator *authenticator )
{
    m_error = ConnectionError;

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
    m_error = SSLError;

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
