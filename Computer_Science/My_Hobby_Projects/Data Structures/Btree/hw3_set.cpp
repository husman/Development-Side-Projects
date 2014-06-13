// hw3_set.cpp
//
//CSc 212: Data Structures (Summer 2013)
//   set class for homework 3
//   --> implemented using B-Trees
//   --> adapted from code written during lectures (7/1/03 - 7/3/03)
//   --> extended by Haleeq Usman for FINAL PROJECT.

#include "hw3_set.h"

namespace HW3 {

bool set::insert(const T& item) {
  if (!loose_insert(item)) return false;
  if (entry_count > MAX) {
    set *left = new set;
    set *right = new set;
    
    std::memmove(left->data, data, MIN*sizeof(T));
    std::memmove(left->subset, subset, (MIN+1)*sizeof(set*));
    left->entry_count = MIN;
    left->child_count = is_leaf() ? 0 : (MIN+1);
    
    std::memmove(right->data, &(data[MIN+1]), MIN*sizeof(T));
    std::memmove(right->subset, &(subset[MIN+1]),(MIN+1)*sizeof(set*));
    right->entry_count = MIN;
    right->child_count = is_leaf() ? 0 : (MIN+1);
    
    data[0] = data[MIN];
    entry_count = 1;
    subset[0] = left;
    subset[1] = right;
    child_count = 2;
  }
  ++cardinal;
  return true;  
}


bool set::loose_insert(const T& item) {
  int i=0;
  while (data[i]<item && i<entry_count) i++;
  if ((data[i] == item) && (i < entry_count)) return false;

  if (is_leaf()) {
    std::memmove(&data[i+1], &data[i], (entry_count-i)*sizeof(T));
    data[i] = item;
    ++entry_count;
    return true;
  } else {
    bool success = subset[i]->loose_insert(item);
    fix_excess(i);
    return success;   
  }
}
  
void set::fix_excess(int i) {
  if ((subset[i]->entry_count) <= MAX) return;

  std::memmove(&subset[i+2], &subset[i+1], (child_count-i)*sizeof(set*));
  std::memmove(&data[i+1], &data[i], (entry_count-i)*sizeof(T));

  data[i] = subset[i]->data[MIN];

  set *right = new set;
  std::memmove(right->data, &(subset[i]->data[MIN+1]), MIN*sizeof(T));
  std::memmove(right->subset, &(subset[i]->subset[MIN+1]),(MIN+1)*sizeof(set*));
  right->entry_count = MIN;
  if (subset[i]->is_leaf()) 
    right->child_count = 0;
  else
    right->child_count = MIN+1;

  subset[i]->entry_count = MIN;
  subset[i]->child_count = right->child_count;

  subset[i+1] = right;
  
  entry_count++;
  child_count++;
}


bool set::loose_erase(const T& item) {
	int i=0;
	while (data[i]<item && i<entry_count) i++;
    
    if (is_leaf()) {
        if (i == entry_count) return false;
        if (data[i] != item) return false;
		std::memmove(&data[i], &data[i+1], (entry_count-i)*sizeof(T));
		--entry_count;
		return true;
	} else if (data[i]!=item) {
		bool success = subset[i]->loose_erase(item);
		fix_shortage(i);
		return success;
	} else {
		subset[i]->remove_biggest(data[i]);
		fix_shortage(i);
		return true;
	}
}
		

void set::remove_biggest(T& target) {
	if (is_leaf()) {
		target = data[entry_count-1];
		entry_count--;
	} else {
	    subset[child_count-1]->remove_biggest(target);
        fix_shortage(child_count-1);
    }
}

void set::fix_shortage(int i) {
	
	if ((subset[i]->entry_count) >= MIN) {
		// NO SHORTAGE
		return;

	} else if (i!=0 && subset[i-1]->entry_count>MIN) {
		// LEFT STEAL
		
		std::memmove(&(subset[i]->data[1]),&(subset[i]->data[0]),
					 (subset[i]->entry_count)*sizeof(T));
		std::memmove(&(subset[i]->subset[1]),&(subset[i]->subset[0]),
					 (subset[i]->child_count)*sizeof(set*));
		
		subset[i]->data[0] = data[i-1];
		data[i-1] = subset[i-1]->data[(subset[i-1]->entry_count)-1];
		
		(subset[i]->entry_count)++;
		(subset[i-1]->entry_count)--;
		
		if (!subset[i]->is_leaf()){
			subset[i]->subset[0] = subset[i-1]->subset[subset[i-1]->child_count-1];
			(subset[i]->child_count)++;
			(subset[i-1]->child_count)--;	
		}

	} else if ((i != (child_count-1)) && subset[i+1]->entry_count>MIN){
		// RIGHT STEAL
		
		subset[i]->data[MIN-1] = data[i];
		data[i] = subset[i+1]->data[0];
		
		subset[i]->subset[MIN] = subset[i+1]->subset[0];
		
		(subset[i]->entry_count)++;
		if (!subset[i]->is_leaf()) (subset[i]->child_count)++;		
		
		std::memmove(&(subset[i+1]->data[0]), &(subset[i+1]->data[1]), 
                (--(subset[i+1]->entry_count))*sizeof(T));
		if (!subset[i+1]->is_leaf())
		std::memmove(&(subset[i+1]->subset[0]), &(subset[i+1]->subset[1]), 
                (--(subset[i+1]->child_count))*sizeof(set*));
		

	} else if (i!=0) {
		// LEFT MERGE
		
		subset[i-1]->data[MIN] = data[i-1];
		std::memmove(&(subset[i-1]->data[MIN+1]),&(subset[i]->data[0]),(MIN-1)*sizeof(T));
		subset[i-1]->entry_count = MAX;

		if (!(subset[i-1]->is_leaf())) {
    		std::memmove(&(subset[i-1]->subset[MIN+1]),&(subset[i]->subset[0]),(MIN)*sizeof(set*));
            subset[i-1]->child_count = MAX+1;
        }
		delete subset[i];

		std::memmove(&data[i-1],&data[i],(entry_count-i)*sizeof(T));
		entry_count--;
	    std::memmove(&subset[i],&subset[i+1],(child_count-i-1)*sizeof(set*));
		child_count--;
		
	} else {
		// RIGHT MERGE
		
		subset[i]->data[MIN-1] = data[i];
		std::memmove(&(subset[i]->data[MIN]),&(subset[i+1]->data[0]),MIN*sizeof(T));
		std::memmove(&(subset[i]->subset[MIN]),&(subset[i+1]->subset[0]),(MIN+1)*sizeof(set*));
		subset[i]->entry_count = MAX;
		if (!(subset[i]->is_leaf())) subset[i]->child_count = MAX+1;
		
		delete subset[i+1];
		
		std::memmove(&(data[i]), &(data[i+1]), (entry_count-i-1)*sizeof(T));
		std::memmove(&(subset[i+1]),&(subset[i+2]),(child_count-i)*sizeof(set*));
		child_count--;
		entry_count--;
	}
	
}


bool set::remove(const T& item) {
	if (!loose_erase(item)) return false;
		
	if (entry_count==0 && !is_leaf()) {
		set *tmp = subset[0];
		std::memmove(data, tmp->data, (tmp->entry_count)*sizeof(T));
		std::memmove(subset, tmp->subset, (tmp->child_count)*sizeof(set*));
		entry_count = tmp->entry_count; 
		child_count = tmp->child_count;
		
		delete tmp;
	}

	--cardinal;
	return true;
	
}

void set::print() {
    for (int i = 0; i<entry_count; i++) {
        if (!is_leaf()) subset[i]->print();
        std::cout << data[i] << " ";
    }
    
	if (!is_leaf()) subset[child_count-1]->print();
}

/* SELF IMPLEMENTED FUNCTIONS */
// Postcondition: Linearly searches through the array 'a'
// and recursively through the active set, searching for
// each element of the array 'a' in the active set.
bool set::matchALL(const T a[], size_t N) {
	static bool result = true;

	if(!result)
		return false;

    for (int i = 0; i<entry_count; i++) {
        if (!is_leaf())
        	if(!subset[i]->matchALL(a,N)) {
        		result = false;
        		return false;
        	}

        for(size_t z = 0; z<N; ++z)
        	if((result = search(a[z])) == false)
        		return false;
    }

	if (!is_leaf())
		if(!subset[child_count-1]->matchALL(a,N))
			result = false;

	return result;
}

// Postcondition: Determines if the array 'a' is bijective
// to the active set. This function also tests for
// equality of the array 'a' and the active set.
bool set::bijectiveTo(const T a[], size_t N) {
	// Make sure the number of elements in each matches.
	bool result = cardinality() == N? true : false;

	// if we have a match, then search for all the
	// elements in the array 'a' in the active set.
	if(result)
		for(size_t i=0; i<N && result; ++i)
			result = search(a[i]);

	// if all the elements of array 'a' are found in
	// the active set, then search through all the
	// elements of the active set and see if they
	// all match with the elements in the array 'a'.
	if(result)
		matchALL(a, N);

	return result;
}

bool set::search(const T &item)
// Postcondition: search the current set for 'item'.
{
	int i=0;
	while(data[i]<item && i<entry_count) ++i;
	if((data[i] == item) && (i < entry_count)) return true;

	if(!is_leaf())
		return subset[i]->search(item);

	return false;
}

void set::clearAll()
// Postcondition: removes all the items in the current set.
{
	int i;

	// Call clear all on subsets if current node has subsets.
	if (!is_leaf())
		for(i=0; i<child_count; ++i)
			subset[i]->clearAll();

	// Clear the subset pointers of the current node
	// and set the entry and child counts to zero.
	for(i=0; i<child_count; ++i) {
		delete subset[i];
		subset[i] = NULL;
	}

	child_count = 0;
	entry_count = 0;
	cardinal = 0;
}

void set::insertAll(const set *s)
// Postcondition: inserts all the items from the subset 's' into
// the current (active) set. This form of the function is recursive.
{
	int i;

	// Insert all the data in the current subset's root node.
	for(i = 0; i<s->entry_count; i++)
		insert(s->data[i]);

	// Insert all the data in the current subset's subset nodes.
	for(i = 0; i<s->child_count; i++)
		insertAll(s->subset[i]);
}

void set::insertAll(const set &s)
// Postcondition: inserts all the items from the set 's' into
// the current (active) set. This form of the function is not
// recursive. However, the recursive subset 'insertAll' form is
// utiltized to recusrivly insert all the items in the subsets.
{
	int i;

	// Insert all the data in the root node.
	for(i = 0; i<s.entry_count; i++)
		insert(s.data[i]);

	// Insert all the data in the subset nodes.
	for(i = 0; i<s.child_count; i++)
		insertAll(s.subset[i]);
}

void set::removeAll(const set &s)
// Postcondition: removes all the items from the set 's' from
// the current (active) set. This form of the function is not
// recursive. However, the recursive subset 'removeAll' form is
// utiltized to recusrivly remove all the items found in the subsets.
{
	int i;

	// Insert all the data in the root node.
	for(i = 0; i<s.entry_count; i++)
		remove(s.data[i]);

	// Insert all the data in the subset nodes.
	for(i = 0; i<s.child_count; i++)
		removeAll(s.subset[i]);
}

void set::removeAll(const set *s)
// Postcondition: removes all the items from the subset 's' from
// the current (active) set. This form of the function is recursive.
{
	int i;

	// Insert all the data in the current subset's root node.
	for(i = 0; i<s->entry_count; i++)
		remove(s->data[i]);

	// Insert all the data in the current subset's subset nodes.
	for(i = 0; i<s->child_count; i++)
		removeAll(s->subset[i]);
}

// Postcondition: removes all the items in
// the array 'a' from the current (active) set.
void set::removeAll(const T a[], size_t N) {
	for(size_t i=0; i<N; ++i)
		remove(a[i]);
}

// + operator (UNION)
set set::operator+(const set &s)
// Postcondition: returns the union
// of the active set with the set 's'
{
	set c = *this;

	// self assignment
	if(this == &s)
		return c;

	// Insert all the items from the
	// source node to the active node.
	c.insertAll(s);

	return c;
}

// += operator (UNION)
set& set::operator+=(const set &s)
// Postcondition: assigns a shallow
// copy of s into the current set.
{
	// self assignment
	if(this == &s)
		return *this;

	// Insert all the items from the
	// source node to the active node.
	insertAll(s);

	return *this;
}

// - operator (COMPLIMENT)
set set::operator-(const set &s)
// Postcondition: returns the
// compliment of active set -  's'
{
	set c;

	if(cardinality() < 1 || this == &s)
		return c;

	c = *this;
	// self assignment
	if(s.cardinality() < 1)
		return c;

	// Remove all intersecting parts
	c.removeAll(c*s);

	return c;
}

// - operator (COMPLIMENT)
set& set::operator-=(const set &s)
// Postcondition: returns the
// compliment of active set -  's'
{
	if(this == &s || cardinality() < 1 || s.cardinality() < 1)
		return *this;

	// Remove all intersecting parts
	removeAll(*this*s);

	return *this;
}

// == operator (EQUALITY)
bool set::operator==(const set &s)
// Postcondition: checks to see if
// set 's'  equals the current set.
{
	// self comparison
	if(this == &s)
		return true;

	if(s.cardinality() != cardinality())
		return false;

	set tmp = *this;
	tmp.insertAll(s);
	if(tmp.cardinality() == cardinality())
		return true;

	return false;
}

// * operator (INTERSECTION)
set set::operator*(const set &s)
// Postcondition: returns the intersection
// of the active set with the set 's'.
{
	set c;

	if(s.cardinality() < 1)
		return c;

	// self intersection
	if(this == &s) {
		c = *this;
		return c;
	}

	int i=0;
	for(i=0; i<s.entry_count; ++i)
		if(search(s.data[i]))
			c.insert(s.data[i]);

	for(i=0; i<s.child_count; ++i)
		c += *this*s.subset[i];

	return c;
}

// * operator (SUBSET INTERSECTION)
set set::operator*(const set *s)
// Postcondition: returns the intersection
// of the active set with the subset 's'.
{
	set c;

	// self intersection
	if(this == s) {
		c = *this;
		return c;
	}

	int i=0;
	for(i=0; i<s->entry_count; ++i)
		if(search(s->data[i]))
			c.insert(s->data[i]);

	for(i=0; i<s->child_count; ++i)
		c += c*s->subset[i];

	return c;
}

// VALUE SEMATICS
// Copy Constructor
set::set(const set &s)
// Postcondition: initializes the
// active set with a shallow copy of s.
{
	// self initialization?
	if(this == &s)
		return;

	entry_count=0; child_count=0; cardinal=0;

	// Insert all the items from the
	// source node to the active node.
	insertAll(s);
}

// Assignment operator
void set::operator=(const set &s)
// Postcondition: assigns a shallow
// copy of s into the current set
{
	// self assignment
	if(this == &s)
		return;

	// clear all items in the current set
	clearAll();

	// Insert all the items from the
	// source node to the active node.
	insertAll(s);
}

// Destructor
set::~set()
// Postcondition: releases resources used by current set.
{
	// Clear all items in the current set.
	// This also takes care of setting
	// the subset pointers to NULL.
	clearAll();
}

/* HELPER FUNCTIONS */
// Postcondition: Does a linear search through the array in search
// for 'value'.Returns TRUE if the value was found in the array.
bool array_contains(const set::T a[], size_t N, const set::T& value) {
	for(size_t i=0; i<N; ++i)
		if(a[i] == value)
			return true;

	return false;
}

// Postcondition: prints the content of the array 'a'.
void print_array(const set::T a[], size_t N) {
	std::cout<<"array:";
	for(size_t i=0; i<N; ++i)
		std::cout<<" "<<a[i];
}

// Postcondition: Prints a notice failure with the size
// of the universe set being used. Then exits the program.
void test_failed(int universe_size) {
	std::cout<<"TESTSUITE FAILED TO COMPLETE WITH UNIVERSE SIZE "
			"["<<universe_size<<"]: Failed!\n";
	exit(0);
}
} // end namespace HW3
