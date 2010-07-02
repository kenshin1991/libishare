/*****************************************************************************
 * ShareOnInternet.h: Configure Video Export to Internet
 *****************************************************************************
 * Copyright (C) 2008-2010 VideoLAN
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

#ifndef SHAREONINTERNET_H
#define SHAREONINTERNET_H

#include <QDialog>
#include "ui_ShareOnInternet.h"

class AbstractSharingService;
class VideoData;

class ShareOnInternet : public QDialog
{
    Q_OBJECT

    enum ServiceProviders
    {
        YOUTUBE = 0,
        VIMEO,
        JUSTIN,
    };

    public:
        ShareOnInternet( QWidget* parent = 0 );
        ~ShareOnInternet();

        QString          getUsername() const;
        QString          getPassword() const;
        quint32          getWidth() const;
        quint32          getHeight() const;
        VideoData        getVideoData() const;

        void             setVideoFile( QString& fileName );

    private:
        void                     publish();

        Ui::ShareOnInternet      m_ui;
        AbstractSharingService*  m_service;
        int                      m_serviceProvider;
        QString                  m_devKey;
        QString                  m_fileName;

        quint32                  m_width;
        quint32                  m_height;

    private slots:
        void    accept();
        void    authFinished();
        void    uploadFinished( QString );
        void    uploadProgress( qint64, qint64 );
        void    serviceError( QString );

    signals:
        void error( QString );
        void finished();
};

#endif // SHAREONINTERNET_H
