/*****************************************************************************
 * YouTubeCommon.h: Generic Video Data Struct
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

#ifndef YOUTUBECOMMON_H
#define YOUTUBECOMMON_H

#include <QString>

class VideoData
{
    public:
        QString title;
        QString category;
        QString description;
        QString keywords;
        bool    isPrivate;
};

typedef VideoData YouTubeVideoData;

/*
 * Error Code References:
 *   Auth: http://code.google.com/apis/accounts/docs/AuthForInstalledApps.html#Errors
 *
 */

/* Status codes for YouTubeService. */
enum YouTubeStatus
{
    Ok = 0,

    BadAuthentication,          // Incorrect User credentials
    CaptchaRequired,            // If server is trying to challenge captcha
    ServiceUnavailable,         // YT Service Unavailable
    UnknownError,               // Unknown Error

    NetworkError,               // Some Network Error
    ConnectionError,            // Connection error
    SSLError,                   // SSL Error

    FileMissing,                // File not file
    XmlError                    // XML Parsing Error
};

#endif // YOUTUBECOMMON_H
