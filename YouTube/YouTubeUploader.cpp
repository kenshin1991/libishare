/*****************************************************************************
 * YouTubeUploader.cpp:
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

#include <QByteArray>
#include <QNetworkRequest>
#include <QString>
#include <QStringList>
#include <QUrl>

#include <QDebug>

YouTubeUploader::YouTubeUploader( YouTubeService* service, QString fileName )
{
    m_service = service;
}

void
YouTubeUploader::setServiceProvider(YouTubeService *service)
{
    m_service = service;

}

QByteArray
YouTubeUploader::getBA( QString token, QString value )
{
    QByteArray bA;
    bA += ( token + value );
    return bA;
}

QNetworkRequest
YouTubeUploader::getNetworkRequest()
{
    QUrl url( UPLOAD_URL );

    QNetworkRequest request;
    request.setUrl( url );

    /* Use the auth string for authentication of uploading */
    request.setRawHeader( "Authorization", getBA( "GoogleLogin auth=",
                                                  m_service->getAuthString() ) );
    /* We've implemented v2.0 protocol */
    request.setRawHeader( "GData-Version", "2");
    /* Developer is used to track API usage */
    request.setRawHeader( "X-GData-Key", getBA("key=", m_service->getDeveloperKey() ) );
    /* Name of the video, the user is uploading */
    request.setRawHeader( "Slug", getBA( m_fileName, "" ) );


    request.setHeader( QNetworkRequest::ContentTypeHeader,
                       "Content-Type: multipart/related; boundary=" );
    request.setHeader( QNetworkRequest::ContentLengthHeader, 0);

    return request;
}
