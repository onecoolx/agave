/* network_win32.cpp - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */
#define INITGUID 1  //for guid defined
#include <windows.h>
#include "config.h"
#include "unicode.h"
#include "network.h"
#include "network_win32.h"

#if defined(WINCE)
    #include <connmgr.h>
    #include <connmgr_status.h>
    #include <connmgr_proxy.h>
#endif

NetServiceImpl::NetServiceImpl(NetService* service)
    : m_service(service)
#if defined(WINCE)
    , m_hConn(0)
    , m_index(0)
#endif
{
#if defined(WINCE)
    memset(&m_guid, 0, sizeof(GUID));
#endif
}

NetServiceImpl::~NetServiceImpl()
{
    releaseConnection();
}

bool NetServiceImpl::isAvaliable(void)
{
#if defined(WINCE)
    bool ret = false;
    HANDLE h = ConnMgrApiReadyEvent();
    if (WaitForSingleObject(h, 2000) == WAIT_OBJECT_0) {
        ret = true;
    }

    if (h) { CloseHandle(h); }

    return ret;
#else
    return true;
#endif
}

bool NetServiceImpl::establishConnection(ConnType type)
{
#if defined(WINCE)
    if (m_hConn) {
        releaseConnection();
    }

    if (!getConnectedGUID(&m_guid)) {
        if (FAILED(ConnMgrMapURL(L"http://agave/", &m_guid, &m_index))) {
            return false;
        }
    }

    CONNMGR_CONNECTIONINFO info;
    memset(&info, 0, sizeof(CONNMGR_CONNECTIONINFO));
    info.cbSize = sizeof(info);
    info.dwParams = CONNMGR_PARAM_GUIDDESTNET;
    info.dwFlags = CONNMGR_FLAG_PROXY_HTTP | CONNMGR_FLAG_PROXY_WAP | CONNMGR_FLAG_PROXY_SOCKS4 | CONNMGR_FLAG_PROXY_SOCKS5;
    info.dwPriority = CONNMGR_PRIORITY_USERBACKGROUND;
    info.guidDestNet = m_guid;
    info.bExclusive = FALSE;
    info.bDisabled = FALSE;

    DWORD dwStatus = 0;

    if (FAILED(ConnMgrEstablishConnection(&info, &m_hConn))) {
        m_hConn = 0;
        return false;
    } else {
        return true;
    }
#else
    return true;
#endif
}

bool NetServiceImpl::establishConnectionSync(ConnType type)
{
#if defined(WINCE)
    if (!m_hConn) {
        releaseConnection();
    }

    if (!getConnectedGUID(&m_guid)) {
        if (FAILED(ConnMgrMapURL(L"http://agave/", &m_guid, &m_index))) {
            return false;
        }
    }

    CONNMGR_CONNECTIONINFO info;
    memset(&info, 0, sizeof(CONNMGR_CONNECTIONINFO));
    info.cbSize = sizeof(info);
    info.dwParams = CONNMGR_PARAM_GUIDDESTNET;
    info.dwFlags = CONNMGR_FLAG_PROXY_HTTP | CONNMGR_FLAG_PROXY_WAP;
    info.dwPriority = CONNMGR_PRIORITY_USERINTERACTIVE;
    info.guidDestNet = m_guid;
    info.bExclusive = FALSE;
    info.bDisabled = FALSE;

    DWORD dwStatus = 0;

    if (FAILED(ConnMgrEstablishConnectionSync(&info, &m_hConn, 30 * 1000, &dwStatus))) {
        m_hConn = 0;
        return false;
    }

    if (dwStatus == CONNMGR_STATUS_CONNECTED) {
        checkConnectType();
        return true;
    } else {
        return false;
    }
    return true;
#else
    return false;
#endif
}

void NetServiceImpl::releaseConnection(void)
{
#if defined(WINCE)
    if (m_hConn) {
        ConnMgrReleaseConnection(m_hConn, FALSE);
        m_hConn = 0;
    }
#endif
}

#if defined(WINCE)
bool NetServiceImpl::getConnectedGUID(GUID* guid)
{
    CONNMGR_CONNECTION_DETAILED_STATUS* pInfo = 0;
    DWORD size = 0;
    bool ret = false;

    ConnMgrQueryDetailedStatus(0, &size); //get item size
    if (size) {

        pInfo = (CONNMGR_CONNECTION_DETAILED_STATUS*)new BYTE[size];

        if (S_OK == ConnMgrQueryDetailedStatus(pInfo, &size)) {

            CONNMGR_CONNECTION_DETAILED_STATUS* p = pInfo;

            while (NULL != p) {
                if (p->dwConnectionStatus == CONNMGR_STATUS_CONNECTED) {
                    *guid = p->guidDestNet;
                    ret = true;
                    break;
                }
                p = p->pNext;
            }
        }
        delete [] pInfo;
    }
    return ret;
}
#endif

