#include "include/Client.hpp"

namespace cfa
{
	Client::Client(uint32_t id, char *username, char *firstname,
					char *lastname, struct sockaddr_in &cliaddr) {
		mId = id;
		mUsername = username;
		mFirstname = firstname;
		mLastname = lastname;
		mCliaddr = cliaddr;
	}
}
