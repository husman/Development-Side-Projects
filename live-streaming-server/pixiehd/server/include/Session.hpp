/*
 * Session.hpp
 *
 *  Created on: Aug 24, 2013
 *      Author: haleeq
 */

#ifndef SESSION_HPP_
#define SESSION_HPP_

#include "Client.hpp"

namespace cfa
{
	class  Session {

	public:
		Session(char *name);
		Session(char *name, Client *client);


		bool operator==(Session const& s) {
			if(this == &s)
				return true;

			return memcmp(mName, s.mName, 10) == 0;
		}

		char *getName() const {
			return mName;
		}

		void addClient(Client *cl);

	public:
		int mId;
		char *mName;
		std::vector<Client *> mClients;

		// Statics
		static size_t current_class_id;
	};

}
#endif /* SESSION_HPP_ */
