#pragma once

#include <winsock2.h>

enum
{
	MAPPED_ADDRESS = 0x0001,
	RESPONSE_ADDRESS = 0x0002,
	CHANGE_REQUEST = 0x0003,
	SOURCE_ADDRESS = 0x0004,
	CHANGED_ADDRESS = 0x0005,
	USERNAME = 0x0006,
	PASSWORD = 0x0007,
	MESSAGE_INTEGRITY = 0x0008,
	ERROR_CODE = 0x0009,
	UNKNOWN_ATTRIBUTES = 0x000a,
	REFLECTED_FROM = 0x000b
};

typedef unsigned short STUN_ATTRIBUTE_TYPE;

class CStunAttribute
{
protected:
	STUN_ATTRIBUTE_TYPE m_nAttributeType;
	unsigned short m_nAttributeLength;

public:
	CStunAttribute(STUN_ATTRIBUTE_TYPE AttributeType);
	CStunAttribute (char *pBuffer);
	
	virtual ~CStunAttribute(void);

	STUN_ATTRIBUTE_TYPE GetAttributeType ();
	unsigned short GetTotalLength ();

	virtual unsigned short GetAttributeLength ();
	virtual void GetBuffer (char* Buffer) = 0;
	virtual string ToString () = 0;
};
