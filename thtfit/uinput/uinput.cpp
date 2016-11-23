#include "uinput.h"

static BOOL_t g_bShouldExitApp = FALSE;
static int uinp_fno = -1;

static struct uinput_user_dev uinp;
static struct input_event event;
static SharedPtr <CUinput> g_Uinput_sp;


//static const char *ir_dev = "/dev/ir";
//static const char *fip_dev = "/dev/fip";
static const char *uinp_dev = "/dev/uinput";

static void terminate(const int sig_no)
{
	if(Sw_LogUinput)
	{
		LOG("uinput: Sig %d\n", sig_no);
	}
	g_bShouldExitApp = TRUE;
	g_Uinput_sp->notifyIrInput();
}

static void install_sig_handler(void)
{
	signal(SIGBUS, terminate);
	signal(SIGFPE, terminate);
	signal(SIGHUP, terminate);
	signal(SIGILL, terminate);
	signal(SIGINT, terminate);
	signal(SIGIOT, terminate);
	signal(SIGPIPE, terminate);
	signal(SIGQUIT, terminate);
	signal(SIGSEGV, terminate);
	signal(SIGSYS, terminate);
	signal(SIGTERM, terminate);
	signal(SIGTRAP, terminate);
	signal(SIGUSR1, terminate);
	signal(SIGUSR2, terminate);
}

static int compare_ir_key(const void *k1, const void *k2)
{
	const IR_KEY *key1 = (const IR_KEY *)k1;
	const IR_KEY *key2 = (const IR_KEY *)k2;
	if (key1->remote_code == key2->remote_code)
		return 0;
	else if (key1->remote_code > key2->remote_code)
		return 1;
	else
		return -1;
}

static int compare_ir_pointer(const void *k1, const void *k2)
{
	const IR_POINTER *key1 = (const IR_POINTER *)k1;
	const IR_POINTER *key2 = (const IR_POINTER *)k2;
	if (key1->remote_code == key2->remote_code)
		return 0;
	else if (key1->remote_code > key2->remote_code)
		return 1;
	else
		return -1;
}

#if 0
static int compare_fip_key(const void *k1, const void *k2)
{
	const FIP_KEY *key1 = (const FIP_KEY *)k1;
	const FIP_KEY *key2 = (const FIP_KEY *)k2;
	if (key1->fip_code == key2->fip_code)
		return 0;
	else if (key1->fip_code > key2->fip_code)
		return 1;
	else
		return -1;
}
#endif

static void nexusIrCallback(void * context,int param)
{
	CUinput* irInput;
	BSTD_UNUSED(param);
	irInput=(typeof(irInput))context;
	do
	{
		if( NULL == irInput )
		{
			PRINT_FILE_LINENO_BUG_STR
			break;	
		}
		irInput->notifyIrInput();

	}while(FALSE);
}

static void send_key(unsigned int key)
{

	int iRet;
	INT_t bIrControlEnabled=-1;
	iRet = getIrControlEnabled(&bIrControlEnabled);
	if(iRet == ERROR_SUCCESS && bIrControlEnabled == 2 )	/*IR is disabled*/
	{
		for(int i=0;i<(int)always_enabled_key_count;i++)
		{
			if(key == always_enabled_key_table[i])
			{
				goto report_key;
			}
		}
		
		if(Sw_LogUinput)
		{
			LOG_BLINE("The key %d is disabled\n",key);
		}
		return;
	}
	else if(iRet == ERROR_SUCCESS && bIrControlEnabled == 0)
	{
		return;	
	}
	
report_key:
	/* Key pressed */
	memset(&event, 0, sizeof(event));
	gettimeofday(&event.time, NULL);

	event.type = EV_KEY;
	event.code = key;
	event.value = 1;
	write(uinp_fno, &event, sizeof(event));

	event.type = EV_SYN;
	event.code = SYN_REPORT;
	event.value = 0;
	write(uinp_fno, &event, sizeof(event));

	/* Key released */
	memset(&event, 0, sizeof(event));
	gettimeofday(&event.time, NULL);

	event.type = EV_KEY;
	event.code = key;
	event.value = 0;
	write(uinp_fno, &event, sizeof(event));

	event.type = EV_SYN;
	event.code = SYN_REPORT;
	event.value = 0;
	write(uinp_fno, &event, sizeof(event));
}

