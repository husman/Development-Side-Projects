#include "include/globals.hpp"

namespace cfa
{
	char * getline() {
		char *line = (char *)malloc(100), * linep = line;
		size_t lenmax = 100, len = lenmax;
		int c;

		if(line == NULL)
			return NULL;

		for(;;) {
			c = fgetc(stdin);
			if(c == EOF)
				break;

			if(--len == 0) {
				len = lenmax;
				char *linen = (char *)realloc(linep, lenmax *= 2);

				if(linen == NULL) {
					free(linep);
					return NULL;
				}
				line = linen + (line - linep);
				linep = linen;
			}

			if((*line++ = c) == '\n')
				break;
		}
		*line = '\0';
		return linep;
	}

	void sendPacket(Packet *pkt, struct sockaddr_in &cliaddr) {
		//send the message
		sendto(sockfd,pkt->data,pkt->size,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
	}

	void writePacketHeader(Packet *pkt, uint8_t ver, uint32_t c_id, uint8_t typ, const Session &sess) {
		uint8_t version = ver;
		uint32_t client_id = c_id;
		uint8_t type = typ;
		//uint32_t payload_size = pkt->size - HEADER_LENGTH;

		memcpy(pkt->cursor, &version, 1); ++pkt->cursor;
		memcpy(pkt->cursor, &client_id, 4); pkt->cursor += 4;
		memcpy(pkt->cursor, &type, 1); ++pkt->cursor;
		memcpy(pkt->cursor, sess.getName(), 10); pkt->cursor+=10;
		//memcpy(pkt->cursor, &payload_size, 4); pkt->cursor += 4;
	}

	Packet *newPacket(size_t payload_length, uint8_t ver, uint32_t c_id, uint8_t typ, const Session &sess) {
		Packet *pkt;
		pkt = (Packet *)malloc(sizeof(Packet));

		pkt->size = payload_length + HEADER_LENGTH;
		pkt->data = (char *)malloc(sizeof(char)*pkt->size);
		pkt->cursor = pkt->data;

		writePacketHeader(pkt, ver, c_id, typ, sess);

		return pkt;
	}

	void writeToPacket(Packet *pkt, const void *data, size_t data_length) {
		memcpy(pkt->cursor, data, data_length); pkt->cursor += data_length;
	}

	void test1(char *data, size_t size) {
		printf("test1() called: data = %s\n", data);
	}

	void dumpPacket(Packet *pkt) {
		size_t i;
		printf("\nPacket Dump:\n");
		for(i=0; i<pkt->size; ++i) {
			if(i>0 && i%16==0)
				printf("\n");
			printf("%02x ", pkt->data[i]);
		}
		printf("\n\n");
	}

	void dumpHex(const char *data, size_t size) {
		size_t i;
		printf("\nHex Dump:\n");
		for(i=0; i<size; ++i) {
			if(i>0 && i%16==0)
				printf("\n");
			printf("%02x ", data[i]);
		}
		printf("\n\n");
	}

	int parseClientPacket(const char *data, size_t size, struct sockaddr_in &cliaddr) {
	    if(size <= 0)
	        return PACKET_TYPE_UNKNOWN;

	    uint8_t version;
	    uint8_t type;
	    uint32_t len;
	    Client *client = new Client();
	    char *roomName = (char *)malloc(sizeof(char)*10);

	    client->mCliaddr = cliaddr;

	    version = *data; ++data;
	    getBytes((void *)&client->mId, data, 4); data += 4;
	    type = *data; ++data;
	    getBytes((void *)roomName, data, 10); data += 10;

	    printf("[DEBUG] Packet version = %02x\n", version);
	    switch(type) {
	    case PACKET_TYPE_REGISTER:
	    {
	        printf("[DEBUG] Received PACKET_TYPE_REGISTER data...\n");

	        // username
	        getBytes((void *)&len, data, 4); data += 4;
	        getBytesAllocateString(&client->mUsername, data, len); data += len;

	        // firstname
	        getBytes((void *)&len, data, 4); data += 4;
	        getBytesAllocateString(&client->mFirstname, data, len); data += len;

	        // lastname
	        getBytes((void *)&len, data, 4); data += 4;
	        getBytesAllocateString(&client->mLastname, data, len); data += len;


	        bool foundSession = false;
	        Session *sess;
			for(std::vector<Session *>::iterator it = sessions.begin(); it != sessions.end() ; ++it) {
				printf("[DEBUG] session roomname: %s\n", (*it)->getName());
				if(memcmp((*it)->getName(), roomName, 10) == 0) {
					sess = *it;
					sess->addClient(client);
					printf("added a new client to session\n");
					foundSession = true;
					break;
				}
			}

			if(!foundSession) {
				sess = new cfa::Session(roomName);
				sess->addClient(client);
				sessions.push_back(sess);
				printf("created a new session\n");
			}

	        printf("Got: roomname = %s client id = %u, username = %s, firstname = %s, lastname = %s\n",
	                roomName, client->mId, client->mUsername, client->mFirstname, client->mLastname);
	    }

	        break;
	    case PACKET_TYPE_SESSION_INFO:
	    {
	    	Session *sess = NULL;
	    	for(std::vector<Session *>::iterator it = sessions.begin(); it != sessions.end() ; ++it) {
				printf("[DEBUG] session roomname: %s\n", (*it)->getName());
				if(memcmp((*it)->getName(), roomName, 10) == 0) {
					sess = *it;
					printf("[DEBUG] FOUND SESSION\n");
					break;
				}
			}
	    	printf("[DEBUG] Received PACKET_TYPE_SESSION_INFO data...\n");
	    	if(sess == NULL) {
	    		printf("[DEBUG] Could not find session.\n");
	    		return type;
	    	}
	    	int i = 1;
	    	for(std::vector<Client *>::iterator it = sess->mClients.begin();
	    			it != sess->mClients.end(); ++it) {
	    		printf("[DEBUG] Got: roomname = %s client id = %u | broadcasting packet to client %d\n",
	    		    			sess->getName(), client->mId, i++);
	    		Packet *pkt = newPacket(10, 0, client->mId, 1, *sess);
	    		writeToPacket(pkt, "Test Test", strlen("Test Test"));
	    		sendPacket(pkt, ((*it)->mCliaddr));
	    		//sendto(sockfd,data,size,0,(struct sockaddr *)&((*it)->mCliaddr),sizeof((*it)->mCliaddr));
	    	}
	    	printf("[DEBUG] Packet sent to clients...\n");

	    }
	    break;
	    default:
	        printf("[DEBUG] Unknown PACKET_TYPE\n");
	        break;
	    }

	    return type;
	}

	void getBytes(void *dest, const char *src, size_t size) {
		char *data = (char *)malloc(sizeof(char)*size);
		memcpy(dest, src, size);
		free(data);
	}

	void getBytesAllocate(char **dest, const char *src, size_t size) {
		*dest = (char *)malloc(sizeof(char)*size);
		memcpy(*dest, src, size);
	}

	void getBytesAllocateString(char **dest, const char *src, size_t size) {
		*dest = (char *)malloc(sizeof(char)*(size+1));
		memcpy(*dest, src, size);
		(*dest)[size] = '\0';
	}

}
