#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mtd/mtd-user.h>
#include <sys/ioctl.h>


#define MACADDR_LEN     8
#define MACADDR_OFFSET  12

#define PARAM_SZ (0x46*2)
#define NUMADDRS   8
#define BUFLENGTH  (7 + 4 * NUMADDRS * 2)*2 /* matching non-os */
#define	BOARD_TYPE_MASK	0x1F
#define	SER_NBR_MASK	0xFF
#define	ADDR_NBR_MASK	0x07

#define DEFAULT_SECTOR_SIZE     512

#define DEFAULT_MACADDR_PARTITION_NAME "flash0.macadr"

enum CMD_ENUM
{
    CMD_SHOW_MAC=0,
    CMD_GET_MAC,
    CMD_SET_MAC
};
class MacAddress
{
public:
    MacAddress(const char *strMacAddrPatition="/dev/mtdblock1");
    virtual ~MacAddress();
    int setMacAddress(char *strMacAddress);
    int setMacAddress(unsigned char iMacAddress[MACADDR_LEN]);
    int getMacAddress(unsigned char iMacAddress[MACADDR_LEN]);
    void showMacAddress();

protected:
    bool checksumBuffer(unsigned char *pDataBuffer,int iLen,unsigned char iChecksum);
protected:
    char m_strFileName[128];    
};


static bool  findMtdPartitionIndexByName(const char *strPartitionName,int *pIndex)
{
    const char strPartFmt[]="/sys/class/mtd/mtd%d/name";
    char strPartPathName[128];
    char strReadPartName[128];
    int iIndex;
    int iFd=-1;
    int iRet=-1;
    bool bRetValue=false;
    
    for(iIndex=0;iIndex<64;iIndex++)
    {
        snprintf(strPartPathName,sizeof(strPartPathName),strPartFmt,iIndex);
        if(access(strPartPathName,F_OK)!=0)
        {
            return false;
        }
        iFd=open(strPartPathName,O_RDONLY);
        if(iFd<=0)
        {
            continue;
        }
        memset(strReadPartName,0x00,sizeof(strReadPartName));
        iRet=read(iFd,strReadPartName,sizeof(strReadPartName)-1);
        close(iFd);
        if(iRet<=0)
        {            
            continue;
        }        
        if(strstr(strReadPartName,strPartitionName))
        {
            bRetValue=true;
            *pIndex=iIndex;
            break;
        }   
        
    }
    return bRetValue;
} 


MacAddress::MacAddress(const char *strMacAddrPatition)
{
    memset(m_strFileName,0x00,sizeof(m_strFileName));
    if(NULL==strMacAddrPatition)
    {
        return ;
    }
    snprintf(m_strFileName,sizeof(m_strFileName)-1,"%s",strMacAddrPatition);
}
MacAddress::~MacAddress()
{
    
}

bool MacAddress::checksumBuffer(unsigned char *pDataBuffer,int iLen,unsigned char iChecksum)
{
    int iLoop=0;
    unsigned char iCurChecksum=0;
    for(iLoop=0;iLoop<iLen;iLoop++)
    {
        iCurChecksum+=pDataBuffer[iLoop];
    }
    if(iCurChecksum==iChecksum)
    {
        return true;
    }
    return false;
}
int MacAddress::setMacAddress(unsigned char iMacAddress[MACADDR_LEN])
{
     int iFd=-1;
    int iRetValue=-1;
    int iLoop=0;
    unsigned char buffer[DEFAULT_SECTOR_SIZE];
    unsigned char iCurChecksum=0;
    int iIndex=0;
    struct erase_info_user ei;
    
    iFd=open(m_strFileName,O_RDWR);
    if(iFd<=0)
    {
        printf("open file %s failed\n",m_strFileName);
        return iRetValue;
    }
    iRetValue=read(iFd,buffer,sizeof(buffer));
    if(iRetValue!=sizeof(buffer))
    {
        printf("read file %s failed\n",m_strFileName);
        goto out;
    }
    buffer[0]=0;
    buffer[1]=0;
    buffer[2]=0;
    buffer[3]=0;
    buffer[4]=0;
    buffer[5]=0;
    buffer[6]=0;
    buffer[7]=NUMADDRS * 2 * 4 * 2 + 4;
    buffer[8]=0;
    buffer[9]=0;
    buffer[10]=0;
    buffer[11]=0;
    
    for(iIndex=0;iIndex<NUMADDRS;iIndex++)
    {
        for(iLoop=0;iLoop<MACADDR_LEN;iLoop++)
        {
            buffer[iIndex*MACADDR_LEN+iLoop+MACADDR_OFFSET]=iMacAddress[iLoop];
        }
    }
    iCurChecksum=0;
    for(iLoop=0;iLoop<PARAM_SZ;iLoop++)
    {
        iCurChecksum+=buffer[iLoop];
    }
    buffer[PARAM_SZ]=iCurChecksum;   
    buffer[PARAM_SZ+1]=iCurChecksum;
    
    
    if(strstr(m_strFileName,"/dev/mtd"))
    {
        int iEraseSize=-1;
        int iTmpFd=-1;
        char strEraseFile[128];
        char strEraseSize[128];
        char *pMtdPoint=NULL;
        
        pMtdPoint=strstr(m_strFileName,"mtd");
        snprintf(strEraseFile,sizeof(strEraseFile),"/sys/class/mtd/%s/erasesize",pMtdPoint);
        
        if(access(strEraseFile,F_OK)!=0)
        {
            iEraseSize=65536;
            printf("file %s can't access,use default value %d\n",strEraseFile,iEraseSize);
        }
        else
        {
            iTmpFd=open(strEraseFile,O_RDONLY);
            iRetValue=read(iTmpFd,strEraseSize,sizeof(strEraseSize));
            close(iTmpFd);
            if(iRetValue<=0)
            {
                iEraseSize=65536;
                printf("file %s read failed ,use default value %d\n",strEraseFile,iEraseSize);
            }
            else
            {
                sscanf(strEraseSize,"%d",&iEraseSize);
                printf("read %s  value  is %d\n",strEraseFile,iEraseSize);
            }
        }
        
        
        
        printf("start erase %s\n",m_strFileName);
        ei.start = 0;
        ei.length =65536;
        iRetValue = ioctl(iFd, MEMERASE, &ei);
        if(iRetValue<0)
        {
            printf("erase %s failed\n",m_strFileName);
        }
    }

#if 1
    lseek(iFd,0,SEEK_SET);
    iRetValue=write(iFd,buffer,sizeof(buffer));
    if(iRetValue==sizeof(buffer))
    {
        iRetValue=0;
    }
#endif
    
out:
    close(iFd);
    return iRetValue;    

}

