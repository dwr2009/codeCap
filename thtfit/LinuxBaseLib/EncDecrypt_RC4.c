#include	"EncDecrypt_RC4.h"
#include	<BaseTypeDef.h>
#include	<BaseErrDef.h>

static void swapints(int *array, int ndx1, int ndx2);

static void swapints(int *array, int ndx1, int ndx2)
{
	int temp = array[ndx1];
	array[ndx1] = array[ndx2];
	array[ndx2] = temp;
}

INT EncDecrypt_RC4(LPBYTE pInData, INT iInSize, LPBYTE pOutData, INT iOutSize, INT * piOutWritten, 
	const char * pszKey)
{
	INT iOutRet = ERROR_SUCCESS;

	do{
		int a, b, i=0, j=0, k;              /* Ambiguously named counters   */
		int iKeyLen = 0;	//密钥长度
		int sbox[256];                      /* Encryption array             */
		int key[256];                       /* Numeric key values           */

		if(NULL == pInData || 0 >= iInSize || NULL == piOutWritten)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
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
			iOutRet = ERROR_INVALID_PARAMETER;
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
	}while(FALSE);

	return iOutRet;
}

