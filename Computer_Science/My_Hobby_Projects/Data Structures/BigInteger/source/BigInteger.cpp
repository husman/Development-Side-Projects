/*
 *	BigInteger/source/BigInteger.cpp
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
 *	This file contains the implementation for our simple BigInteger class.
 *
 */

#include "headers/globals.h"

BigInteger::BigInteger() {
	// Nothing to do for now
}

BigInteger::BigInteger(unsigned int value) {
	// We allocate memory for our int_stacks placeholder
	// using each digit of the unsigned integer value.
	get_digits(digits, value);

}

BigInteger::BigInteger(char *value) {
	// We allocate memory for our int_stacks placeholder
	// using the elements of the character array.
	int len, i;
	len = strlen(value);

	for (i = 0; i < len; ++i)
		digits.push_back(value[i]);
}

BigInteger::BigInteger(std::vector<char> &vect) {
	digits = vect;
}

void BigInteger::get_digits(std::vector<char> &vect, unsigned int value) {
	if (value > 9) {
		get_digits(vect, value / 10);
	}
	// Another way I can experiment with for performance boost
	// is to cast the int into a char using 'OR' for its first
	// 8 bits. I am not sure how static_cast works. So its worth
	// the experimentation later. For simplicity, we do this for now.
	vect.push_back(static_cast<char>('0' + (value % 10)));
}

int BigInteger::isGTZero() {
	int return_val;
	return_val = 0;
	if (digits.size() > 1) {
		for (it = digits.begin(); it != digits.end(); ++it) {
			if (*it != '0') {
				return_val = 1;
				break;
			}
		}
	} else {
		if (digits.at(0) != '0')
			return_val = 1;
	}
	return return_val;
}

size_t BigInteger::getLen() {
	return digits.size();
}

int BigInteger::equalTo(const BigInteger &big_int) {
	size_t offset, size;
	int return_val, i;
	offset = 0;
	if (big_int.digits.size() < digits.size()) {
		size = big_int.digits.size();
		for (offset = 0; offset < (digits.size() - big_int.digits.size());
				++offset) {
			if (digits[offset] != '0') {
				return 0;
			}
		}
	} else if (big_int.digits.size() > digits.size()) {
		size = digits.size();
		for (offset = 0; offset < (big_int.digits.size() - digits.size());
				++offset) {
			if (big_int.digits[offset] != '0')
				return 0;
		}
	} else {
		size = big_int.digits.size();
	}

	return_val = 1;
	for (i = 0; i<size; ++i) {
		if (big_int.digits.size() < digits.size()) {
			if (digits[i + offset] != big_int.digits[i]) {
				return_val = 0;
				break;
			}
		} else {
			if (digits[i] != big_int.digits[i + offset]) {
				return_val = 0;
				break;
			}
		}
	}

	return return_val;
}

void BigInteger::toString() {
	for (it = digits.begin(); it != digits.end(); ++it)
		std::cout << *it;
}

unsigned int BigInteger::toUInteger() {
	return (unsigned int) atoi(digits.data());
}

std::ostream &operator<<(std::ostream &output, const BigInteger &big_int) {
	std::vector<char>::const_iterator it;
	for (it = big_int.digits.begin(); it != big_int.digits.end(); ++it)
		output << *it;
	return output;
}

std::istream &operator>>(std::istream &input, BigInteger &big_int) {
	char c;
	while ((c != '\n') && (c != '\r\n')) {
		input.get(c);
		if ((c != '\n') && (c != '\r\n')) {
			big_int.digits.push_back(c);
		}
	}
	return input;
}

