/*
 * globals.hpp
 *
 *  Created on: Aug 24, 2013
 *      Author: haleeq
 */

#ifndef GLOBALS_HPP_
#define GLOBALS_HPP_


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

// Our own headers
#include "util.hpp"
#include "Session.hpp"

// Global network socket (Main thread)
extern int sockfd;

// Global session container
extern std::vector<cfa::Session *> sessions;


#endif /* GLOBALS_HPP_ */
