/*****************************************************************************
 * YouTubeFeedParser.cpp:
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

#include "YouTubeFeedParser.h"
#include <QDebug>

YouTubeFeedParser::YouTubeFeedParser( const QString& xml )
    : QXmlStreamReader( xml )
{
}

bool
YouTubeFeedParser::read()
{
    qDebug() << "[XML] reading...";
    while( !atEnd() )
    {
        readNext();
        if( isStartElement() )
        {
            if( name() == "entry" &&
                attributes().value("encoding") == "UTF-8" &&
                attributes().value("version") == "1.0" )
                readFeed();
            else
                raiseError( QObject::tr( "The XMLStream is not a valid YouTube Feed" ) );
        }
    }

    return !error();
}

void
YouTubeFeedParser::readUnknownElement()
{
    Q_ASSERT( isStartElement() );

    while( !atEnd() )
    {
        readNext();

        if( isEndElement() )
            break;

        if( isStartElement() )
            readUnknownElement();
    }
}

void
YouTubeFeedParser::readFeed()
{
    qDebug() << "[XML] parsing feed...";

    Q_ASSERT( isStartElement() && name() == "entry" );

    while( !atEnd() )
    {
        readNext();

        if( isEndElement() )
            break;

        if( isStartElement() )
        {
            if (name() == "media:group")
                readMediaGroup();
            else
                readUnknownElement();
        }
    }
}

void
YouTubeFeedParser::readMediaGroup()
{
        qDebug() << "[XML] media group...";

    Q_ASSERT( isStartElement() && name() == "media:group" );

    while( !atEnd() )
    {
        readNext();

        if( isEndElement() )
            break;

        if( isStartElement() )
        {
            if( name() == "media:player" )
                readVideoUrl();
            else
                readUnknownElement();
        }
    }
}

void
YouTubeFeedParser::readVideoUrl()
{
    Q_ASSERT( isStartElement() && name() == "media:player" );

    if( name() == "media:player" )
    {
        videoUrl = attributes().value("url").toString();
        qDebug() << "[XML] reading url..."
                << videoUrl;
    }
}

const QString&
YouTubeFeedParser::getVideoUrl()
{
    return videoUrl;
}
