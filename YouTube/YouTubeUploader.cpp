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
    m_fileName = fileName;
    uploadInit();
}

void
YouTubeUploader::uploadInit()
{
    m_boundary = QString( QString::number( qrand(), 8 ).toAscii() );

    API_XML_REQUEST =
            "<?xml version='1.0'?>"
            "<entry "
              "xmlns='http://www.w3.org/2005/Atom' "
              "xmlns:media='http://search.yahoo.com/mrss/' "
              "xmlns:yt='http://gdata.youtube.com/schemas/2007'>"
              "<media:group>"
                "<media:title type='plain'>%1</media:title>"              // 1 title
                "<media:description type='plain'>"
                  "%2"
                "</media:description>"                                    // 2 description
                "<media:category scheme='http://gdata.youtube.com/schemas/2007/categories.cat'>"
                  "%3"                                                    // 3 category
                "</media:category>"
                "<media:keywords>%4</media:keywords>"                     // 4 key words
              "</media:group>"
            "</entry>";

    API_XML_REQUEST.arg("test", "testing.... man yo!", "People", "vlmc, hacking, testing");

    qDebug() << m_boundary << API_XML_REQUEST;
}

void
YouTubeUploader::setServiceProvider(YouTubeService *service)
{
    m_service = service;

}

QNetworkRequest
YouTubeUploader::getNetworkRequest()
{
    QUrl url( UPLOAD_URL );

    QNetworkRequest request;
    request.setUrl( url );

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

    request.setHeader( QNetworkRequest::ContentTypeHeader,
                       QByteArray( "Content-Type: multipart/related; boundary=" )
                       .append( m_boundary ) );

    return request;
}

QByteArray
YouTubeUploader::getMimeHead()
{
    QByteArray data;
    data.append( "--" + m_boundary + "\r\n" );
    data.append( "Content-Type: application/atom+xml; charset=UTF-8\r\n\r\n" );
    data.append( API_XML_REQUEST );
    data.append( "\r\n--" + m_boundary + "\r\n" );
    data.append( "Content-Type: video/mp4\r\nContent-Transfer-Encoding: binary\r\n\r\n");

    return data;
}

QByteArray
YouTubeUploader::getMimeTail()
{
    QByteArray data;
    data.append( "--" + m_boundary + "\r\n" );

    return data;
}