static void send_movement(int x, int y)
{
	memset(&event, 0, sizeof(event));
	gettimeofday(&event.time, NULL);

	/* Send in movement */
	if (x != 0) {
		event.type = EV_REL;
		event.code = REL_X;
		event.value = x;
		write(uinp_fno, &event, sizeof(event));
	}
	if (y != 0) {
		event.type = EV_REL;
		event.code = REL_Y;
		event.value = y;
		write(uinp_fno, &event, sizeof(event));
	}
	if ((x != 0) || (y != 0)) {
		event.type = EV_SYN;
		event.code = SYN_REPORT;
		event.value = 0;
		write(uinp_fno, &event, sizeof(event));
	}
}

static int setup_uinput_device(void)
{
	int iOutRet = UIN_PEC_SUCCESS, iRet,i;

	memset(&uinp, 0, sizeof(uinp));
	strncpy(uinp.name,"Simulated Input", UINPUT_MAX_NAME_SIZE);
	uinp.id.version = 1; /* anything */
	uinp.id.bustype = BUS_VIRTUAL;
	
	if( uinp_fno < 0 )
	{
		iOutRet=UIN_PEC_ERR_FDNO;
		goto PROC_EXIT;
	}
	
	//support KEY event
	ioctl(uinp_fno, UI_SET_EVBIT, EV_KEY);

	{
		//Set customized keys
		CUinput::IR_KEY_MAP & IrKeyMapTable = g_Uinput_sp->getIrKeyMapTable();
		CUinput::IR_KEY_MAP::iterator itIrKeyMapItem;
		for(itIrKeyMapItem = IrKeyMapTable.begin(); itIrKeyMapItem != IrKeyMapTable.end(); itIrKeyMapItem++)
		{
			iRet = ioctl(uinp_fno, UI_SET_KEYBIT, itIrKeyMapItem->second->m_LinuxInputKeyCode);
			if(0 != iRet)
			{
				LOG_BLINE("SetKeyBit(%d) err %s\n", itIrKeyMapItem->second->m_LinuxInputKeyCode, strerror(errno));
			}
		}
	}

	for (i = 0; i < (int)(sizeof(nec_key_table) / sizeof(IR_KEY)); i++)
		ioctl(uinp_fno, UI_SET_KEYBIT, nec_key_table[i].key_code);
#if 0
	for (i = 0; i < (int)(sizeof(ref1_table) / sizeof(FIP_KEY)); i++)
		ioctl(uinp_fno, UI_SET_KEYBIT, ref1_table[i].key_code);
#endif
	for (i = 0; i < (int)(sizeof(nec_pointer_table) / sizeof(IR_POINTER)); i++) {
		if (nec_pointer_table[i].type)
			ioctl(uinp_fno, UI_SET_KEYBIT, nec_pointer_table[i].key_code);
	}


	write(uinp_fno, &uinp, sizeof(uinp));

	if (ioctl(uinp_fno, UI_DEV_CREATE)) 
	{
		printf("Unable to create UINPUT device\n");
		return -1;
	}

PROC_EXIT:
	
	return iOutRet;
}

static const IR_POINTER *search_ir_pointer(const IR_POINTER *table, const unsigned int size, const unsigned int remote_code)
{
	IR_POINTER dummy;
	dummy.remote_code = remote_code;
	return (const IR_POINTER *)bsearch(&dummy, table, size, sizeof(IR_POINTER), compare_ir_pointer);
}


static const IR_KEY *search_ir_key(const IR_KEY *table, const unsigned int size, const unsigned int remote_code)
{
	IR_KEY dummy;
	dummy.remote_code = remote_code;
	return (const IR_KEY *)bsearch(&dummy, table, size, sizeof(IR_KEY), compare_ir_key);
}

CUinput::CIrKeyMapItem::CIrKeyMapItem()
{
	m_LinuxInputKeyCode = 0;
	m_bRepeat = FALSE;
}

CUinput::CIrKeyMapItem::~CIrKeyMapItem()
{

}

CUinput::CUinput()
{
	m_IrKeyMap.clear();
	m_idLedBusyTimer = 0;
	m_bLedInBusy = FALSE;
	client=NULL;
	m_iEventMode=MODE_KEY;	
	m_LastIrCode = 0;
	m_RepeatableKeyChkStartTimeMs = 0;
}