int MacAddress::setMacAddress(char *strMacAddress)
{
    unsigned char iMacAddr[MACADDR_LEN];
    int iLen=strlen(strMacAddress);
    
    memset(iMacAddr,0x00,sizeof(iMacAddr));
    if(iLen<17)
    {
        printf("bad Mac Address format\n");
        return -1;
    }
    sscanf(strMacAddress,"%02x-%02x-%02x-%02x-%02x-%02x",
        &iMacAddr[0],&iMacAddr[1],&iMacAddr[2],&iMacAddr[3],&iMacAddr[4],&iMacAddr[5]);
        
    printf("receive mac address:%02X-%02X-%02X-%02X-%02X-%02X\n",
        iMacAddr[0],iMacAddr[1],iMacAddr[2],iMacAddr[3],iMacAddr[4],iMacAddr[5]);
    
    return setMacAddress(iMacAddr);
}
int MacAddress::getMacAddress(unsigned char iMacAddress[MACADDR_LEN])
{
   return 0;    
}

void MacAddress::showMacAddress()
{
    int iFd=-1;
    int iRetValue=0;
    int iLoop=0;
    unsigned char buffer[DEFAULT_SECTOR_SIZE];
    
    
    iFd=open(m_strFileName,O_RDWR);
    if(iFd<=0)
    {
        printf("open file %s failed\n",m_strFileName);
        return ;
    }
    
    iRetValue=read(iFd,buffer,sizeof(buffer));
    if(iRetValue!=sizeof(buffer))
    {
        printf("read file %s failed\n",m_strFileName);
        goto out;
    }
    if(!checksumBuffer(buffer,PARAM_SZ,buffer[PARAM_SZ]))
    {
        printf("checksum  failed\n");
        //goto out;
    }
    printf("\nMacAddress is : ");
    for(iLoop=0;iLoop<6;iLoop++)
    {
        printf("%02X ",buffer[iLoop+MACADDR_OFFSET]);
    }
    printf("\n\n");
    {
        int iLineCount=0;
        for(iLineCount=0;iLineCount<512/16;iLineCount++)
        {
            printf("%04x: ",iLineCount*16);
            for(iLoop=0;iLoop<16;iLoop++)
            {
                printf("%02X ",buffer[iLoop+16*iLineCount]);
                
            }
            printf("\n");
        }
    }
    
    
    
out:
    close(iFd);
    return ;
}


static void usage(int iArgc,char **ppArgv)
{
    printf("eg:\n");
    printf("%s:\n",ppArgv[0]);
    printf("  -f fakemtdFileName\n");
    printf("  -s 00-12-34-56-78-AB\n");
    printf("  -r read MacAddress\n");
    printf("\n\n");
    printf("example\n");
    printf("%s  -r\n",ppArgv[0]);
    printf("%s  -s 00-12-34-56-78-AB\n",ppArgv[0]);
    
   
}



int main(int iArgc,char **ppArgv)
{
    int iArgCount=1;
    char *pFileName=NULL,*pstrMacAddress=NULL;
    CMD_ENUM iCmd=CMD_SHOW_MAC;
    char strPartPathName[128];
    int iIndex=-1;
    if(iArgc<=1)
    {
        usage(iArgc,ppArgv);
        return 0;
    }
    while(iArgCount<iArgc)
    {
        if(strcmp(ppArgv[iArgCount],"-f")==0)
        {
            iArgCount++;
            pFileName=ppArgv[iArgCount];
        }
        else if(strcmp(ppArgv[iArgCount],"-r")==0)
        {
            iCmd=CMD_GET_MAC;
        }
        else if(strcmp(ppArgv[iArgCount],"-s")==0)
        {
            iCmd=CMD_SET_MAC;
            iArgCount++;
            pstrMacAddress=ppArgv[iArgCount];
        }
        else if(strcmp(ppArgv[iArgCount],"-h")==0)
        {
            usage(iArgc,ppArgv);
            return 0;
        }
        else
        {
        }
        iArgCount++;
    }
    
    if(NULL==pFileName)
    {         
        if(!findMtdPartitionIndexByName(DEFAULT_MACADDR_PARTITION_NAME,&iIndex))
        {
            printf("Can't find partition\n");
            return -1;
        }
        snprintf(strPartPathName,sizeof(strPartPathName),"/dev/mtd%d",iIndex);
        printf("Fix partition is %s\n",strPartPathName);
        pFileName=strPartPathName;
    }

    {
        
        
        MacAddress macAddr(pFileName);
        
        switch(iCmd)
        {
        default:
        case CMD_GET_MAC:
            macAddr.showMacAddress();
            break;
        case CMD_SET_MAC:
            macAddr.setMacAddress(pstrMacAddress);
            break;
        }
        
    }
    return 0;
}
