/*****************************************************************************
 * YouTubeService.h:
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

#ifndef YOUTUBESERVICE_H
#define YOUTUBESERVICE_H

#include <QObject>
#include <QString>

class YouTubeService : public QObject
{
    Q_OBJECT

    public:

        /**
         *  \brief Contructor
         *  \param  developerKey identifies the YouTube developer that is submitting an
         *          API request.
         */
        YouTubeService(const QString& developerKey);

        /** Destructor. */
        ~YouTubeService();

        static void initGlobal();
        static void destroyGlobal();

        void setProxy(const std::string& proxy, const std::string& user = std::string(),
        const std::string& password = std::string());

/*
        inline YouTubeServiceState authenticate(
        const YouTubeAuthenticationData& authenticationData);

        inline bool isAuthenticated() const;

        inline YouTubeServiceState search(const YouTubeSearchQuery& searchQuery,
        YouTubeSearchResult& searchResults);

        inline YouTubeServiceState upload(const YouTubeUploadData& uploadData,
        YouTubeVideoEntry& videoEntry);

        template<class RequestOperator, class ResponseOperator>
        YouTubeServiceState service(
        RequestOperator requestOperator, const typename RequestOperator::Input& input,
        ResponseOperator responseOperator, typename ResponseOperator::Output& output);
*/
        int getStatus() const;

    private:

        static size_t writeData(void* buffer, size_t elementSize, size_t numElements,
        void* userData);

        static void debugDump(const char* text, FILE* stream, unsigned char* ptr,
        size_t size, void* userData);
        //static int debugTrace(CURL* handle, curl_infotype type, char* data,
        //size_t size, void* userData);

        private:

        //CURL* m_CurlHandle;             ///< Context for sending data via curl.

        QString m_developerKey;         ///< Developer ID.
        QString m_authenticationToken;  ///< Token used to identify an authenticated session.
        QString m_writeBuffer;          ///< Contains the results received from the web server after a request.
        int     m_statusCode;           ///< Status code of last request.
};

#endif // YOUTUBESERVICE_H
