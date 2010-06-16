/*****************************************************************************
 * YouTubeService.cpp:
 *****************************************************************************
 * Copyright (C) 2010 VideoLAN
 *
 * Authors: Rohit Yadav <rohityadav89@gmail.com>
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

YouTubeService::YouTubeService( const QString& username, const QString& password, const QString& devKey ) :
        m_developerKey( devKey )
{
    YouTubeService();
}

YouTubeService::YouTubeService()
{
    m_ytAuth = new YouTubeAuthentication();

    connect(m_ytAuth, SIGNAL(finished()), this, SIGNAL(authenticationFinished()));
    connect(m_ytAuth, SIGNAL(error(QString)), this, SIGNAL(error(QString)));    
}

YouTubeService::~YouTubeService()
{
    delete m_ytAuth;
}

void YouTubeService::setDeveloperKey(const QString& devKey)
{
    m_devKey = devKey;
}

void YouTubeService::setCredentials( const QString& username, const QString& password )
{
    m_ytAuth->setCredentials(username, password);
}


/* Different services */
void YouTubeService::authenticate()
{
    m_ytAuth->authenticate();
}

bool YouTubeService::upload(const QString& file)
{
    if( isAuthenticated() )
    {
        /* Upload Stuff here :) */


        return true;
    }
    return false;
}

void YouTubeService::search(const QString& search)
{
}

/* Check service states */
bool YouTubeService::isAuthenticated()
{
    return m_ytAuth->isAuthenticated();
}

bool YouTubeService::isUploaded()
{
}

