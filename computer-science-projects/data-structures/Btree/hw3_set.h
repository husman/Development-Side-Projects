// hw3_set.h
//
//CSc 212: Data Structures (Summer 2013)
//   set class for homework 3
//   --> implemented using B-Trees
//   --> adapted from code written during lectures (7/1/03 - 7/3/03)
//   --> extended by Haleeq Usman for FINAL PROJECT.

#include <cstring>
#include <iostream>
#include <cstdlib>
#include <ctime>

namespace HW3 {

class set {
    public:
        typedef int T;
        set() {entry_count=0; child_count=0; cardinal=0;}
    
        bool insert(const T&);
        bool remove(const T&);
        void print();

        // Postcondition: Linearly searches through the array 'a'
        // and recursively through the active set, searching for
        // each element of the array 'a' in the active set.
        bool matchALL(const T a[], size_t N);

        // Postcondition: Determines if the array 'a' is bijective
        // to the active set. This function also tests for
        // equality of the array 'a' and the active set.
        bool bijectiveTo(const T a[], size_t N);

        /* SELF IMPLEMENTED FUNCTIONS */
        // Postcondition: search the current set for 'item'.
        bool search(const T &item);

        // Postcondition: returns the number
        // of elements in the active set.
        size_t cardinality() const {return cardinal;};

        // + operator (UNION)
		// Postcondition: returns the union
        // of the active set with the set 's'
		set operator+(const set &s);

		// += operator (UNION)
		// Postcondition: unions a shallow
		// copy of s into the current set.
		set& operator+=(const set &s);

		// - operator (COMPLIMENT)
		// Postcondition: returns the
		// compliment of active set -  's'
		set operator-(const set &s);

		// -= operator (COMPLIMENT)
		// Postcondition: returns the
		// compliment of active set -  's'
		set& operator-=(const set &s);

		// == operator (EQUALITY)
		// Postcondition: checks to see if
		// set 's'  equals the current set.
		bool operator==(const set &s);

		// * operator (INTERSECTION)
		// Postcondition: returns the intersection
		// of the active set with the set 's'.
		set operator*(const set &s);

		// * operator (SUBSET INTERSECTION)
		// Postcondition: returns the intersection
		// of the active set with the subset 's'.
		set operator*(const set *s);

        // VALUE SEMATICS
		// Copy Constructor
		// Postcondition: initializes the
		// active set with a shallow copy of s.
		set(const set &s);

        // Assignment operator
        // Postcondition: assigns a shallow
        // copy of s into the current set.
        void operator=(const set &s);

        // Destructor
        // Postcondition: releases resources used by current set.
        ~set();

        // Postcondition: removes all the items in
		// the array 'a' from the current (active) set.
		void removeAll(const T a[], size_t N);

    private:
        static const int MIN = 3;
        static const int MAX = 2*MIN;
        T data[MAX+1];
        int entry_count;
        set* subset[MAX+2];
        int child_count;
        int cardinal;

        bool is_leaf() {return (child_count == 0);}
        bool loose_insert(const T&);
        void fix_excess(int);
        bool loose_erase(const T&); 
        void remove_biggest(T&);
        void fix_shortage(int);

        // Postcondition: removes all the items in the current set.
        void clearAll(void);

        // Postcondition: inserts all the items from the set 's' into
        // the current (active) set. This form of the function is not
        // recursive. However, the recursive subset 'insertAll' form is
        // utiltized to recusrivly insert all the items in the subsets.
        void insertAll(const set &s);

        // Postcondition: inserts all the items from the subset 's' into
        // the current (active) set. This form of the function is recursive.
        void insertAll(const set *s);

        // Postcondition: removes all the items from the set 's' from
		// the current (active) set. This form of the function is not
		// recursive. However, the recursive subset 'removeAll' form is
		// utiltized to recusrivly remove all the items found in the subsets.
		void removeAll(const set &s);

		// Postcondition: removes all the items from the subset 's' from
		// the current (active) set. This form of the function is recursive.
		void removeAll(const set *s);
};

/* HELPER FUNCTIONS */
// Postcondition: Does a linear search through the array in search
// for 'value'.Returns TRUE if the value was found in the array.
bool array_contains(const set::T a[], size_t N, const set::T& value);

// Postcondition: prints the content of the array 'a'.
void print_array(const set::T a[], size_t N);

// Postcondition: Prints a notice failure with the size
// of the universe set being used. Then exits the program.
void test_failed(int universe_size);

} // end namespace HW3
