#include "StdAfx.h"
#include "StunUsernameAttribute.h"

CStunUsernameAttribute::CStunUsernameAttribute(const char *pszUsername):
	CStunAttribute(USERNAME)
{
	m_nAttributeLength = strlen (pszUsername);
	m_pszUsername = new char [m_nAttributeLength + 1];
	strncpy_s (m_pszUsername, m_nAttributeLength + 1, pszUsername, m_nAttributeLength);
}

CStunUsernameAttribute::CStunUsernameAttribute(char *pBuffer, int nDummy):
	CStunAttribute(pBuffer)
{
	char *ptrToBuffer = pBuffer + 4;

	m_pszUsername = new char [m_nAttributeLength];
	memcpy_s (m_pszUsername, m_nAttributeLength, pBuffer, m_nAttributeLength);
}

CStunUsernameAttribute::~CStunUsernameAttribute(void)
{
	delete []m_pszUsername;
}

void CStunUsernameAttribute::GetBuffer (char *pBuffer)
{
	unsigned short nAttributeType = htons (m_nAttributeType);
	unsigned short nAttributeLength = htons (GetAttributeLength ());

	int nOffset = 0, nSize = GetTotalLength ();

	memcpy_s (pBuffer, nSize, &nAttributeType, sizeof (nAttributeType));
	nOffset += sizeof (nAttributeType);

	memcpy_s (pBuffer + nOffset, nSize - nOffset, &nAttributeLength, sizeof (nAttributeLength));
	nOffset += sizeof (nAttributeLength);

	memcpy_s (pBuffer + nOffset, nSize - nOffset, m_pszUsername, _tcslen (m_pszUsername));
}

const char *CStunUsernameAttribute::GetUsername ()
{
	return m_pszUsername;
}

string CStunUsernameAttribute::ToString()
{
	string strUsernameAttribute = string ("Username attribute: Username = ") + m_pszUsername;
	return strUsernameAttribute;
}