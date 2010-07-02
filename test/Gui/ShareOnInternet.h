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

#include <QWidget>
#include "ui_ShareOnInternet.h"

class AbstractSharingService;
class VideoData;

class ShareOnInternet : public QWidget
{
    Q_OBJECT

    public:
        ShareOnInternet( QWidget* parent = 0 );
        void setData( const VideoData& data );

        QString          username() const;
        QString          password() const;
        quint32          width() const;
        quint32          height() const;
        VideoData        videoData() const;

    private slots:
        virtual void    accept();

    private:
        Ui::ShareOnInternet      m_ui;
        AbstractSharingService*  m_service;
        QString                  devKey;
        quint32                  m_width;
        quint32                  m_height;
        quint32                  m_serviceProvider;
};

#endif // SHAREONINTERNET_H
