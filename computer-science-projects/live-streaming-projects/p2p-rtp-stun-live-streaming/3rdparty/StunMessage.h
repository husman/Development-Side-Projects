#pragma once

#include "StunHeader.h"
#include "StunAddressAttribute.h"
#include "StunChangeRequestAttribute.h"
#include "StunErrorCodeAttribute.h"
#include "StunUnknownAttributes.h"
#include "StunUsernameAttribute.h"
#include "StunPasswordAttribute.h"

#define MAX_ATTRIBUTES 12

class CStunMessage
{
public:
	CStunMessage (STUN_MESSAGE_TYPE MessageType);
	virtual ~CStunMessage (void) = 0;
	CStunMessage (char *pBuffer);
	
	unsigned short GetTotalLength ();
	bool IsMessageValid ();
	STUN_MESSAGE_TYPE GetMessageType ();

	virtual char *GetBuffer ();

	void RemoveAttribute(STUN_ATTRIBUTE_TYPE nAttributeType);
	CStunAttribute *GetAttribute (STUN_ATTRIBUTE_TYPE nAttributeType);

	bool HasAttribute (STUN_ATTRIBUTE_TYPE nAttributeType);
	string ToString ();

protected:
	void AddAttribute(CStunAttribute *pStunAttribute);

private:
	bool m_bValidMessage;
	CStunHeader m_StunHeader;
	CStunAttribute *m_pArrAttributes [MAX_ATTRIBUTES];
};
