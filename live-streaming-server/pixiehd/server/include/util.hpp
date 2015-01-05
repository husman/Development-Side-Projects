/*
 * util.hpp
 *
 *  Created on: Sep 22, 2013
 *      Author: haleeq
 */

#ifndef UTIL_HPP_
#define UTIL_HPP_

namespace cfa
{
	// statics/constants (persistent)
	static const int HEADER_LENGTH = 16;

	enum {
		PACKET_TYPE_REGISTER,
		PACKET_TYPE_SESSION_INFO,
		PACKET_TYPE_CHECKIN,
		PACKET_TYPE_CREATE_AUDIO_STREAM,
		PACKET_TYPE_UNKNOWN
	};

	// Necessary data structures
	typedef struct {
		char *cursor;
		char *data;
		size_t size;
	} Packet;

	// Function prototypes
	char * getline();
	void sendPacket(Packet *pkt, struct sockaddr_in &cliaddr);
	void writePacketHeader(Packet *pkt, uint8_t ver, uint32_t c_id, uint8_t typ, char *roomName);
	Packet *newPacket(size_t payload_length, uint8_t ver, uint32_t c_id, uint8_t typ);
	void writeToPacket(Packet *pkt, const void *data, size_t data_length);
	void dumpPacket(Packet *pkt);
	void dumpHex(const char *data, size_t size);
	int parseClientPacket(const char *data, size_t size, struct sockaddr_in &cliaddr);
	void getBytes(void *dest, const char *src, size_t size);
	void getBytesAllocate(char **dest, const char *src, size_t size);
	void getBytesAllocateString(char **dest, const char *src, size_t size);

}


#endif /* UTIL_HPP_ */
