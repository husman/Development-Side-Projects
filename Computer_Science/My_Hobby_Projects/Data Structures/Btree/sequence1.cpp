//============================================================================
// Name        : sequence1.cpp
// Author      : Haleeq Usman
// Description : Programming Assigment 1 implementation file
//============================================================================

#include "sequence1.h"
#include <algorithm>

using namespace std;

namespace main_savitch_3
{
// DEFAULT CONSTRUCTOR
sequence::sequence()
// Postcondition: creates an empty sequence object.
{
	data = new value_type[CAPACITY]; // Allocate dynamic memory for object.
	used = 0; // Set the number of elements in new sequence object to 0.
	current_index = 0; // Set the current_index cursor to the start.
}

// VALUE SEMATICS BIG-3
// COPY CONSTRUCTOR
sequence::sequence(const sequence &seq)
// Postcondition: Makes a shallow copy of 'seq' and assigns the copy to
// the current (new object) being instantiated. The newly created copy's
// cursor will be placed at the same location as the object being copied.
{
	data = new value_type[CAPACITY];
	copy(seq.data, seq.data + seq.used, data);
	used = seq.used;
	current_index = seq.current_index;

}

// ASSIGNMENT OVERLOADER
void sequence::operator =(const sequence &seq)
// Postcondition: Makes a shallow copy of 'seq' and assigns the copy to
// the current (new object) being instantiated. The newly created copy's
// cursor will be placed at the same location as the object being copied.
{
	if(this == &seq)
		return;

	// No need to re-allocate because both data would have the same capacity.
	// Let's copy over the data and set the used and current_index the same.
	copy(seq.data, seq.data + seq.used, data);
	used = seq.used;
	current_index = seq.current_index;

}

// DESTRUCTOR
sequence::~sequence()
// Postcondition: Frees used resources
{
	// Free dynamic memory
	delete[] data;
}

// CONSTANT MEMBER FUNCTIONS
sequence::size_type sequence::size() const
// Postcondition: Returns the number of elements in the sequence.
{
	return used;
}

bool sequence::is_item() const
// Postcondition: Returns true if the current item is an element of the sequence
{
	if(current_index == used)
		return false;

	return true;
}

sequence::value_type sequence::current() const
// Precondition: is_item( ) returns true.
// Postcondition: The item returned is the current item in the sequence.
// If the precondition is not met, then the return value is unreliable.
{
	value_type val;
	if(is_item())
		val = data[current_index];

	return val;
}

// MODIFICATION MEMBER FUNCTIONS
void sequence::start()
// Postcondition: The first item on the sequence becomes the current item
// (but if the sequence is empty, then there is no current item).
{
	if(used > 0)
		current_index = 0;
}

void sequence::advance()
// Precondition: is_item returns true.
// Postcondition: If the current item was already the last item in the
// sequence, then there is no longer any current item. Otherwise, the new
// current item is the item immediately after the original current item.
{
	if(!is_item())
		return;

	++current_index;
}

void sequence::insert(const value_type& entry)
// Precondition: size( ) < CAPACITY.
// Postcondition: A new copy of entry has been inserted in the sequence
// before the current item. If there was no current item, then the new entry
// has been inserted at the front of the sequence. In either case, the newly
// inserted item is now the current item of the sequence.
{
	if(size() >= CAPACITY)
		return;

	if(!is_item())
		current_index = 0;

	if(used > 0) {
		size_type i;
		for(i=used; i>current_index; --i)
			data[i] = data[i-1];
		data[current_index] = entry;
	} else
		data[current_index] = entry;

	++used;
}

void sequence::attach(const value_type& entry)
// Precondition: size( ) < CAPACITY.
// Postcondition: A new copy of entry has been inserted in the sequence after
// the current item. If there was no current item, then the new entry has
// been attached to the end of the sequence. In either case, the newly
// inserted item is now the current item of the sequence.
{
	if(size() >= CAPACITY)
		return;

	if(used > 0) {
		size_type i;
		for(i=used-1; i>current_index; --i)
			data[i+1] = data[i];
		data[i+1] = entry;
		++current_index;
	} else
		data[current_index] = entry;

	++used;
}

void sequence::remove_current()
{
	assert(is_item());

	if(size() > 1) {
		size_type i;
		for(i=current_index; i<used; ++i)
			data[i] = data[i+1];
	}

	--used;
}

}

