#ifndef	_BASEERRDEF_H_
#define	_BASEERRDEF_H_

#define 	SOCKET_ERROR            				(-1)

#define	ERR_BASE							0
#define	ERROR_SUCCESS						0
#define	ERR_SUCCESS							0
#define	MSG_IS_PROCESSED					0
#define	ERROR_INVALID_PARAMETER			1
#define	ERR_INV_PARAM					1
#define	ERROR_NOT_ENOUGH_BUFFER			2
#define	ERROR_OUT_OF_MEMORY				3
#define	ERR_OUT_OF_MEMORY				3
#define	ERROR_FILE_OPEN_FAIL				4
#define	ERR_FILE_OPEN_FAIL					4
#define	ERROR_FILE_READ_FAIL				5
#define	ERR_FILE_READ_FAIL					5
#define	ERROR_FILE_WRITE_FAIL				6
#define	ERR_FILE_WRITE_FAIL					6
#define	ERROR_FILE_SEEK_FAIL				7
#define	ERROR_FILE_STAT_FAIL				8
#define	ERROR_FILE_CHMOD_FAIL				9
#define	ERROR_CALL_SYSTEM_FAILED			10
#define	ERROR_CRT_SELECT_CALL_FAILED		11
#define	ERROR_NOT_FOUND					12
#define	ERR_NOT_FOUND					12
#define	ERROR_ALREADY_EXIST				13
#define	ERROR_INDEX_IS_OUT_OF_RANGE		14
#define	ERROR_DEST_HAS_DATA				15
#define	ERROR_OUT_OF_RANGE				16
#define	ERR_OUT_OF_RANGE				16
#define	ERROR_RECV_TIMEOUT				17
#define	ERROR_EXECL_CALL_FAIL				18
#define	ERROR_VFORK_CALL_FAIL				19
#define	ERROR_CREATE_PROCESS_FAILED		20
#define	ERROR_CREATE_SOCKET_FAIL			21
#define	ERROR_SOCKET_SEND_FAIL			22
#define	ERR_SOCKET_SEND_FAIL			22
#define	ERR_SCK_CONN_CLOSED_BY_PEER		23
#define	ERROR_SOCKET_RECV_FAIL			24
#define	ERR_SOCKET_RECV_FAIL			24
#define	ERROR_SOCKET_IOCTL_FAIL			25
#define	ERR_SCK_REQUEST_STOP_RECV		26
#define	ERROR_IOCTL_FAILED					27
#define	ERR_IOCTL_FAILED					27
#define	ERROR_NO_CHANGE					28
#define	ERROR_INVALID_STATE				29
#define	ERR_INVALID_STATE				ERROR_INVALID_STATE
#define	ERR_INV_STATE					ERR_INVALID_STATE
#define	ERROR_NOT_SUPPORTED				30
#define	ERROR_LOAD_FAIL					31
#define	ERROR_FS_MOUNT_FAILED			32
#define	ERROR_UNKNOWN					33
#define	ERR_UNKNOWN						33
#define	ERROR_MEM_MAP_FAIL				34
#define	ERROR_LOAD_PNG_FILE_FAIL			35
#define	ERROR_LOAD_JPEG_FILE_FAIL		36
#define	ERROR_PTHREAD_ATTR_INIT_FAIL	37
#define	ERROR_PTHREAD_ATTR_SET_FAIL		38
#define	ERROR_PTHREAD_CREATE_FAIL		39
#define	ERR_DATA_VERIFY_FAIL				40
#define	ERR_CREATE_SHARE_MEM_FAIL		41
#define	ERR_HTTP_STAT_FAIL				42
#define	ERR_WAITPID_FAIL					43
#define	ERR_TIMEOUT						44
#define	ERR_PROCESS_RETURN_FAIL			45
#define	ERR_CREATE_WINDOW_FAIL			46
#define	ERR_HIDE_WINDOW_FAIL				47
#define	ERR_REGISTER_IME_FAIL				48
#define	ERR_ACTIVATE_IME_FAIL				49
#define	ERR_TRY_WAIT_FAIL					50
#define	ERR_SYSTEM_CALL_UMOUNT_FAIL		51
#define	ERR_GETADDRINFO_FAIL				80
#define	ERR_PENGING						81
#define	ERR_INIT_THREAD_ATTR_FAIL		82
#define	ERR_SET_THREAD_ATTR_FAIL			83
#define	ERR_ENCODE_FAIL					87
#define	ERR_DETECTION_FAIL				88
#define	ERR_SETSOCKOPT_FAIL				89
#define	ERR_SOCKET_SETSOCKOPT_FAIL		90
#define	ERR_MUTEX_LOCK_FAIL				91
#define	ERR_MUTEX_UNLOCK_FAIL			92
#define	ERR_INVALID_HANDLE				93
#define	ERR_ALREADY_RUNNING				94
#define	ERR_ALREADY_QUEUED				95
#define	ERR_MICROWINDOWS_INIT_FAIL		96
#define	ERR_END_OF_FILE					97
#define	ERR_CREATE_OBJ_FAIL				98
#define	ERR_REGISTER_FAIL					99
#define	ERR_SCK_CREATE_FAIL				100
#define	ERR_ALREADY_INITIALIZED			101
#define	ERR_NOT_INITIALIZED				102
#define	ERR_MUTEX_CREATE_FAIL			103
#define	ERROR_NO_FLASH_PROBED			( 104)
#define	ERROR_FAILED						( 105)
#define	ERR_FAILED						( 105)
#define	ERROR_INVALID_CHARSET_TYPE		( 106)
#define	ERR_INVALID_CMDLINE_PARA			( 110)
#define	ERROR_ALREADY_INITED				( 111)
#define	ERROR_TRY_WAIT_FAIL				( 113)
#define	ERROR_I2C_READ_FAIL			 	( 114)
#define	ERROR_I2C_WRITE_FAIL			 	( 115)
#if	!defined(_WIN32)
#define	ERROR_OUTOFMEMORY				ERROR_OUT_OF_MEMORY
#endif
#define	ERR_END_OF_STREAM			 	( 116)
#define	ERR_NOT_FOUND_MEDIA_STREAM	 	( 117)
#define	ERR_ALREADY_OPENED				( 118)
#define	ERR_SCK_BIND_FAIL				( 119)
#define	ERR_SCK_GETOPT_FAIL				( 120)
#define	ERR_SCK_CONNECT_FAIL			( 121)
#define	ERR_SCK_SETOPT_FAIL				( 122)
#define	ERR_SCANF_FAILURE				( 123)
#define	ERR_CREATE_RUA_INST_FAIL		( 124)
#define	ERR_TC_GET_ATTR_FAIL			( 125)
#define	ERR_CFSETISPEED_FAIL			( 126)
#define	ERR_TC_FLUSH_FAIL				( 127)
#define	ERR_TC_SET_ATTR_FAIL			( 128)
#define	ERR_PRINTF_FAILURE				( 129)
#define	ERR_CONVERT_FAIL				( 130)
#define	ERR_GETSOCKNAME_FAIL			( 131)
#define	ERR_SOCK_NOT_IPv4ADDR			( 132)
#define	ERR_INVALID_IPv4ADDR			( 133)
#define	ERR_FILE_SIZE_MISMATCH			( 134)
#define	ERR_INV_FILE_DATA				( 135)
#define	ERR_MISMATCH					( 136)
#define	ERR_HOOK_SIGNAL_FAIL			( 137)
#define	ERR_CONNECTION_FAIL				138
#define	ERR_SETUP_FAIL					139
#define	ERR_REMOVE_REG_INFO_FAIL		140
#define	ERR_BUSY						141
#define	ERR_LINUX_SIGNAL_INT			142
#define	ERR_INIT_FAIL					143
#define	ERR_EXIT_FAIL					144
#define	ERR_INVALID_CMD					145
#define	ERR_PARSE_XML_FAIL				146
#define	ERR_NO_QDBUS_CONN				147
#define	ERR_DBUS_CALL_ERR				148
#define	ERR_DBUS_SEND_ERR				149
#define	ERR_JSCRIPT_ERR					150
#define	ERR_INVALID_REPLY				151
#define	ERR_SYMLINK_FAIL				152
#define	ERR_GET_SYS_TIME_FAIL			153
#define	ERR_SYS_SET_ENV_FAIL			154
#define	ERR_SYS_remove_FAIL				155
#define	ERR_SYS_mkfifo_FAIL				156
#define	ERR_NOT_EXIST					157
#define	ERR_QT_SIGNAL_CONN_FAIL			158
#define	ERR_INVALID_PATH				159
#define	ERR_RENAME_FAIL					160
#define	ERR_INTERNAL_FAIL				161
#define	ERR_INVALID_FILENAME			162
#define	ERR_FILE_SIZE_TOO_LARGE			163
#define	ERR_XML_NO_ROOT_ELEMENT			164
#define	ERR_INVALID_ID					165
#define	ERR_FILE_CLOSE_FAIL				166
#define	ERR_SET_SYS_TIME_FAIL			167
#define	ERR_KILL_PROCESS_FAIL			168
#define	ERR_HOST_NOT_FOUND				169
#define	ERR_LOGIN_AUTH_FAIL				170
#define	ERR_CONNECTIN_REFUSED			171
#define	ERR_TEMPORARY_NETWORK_FAIL		172
#define	ERR_RUN_ExtractSysCfg_FAIL		173
#define	ERR_RUN_ResetSysCfg_FAIL		174
#define	ERR_INVALID_DATA_TYPE			175
#define	ERR_CREATE_DIR_FAIL				176
#define	ERR_INVALID_FILE_SIGNATURE		177
#define	ERR_INVALID_FILE_VERSION		178
#define	ERR_INVALID_FILE_DATA			179
#define	ERR_FILE_COPY_FAIL				180
#define	ERR_NO_MORE_RESOURCE			181
#define	ERR_FILE_NOT_OPENED				182
#define	ERR_SET_ATTR_FAIL				183
#define	ERR_GET_ATTR_FAIL				184
#define	ERR_DOWNLOAD_FAIL				185
#define	ERR_INCORRECT_THREAD_CONTEXT	186
#define	ERR_DEL_FAIL					187
#define	ERR_FILE_TRUNCATE_FAIL			188
#define	ERR_WRITE_LOG_PENGDING			189
#define	ERR_MUTEX_TRY_LOCK_FAIL			190
#define	ERR_CONTENT_NOT_FOUND			191
#define	ERR_NO_MORE_DATA				192
#define	ERR_NO_DEV						193
#define	ERR_SNMP_SET_FAIL				194
#define	ERR_SNMP_GET_FAIL				195
#define	ERR_NOT_READY					196
#define	ERR_IS_USING					197
#define	ERR_PAINTER_BEGIN_FAIL			198
#define	ERR_CREATE_PIPE_FAILED			199
#define	ERR_GST_PLAY_FAIL				200
#define	ERR_GST_STOP_FAIL				201

