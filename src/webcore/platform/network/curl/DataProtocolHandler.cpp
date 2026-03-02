
#include "config.h"
#include "ResourceHandle.h"
#include "ResourceHandleClient.h"
#include "ResourceHandleInternalCurl.h"
#include "MIMETypeRegistry.h"
#include "DataProtocolHandler.h"
#include "HTTPParsers.h"
#include "Base64.h"

namespace WebCore {

void parseDataUrl(ResourceHandle* handle)
{
    String data = handle->request().url().url();

    ASSERT(data.startsWith("data:", false));

    String header;
    bool base64 = false;

    int index = data.find(',');
    if (index != -1) {
        header = data.substring(5, index - 5).lower();
        data = data.substring(index + 1);

        if (header.endsWith(";base64")) {
            base64 = true;
            header = header.left(header.length() - 7);
        }
    } else
        data = String();

    data = KURL::decode_string(data);

    if (base64) {
        CString latin1Data = data.latin1();
        Vector<char> out;
        if (base64Decode(latin1Data.data(), latin1Data.length(), out))
            data = String(out.data(), out.size());
        else
            data = String();
    }

    if (header.isEmpty())
        header = "text/plain;charset=US-ASCII";

    ResourceHandleClient* client = handle->getInternal()->client();

    ResourceResponse response;

    response.setMimeType(extractMIMETypeFromMediaType(header));
    response.setTextEncodingName(extractCharsetFromMediaType(header));
    response.setExpectedContentLength(data.length());
    response.setHTTPStatusCode(200);

    client->didReceiveResponse(handle, response);

    if (!data.isEmpty()) {
        CString latin1Data = data.latin1();
        client->didReceiveData(handle, latin1Data.data(), latin1Data.length(), 0);
    }

    client->didFinishLoading(handle);
}

} /* namespace WebCore */
