#pragma once
#include "stunattribute.h"

#define MAX_UNKNOWN_ATTRIBUTES 16

class CStunUnknownAttributes :
	public CStunAttribute
{
private:
	int m_nAttributesCount;
	unsigned short m_UnknownAttributes [MAX_UNKNOWN_ATTRIBUTES];

public:
	CStunUnknownAttributes(void);
	CStunUnknownAttributes(char *pBuffer);

	~CStunUnknownAttributes(void);

	void GetBuffer (char *pBuffer);
	bool AddAttribute (STUN_ATTRIBUTE_TYPE AttributeType);

	int GetUnknownAttributesCount(void);
	unsigned short *GetUnknownAttributes(void);
	string ToString ();
};
