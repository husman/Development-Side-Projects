#include "sequence3.h"
#include <iostream>
namespace main_savitch_5 {
// DEFAULT CONSTRUCTOR
sequence::sequence()
// Postcondition: creates an empty sequence object.
{
	head_ptr = tail_ptr = precursor = cursor = NULL;
	many_nodes = 0;
}

// VALUE SEMATICS BIG-3
// COPY CONSTRUCTOR
sequence::sequence(const sequence& source)
// Postcondition: Makes a shallow copy of 'seq' and assigns the copy to
// the current (new object) being instantiated. The newly created copy's
// cursor will be placed at the same location as the object being copied.
{
	if(this == &source)
		return;

	precursor = cursor = NULL;
	many_nodes = 0;

	list_copy(source.head_ptr, head_ptr, tail_ptr);

	// Find precursor and cursor position
	if(source.is_item()) {
			start();
			while(is_item()) {
				if(current() == source.current())
					break;
				advance();
			}
		}
	many_nodes = source.size();
}

// ASSIGNMENT OVERLOADER
void sequence::operator =(const sequence& source)
// Postcondition: Makes a shallow copy of 'seq' and assigns the copy to
// the current (new object) being instantiated. The newly created copy's
// cursor will be placed at the same location as the object being copied.
{
	if(this == &source)
		return;

	list_clear(head_ptr);
	cursor = precursor = NULL;
	many_nodes = 0;
	list_copy(source.head_ptr, head_ptr, tail_ptr);

	// Find precursor and cursor position
	if(source.is_item()) {
		start();
		while(is_item()) {
			if(current() == source.current())
				break;
			advance();
		}
	}

	many_nodes = source.size();
}

// DESTRUCTOR
sequence::~sequence()
// Postcondition: Frees used resources
{
	list_clear(head_ptr);
	head_ptr = tail_ptr = precursor = cursor = NULL;
}

// CONSTANT MEMBER FUNCTIONS
size_t sequence::size() const
// Postcondition: Returns the number of elements in the sequence.
{
	return many_nodes;
}

bool sequence::is_item() const
// Postcondition: Returns true if the current item is an element of the sequence
{
	return cursor != NULL;
}

// MODIFICATION MEMBER FUNCTIONS
void sequence::start()
// Postcondition: The first item on the sequence becomes the current item
// (but if the sequence is empty, then there is no current item).
{
	cursor = head_ptr;
	precursor = NULL;
}

void sequence::advance()
// Precondition: is_item returns true.
// Postcondition: If the current item was already the last item in the
// sequence, then there is no longer any current item. Otherwise, the new
// current item is the item immediately after the original current item.
{
	if(!is_item())
		return;

	precursor = cursor;
	cursor = cursor->link();
}

void sequence::insert(const value_type& entry)
// Postcondition: A new copy of entry has been inserted in the sequence
// before the current item. If there was no current item, then the new entry
// has been inserted at the front of the sequence. In either case, the newly
// inserted item is now the current item of the sequence.
{
	if(size() == 0) {
		cursor = new node(entry);
		head_ptr = tail_ptr = cursor;
	} else if(cursor == tail_ptr) {
		node *tmp_node = new node(entry, head_ptr);
		head_ptr = cursor = tmp_node;
	} else {
		if(precursor == NULL) {
			precursor = new node(entry, cursor);
			head_ptr = cursor = precursor;
			precursor = NULL;
		} else {
			list_insert(precursor, entry);
			cursor = precursor->link();
		}
	}

	if(cursor != NULL && cursor->link() == NULL)
		tail_ptr = cursor->link();

	++many_nodes;
}

void sequence::attach(const value_type& entry)
// Postcondition: A new copy of entry has been inserted in the sequence after
// the current item. If there was no current item, then the new entry has
// been attached to the end of the sequence. In either case, the newly
// inserted item is now the current item of the sequence.
{

	if(size() == 0) {
		cursor = new node(entry, tail_ptr);
		head_ptr = cursor;
		tail_ptr = cursor->link();
	} else if(cursor == tail_ptr) {
		list_insert(precursor, entry);
		cursor = precursor->link();
	} else {
		precursor = cursor;
		list_insert(cursor, entry);
		cursor = cursor->link();
	}

	if(cursor != NULL && cursor->link() == NULL)
		tail_ptr = cursor->link();;

	++many_nodes;
}

void sequence::remove_current()
// Precondition: size() > 0.
// Postcondition: Removes the current item from the sequence.
{
	assert(size() > 0);

	if(cursor == head_ptr) {
		list_head_remove(cursor);
		head_ptr = cursor;
	} else {
		list_remove(precursor);
		cursor = precursor->link();;
	}

	--many_nodes;

}

sequence::value_type sequence::current( ) const
// Precondition: is_item( ) returns true.
// Postcondition: The item returned is the current item in the sequence.
// If the precondition is not met, then the return value is unreliable.
{
	value_type val;

	if(is_item())
		val = cursor->data();

	return val;
}

}

