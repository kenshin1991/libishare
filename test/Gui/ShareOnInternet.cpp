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
#include "ui_ShareOnInternet.h"

#include <QMessageBox>

ShareOnInternet::ShareOnInternet()
{
    m_ui.setupUi( this );
    setWindowFlags( windowFlags() | Qt::Sheet ); // Qt::Sheet is for UI on Mac
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

    /* Error checks here */
    QDialog::accept();
}

QString
ShareOnInternet::username() const
{
    return m_ui.username->text();
}

QString
ShareOnInternet::password() const
{
    return m_ui.password->text();
}

QString
ShareOnInternet::category() const
{
    QString cat = m_ui.category->currentText();
    return cat.split(" & ").at(0);
}

QString
ShareOnInternet::title() const
{
    return m_ui.title->text();
}

QString
ShareOnInternet::description() const
{
    return m_ui.description->toPlainText();
}

QString
ShareOnInternet::keywords() const
{
    return m_ui.keywords->text();
}

quint32
ShareOnInternet::width() const
{  
    return m_width;
}

quint32
ShareOnInternet::height() const
{
    return m_height;
}

bool
ShareOnInternet::videoPrivacy() const
{
    return m_ui.videoPrivacy->isChecked();
}