CUinput::~CUinput()
{
	INT_t iRet;
	close();
	if(m_bLedInBusy)
	{
		iRet = Mp7xxCommon::LedIndicator_EndBusy();
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		m_bLedInBusy = FALSE;
	}
}

CUinput::IR_KEY_MAP & CUinput::getIrKeyMapTable()
{
	CAutoLock AutoRangeLocker(&m_IrKeyMapMutex);
	return m_IrKeyMap;
}

INT_t CUinput::LoadIrKeyMapTable()
{
	INT_t iOutRet = UIN_PEC_SUCCESS, iRet;
	bool bRet;
	TiXmlDocument2 MapTableXmlDoc;
	SharedPtr <CIrKeyMapItem> IrKeyMapItem_sp;
	const TiXmlElement * pRootEle = NULL;
	const TiXmlElement * pEle_IrKeyMapping = NULL;
	const TiXmlElement * pEle_Key = NULL;
	UINT32_t IrKeyCode;
	const char * pszIrKeyCodePropVal = NULL;
	const char * pszLinuxInputKeyCodePropVal = NULL;
	const char * pszRepeatPropVal = NULL;

	CAutoLock AutoRangeLocker(&m_IrKeyMapMutex);
	
	do
	{
		{
			int bExist = FALSE;
			iRet = ExistFile(UINPUT_XML_CFG_FILE, &bExist);
			if(ERROR_SUCCESS == iRet)
			{
				if(FALSE == bExist)
				{
					always_enabled_key_table[always_enabled_key_count++] = ALWAYS_ENABLED_KEY_MENU;
					always_enabled_key_table[always_enabled_key_count++] = ALWAYS_ENABLED_KEY_OK;
					always_enabled_key_table[always_enabled_key_count++] = ALWAYS_ENABLED_KEY_UP;
					always_enabled_key_table[always_enabled_key_count++] = ALWAYS_ENABLED_KEY_DOWN;
					always_enabled_key_table[always_enabled_key_count++] = ALWAYS_ENABLED_KEY_LEFT;
					always_enabled_key_table[always_enabled_key_count++] = ALWAYS_ENABLED_KEY_RIGHT;
					break;
				}
			}
			else
			{
				iOutRet = iRet;
				break;
			}
		}
	
		m_IrKeyMap.clear();
		bRet = MapTableXmlDoc.LoadFile(UINPUT_XML_CFG_FILE);
		if(FALSE == bRet)
		{
			iOutRet = ERR_PARSE_XML_FAIL;
			LOG("Uinput.Load(%s) err(%s),line:%d,col:%d\n", UINPUT_XML_CFG_FILE, MapTableXmlDoc.ErrorDesc(),MapTableXmlDoc.ErrorRow(), MapTableXmlDoc.ErrorCol());
			break;
		}
		pRootEle = MapTableXmlDoc.RootElement();
		if(NULL == pRootEle)
		{
			break;
		}
		pEle_IrKeyMapping = pRootEle->FirstChildElement("IrKeyMapping");
		if(NULL == pEle_IrKeyMapping)
		{
			break;
		}
		pEle_Key = pEle_IrKeyMapping->FirstChildElement("Key");
		if(NULL == pEle_Key)
		{
			break;
		}
		while(pEle_Key)
		{
			pszIrKeyCodePropVal = pEle_Key->Attribute("IrCode");
			pszLinuxInputKeyCodePropVal = pEle_Key->Attribute("LinuxInputCode");
			pszRepeatPropVal = pEle_Key->Attribute("Repeat");
			if( pszLinuxInputKeyCodePropVal &&(always_enabled_key_count < ALWAYS_ENABLED_KEY_MAX) )
			{
				unsigned int key_code = atoi( pszLinuxInputKeyCodePropVal );
				const char *pszIsAlwaysEnabledKey = pEle_Key->Attribute("AlwaysEnabled");
				if( pszIsAlwaysEnabledKey == NULL )
				{
					switch( key_code )
					{/*default always_enabled key*/
					case ALWAYS_ENABLED_KEY_MENU:
					case ALWAYS_ENABLED_KEY_OK:
					case ALWAYS_ENABLED_KEY_UP:
					case ALWAYS_ENABLED_KEY_DOWN:
					case ALWAYS_ENABLED_KEY_LEFT:
					case ALWAYS_ENABLED_KEY_RIGHT:
						always_enabled_key_table[always_enabled_key_count] = key_code;
						always_enabled_key_count++;
						break;
					}
				}
				else
				{
					unsigned int is_always_enabled = atoi(pszIsAlwaysEnabledKey);
					if( is_always_enabled )
					{
						always_enabled_key_table[always_enabled_key_count] = key_code;
						always_enabled_key_count++;
					}
				}
			}
			
			if(pszIrKeyCodePropVal && pszLinuxInputKeyCodePropVal)
			{
				IrKeyMapItem_sp = SharedPtr <CIrKeyMapItem> (new CIrKeyMapItem);
				if(IrKeyMapItem_sp.isNull())
				{
					iOutRet = ERROR_OUT_OF_MEMORY;
					break;
				}
				IrKeyCode = 0;
				iRet = sscanf(pszIrKeyCodePropVal, "%x", &IrKeyCode);
				if(1 != iRet)
				{
					LOG("IrKeyMapTable IrKey %s\n", pszIrKeyCodePropVal);
				}
				IrKeyMapItem_sp->m_LinuxInputKeyCode = atoi(pszLinuxInputKeyCodePropVal);
				if(pszRepeatPropVal)
				{
					IrKeyMapItem_sp->m_bRepeat = atoi(pszRepeatPropVal);
				}
				if(Sw_LogUinput)
				{
					LOG_BLINE("Ir:0x%08x,Linux:%d,Repeat:%d\n", IrKeyCode, IrKeyMapItem_sp->m_LinuxInputKeyCode, IrKeyMapItem_sp->m_bRepeat);
				}
				if(IrKeyCode && IrKeyMapItem_sp->m_LinuxInputKeyCode)
				{
					try
					{
						m_IrKeyMap[IrKeyCode] = IrKeyMapItem_sp;
					}
					catch(std::bad_alloc & BadAllocEx)
					{
						iOutRet = ERROR_OUT_OF_MEMORY;
						break;
					}
				}
			}
			pEle_Key = pEle_Key->NextSiblingElement("Key");
		}
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
	}while(FALSE);

	return iOutRet;
}


