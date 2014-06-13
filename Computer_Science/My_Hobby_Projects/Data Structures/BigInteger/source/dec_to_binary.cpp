/*
 *	BigInteger/source/dec_to_binary.cpp
 *
 *	Date:			Sep. 9, 2012
 *	Author:			Haleeq Usman
 *	Dev. IDE:		Eclipse IDE for C/C++ Developers
 *		 			Version: Indigo Service Release 2
 *		 			Build id: 20120216-1857
 *	Dev. OS:		Windows 7 Home Premium SP1 (64-bitS)
 *	Dev. System:	Dell XPS 8300, 8GB RAM Intel(R) Core(TM)
 *					i7-2600 CPU @ 3.40Ghz (quad core at 3.40Ghz)
 *
 *	This file contains the implemenation for the decimal to binary
 *	converter.
 *
 */

#include "headers/globals.h"

void dec_to_binary32() {
	time_t start_time;
	double elapsed_time;
	BigInteger input;
	std::vector<char> output;
	std::vector<char>::const_iterator it;
	int i;

	std::cout << "Please enter a whole number (any length): ";
	std::cin >> input;

	std::cout << "Your number, " << input << ", in binary is:\n";

	start_time = time(NULL);

	while (1) {
		if((input % 2).isGTZero())
			output.push_back('1');
		else
			output.push_back('0');
		input /= 2;
		if(!input.isGTZero()) {
			break;
		}
	}

	size_t num_bits = 32;
	if(output.size() < num_bits) {
		num_bits -= output.size();
		for(i=0; i<num_bits; ++i) {
			output.push_back('0');
		}
	}
	reverse(output.begin(), output.end());
	i = 0;
	for(it = output.begin(); it != output.end(); ++it) {
		++i;
		std::cout << *it;
		if(i % 4 == 0)
			printf(" ");
	}
	printf("\n");

	// Let's measure how long it took to hit the end of the function
	elapsed_time = difftime(time(NULL), start_time);
	printf(
			"The function sum_ints_fast() took ~%.2lf seconds --> ~%.2lf minutes\n",
			elapsed_time, elapsed_time / 60);
}
