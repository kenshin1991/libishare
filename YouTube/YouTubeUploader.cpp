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
    qDebug() << fileName;
    fileName = "test.mp4";
    uploadInit();
}

void
YouTubeUploader::uploadInit()
{
    m_boundary = QString( QString::number( qrand(), 10 ).toAscii() );

    API_XML_REQUEST =
            "<?xml version='1.0'?>\r\n"
            "<entry \r\n"
            "  xmlns='http://www.w3.org/2005/Atom'\r\n"
            "  xmlns:media='http://search.yahoo.com/mrss/' \r\n"
            "  xmlns:yt='http://gdata.youtube.com/schemas/2007'>\r\n"
            "  <media:group>\r\n"
            "    <media:title type='plain'>%1</media:title>\r\n"              // 1 title
            "    <media:description type='plain'>%2</media:description>\r\n"  // 2 description
            "    <media:category scheme='http://gdata.youtube.com/schemas/2007/categories.cat'>"
            "      %3"                                                        // 3 category
            "    </media:category>\r\n"
            "    <media:keywords>%4</media:keywords>\r\n"                     // 4 key words
            "  </media:group>\r\n"
            "</entry>\r\n";

    API_XML_REQUEST = API_XML_REQUEST.arg("test", "testing.... man yo!", "People", "vlmc, hacking, testing");

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
    qDebug() << "URL" << url.toString();

    QNetworkRequest request(url);

    /* Use the auth string for authentication of uploading */
    request.setRawHeader( "Authorization", QByteArray( "GoogleLogin auth=" )
                          .append( m_service->getAuthString() ) );
    /* We've implemented v2.0 protocol */
    request.setRawHeader( "GData-Version", "2");
    /* Developer is used to track API usage */
    request.setRawHeader( "X-GData-Key", QByteArray("key=")
                          .append( m_service->getDeveloperKey() ) );
    /* Name of the video, the user is uploading */
    request.setRawHeader( "Slug", QByteArray("").append( "test.mp4" ) );

    request.setRawHeader( "Content-Type",
                       QByteArray( "multipart/related; boundary=" )
                       .append( m_boundary ) );

    qDebug() << "[UPLOADER, AUTH STR]"
            << m_service->getAuthString();

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

    qDebug() << "HEAD" << data;
    return data;
}

QByteArray
YouTubeUploader::getMimeTail()
{
    QByteArray data;
    data.append( "--" + m_boundary + "--");

    qDebug() << "TAIL" << data;

    return data;
}