void CUinput::loadIrRepeatKeyTable()
{		
	unsigned int i;
	for (i = 0; i < sizeof(nec_key_table) / sizeof(IR_KEY); i++) 
	{
		if (nec_key_table[i].repeat != 0)
		{
			if(ALWAYS_ENABLED_REPEAT_KEY_MAX > (repeatkeycount + 1))
			{
				always_enabled_repeat_key_table[repeatkeycount++] = nec_key_table[i].remote_code;
			}
		}
	}
	
	{
		//Set customized IR repeatable keys
		CUinput::IR_KEY_MAP & IrKeyMapTable = getIrKeyMapTable();
		CUinput::IR_KEY_MAP::iterator itIrKeyMapItem;
		for(itIrKeyMapItem = IrKeyMapTable.begin(); itIrKeyMapItem != IrKeyMapTable.end(); itIrKeyMapItem++)
		{
			if(itIrKeyMapItem->second->m_bRepeat)
			{
				if(ALWAYS_ENABLED_REPEAT_KEY_MAX > (repeatkeycount + 1))
				{
					always_enabled_repeat_key_table[repeatkeycount++] = itIrKeyMapItem->first;
				}
			}
		}
	}	
}


INT_t CUinput::open()
{
	int iRet=UIN_PEC_SUCCESS;
	
	uint32_t filter = 0;
	NEXUS_InputClientSettings 	settings;
	NxClient_JoinSettings 		joinSettings;
	NxClient_AllocSettings 		allocSettings;
	NxClient_AllocResults 		allocResults;

	CAutoLock AutoRangeLocker(&m_IrinputMutex);

	if( NULL !=client )
	{
		NxClient_Uninit();
	}

    NxClient_GetDefaultJoinSettings(&joinSettings);
	iRet = NxClient_Join(&joinSettings);
	if( iRet)
	{
		//fprintf(stderr,"NEXUS_Platform_Join err:%d\n",iRet);
		iRet=UIN_PEC_ERR_ExternalError;
		goto PROC_EXIT;
	}

	NxClient_GetDefaultAllocSettings(&allocSettings);
    allocSettings.inputClient = 1;
    iRet = NxClient_Alloc(&allocSettings, &allocResults);
    if (iRet)
    {
    	//fprintf(stderr,"NEXUS_InputClient_Acquire err:%d\n",iRet);
    	iRet=UIN_PEC_OUT_OF_MEMORY;	
		goto PROC_EXIT;
    }

	if(allocResults.inputClient[0].id)
	{
		client = NEXUS_InputClient_Acquire(allocResults.inputClient[0].id);
		if(NULL == client)
		{
			iRet=UIN_PEC_ERR_ExternalError;	
			goto FREE_PROC_EXIT;
		}
	}
	else
	{
		iRet=UIN_PEC_ERR_ExternalError;
		goto FREE_PROC_EXIT;	
	}

	NEXUS_InputClient_GetSettings(client, &settings);
	filter |= 1<<NEXUS_InputRouterDevice_eIrInput;
	settings.filterMask = filter;
	settings.codeAvailable.callback = nexusIrCallback;
    settings.codeAvailable.context  = this;
	
	iRet = NEXUS_InputClient_SetSettings(client, &settings);
	if(UIN_PEC_SUCCESS != iRet)
	{
		//fprintf(stderr,"NEXUS_InputClient_SetSettings:%d\n",iRet);
		iRet=UIN_PEC_ERR_ExternalError;
	}

	goto PROC_EXIT; 

FREE_PROC_EXIT:
	NxClient_Free(&allocResults);

PROC_EXIT:
	return iRet;
}