//RUA err
#define	ERR_RUA_GET_MIXER_SRC_ID_FAIL		300
#define	ERR_RUA_SET_MIXER_SRC_WINDOW_FAIL	301
#define	ERR_RUA_SET_VALIDATE_FAIL			302
#define	ERR_RUA_GET_VOLUME_FAIL				303
#define	ERR_RUA_SET_VOLUME_FAIL				304
#define	ERR_RUA_SET_PROP_FAIL				305
#define	ERR_RUA_GET_PROP_FAIL				306
#define	ERR_RUA_GET_MIXER_SRC_WINDOW_FAIL	307
#define	ERR_RUA_SET_PENDING					308
#define	ERR_RUA_EXCHANGE_PROP_FAIL			309
#define	ERR_RUA_LOCK_FAIL					310
#define	ERR_RUA_MAP_FAIL					311
#define	ERR_RUA_UNLOCK_FAIL					312


/*Enabled features*/
#define ERR_INVALID_DEVICE_KEY      422

#define ERR_DISABLE_PLAYER          423
#define ERR_DISABLE_DOLBY           424
#define ERR_DISABLE_WMA             425
#define ERR_DISABLE_ADOBE_FLASH     426

//NexusExt err no
#define	ERR_NxClientJoin_FAILED			(500+1)
#define	ERR_NxClientAlloc_FAILED		(500+2)
#define	ERR_NxClientConn_FAILED			(500+3)
#define	ERR_CmdLineParseErr				(500+4)
#define	ERR_CmdLineProbeErr				(500+5)
#define	ERR_FilePlay_OpenFail			(500+6)
#define	ERR_PlayPump_OpenFail			(500+7)
#define	ERR_PlaybackCreate_FAIL			(500+8)
#define	ERR_FileIoCustom_AttachFail		(500+9)
#define	ERR_Cdxa_File_Attac_Fail		(500+10)
#define	ERR_PcmFile_AttachFail			(500+11)
#define	ERR_FileIoSticky_AttachFail		(500+12)
#define	ERR_StcChannel_OpenFail			(500+13)
#define	ERR_SyncChannel_CreateFail		(500+14)
#define	ERR_Playback_SetSettingsErr		(500+15)
#define	ERR_MediaPlayer_CreateFail		(500+16)
#define	ERR_MediaPlayer_StartFail		(500+17)
#define	ERR_MediaPlayer_GetVolumeFail	(500+18)
#define	ERR_MediaPlayer_SetVolumeFail	(500+19)
#define	ERR_OpenNexusGpio_FAIL			(520)
#define	ERR_OpenNexusI2c_FAIL			(521)
#define	ERR_NexusI2cWrite_FAIL			(522)
#define	ERR_Nexus_Player_Trick_Err		(523)
#define	ERR_Nexus_Player_GetStatus_Err	(524)
#define	ERR_Nexus_GetDispStatus_Err		(525)
#define	ERR_Nexus_SetDispSetting_Err	(526)
#define	ERR_Nexus_setVidWinAspectRatio_Err	(527)
#define	ERR_Nexus_setSyncTime_Err		(528)
#define	ERR_Nexus_OpenParserBand_Err	(529)

//Tuner
#define ERR_Tuner_Scan_Program (600 +1 )
#define ERR_Tuner_Not_Found (600 +2 )



#define	ERR_BASE_MAX					(5000)

#endif	//_BASEERRDEF_H_

