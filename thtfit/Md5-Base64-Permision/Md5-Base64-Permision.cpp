#include <QFile>
#include <QCryptographicHash> 
#include <QEvent>
#include <QDebug>
#define ENCDECRYPT_KEY "s6s9_234&-12"
const int maxn=256+10;

#define swap(a,b) \
do{ \
    char tmp; \
    tmp=a;\
    a=b;\
    b=tmp;    \
}while(false);

void init(char *s, char *t, int len, char *key){
    for(int i = 0; i < maxn; i++){
        s[i] = i;
        t[i] = key[i % len];
    }
    for(int i = 0, j = 0; i < maxn; i++){
        j = (j + s[i] + t[i]) % maxn;
        swap(s[i], s[j]);
    }
}
void RC4(char *s, char *data, char *ans){
    int i = 0, j = 0;
    int l = strlen(data);    
    for(int k = 0; k < l; k ++){
        i = (i + 1) % maxn;
        j = (j + s[i]) % maxn;
        swap(s[i], s[j]);
        int t = (s[i] + s[j]) % maxn;
        ans[k] = s[t] ^ data[k];
        //cout<<ans[k];
    }    
    ans[l] = '\0';
}



static QString GetFileMD5(QFile * tempFile)
{
	QString strMD5Value = "";
	do
	{
		if (tempFile == NULL)
		{
			break;
		}
		if (!tempFile->exists())
		{
			return strMD5Value;
		}		
		if (!tempFile->open(QIODevice::ReadOnly)) 
		{
			return strMD5Value;
		}
		

		QByteArray hash = QCryptographicHash::hash(tempFile->readAll(),
										QCryptographicHash::Md5);
		strMD5Value = hash.toHex();//.toUpper();
		
		tempFile->close();
	}while(false);
	
	return strMD5Value;
}





static void swapints(int *array, int ndx1, int ndx2)

{

	int temp = array[ndx1];

	array[ndx1] = array[ndx2];

	array[ndx2] = temp;

}



static int EncDecrypt_RC4(unsigned char * pInData, int iInSize, unsigned char *  pOutData, int iOutSize, int * piOutWritten, 

	char * pszKey)

{

	int iOutRet = 0;



	do{

		int a, b, i=0, j=0, k;              /* Ambiguously named counters   */

		int iKeyLen = 0;	//密钥长度

		int sbox[256];                      /* Encryption array             */

		int key[256];                       /* Numeric key values           */



		if(NULL == pInData || 0 >= iInSize || NULL == piOutWritten)

		{

			iOutRet = -1;

			break;

		}



		if(NULL == pOutData && 0 == iOutSize)	

		{

			//返回转换结果需要的大小

			*piOutWritten = iInSize;

			break;

		}



		if(NULL == pOutData)

		{

			iOutRet = -1;

			break;

		}



		iKeyLen = strlen(pszKey);



		for (a=0; a < 256; a++)

		{

		    key[a] = pszKey[a % iKeyLen];

		    sbox[a] = a;

		}



		for (a=0, b=0; a < 256; a++)

		{

		    b = (b + sbox[a] + key[a]) % 256;

		    swapints(sbox, a, b);

		}



		for (a=0; a < iInSize; a++)

		{

		    i = (i + 1) % 256;

		    j = (j + sbox[i]) % 256;

		    swapints(sbox, i, j);

		    k = sbox[(sbox[i] + sbox[j]) % 256];

		    pOutData[a] = pInData[a] ^ k;

		}



		if(piOutWritten)

		{

			//实际写入的长度

			*piOutWritten = iInSize;

		}

	}while(false);



	return iOutRet;

}

static void  usage(int iArgc, char **ppArgv)
{
    printf("%s:\n",ppArgv[0]);
    printf("-k keyvalue\n");
    printf("-f file\n");
}

int main(int iArgc, char **ppArgv)
{
    QByteArray EncData;
    QByteArray md5Array;
    QByteArray savedArray;
    QString strBase64;
    int iRetBytes=0;
    QString strMd5Value;    
    int iLoop=1;
    char *pKey=ENCDECRYPT_KEY,*pFileName=NULL;
    
    int iByteRet=0;
    if(iArgc<3)
    {
        usage(iArgc,ppArgv);
        return -1;
    }
    while(iLoop<iArgc)
    {
        if(strcmp(ppArgv[iLoop],"-k")==0)
        {
            iLoop++;
            pKey=ppArgv[iLoop];
            iLoop++;
            continue;            
        }
        if(strcmp(ppArgv[iLoop],"-f")==0)
        {
            iLoop++;
            pFileName=ppArgv[iLoop];
            iLoop++;
            continue;            
        }
        iLoop++;
    }
    if(NULL==pFileName)
    {
        usage(iArgc,ppArgv);
        return -1;
    }
    {
        QFile file(pFileName);
        strMd5Value=GetFileMD5(&file);
    }
    md5Array=strMd5Value.toLatin1();
    
    EncData.resize(md5Array.size());
    
    EncDecrypt_RC4((unsigned char *)md5Array.data(),md5Array.size(),(unsigned char *)EncData.data(),EncData.size(),&iRetBytes,pKey);
    
    strBase64=QString(EncData.toBase64());
    savedArray=strBase64.toLatin1();
    printf("%s=\"%s\"\n",pFileName,savedArray.data());
    //qDebug()<<strMd5Value;
    //qDebug()<<strBase64;
    return 0;
}

