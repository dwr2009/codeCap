/*
 * wpa_gui - WpaGui class
 * Copyright (c) 2005-2006, Jouni Malinen <j@w1.fi>
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

#ifndef WPAS_CTRL_H
#define WPAS_CTRL_H

#include <QSharedPointer>
#include <QDBusConnection>
#include <BaseTypeDef.h>
#include <QObject>
#include <QDBusAbstractAdaptor>
#include <QProcess>
#include <QTimer>
#include <stdlib.h>
#include "wpa_ctrl.h"

#include <ErrPrintHelper.h>

#if 0
#define LOGE printf
#endif
#define LOGV printf
#define LOGD printf
#define DBG() printf("==> %s() %s:%d \n", __FUNCTION__, __FILE__, __LINE__)

class WpasCtrl
{
public:
	WpasCtrl();
	~WpasCtrl();

    int openCtrlConnection(const char *ifname);
	int closeCtrlConnection();
    int ctrlRequest(const char *cmd, char *reply, size_t reply_len);
    BOOL_t HasWpaCtrlEvent();
    int RecvEvent(char *buf, size_t buflen);
	int getEventNotifyFd();

private:
	struct wpa_ctrl *ctrl_conn;
	struct wpa_ctrl *monitor_conn;

	int pingsToStatusUpdate;

	QString wpaStateTranslate(char *state);

	bool wpsRunning;
	QString bssFromScan;
	bool connectedToService;
};

#endif // WPAS_CTRL_H
