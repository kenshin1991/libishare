/*****************************************************************************
 * AbstractSharingService.h:
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

#ifndef ABSTRACTSHARINGSERVICE_H
#define ABSTRACTSHARINGSERVICE_H

#include <QObject>

class VideoData;

class AbstractSharingService : public QObject
{
    Q_OBJECT

    public:
        AbstractSharingService( QObject* parent = 0 ) : QObject( parent ) {}

        virtual void authenticate(){}             // Authenticate the service
        virtual bool upload(){ return true; }     // Upload video
        virtual void search( QString& ){}         // Search for a video
        virtual bool abort(){ return true; }      // Abort on going service

        virtual const VideoData& getVideoData(){};

        virtual void setCredentials( const QString&, const QString& ){}
        virtual void setDeveloperKey( const QString& ){}
        virtual void setProxyCredentials( const QString&, const QString& ){}
        virtual void setVideoParameters( const QString&, const VideoData& ){}

    signals:
        void authOver();
        void uploadOver( QString );
        void uploadProgress( qint64, qint64 );
        void error( QString );
};

#endif // ABSTRACTSHARINGSERVICE_H
