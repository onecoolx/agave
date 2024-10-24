/*
 * Copyright (C) 2005, 2006 Apple Computer, Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer. 
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution. 
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SubresourceLoader_h
#define SubresourceLoader_h
 
#include "ResourceHandleClient.h"
#include "ResourceLoader.h"
#include <wtf/PassRefPtr.h>
 
 
namespace WebCore {

    class FormData;
    class String;
    class ResourceHandle;
    struct ResourceRequest;
    class SubresourceLoaderClient;
    
    class SubresourceLoader : public ResourceLoader {
    public:
        static PassRefPtr<SubresourceLoader> create(Frame*, SubresourceLoaderClient*, const ResourceRequest&, bool skipCanLoadCheck = false, bool sendResourceLoadCallbacks = true, bool shouldContentSniff = true);
        
        virtual ~SubresourceLoader();

        virtual bool load(const ResourceRequest&);
        
        virtual void willSendRequest(ResourceRequest&, const ResourceResponse& redirectResponse);
        virtual void didReceiveResponse(const ResourceResponse&);
        virtual void didReceiveData(const char*, int, long long lengthReceived, bool allAtOnce);
        virtual void didFinishLoading();
        virtual void didFail(const ResourceError&);
        virtual void receivedCancellation(const AuthenticationChallenge&);

    private:
        SubresourceLoader(Frame*, SubresourceLoaderClient*, bool sendResourceLoadCallbacks, bool shouldContentSniff);

        virtual void didCancel(const ResourceError&);
        SubresourceLoaderClient* m_client;
        bool m_loadingMultipartContent;
    };

}

#endif // SubresourceLoader_h
