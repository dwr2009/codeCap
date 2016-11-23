/*
 * wpa_gui - WpaCtrl class
 * Copyright (c) 2005-2010, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 */

#include <cstdio>

#include <QProcess>
#include <QSharedPointer>
#include <QObject>
#include <QDebug>

#include "includes.h"

#include "WpasCtrl.h"
#include "dirent.h"

#include <unistd.h> // access

#include <stdio.h>
#include <string.h>
#include <BaseErrDef.h>
#include "SystemSrvApi.h"
#include <DbgLogSwitchDef.h>
#include <DateTime.h>

WpasCtrl::WpasCtrl()
{
	ctrl_conn = NULL;
	monitor_conn = NULL;
	connectedToService = false;
}

WpasCtrl::~WpasCtrl()
{
    closeCtrlConnection();
}

int WpasCtrl::openCtrlConnection(const char *iface)
{
	char ifname[256];
	unsigned long llCurTimer=0,llDiffTimer=0,llLastTimer=0;
	const unsigned long llWaitTimer=5000;

	snprintf(ifname, sizeof(ifname), "%s/%s", WpaSupplicant_INTERFACE_DIR_PATH, iface);
	ifname[sizeof(ifname)-1] = '\0';
	/*if(NULL == ctrl_conn)*/
	llLastTimer=GetSysUpTimeMs();
	
	do{
		if(NULL!=ctrl_conn)
			break;
		
		ctrl_conn = wpa_ctrl_open(ifname);
		llCurTimer=GetSysUpTimeMs();
		llDiffTimer=llCurTimer-llLastTimer;
		if(NULL==ctrl_conn){
			
			if(llDiffTimer<llWaitTimer){
				usleep(10*1000);
				continue;
			}
		}
		if (ctrl_conn == NULL) {			
			LOGE("Unable to open connection to supplicant on \"%s\": %s\n",
	             	ifname, strerror(errno));
	        	return -1;
	    	}
		break;
	}while(true);
	//event connection
	/*if(NULL == monitor_conn)*/
	
	llLastTimer=GetSysUpTimeMs();
	do{
		if(NULL!=monitor_conn)
			break;
		
		monitor_conn = wpa_ctrl_open(ifname);
		llCurTimer=GetSysUpTimeMs();
		llDiffTimer=llCurTimer-llLastTimer;
		
		if (NULL==monitor_conn){			
			if(llDiffTimer<llWaitTimer){
				usleep(10*1000);
				continue;
			}
		}
		if (monitor_conn == NULL) {
			wpa_ctrl_close(ctrl_conn);
			ctrl_conn = NULL;
			return -1;
		}
		if (wpa_ctrl_attach(monitor_conn) != 0) {
			wpa_ctrl_close(monitor_conn);
			wpa_ctrl_close(ctrl_conn);
			ctrl_conn = monitor_conn = NULL;
			return -1;
		}
		break;
	}while(true);
	
    return 0;
}

int WpasCtrl::closeCtrlConnection()
{
    if (ctrl_conn != NULL) {
        wpa_ctrl_close(ctrl_conn);
        ctrl_conn = NULL;
    }
    if (monitor_conn != NULL) {
        wpa_ctrl_close(monitor_conn);
        monitor_conn = NULL;
    }

	return ERROR_SUCCESS;
}

int WpasCtrl::ctrlRequest(const char *cmd, char *reply, size_t reply_len)
{
    int ret;

    if (ctrl_conn == NULL) {
		if(Sw_LogWifiConnection)
		{
        	LOGV("Not connected to wpa_supplicant - \"%s\" command dropped.\n", cmd);
		}
        return -1;
    }

    ret = wpa_ctrl_request(ctrl_conn, cmd, strlen(cmd), reply, &reply_len, NULL);
    if (ret == -2) {
        LOGD("'%s' command timed out.\n", cmd);
        return -2;
    } else if (ret < 0 || strncmp(reply, "FAIL", 4) == 0) {
        LOGD("wpa_ctrl_request FAIL!\n");
        return -1;
    }
	if (strncmp(cmd, "PING", 4) == 0) {
        reply[reply_len] = '\0';
    }

    // Strip off trailing newline
    if (reply_len > 0 && reply[reply_len-1] == '\n')
        reply[reply_len-1] = '\0';
    else
        reply[reply_len] = '\0';

	if(Sw_LogWifiConnection)
	{
		LOGV("WpaSent: '%s'\nWpaRecved:\n%s\n", cmd, reply);
	}
	
    return 0;
}

BOOL_t WpasCtrl::HasWpaCtrlEvent()
{
	if (NULL == monitor_conn) {
        return FALSE;
    }

	return wpa_ctrl_pending(monitor_conn);
}

int WpasCtrl::RecvEvent(char *buf, size_t buflen)
{
    size_t nread = buflen - 1;
    int result;
    
    if (monitor_conn == NULL) {
        LOGD("Connection closed\n");
        strncpy(buf, WPA_EVENT_TERMINATING " - connection closed", buflen-1);
        buf[buflen-1] = '\0';
        return strlen(buf);
    }

    result = wpa_ctrl_recv(monitor_conn, buf, &nread);
    if (result < 0) {
        LOGD("wpa_ctrl_recv failed: %s\n", strerror(errno));
        strncpy(buf, WPA_EVENT_TERMINATING " - recv error", buflen-1);
        buf[buflen-1] = '\0';
        return strlen(buf);
    }
    buf[nread] = '\0';

	// Check for EOF on the socket
    if (result == 0 && nread == 0) {
        // Fabricate an event to pass up
        LOGD("Received EOF on supplicant socket\n");
        strncpy(buf, WPA_EVENT_TERMINATING " - signal 0 received", buflen-1);
        buf[buflen-1] = '\0';
        return strlen(buf);
    }

    /*
     * Events strings are in the format
     *
     *     <N>CTRL-EVENT-XXX 
     *
     * where N is the message level in numerical form (0=VERBOSE, 1=DEBUG,
     * etc.) and XXX is the event name. The level information is not useful
     * to us, so strip it off.
     */
    if (buf[0] == '<') {
        char *match = strchr(buf, '>');
        if (match != NULL) {
            nread -= (match+1-buf);
            memmove(buf, match+1, nread+1);
        }
    }

	if(Sw_LogWifiConnection)
	{
		LOGD("WpaCtrlEvt: \"%s\"\n", buf);
	}

    return nread;
}

int WpasCtrl::getEventNotifyFd()
{
	int iOutFd = INVALID_FILE_DESCRIPTOR;

	if(monitor_conn)
	{
		iOutFd = wpa_ctrl_get_fd(monitor_conn);
	}

	return iOutFd;
}

