#include "include/globals.hpp"

namespace cfa
{
	size_t Session::current_class_id = 0;

	// Constructor
	Session::Session(char *name) {
		mName = name;

		mId = current_class_id++;
	}

	Session::Session(char *name, Client *client) {
		mClients.push_back(client);

		mName = name;

		mId = current_class_id++;
	}

	void Session::addClient(Client *cl) {
		mClients.push_back(cl);
	}

}