void CUinput::close()
{
	CAutoLock AutoRangeLocker(&m_IrinputMutex);
	
	if( NULL != client)
	{
		NxClient_Uninit();
		client=NULL;
	}	
}

INT_t CUinput::processEvent()
{
	int iOutRet=UIN_PEC_SUCCESS,iRet;
    unsigned           num;
	NEXUS_InputRouterCode code;

	CAutoLock AutoRangeLocker(&m_IrinputMutex);

	if(NULL == client)
	{
		iRet=UIN_PEC_ERR_ExternalError;
		iOutRet=iRet;
		goto PROC_EXIT;
	}
	
	iRet = NEXUS_InputClient_GetCodes(client, &code, 1, &num);

	if(iRet != UIN_PEC_SUCCESS)
	{
		iRet=UIN_PEC_ERR_ExternalError;
		iOutRet=iRet;
		goto PROC_EXIT;
	}

	if( 0 == num )
		goto PROC_EXIT;

	switch (code.deviceType)
	{
	case NEXUS_InputRouterDevice_eIrInput:
		iRet=processIrEvent(code);
		if( UIN_PEC_SUCCESS!=iRet)
			iOutRet=iRet;
		break;
	case NEXUS_InputRouterDevice_eKeypad:
	case NEXUS_InputRouterDevice_eEvdev:
	case NEXUS_InputRouterDevice_eUhfInput:
	case NEXUS_InputRouterDevice_eBluetoothRemote:
	default:
		fprintf(stderr,"unknown deviceType %d\n", code.deviceType);
		break;
	}

PROC_EXIT:

	return iOutRet;	
}

