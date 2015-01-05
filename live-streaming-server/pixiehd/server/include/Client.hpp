/*
 * Client.hpp
 *
 *  Created on: Aug 24, 2013
 *      Author: haleeq
 */

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>

namespace cfa
{
	class  Client {

		public:
			Client() {
				mId = 0;
				mUsername = NULL;
				mFirstname = NULL;
				mLastname = NULL;
			}

			Client(uint32_t id, char *username, char *firstname,
					char *lastname, struct sockaddr_in &cliaddr);

			bool operator==(Client const& c) {
				if(this == &c)
					return true;

				return mId == c.mId;
			}

		public:
			uint32_t mId;
			char *mUsername;
			char *mFirstname;
			char *mLastname;

			// Network related
			struct sockaddr_in mCliaddr;

	};
}


#endif /* CLIENT_HPP_ */