BigInteger BigInteger::addVectors(const std::vector<char> &val_digits) {
	std::vector<char> tmp_digits;
	size_t val_size, digits_size, i;
	int tmp_val, remainder, carry;
	val_size = val_digits.size();
	digits_size = digits.size();

	// We transverse in the opposite direction and apply
	// Ordinary base 10 addition.
	carry = 0;
	tmp_val = 0;
	i = 1;
	if (val_size < digits.size()) {
		for (it = digits.end() - 1; it != digits.begin() - 1; --it) {
			tmp_val = val_size >= i ? val_digits.at(val_size - i) - '0' : 0;
			tmp_val += *it - '0' + carry;
			remainder = tmp_val % 10;
			carry = 0;
			if (tmp_val > 9)
				carry = 1;
			tmp_digits.push_back(static_cast<char>('0' + remainder));
			i++;
		}
	} else {
		for (it = val_digits.end() - 1; it != val_digits.begin() - 1; --it) {
			tmp_val = digits_size >= i ? digits.at(digits_size - i) - '0' : 0;
			tmp_val += *it - '0' + carry;
			remainder = tmp_val % 10;
			carry = 0;
			if (tmp_val > 9)
				carry = 1;
			tmp_digits.push_back(static_cast<char>('0' + remainder));
			i++;
		}
	}

	if (carry > 0)
		tmp_digits.push_back(static_cast<char>('0' + carry));

	reverse(tmp_digits.begin(), tmp_digits.end());
	BigInteger return_val(tmp_digits);
	return return_val;
}

BigInteger BigInteger::subtractVectors(const std::vector<char> &val_digits) {
	std::vector<char> tmp_digits;
	size_t val_size, digits_size, i;
	int tmp_val, tmp_digit, carried;

	val_size = val_digits.size();
	digits_size = digits.size();

	// We transverse in the opposite direction and apply
	// Ordinary base 10 addition.
	carried = 0;
	tmp_val = 0;
	tmp_digit = 0;
	i = 1;
	if (val_size <= digits.size()) {
		for (it = digits.end() - 1; it != digits.begin() - 1; --it) {
			tmp_val = val_size >= i ? val_digits.at(val_size - i) - '0' : 0;
			tmp_digit = (*it - '0') - carried;
			if (tmp_digit < tmp_val) {
				carried = 1;
				tmp_val = tmp_digit + 10 - tmp_val;
			} else {
				carried = 0;
				tmp_val = tmp_digit - tmp_val;
			}
			tmp_digits.push_back(static_cast<char>('0' + tmp_val));
			i++;
		}
	} else {
		tmp_digits.push_back('0');
	}

	if (carried == 1) {
		tmp_digits.clear();
		tmp_digits.push_back('0');
	}

	reverse(tmp_digits.begin(), tmp_digits.end());
	BigInteger return_val(tmp_digits);

	return return_val;
}

BigInteger &BigInteger::operator=(unsigned int value) {
	digits.erase(digits.begin(), digits.end());
	get_digits(digits, value);
	return *this;
}

BigInteger &BigInteger::operator=(BigInteger &value) {
	digits = value.digits;
	return *this;
}

/**
 * Implementation for our unsigned integer addition
 */
BigInteger &BigInteger::operator+=(const BigInteger &value) {
	this->digits = operator +(value).digits;
	return *this;
}

BigInteger BigInteger::operator+(unsigned int value) {
	std::vector<char> val_digits;
	get_digits(val_digits, value);
	return addVectors(val_digits);
}

BigInteger BigInteger::operator+(const BigInteger &big_int) {
	BigInteger return_val(addVectors(big_int.digits));
	return return_val;
}

BigInteger BigInteger::operator+(char *value) {
	std::vector<char> val_digits, tmp_digits;
	size_t val_size, digits_size, i;
	int tmp_val, remainder, carry, dummy;
	val_size = strlen(value);
	digits_size = digits.size();

	// We transverse in the opposite direction and apply
	// Ordinary base 10 addition.
	carry = 0;
	tmp_val = 0;
	i = 1;
	if (val_size < digits.size()) {
		for (it = digits.end() - 1; it != digits.begin() - 1; --it) {
			tmp_val = val_size >= i ? value[val_size - i] - '0' : 0;
			tmp_val += *it - '0' + carry;
			remainder = tmp_val % 10;
			carry = 0;
			if (tmp_val > 9)
				carry = 1;
			tmp_digits.push_back(static_cast<char>('0' + remainder));
			i++;
		}
	} else {
		val_size--;
		for (dummy = val_size; dummy >= 0; --dummy) {
			tmp_val = digits_size >= i ? digits.at(digits_size - i) - '0' : 0;
			tmp_val += value[dummy] - '0' + carry;
			remainder = tmp_val % 10;
			carry = 0;
			if (tmp_val > 9)
				carry = 1;
			tmp_digits.push_back(static_cast<char>('0' + remainder));
			i++;
		}
	}

	if (carry > 0)
		tmp_digits.push_back(static_cast<char>('0' + carry));

	reverse(tmp_digits.begin(), tmp_digits.end());
	BigInteger return_val(tmp_digits);

	return return_val;
}

