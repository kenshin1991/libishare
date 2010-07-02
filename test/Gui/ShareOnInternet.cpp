/*****************************************************************************
 * ShareOnInternet.cpp: Configure Video Export to Internet
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

#include "ShareOnInternet.h"
#include "AbstractSharingService.h"
#include "Vimeo/VimeoService.h"
#include "YouTube/YouTubeService.h"

#include <QMessageBox>
#include <QDebug>

ShareOnInternet::ShareOnInternet( QWidget* parent )
    : QDialog( parent, Qt::Sheet | Qt::Dialog )
{
    m_service = NULL;
    m_ui.setupUi( this );
    m_ui.progressBar->setVisible( false );
    devKey = "AI39si7FOtp165Vq644xVkuka84TVQNbztQmQ1dC9stheBfh3-33RZaTu7eJkYJzvxp6XNbvlr4M6-ULjXDERFl62WIo6AQIEQ";
}

void
ShareOnInternet::accept()
{
    if( m_ui.title->text().isEmpty() |
        m_ui.username->text().isEmpty() |
        m_ui.password->text().isEmpty() )
    {
        QMessageBox::critical( this, tr("Error"),
                               tr("'Username' or 'Password' or 'Title' cannot be empty."
                                  "Please check these fields.") );
        return;
    }

    switch( m_ui.videoSize->currentIndex() )
    {
        case 0:  m_width = 480;  m_height = 272; break;
        case 1:  m_width = 640;  m_height = 360; break;
        case 2:  m_width = 960;  m_height = 540; break;
        case 3:  m_width = 1280; m_height = 720; break;
        default: m_width = 640;  m_height = 360;
    }

    /* Add here code to update service provider, default: YouTube */
    QString temp = "/home/rohit/GSoC/VideoLan/libishare/videos/mp4.mp4";
    publish( temp );

    /* Error checks here */
    //QDialog::accept();
}

void
ShareOnInternet::publish( QString& fileName )
{
    QString username     = getUsername();
    QString password     = getPassword();
    quint32 width        = getWidth();
    quint32 height       = getHeight();
    VideoData videoData  = getVideoData();

    qDebug() << "[SHARE ON INTERNET]: Going to publish video!"
            << username << password << videoData.title;

    if( !m_service )
    {
        qDebug() << "[SHARE ON INTERNET]: Created Service object";
        m_service = new YouTubeService( devKey, username, password );
    }
    else
        m_service->setCredentials( username, password );

    m_service->authenticate();
    m_service->setVideoParameters( fileName, videoData );


    connect( m_service, SIGNAL(authOver()), this, SLOT(authFinished()) );
    connect( m_service, SIGNAL(error(QString)), this, SLOT(serviceError(QString)) );
}

void
ShareOnInternet::authFinished()
{
    qDebug() << "[AUTH FINISHED]";
    m_ui.progressBar->setEnabled( true );
    m_ui.progressBar->setVisible( true );

    /*On Finish, extract out the auth token and upload a test video */
    disconnect( m_service, SIGNAL(authOver()), this, SLOT(authFinished()) );

    connect( m_service, SIGNAL(uploadOver(QString)), this, SLOT(uploadFinished(QString)));
    connect( m_service, SIGNAL(uploadProgress(qint64,qint64)),
             this, SLOT(uploadProgress(qint64,qint64)) );

    if( !m_service->upload() )
    {
        qDebug() << "[AUTH FAILED]";
        /* Add code here to work on fallback... */
    }

    qDebug() << "[UPLOAD STARTED]";

    /* TODO: Add code to activate Abort button etc. */
    /* YouTubeService will check if the auth token is expired
       or it's not authenticated yet... Then if it's true, it will upload*/
}

void
ShareOnInternet::uploadFinished( QString result )
{
    /* Add code here to abort stuff */
    qDebug() << "[Upload Finished]: " << result;
    m_ui.progressBar->setEnabled( false );
    m_ui.progressBar->setVisible( false );
}

void
ShareOnInternet::uploadProgress(qint64 received, qint64 total)
{
    if( total != 0 )
    {
        qint64 progress = received * 100 / total;
        m_ui.progressBar->setValue( progress );
        m_ui.statusLabel->setText( QString("Uploaded: %1").arg( received ) );
    }
}

void
ShareOnInternet::serviceError(QString e)
{
    qDebug() << "[SERVICE ERROR]: " << e;
    emit error( e );
}

QString
ShareOnInternet::getUsername() const
{
    return m_ui.username->text();
}

QString
ShareOnInternet::getPassword() const
{
    return m_ui.password->text();
}

quint32
ShareOnInternet::getWidth() const
{
    return m_width;
}

quint32
ShareOnInternet::getHeight() const
{
    return m_height;
}

VideoData
ShareOnInternet::getVideoData() const
{
    VideoData data;

    data.title       = m_ui.title->text();
    data.category    = m_ui.category->currentText().split(" & ").at( 0 );
    data.description = m_ui.description->toPlainText();
    data.keywords    = m_ui.keywords->text();
    data.isPrivate   = m_ui.videoPrivacy->isChecked();

    return data;
}

void
ShareOnInternet::setData( const VideoData &data )
{
    qDebug() << "Setting up backed up video data";
    m_ui.title->setText( data.title );
    m_ui.description->setPlainText( data.description );
    m_ui.keywords->setText( data.keywords );
    if( data.isPrivate )
        m_ui.videoPrivacy->setChecked( true );
}
