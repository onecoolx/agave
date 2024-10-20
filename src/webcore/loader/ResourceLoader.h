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

#ifndef ResourceLoader_h
#define ResourceLoader_h

#include "ResourceHandleClient.h"
#include "ResourceRequest.h"
#include "ResourceResponse.h"
#include "ResourceLoader.h"
#include "Shared.h"
#include "AuthenticationChallenge.h"
#include "KURL.h"

#include <wtf/Forward.h>

namespace WebCore {

    class DocumentLoader;
    class Frame;
    class FrameLoader;
    class ResourceHandle;
    class SharedBuffer;
    
    class ResourceLoader : public Shared<ResourceLoader>, protected ResourceHandleClient {
    public:
        virtual ~ResourceLoader();

        void cancel();

        virtual bool load(const ResourceRequest&);

        FrameLoader* frameLoader() const;
        DocumentLoader* documentLoader() const { return m_documentLoader.get(); }
        
        virtual void cancel(const ResourceError&);
        ResourceError cancelledError();
        ResourceError blockedError();
        
        virtual void setDefersLoading(bool);

        void setIdentifier(unsigned long identifier) { m_identifier = identifier; }
        unsigned long identifier() const { return m_identifier; }

        virtual void releaseResources();
        const ResourceResponse& response() const;

        virtual void addData(const char*, int, bool allAtOnce);
        virtual PassRefPtr<SharedBuffer> resourceData();
        void clearResourceData();
        
        virtual void willSendRequest(ResourceRequest&, const ResourceResponse& redirectResponse);
        virtual void didReceiveResponse(const ResourceResponse&);
        virtual void didReceiveData(const char*, int, long long lengthReceived, bool allAtOnce);
        void willStopBufferingData(const char*, int);
        virtual void didFinishLoading();
        virtual void didFail(const ResourceError&);
        virtual void wasBlocked();

        void didReceiveAuthenticationChallenge(const AuthenticationChallenge&);
        void didCancelAuthenticationChallenge(const AuthenticationChallenge&);
        virtual void receivedCancellation(const AuthenticationChallenge&);

        // ResourceHandleClient
        virtual void willSendRequest(ResourceHandle*, ResourceRequest&, const ResourceResponse& redirectResponse);        
        virtual void didReceiveResponse(ResourceHandle*, const ResourceResponse&);
        virtual void didReceiveData(ResourceHandle*, const char*, int, int lengthReceived);
        virtual void didFinishLoading(ResourceHandle*);
        virtual void didFail(ResourceHandle*, const ResourceError&);
        virtual void wasBlocked(ResourceHandle*);
        virtual void willStopBufferingData(ResourceHandle*, const char* data, int length) { willStopBufferingData(data, length); } 
        virtual void didReceiveAuthenticationChallenge(ResourceHandle*, const AuthenticationChallenge& challenge) { didReceiveAuthenticationChallenge(challenge); } 
        virtual void didCancelAuthenticationChallenge(ResourceHandle*, const AuthenticationChallenge& challenge) { didCancelAuthenticationChallenge(challenge); } 
        virtual void receivedCancellation(ResourceHandle*, const AuthenticationChallenge& challenge) { receivedCancellation(challenge); }
        virtual void willCacheResponse(ResourceHandle*, CacheStoragePolicy&);
        ResourceHandle* handle() const { return m_handle.get(); }
        bool sendResourceLoadCallbacks() const { return m_sendResourceLoadCallbacks; }

        void setShouldBufferData(bool shouldBufferData) { m_shouldBufferData = shouldBufferData; }

    protected:
        ResourceLoader(Frame*, bool sendResourceLoadCallbacks, bool shouldContentSniff);

        virtual void didCancel(const ResourceError&);
        void didFinishLoadingOnePart();

        const ResourceRequest& request() const { return m_request; }
        void setRequest(const ResourceRequest& request) { m_request = request; }
        bool reachedTerminalState() const { return m_reachedTerminalState; }
        bool cancelled() const { return m_cancelled; }
        bool defersLoading() const { return m_defersLoading; }

        RefPtr<ResourceHandle> m_handle;

    private:
        ResourceRequest m_request;

        bool m_reachedTerminalState;
        bool m_cancelled;
        bool m_calledDidFinishLoad;

        bool m_sendResourceLoadCallbacks;
        bool m_shouldContentSniff;
        bool m_shouldBufferData;
protected:
        // FIXME: Once everything is made cross platform, these can be private instead of protected
        RefPtr<Frame> m_frame;
        RefPtr<DocumentLoader> m_documentLoader;
        ResourceResponse m_response;
        unsigned long m_identifier;

        KURL m_originalURL;
        RefPtr<SharedBuffer> m_resourceData;
        bool m_defersLoading;
        ResourceRequest m_deferredRequest;
    };

}

#endif