/**
 * Implementation for our unsigned integer subtraction
 */
BigInteger &BigInteger::operator-=(const BigInteger &value) {
	this->digits = operator -(value).digits;
	return *this;
}

BigInteger BigInteger::operator-(unsigned int value) {
	std::vector<char> val_digits;
	get_digits(val_digits, value);
	return subtractVectors(val_digits);
}

BigInteger BigInteger::operator-(const BigInteger &big_int) {
	BigInteger return_val(subtractVectors(big_int.digits));
	return return_val;
}

BigInteger BigInteger::operator-(char *value) {
	std::vector<char> val_digits, tmp_digits;
	size_t val_size, digits_size, i;
	int tmp_val, tmp_digit, carried, dummy;
	val_size = strlen(value);
	digits_size = digits.size();

	// We transverse in the opposite direction and apply
	// Ordinary base 10 addition.
	carried = 0;
	tmp_val = 0;
	tmp_digit = 0;
	i = 1;
	if (val_size <= digits.size()) {
		for (it = digits.end() - 1; it != digits.begin() - 1; --it) {
			tmp_val = val_size >= i ? value[val_size - i] - '0' : 0;
			tmp_digit = (*it - '0') - carried;
			if (tmp_digit < tmp_val) {
				carried = 1;
				tmp_val = tmp_digit + 10 - tmp_val;
			} else {
				carried = 0;
				tmp_val = tmp_digit - tmp_val;
			}
			tmp_digits.push_back(static_cast<char>('0' + tmp_val));
			i++;
		}
	} else {
		tmp_digits.push_back('0');
	}

	if (carried == 1) {
		tmp_digits.clear();
		tmp_digits.push_back('0');
	}

	reverse(tmp_digits.begin(), tmp_digits.end());
	BigInteger return_val(tmp_digits);

	return return_val;
}

/**
 * Implementation for our unsigned integer division
 */

BigInteger &BigInteger::operator/=(const BigInteger &value) {
	this->digits = operator /(value).digits;
	return *this;
}

BigInteger BigInteger::operator/(unsigned int value) {
	BigInteger tmp_val(digits), return_val;

	if (tmp_val.toUInteger() == value) {
		return_val = 1;
		return return_val;
	}

	return_val = 0;
	while (1) {
		tmp_val -= value;
		if (tmp_val.toUInteger() == value)
			return_val += 1;
		if (tmp_val.isGTZero())
			return_val += 1;
		else
			break;
	}

	return return_val;
}

BigInteger BigInteger::operator/(const BigInteger &big_int) {
	BigInteger tmp_val(digits), return_val;

	if (tmp_val.equalTo(big_int)) {
		return_val = 1;
		return return_val;
	}

	return_val = 0;
	while (1) {
		if (tmp_val.equalTo(big_int)) {
			return_val += 1;
		}
		tmp_val -= big_int;
		if (tmp_val.isGTZero())
			return_val += 1;
		else
			break;
	}

	return return_val;
}

BigInteger BigInteger::operator%(unsigned int value) {
	BigInteger tmp_val(digits), remainder_val;
	while (1) {
		if (tmp_val.toUInteger() == value) {
			remainder_val = 0;
		} else {
			remainder_val = tmp_val;
		}
		tmp_val -= value;
		if (!tmp_val.isGTZero())
			break;
	}
	return remainder_val;
}

BigInteger::~BigInteger() {
	// TODO destructor stub
}
