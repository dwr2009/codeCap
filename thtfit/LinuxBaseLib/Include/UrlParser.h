#ifndef	_URL_PARSER_H_
#define	_URL_PARSER_H_

#include <BaseTypeDef.h>
#define	URL_FIELD_BUF_SIZE			1024

struct _URL_PARSER
{
	INT8 pszProtocol[16];
	INT8 pszUserName[32];
	INT8 pszPassword[32];
	INT8 pszHostName[128];
	INT8 pszHostPort[8];
	INT8 pszPath[1024];
};

typedef struct _URL_PARSER URL_PARSER;
typedef URL_PARSER * P_URL_PARSER;

enum URL_SCAN_STAT
{
	URL_SCANSTAT_EXPECT_PROTOCOL,
	URL_SCANSTAT_EXPECT_USERNAME,
	URL_SCANSTAT_EXPECT_PASSWORD,
	URL_SCANSTAT_EXPECT_HOSTNAME,
	URL_SCANSTAT_EXPECT_HOSTPORT,
	URL_SCANSTAT_IN_PROTOCOL,
	URL_SCANSTAT_IN_PATH,
	URL_SCANSTAT_IN_USERNAME,
	URL_SCANSTAT_IN_PASSWORD,
	URL_SCANSTAT_IN_HOSTNAME,
	URL_SCANSTAT_IN_HOSTPORT,
};

class CUrlParser
{
public:
	CUrlParser();
	virtual ~CUrlParser();
	
	INT_t UrlParser_ParseUrl(P_URL_PARSER pThis, LPCSTR pszUrl);
};

#endif