void NetServiceImpl::checkConnectType(void)
{
#if defined(WINCE)
    CONNMGR_CONNECTION_DETAILED_STATUS* pInfo = 0;
    DWORD size = 0;

    ConnMgrQueryDetailedStatus(0, &size); //get item size
    if (size) {

        pInfo = (CONNMGR_CONNECTION_DETAILED_STATUS*)new BYTE[size];

        if (S_OK == ConnMgrQueryDetailedStatus(pInfo, &size)) {

            CONNMGR_CONNECTION_DETAILED_STATUS* p = pInfo;

            while (NULL != p) {
                if ((p->dwConnectionStatus == CONNMGR_STATUS_CONNECTED)
                    && ((p->dwType == CM_CONNTYPE_CELLULAR) || (p->dwType == CM_CONNTYPE_PROXY))) {

                    std::vector<NetConfItem>& confs = m_service->getConfigs();

                    for (size_t i = 0; i < confs.size(); i++) {
                        if (p->guidDestNet == confs[i].impl->guid) {
                            if (confs[i].isProxy) {
                                m_service->setProxy(confs[i]);
                            } else {
                                m_service->clearProxy();
                            }
                            break;
                        }
                    }
                    break;
                }
                p = p->pNext;
            }
        }
        delete [] pInfo;
    }
#endif
}

bool NetServiceImpl::isConnected(void)
{
#if defined(WINCE)
    DWORD dwStatus = 0;
    if (SUCCEEDED(ConnMgrConnectionStatus(m_hConn, &dwStatus))) {
        if (dwStatus == CONNMGR_STATUS_CONNECTED) {
            checkConnectType();
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
#else
    return true;
#endif
}

#define KEY_PATH L"\\Comm\\ConnMgr\\Providers\\{EF097F4C-DC4B-4c98-8FF6-AEF805DC0E8E}\\"

void NetServiceImpl::readNetConfigs()
{
#if defined(WINCE)
    int index = 0;
    HRESULT hr = 0;
    CONNMGR_DESTINATION_INFO info = {0};

    while ((hr = ConnMgrEnumDestinations(index, &info)) == S_OK) {
        NetConfItem item;
        item.name = info.szDescription;
        // get proxy info

        // platform info
        item.impl = new NetConfItemImpl;
        item.impl->index = index;
        item.impl->guid = info.guid;

        //read all connects, proxy.
        TCHAR guidstr[64] = {'H', 'T', 'T', 'P', '-'};
        StringFromGUID2(info.guid, guidstr + 5, 58);

        HKEY rKey;
        TCHAR regpath[512] = {0};
        lstrcpy(regpath, KEY_PATH);
        lstrcat(regpath, guidstr);
        if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, regpath, 0, KEY_READ, &rKey) == ERROR_SUCCESS) {
            DWORD type = 0, enable = 0, size = sizeof(DWORD);
            if (RegQueryValueEx(rKey, L"Enable", 0, &type, (LPBYTE)&enable, &size) == ERROR_SUCCESS) {
                item.isProxy = enable ? true : false;
                if (enable) { // read proxy setings
                    TCHAR value[64] = {0};
                    size = sizeof(value);
                    if (RegQueryValueEx(rKey, L"Proxy", 0, &type, (LPBYTE)value, &size) == ERROR_SUCCESS) {
                        size_t len = lstrlen(value);

                        std::string host = Unicode::ConvertUTF16ToUTF8(ustring(value));

                        size_t pos = host.find_first_of(':');

                        item.hostip = host.substr(0, pos);
                        item.port = host.substr(pos + 1, len - pos - 1);
                    }

                    memset(value, 0, sizeof(value));

                    if (RegQueryValueEx(rKey, L"Username", 0, &type, (LPBYTE)value, &size) == ERROR_SUCCESS) {

                        std::string name = Unicode::ConvertUTF16ToUTF8(ustring(value));
                        item.user = name;
                    }

                    memset(value, 0, sizeof(value));

                    if (RegQueryValueEx(rKey, L"Password", 0, &type, (LPBYTE)value, &size) == ERROR_SUCCESS) {

                        std::string pass = Unicode::ConvertUTF16ToUTF8(ustring(value));
                        item.pass = pass;
                    }

                }
            }
        }
        RegCloseKey(rKey);

        std::vector<NetConfItem>& confs = m_service->getConfigs();
        confs.push_back(item);
        index++;
    }
#endif
}