INT_t CUinput::processIrEvent(NEXUS_InputRouterCode& ircode)
{
	int iOutRet = ERR_SUCCESS;
	const IR_POINTER *ir_pointer;
	unsigned int input_code;	
	const IR_KEY *ir_key;
	NEXUS_InputRouterCode irinputcode = ircode;

	input_code = irinputcode.data.irInput.code;	

	if(Sw_LogUinput)
	{
		LOG("IrKey: 0x%08x	repeate: %d\n", input_code, irinputcode.data.irInput.repeat);
	}

	//Not too fast even it is a repeatable key
	if(TRUE == irinputcode.data.irInput.repeat)
	{
		if(input_code == m_LastIrCode)
		{
			if(400 > GetSysUpTimeMs() - m_RepeatableKeyChkStartTimeMs)	//in 400ms
			{
				//ignore
				goto PROC_EXIT;
			}
		}
	}
	else
	{
		m_LastIrCode = input_code;
		m_RepeatableKeyChkStartTimeMs = GetSysUpTimeMs();
	}
	
	ir_pointer = NULL;
	if (m_iEventMode != MODE_KEY) 
	{		
		ir_pointer = search_ir_pointer(nec_pointer_table, sizeof(nec_pointer_table) / sizeof(IR_POINTER), input_code);
		if (ir_pointer != NULL) 
		{
			if (ir_pointer->type) 
			{				
				send_key(ir_pointer->key_code);
			} 
			else 
			{				
				send_movement(ir_pointer->rel_pos[0], ir_pointer->rel_pos[1]);
			}
		}
	}
	if (ir_pointer == NULL) 
	{
		IR_KEY FindedIrKey;		
		ir_key = NULL;
		{
			IR_KEY_MAP & IrKeyMapTable = getIrKeyMapTable();
			IR_KEY_MAP::iterator itIrKeyMapItem;
			itIrKeyMapItem = IrKeyMapTable.find(input_code);
			if(itIrKeyMapItem != IrKeyMapTable.end())
			{
				FindedIrKey.remote_code = itIrKeyMapItem->first;
				FindedIrKey.key_code = itIrKeyMapItem->second->m_LinuxInputKeyCode;
				FindedIrKey.repeat = itIrKeyMapItem->second->m_bRepeat;
				FindedIrKey.desc = "";
				ir_key = &FindedIrKey;
			}
		}

		if(NULL == ir_key)
		{			
			ir_key = search_ir_key(nec_key_table, sizeof(nec_key_table) / sizeof(IR_KEY), input_code);
		}
		
		if (ir_key != NULL) 
		{
			fprintf(stderr,"ir_key->remote: 0x%08x, key: 0x%08x, desc: %s\n\n", ir_key->remote_code, ir_key->key_code, ir_key->desc);
			if((TRUE == irinputcode.data.irInput.repeat) && (FALSE == ir_key->repeat))	//not repeatable
			{
				//ignore
			}
			else	//other cases
			{
				send_key(ir_key->key_code);			
				if (ir_key->key_code == KEY_MODE) 
				{
					m_iEventMode = ((MODE_KEY==m_iEventMode) ? MODE_POINTER : MODE_KEY);
					printf("Change to %s mode.\n", (MODE_POINTER==m_iEventMode) ? "pointer" : "key");
				}
			}
		}
		else 
		{
			fprintf(stderr,"unknown remote: 0x%08x\n", input_code);
		}
	}

PROC_EXIT:

	return iOutRet;	
}

void CUinput::notifyIrInput(void)
{
	m_EventOnIrInput.SetEvent();
}

INT_t CUinput::waitIrInputEventTimeout(INT_t iTimeoutMs)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	
	iRet=m_EventOnIrInput.WaitEventTimeout(iTimeoutMs);
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = iRet;
	}
	
	return iOutRet; 	
}

INT_t CUinput::resetIrInputEvent()
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;

	iRet=m_EventOnIrInput.ResetEvent();
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = iRet;
	}
	
	return iOutRet; 	
}

INT_t CUinput::getNextTimerTimeoutMs(OUT INT_t & NextTimeoutMs)
{
	return m_oTimerList.getNextTimeoutMs(OUT NextTimeoutMs);
}

INT_t CUinput::CheckAndTrigTimer()
{
	return m_oTimerList.CheckAndTrig();
}

INT_t CUinput::On_IrInput()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	do
	{
		if(0 < m_idLedBusyTimer)
		{
			iRet = m_oTimerList.UnregisterTimer(m_idLedBusyTimer);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			m_idLedBusyTimer = 0;
		}

		if(m_bLedInBusy)
		{
			iRet = Mp7xxCommon::LedIndicator_EndBusy();
			if(ERROR_SUCCESS != iRet)
			{
//				PRINT_BFILE_LINENO_IRET_STR;
			}
			m_bLedInBusy = FALSE;
		}

		iRet = Mp7xxCommon::LedIndicator_BeginBusy();
		if(ERROR_SUCCESS != iRet)
		{
//			PRINT_BFILE_LINENO_IRET_STR;
		}
		
		m_bLedInBusy = TRUE;
		iRet = m_oTimerList.RegisterTimer(m_this_wp, OUT m_idLedBusyTimer, 100);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}while(FALSE);

	return iOutRet;
}

VOID CUinput::OnTimer(UINT_t TimerId)
{
	INT iRet;

	do
	{
		if(TimerId == m_idLedBusyTimer)
		{
			if(m_bLedInBusy)
			{
				iRet = Mp7xxCommon::LedIndicator_EndBusy();
				if(ERROR_SUCCESS != iRet)
				{
//					PRINT_BFILE_LINENO_IRET_STR;
				}
				m_bLedInBusy = FALSE;
			}
			iRet = m_oTimerList.UnregisterTimer(m_idLedBusyTimer);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			m_idLedBusyTimer = 0;
		}
	}while(FALSE);
}

