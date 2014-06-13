/*
 *	BigInteger/source/headers/BigInteger.h
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
 *	This file contains the definition for our simple BigInteger class.
 *
 */

#ifndef BIGINTEGER_H_
#define BIGINTEGER_H_

#include <iostream>

class BigInteger {
	friend std::ostream &operator<<(std::ostream &output, const BigInteger &big_int);
	friend std::istream &operator>>(std::istream &input, BigInteger &big_int);
public:
	// We only add few constructors for our BigInteger to keep
	// things simple.
	BigInteger(void);
	BigInteger(unsigned int value);
	BigInteger(char *value);
	BigInteger(std::vector<char> &vect);

	// When garbage collection is ready, we must
	// deallocate the resources we are using.
	// We make the deconstructor virtual in case of inheritance
	// or deletion of the current object while utilizing polymorphism.
	virtual ~BigInteger();

	// This will return the numeric value as a string.
	void toString(void);

	// This will return the numeric value of our BigInteger.
	unsigned int toUInteger(void);

	/**
	 * Returns 1 if the BigInteger is greater than 1.
	 * Returns 0 otherwise.
	 */
	int isGTZero(void);

	/**
	 * Returns 1 if the two BigIntegers have equal values.
	 */
	int equalTo(const BigInteger &big_int);

	/**
	 * Returns size of vector in our BigInteger.
	 */
	size_t getLen(void);

	// Let's overload a few operators to give the class
	// the feel of an ordinary data type. We overload
	// only a few operators to keep things simple.

	// We take the second argument by a constant reference
	// sometimes so that we do not utilize additional memory
	// creating a second copy of the argument, but we make sure
	// that the value cannot change (const). This makes it act
	// like it was passed byVal, but without the resource
	// hogging associated with byVal.


	// Assignment operator overload definitions.
	/**
	 * Assigns an unsigned integer value to our BigInteger.
	 */
	BigInteger &operator=(unsigned int value);

	/**
	 * Assigns the value of one big integer to the other.
	 */
	BigInteger operator=(const BigInteger &value);

	/**
	 * Assigns a BigInteger value to our BigInteger (reference).
	 */
	BigInteger &operator=(BigInteger &value);

	/**
	 * Adds a BigInteger to our BigInteger object
	 * and sets the result to our BigInteger.
	 */
	BigInteger &operator+=(const BigInteger &value);


	// Addition operator overload definitions.
	/**
	 * Adds an unsigned integer value to our BigInteger object.
	 */
	BigInteger operator+(unsigned int value);

	/**
	 * Adds the string representation of an integer value to our BigInteger object.
	 */
	BigInteger operator+(char *value);

	/**
	 * Adds the value of a BigInteger to our object.
	 */
	BigInteger operator+(const BigInteger &value);


	// Subtraction operator overload definitions.
	/**
	 *  Subtracts an unsigned integer value from our BigInteger object.
	 */
	BigInteger operator-(unsigned int value);

	/**
	 * Subtracts the string representation of an integer value from
	 * our BigInteger object.
	 */
	BigInteger operator-(char *value);

	/**
	 * Subtracts the value of a BigInteger from our object.
	 */
	BigInteger operator-(const BigInteger &value);

	/**
	 * Subtracts a BigInteger from our BigInteger object
	 * and sets the result to our BigInteger.
	 */
	BigInteger &operator-=(const BigInteger &value);


	// Division operator overload definition
	/**
	 * Divides our BigInteger object by the unsigned integer value.
	 */
	BigInteger operator/(unsigned int value);

	/**
	 * Divides our BigInteger object by another BigInteger
	 * and sets the result to our BigInteger.
	 */
	BigInteger &operator/=(const BigInteger &value);

	/**
	 * Divides our BigInteger object by another BigInteger.
	 */
	BigInteger operator/(const BigInteger &big_int);

	/**
	 * Modules our BigInteger object by an unsgined integer.
	 */
	BigInteger operator%(unsigned int value);

private:
	// This will be the vector to hold our digits.
	std::vector<char> digits;
	// Lets store the iterator to our vector.
	std::vector<char>::const_iterator it;

	/**
	 * retrieves the digits in the unsigned int 'value' and places
	 * them into the character vector 'digits'.
	 */
	void get_digits(std::vector<char> &vect, unsigned int value);

	/**
	 * Adds the element of a vector to our BigInteger.
	 */
	BigInteger addVectors(const std::vector<char> &val_digits);

	/**
	 * Subtracts the element of a vector from our BigInteger.
	 */
	BigInteger subtractVectors(const std::vector<char> &val_digits);

};
#endif /* BIGINTEGER_H_ */

