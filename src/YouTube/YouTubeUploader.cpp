/*****************************************************************************
 * YouTubeUploader.cpp: YouTube Video Uploader class
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
#include "YouTubeUploader.h"
#include "YouTubeFeedParser.h"
#include "UploaderIODevice.h"

#include <QByteArray>
#include <QNetworkRequest>
#include <QString>
#include <QStringList>
#include <QUrl>

#include <QDebug>

YouTubeUploader::YouTubeUploader( YouTubeService* service, const QString& fileName )
{
    m_service  = service;
    m_fileName = fileName;

    m_nam = new QNetworkAccessManager( this );

    /* In case the proxy asks for credentials, handle it */
    connect( m_nam, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)),
            m_service, SLOT(proxyAuthRequired(QNetworkReply*,QAuthenticator*)) );

    /* If SSL is available, handle SSL errors for better security */
    #ifndef QT_NO_OPENSSL
    connect( m_nam, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
            m_service, SLOT(sslErrors(QNetworkReply*,QList<QSslError>)) );
    #endif

    m_ioDevice = NULL;
    uploadInit();
}

YouTubeUploader::~YouTubeUploader()
{
    m_service = NULL;
    delete m_nam;
    if( m_ioDevice )
        delete m_ioDevice;
}

void
YouTubeUploader::uploadInit()
{
    m_boundary = QString( QString::number( qrand(), 10 ).toAscii() );

    QString privateToken = "";

    if( m_videoData.isPrivate )
        privateToken = "    <yt:private/>\r\n";

    API_XML_REQUEST =
        "<?xml version='1.0'?>\r\n"
        "<entry\r\n"
        "  xmlns='http://www.w3.org/2005/Atom'\r\n"
        "  xmlns:media='http://search.yahoo.com/mrss/'\r\n"
        "  xmlns:yt='http://gdata.youtube.com/schemas/2007'>\r\n"
        "  <media:group>\r\n"
        "    <media:title type='plain'>%1</media:title>\r\n"              // 1 title
        "    <media:description type='plain'>%2</media:description>\r\n"  // 2 description
        "    <media:category scheme='http://gdata.youtube.com/schemas/2007/categories.cat'>%3\r\n" // 3 category
        "    </media:category>\r\n"
        "    <media:keywords>%4</media:keywords>\r\n%5"                   // 4 key words, 5 private video                                                        // 5 isPrivate
        "  </media:group>\r\n"
        "</entry>\r\n";

    API_XML_REQUEST = API_XML_REQUEST.arg( m_videoData.title, m_videoData.description,
                                           m_videoData.category, m_videoData.keywords,
                                           privateToken );
}

bool
YouTubeUploader::upload()
{
    /* Upload Stuff here :) */
    if( !m_ioDevice )
        m_ioDevice = new UploaderIODevice( static_cast<QObject*>( this ), m_fileName,
                                           getMimeHead(), getMimeTail() );
    else
        m_ioDevice->setFile( m_fileName );

    qDebug() << "[YT UPLOADER]: In Upload";

    QNetworkRequest request = getNetworkRequest();
    request.setHeader( QNetworkRequest::ContentLengthHeader, m_ioDevice->size() );
    request.setRawHeader( "Connection", "close" );

    if( m_ioDevice->openFile() )
    {
        QNetworkReply* reply = m_nam->post( request, m_ioDevice );
        m_service->m_state = UPLOAD_START;

        qDebug() << "[YT UPLOADER]: File opened, ready to upload";

        connect( reply, SIGNAL(finished()), this, SLOT(uploadFinished()) );
        connect( reply, SIGNAL(uploadProgress(qint64,qint64)),
                 this, SIGNAL(uploadProgress(qint64,qint64)) );
        connect( reply, SIGNAL(error(QNetworkReply::NetworkError)),
                 m_service, SLOT(networkError(QNetworkReply::NetworkError)) );

        return true;
    }
    qDebug() << "[YT UPLOADER]: File opening failed.";
    return false;
}

void
YouTubeUploader::uploadFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply *>( sender() );
    const QByteArray data = reply->readAll();

    m_service->m_state = UPLOAD_FINISH;

    YouTubeFeedParser parser( data );
    parser.read();

    Q_ASSERT( parser.getVideoId() != "" );
    QString videoUrl = VIDEO_URL + parser.getVideoId();

    emit uploadOver( QString( videoUrl ) );

    disconnect( reply, SIGNAL(finished()), this, SLOT(uploadFinished()) );
    disconnect( reply, SIGNAL(uploadProgress(qint64,qint64)),
                this, SIGNAL(uploadProgress(qint64,qint64)) );
    disconnect( reply, SIGNAL(error(QNetworkReply::NetworkError)),
                m_service, SLOT(networkError(QNetworkReply::NetworkError)) );

    reply->close();
    reply->deleteLater();

    if( m_ioDevice )
        delete m_ioDevice;

    m_ioDevice = NULL;
}

QNetworkRequest
YouTubeUploader::getNetworkRequest()
{
    QUrl url( UPLOAD_URL );

    QNetworkRequest request;
    request.setUrl(url);

    /* Use the auth string for authentication of uploading */
    request.setRawHeader( "Authorization", QByteArray( "GoogleLogin auth=" )
                          .append( m_service->getAuthString() ) );

    /* We've implemented v2.0 protocol */
    request.setRawHeader( "GData-Version", "2");

    /* Developer is used to track API usage */
    request.setRawHeader( "X-GData-Key", QByteArray("key=")
                          .append( m_service->getDeveloperKey() ) );

    /* Name of the video, the user is uploading */
    request.setRawHeader( "Slug", QByteArray("").append( m_fileName ) );

    request.setRawHeader( "Content-Type",
                       QByteArray( "multipart/related; boundary=" )
                       .append( m_boundary ) );

    return request;
}

QByteArray
YouTubeUploader::getMimeHead()
{
    QByteArray data;
    data.append( "\r\n\r\n--" + m_boundary + "\r\n" );
    data.append( "Content-Type: application/atom+xml; charset=UTF-8\r\n\r\n" );
    data.append( API_XML_REQUEST );
    data.append( "--" + m_boundary + "\r\n" );
    data.append( "Content-Type: video/*\r\nContent-Transfer-Encoding: binary\r\n\r\n");

    return data;
}

QByteArray
YouTubeUploader::getMimeTail()
{
    QByteArray data;
    data.append( "\r\n--" + m_boundary + "--\r\n\r\n");

    return data;
}

const YouTubeVideoData&
YouTubeUploader::getVideoData()
{
    return m_videoData;
}

void
YouTubeUploader::setServiceProvider(YouTubeService *service)
{
    m_service = service;
}

void
YouTubeUploader::setVideoFile( const QString& fileName )
{
    m_fileName = fileName;
}

void
YouTubeUploader::setVideoData( const YouTubeVideoData& data )
{
    m_videoData = data;
    uploadInit();
}