int main(int argc,char*argv[])
{
	int iOutRet=UIN_PEC_SUCCESS,iRet;	
	BOOL_t bRegistered_DbgLogSw = FALSE;
	
	iRet = RegDbgLogSwitch();
	if(UIN_PEC_SUCCESS == iRet)
	{
		bRegistered_DbgLogSw = TRUE;
	}
	else
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}

	if (argc != 1) 
	{
		printf("usage: %s\n", argv[0]);
		iOutRet = UIN_PEC_ERR_InvalidParameter;
		goto PROC_EXIT;
	}

	g_Uinput_sp = SharedPtr <CUinput> (new CUinput);
	if(NULL == g_Uinput_sp.get())
	{
		iOutRet = UIN_PEC_OUT_OF_MEMORY;
		goto PROC_EXIT;
	}

	g_Uinput_sp->m_this_wp = g_Uinput_sp;

	iRet=g_Uinput_sp->LoadIrKeyMapTable();
	if( iRet != UIN_PEC_SUCCESS )
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}

	g_Uinput_sp->loadIrRepeatKeyTable();
	
	qsort(nec_key_table, sizeof(nec_key_table) / sizeof(IR_KEY), sizeof(IR_KEY), compare_ir_key);
	qsort(nec_pointer_table, sizeof(nec_pointer_table) / sizeof(IR_POINTER), sizeof(IR_POINTER), compare_ir_pointer);
	//qsort(ref1_table, sizeof(ref1_table) / sizeof(FIP_KEY), sizeof(FIP_KEY), compare_fip_key);

	if ( (uinp_fno = open(uinp_dev, O_WRONLY | O_NDELAY)) < 0) 
	{
		fprintf(stderr,"error opening %s\n", uinp_dev);
		iOutRet=UIN_PEC_ERR_FDNO;
		goto PROC_EXIT;
	} 
	else if (setup_uinput_device() < 0) 
	{
		close(uinp_fno);
		uinp_fno = -1;
		goto PROC_EXIT;
	}

	if((iRet=g_Uinput_sp->open()) != UIN_PEC_SUCCESS)
	{
		g_Uinput_sp->close();
		iOutRet=iRet;
		goto PROC_EXIT;
	}

	install_sig_handler();
	
	while(TRUE)
	{
		INT_t iNextTimeoutMs;
		INT_t DefaultTimeoutMs = (2*1000);
		
		if(g_bShouldExitApp)
		{
			break;
		}
		iRet = g_Uinput_sp->getNextTimerTimeoutMs(OUT iNextTimeoutMs);
		if(ERROR_SUCCESS == iRet)
		{
			if( iNextTimeoutMs < 0 ) 
			{
				iNextTimeoutMs = DefaultTimeoutMs;
			}
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
			iNextTimeoutMs = DefaultTimeoutMs;
		}
		
		iRet=g_Uinput_sp->waitIrInputEventTimeout(iNextTimeoutMs);

		if( iRet == ERR_SUCCESS)
		{
			if(g_bShouldExitApp)
			{
				break;
			}
//            printf("\nresetIrInputEvent\n");
			iRet=g_Uinput_sp->resetIrInputEvent();
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_CRT_ERR;
			}
			
			g_Uinput_sp->On_IrInput();
		
			iRet=g_Uinput_sp->processEvent();
			if(iRet != UIN_PEC_SUCCESS)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
		else if (ERR_TIMEOUT == iRet)
		{
			iRet = g_Uinput_sp->CheckAndTrigTimer();
			iOutRet = iRet;
		}
		else
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
			usleep(50*1000);
		}			
	}
	
PROC_EXIT:

	if (uinp_fno >= 0) 
	{
		iRet = ioctl(uinp_fno, UI_DEV_DESTROY);
		if(0 != iRet)
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
		}
		iRet = close(uinp_fno);
		if(0 != iRet)
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
		}
		uinp_fno = -1;
	}

	g_Uinput_sp->close();

	g_Uinput_sp.Clear();

	if(bRegistered_DbgLogSw)
	{
		bRegistered_DbgLogSw = FALSE;
		iRet = UnregDbgLogSwitch();
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}

	return iOutRet; 	
}

