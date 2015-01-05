
// FILE: seq_ex3.cpp
// Written by: Michael Main (main@colorado.edu) - Oct 31, 1997
// Non-interactive test program for the sequence class using a linked list
// with improved test for heap leaks.
//
// DESCRIPTION:
// Each function of this program tests part of the sequence class, returning
// some number of points to indicate how much of the test was passed.
// A description and result of each test is printed to cout.
// Maximum number of points awarded by this program is determined by the
// constants POINTS[1], POINTS[2]...
//
// WHAT'S NEW:
// This new version of the test program includes an improved test for heap
// leaks by overriding the new and delete operators. Users should consider
// placing these new and delete operators into a separate cpp file, but
// I have included everything in one file for easier distribution.


#include <iostream>     // Provides cout and cin
#include <cstdlib>      // Provides EXIT_SUCCESS
#include <string>   // Provides memcpy.
#include "sequence3.h"    // Provides the sequence class with double items.
using namespace std;
using namespace main_savitch_5;

// Descriptions and points for each of the tests:
const sequence::size_type MANY_TESTS = 7;
const int YOURPOINTS[MANY_TESTS+1] = {
    70,  // Total points for all tests.
     14,  // Test 1 points
     14,  // Test 2 points
     14,  // Test 3 points
     7,  // Test 4 points
     7,  // Test 5 points
     7,  // Test 6 points
     7   // Test 7 points
};
const char DESCRIPTION[MANY_TESTS+1][256] = {
    "tests for sequence Class with a linked list",
    "Testing insert, attach, and the constant member functions",
    "Testing situations where the cursor goes off the list",
    "Testing remove_current",
    "Testing the copy constructor",
    "Testing the assignment operator",
    "Testing insert/attach for somewhat larger lists",
    "Testing for possible heap leaks"
};



// **************************************************************************
// Replacements for new and delete:
// The next two functions replace the new and delete operators. Any code
// that is linked with this .cpp file will use these replacements instead
// of the standard new and delete. The replacements provide three features:
// 1. The global variable memory_used_now keeps track of how much memory has
//    been allocated by calls to new. (The global variable is static so that
//    it cannot be accessed outside of this .cpp file.)
// 2. The new operator fills all newly allocated memory with a GARBAGE char.
// 3. Each block of newly allocated memory is preceeded and followed by a
//    border of BORDER_SIZE characters. The first part of the front border
//    contains a copy of the size of the allocated memory. The rest of the
//    border contains a BORDER char.
// During any delete operation, the border characters are checked. If any
// border character has been changed from BORDER to something else, then an
// error message is printed and the program is halted. This stops most
// cases of writing beyond the ends of the allocated memory.
// **************************************************************************

const  sequence::size_type BORDER_SIZE     = 2*sizeof(double);
const  char   GARBAGE         = 'g';
const  char   BORDER          = 'b';
static sequence::size_type memory_used_now = 0;

void* operator new(sequence::size_type size)
{
    char   *whole_block;   // Pointer to the entire block that we get from heap
    sequence::size_type *size_spot;     // Spot in the block where to store a copy of size
    char   *front_border;  // The border bytes in front of the user's memory
    char   *middle;        // The memory to be given to the calling program
    char   *back_border;   // The border bytes at the back of the user's memory
    sequence::size_type i;              // Loop control variable

    // Allocate the block of memory for the user and for the two borders.
    whole_block = (char *) malloc(2*BORDER_SIZE + size);
    if (whole_block == NULL)
    {
        cout << "Insufficient memory for a call to the new operator." << endl;
        exit(0);
    }

    // Figure out the start points of the various pieces of the block.
    size_spot = (sequence::size_type *) whole_block;
    front_border = (char *) (whole_block + sizeof(sequence::size_type));
    middle = (char *) (whole_block + BORDER_SIZE);
    back_border = middle + size;

    // Put a copy of the size at the start of the block.
    *size_spot = size;

    // Fill the borders and the middle section.
    for (i = 0; i < BORDER_SIZE - sizeof(sequence::size_type); i++)
        front_border[i] = BORDER;
    for (i = 0; i < size; i++)
        middle[i] = GARBAGE;
    for (i = 0; i < BORDER_SIZE; i++)
        back_border[i] = BORDER;

    // Update the global static variable showing how much memory is now used.
    memory_used_now += size;

    return middle;
}

void operator delete(void* p)
{
    char   *whole_block;   // Pointer to the entire block that we get from heap
    sequence::size_type *size_spot;     // Spot in the block where to store a copy of size
    char   *front_border;  // The border bytes in front of the user's memory
    char   *middle;        // The memory to be given to the calling program
    char   *back_border;   // The border bytes at the back of the user's memory
    sequence::size_type i;              // Loop control variable
    sequence::size_type size;           // Size of the block being returned
    bool   corrupt;        // Set to true if the border was corrupted

    // Figure out the start of the pieces of the block, and the size.
    whole_block = ((char *) (p)) - BORDER_SIZE;
    size_spot = (sequence::size_type *) whole_block;
    size = *size_spot;
    front_border = (char *) (whole_block + sizeof(sequence::size_type));
    middle = (char *) (whole_block + BORDER_SIZE);
    back_border = middle + size;

    // Check the borders for the BORDER character.
    corrupt = false;
    for (i = 0; i < BORDER_SIZE - sizeof(sequence::size_type); i++)
        if (front_border[i] != BORDER)
            corrupt = true;
    for (i = 0; i < BORDER_SIZE; i++)
        if (back_border[i] != BORDER)
            corrupt = true;

    if (corrupt)
    {
        cout << "The delete operator has detected that the program wrote\n";
        cout << "beyond the ends of a block of memory that was allocated\n";
        cout << "by the new operator. Program will be halted." << endl;
        exit(0);
    }
    else
    {
        // Fill memory with garbage in case program tries to use it
        // even after the delete.
        for (i = 0; i < size + 2*BORDER_SIZE; i++)
            whole_block[i] = GARBAGE;
        free(whole_block);
        memory_used_now -= size;
    }

}


// **************************************************************************
// bool test_basic(const sequence& test, sequence::size_type s, bool has_cursor)
//   Postcondition: A return value of true indicates:
//     a. test.size() is s, and
//     b. test.is_item() is has_cursor.
//   Otherwise the return value is false.
//   In either case, a description of the test result is printed to cout.
// **************************************************************************
bool test_basic(const sequence& test, sequence::size_type s, bool has_cursor)
{
    bool answer;

    cout << "Testing that size() returns " << s << " ... ";
    cout.flush( );
    answer = (test.size( ) == s);
    cout << (answer ? "Passed." : "Failed.") << endl;

    if (answer)
    {
        cout << "Testing that is_item() returns ";
        cout << (has_cursor ? "true" : "false") << " ... ";
        cout.flush( );
        answer = (test.is_item( ) == has_cursor);
        cout << (answer ? "Passed." : "Failed.") << endl;
    }

    return answer;
}


// **************************************************************************
// bool test_items(sequence& test, sequence::size_type s, sequence::size_type i, double items[])
//   The function determines if the test list has the correct items
//   Precondition: The size of the items array is at least s.
//   Postcondition: A return value of true indicates that test.current()
//   is equal to items[i], and after test.advance() the result of
//   test.current() is items[i+1], and so on through items[s-1].
//   At this point, one more advance takes the cursor off the list.
//   If any of this fails, the return value is false.
//   NOTE: The test list has been changed by advancing its cursor.
// **************************************************************************
bool test_items(sequence& test, sequence::size_type s, sequence::size_type i, double items[])
{
    bool answer = true;

    cout << "The cursor should be at item [" << i << "]" << " of the sequence\n";
    cout << "(counting the first item as [0]). I will advance the cursor\n";
    cout << "to the end of the sequence, checking that each item is correct...";
    cout.flush( );
    while ((i < s) && test.is_item( ) && (test.current( ) == items[i]))
    {
        i++;
        test.advance( );
    }

    if ((i != s) && !test.is_item( ))
    {   // The test.is_item( ) function returns false too soon.
        cout << "\n    Cursor fell off the list too soon." << endl;
        answer = false;
    }
    else if (i != s)
    {   // The test.current( ) function returned a wrong value.
        cout << "\n    The item [" << i << "] should be " << items[i] << ",\n";
        cout << "    but it was " << test.current( ) << " instead.\n";
        answer = false;
    }
    else if (test.is_item( ))
    {   // The test.is_item( ) function returns true after moving off the sequence.
        cout << "\n    The cursor was moved off the list,";
        cout << " but is_item still returns true." << endl;
        answer = false;
    }

    cout << (answer ? "Passed." : "Failed.") << endl;
    return answer;
}


// **************************************************************************
// bool correct(sequence& test, sequence::size_type s, sequence::size_type cursor_spot, double items[])
//   This function determines if the sequence (test) is "correct" according to
//   these requirements:
//   a. it has exactly s items.
//   b. the items (starting at the front) are equal to
//      items[0] ... items[s-1]
//   c. if cursor_spot < s, then test's cursor must be at
//      the location given by cursor_spot.
//   d. if cursor_spot >= s, then test must not have a cursor.
// NOTE: The function also moves the cursor off the sequence.
// **************************************************************************
bool correct(sequence& test, sequence::size_type size, sequence::size_type cursor_spot, double items[])
{
    bool has_cursor = (cursor_spot < size);

    // Check the sequence's size and whether it has a cursor.
    if (!test_basic(test, size, has_cursor))
    {
        cout << "Basic test of size() or is_item() failed." << endl << endl;
        return false;
    }

    // If there is a cursor, check the items from cursor to end of the list.
    if (has_cursor && !test_items(test, size, cursor_spot, items))
    {
        cout << "Test of the list's items failed." << endl << endl;
        return false;
    }

    // Restart the cursor at the front of the list and test items again.
    cout << "I'll call start() and look at the items one more time..." << endl;
    test.start( );
    if (has_cursor && !test_items(test, size, 0, items))
    {
        cout << "Test of the list's items failed." << endl << endl;
        return false;
    }

    // If the code reaches here, then all tests have been passed.
    cout << "All tests passed for this list." << endl << endl;
    return true;
}


// **************************************************************************
// int test1( )
//   Performs some basic tests of insert, attach, and the constant member
//   functions. Returns YOURPOINTS[1] if the tests are passed. Otherwise returns 0.
// **************************************************************************
int test1( )
{
    sequence empty;                            // An empty list
    sequence test;                             // A list to add items to
    double items1[4] = { 5, 10, 20, 30 };  // These 4 items are put in a list
    double items2[4] = { 10, 15, 20, 30 }; // These are put in another list

    // Test that the empty list is really empty
    cout << "Starting with an empty sequence." << endl;
    if (!correct(empty, 0, 0, items1)) return 0;

    // Test the attach function to add something to an empty list
    cout << "I am now using attach to put 10 into an empty sequence." << endl;
    test.attach(10);
    if (!correct(test, 1, 0, items2)) return 0;

    // Test the insert function to add something to an empty list
    cout << "I am now using insert to put 10 into an empty sequence." << endl;
    test = empty;
    test.insert(10);
    if (!correct(test, 1, 0, items2)) return 0;

    // Test the insert function to add an item at the front of a list
    cout << "I am now using attach to put 10,20,30 in an empty sequence.\n";
    cout << "Then I move the cursor to the start and insert 5." << endl;
    test = empty;
    test.attach(10);
    test.attach(20);
    test.attach(30);
    test.start( );
    test.insert(5);
    if (!correct(test, 4, 0, items1)) return 0;

    // Test the insert function to add an item in the middle of a list
    cout << "I am now using attach to put 10,20,30 in an empty sequence.\n";
    cout << "Then I move the cursor to the start, advance once, ";
    cout << "and insert 15." << endl;
    test = empty;
    test.attach(10);
    test.attach(20);
    test.attach(30);
    test.start( );
    test.advance( );
    test.insert(15);
    if (!correct(test, 4, 1, items2)) return 0;

    // Test the attach function to add an item in the middle of a list
    cout << "I am now using attach to put 10,20,30 in an empty sequence.\n";
    cout << "Then I move the cursor to the start and attach 15 ";
    cout << "after the 10." << endl;
    test = empty;
    test.attach(10);
    test.attach(20);
    test.attach(30);
    test.start( );
    test.attach(15);
    if (!correct(test, 4, 1, items2)) return 0;

    // All tests have been passed
    cout << "All tests of this first function have been passed." << endl;
    return YOURPOINTS[1];
}


// **************************************************************************
// int test2( )
//   Performs a test to ensure that the cursor can correctly be run off the end
//   of the list. Also tests that attach/insert work correctly when there is
//   no cursor. Returns YOURPOINTS[2] if the tests are passed. Otherwise returns 0.
// **************************************************************************
int test2( )
{
    const sequence::size_type TESTSIZE = 30;
    sequence test;
    sequence::size_type i;

    // Put three items in the list
    cout << "Using attach to put 20 and 30 in the sequence, and then calling\n";
    cout << "advance, so that is_item should return false ... ";
    cout.flush( );
    test.attach(20);
    test.attach(30);
    test.advance( );
    if (test.is_item( ))
    {
        cout << "failed." << endl;
        return 0;
    }
    cout << "passed." << endl;

    // Insert 10 at the front and run the cursor off the end again
    cout << "Inserting 10, which should go at the sequence's front." << endl;
    cout << "Then calling advance three times to run cursor off the list ...";
    cout.flush( );
    test.insert(10);
    test.advance( ); // advance to the 20
    test.advance( ); // advance to the 30
    test.advance( ); // advance right off the list
    if (test.is_item( ))
    {
        cout << " failed." << endl;
        return false;
    }
    cout << " passed." << endl;

    // Attach more items until the list becomes full.
    // Note that the first attach should attach to the end of the list.
    cout << "Calling attach to put the numbers 40, 50, 60 ...";
    cout << TESTSIZE*10 << " at the sequence's end." << endl;
    for (i = 4; i <= TESTSIZE; i++)
        test.attach(i*10);

    // Test that the list is correctly filled.
    cout << "Now I will test that the list has 10, 20, 30, ...";
    cout << TESTSIZE*10 << "." << endl;
    test.start( );
    for (i = 1; i <= TESTSIZE; i++)
    {
        if ((!test.is_item( )) || test.current( ) != i*10)
        {
            cout << "    Test failed to find " << i*10 << endl;
            return 0;
        }
        test.advance( );
    }
    if (test.is_item( ))
    {
        cout << "    There are too many items on the sequence." << endl;
        return false;
    }

    // All tests passed
    cout << "All tests of this second function have been passed." << endl;
    return YOURPOINTS[2];
}


// **************************************************************************
// int test3( )
//   Performs basic tests for the remove_current function.
//   Returns YOURPOINTS[3] if the tests are passed.
//   Otherwise returns 0.
// **************************************************************************
int test3( )
{
    const sequence::size_type TESTSIZE = 30;

    sequence test;

    // Within this function, I create several different sequences using the
    // items in these arrays:
    double items1[1] = { 30 };
    double items2[2] = { 10, 30 };
    double items3[3] = { 10, 20, 30 };

    sequence::size_type i;       // for-loop control variable

    // Build a list with three items 10, 20, 30, and remove the middle,
    // and last and then first.
    cout << "Using attach to build a list with 10,30." << endl;
    test.attach(10);
    test.attach(30);
    cout << "Insert a 20 before the 30, so entire sequence is 10,20,30." << endl;
    test.insert(20);
    if (!correct(test, 3, 1, items3)) return 0;
    cout << "Remove the 20, so entire sequence is now 10,30." << endl;
    test.start( );
    test.advance( );
    test.remove_current( );
    if (!correct(test, 2, 1, items2)) return 0;
    cout << "Remove the 30, so entire sequence is now just 10 with no cursor.";
    cout << endl;
    test.start( );
    test.advance( );
    test.remove_current( );
    if (!correct(test, 1, 1, items2)) return 0;
    cout << "Set the cursor to the start and remove the 10." << endl;
    test.start( );
    test.remove_current( );
    if (!correct(test, 0, 0, items2)) return 0;

    // Build a list with three items 10, 20, 30, and remove the middle,
    // and then first and then last.
    cout << "Using attach to build another list with 10,30." << endl;
    test.attach(10);
    test.attach(30);
    cout << "Insert a 20 before the 30, so entire sequence is 10,20,30." << endl;
    test.insert(20);
    if (!correct(test, 3, 1, items3)) return 0;
    cout << "Remove the 20, so entire sequence is now 10,30." << endl;
    test.start( );
    test.advance( );
    test.remove_current( );
    if (!correct(test, 2, 1, items2)) return 0;
    cout << "Set the cursor to the start and remove the 10," << endl;
    cout << "so the sequence should now contain just 30." << endl;
    test.start( );
    test.remove_current( );
    if (!correct(test, 1, 0, items1)) return 0;
    cout << "Remove the 30 from the sequence, resulting in an empty sequence." << endl;
    test.start( );
    test.remove_current( );
    if (!correct(test, 0, 0, items1)) return 0;

    // Build a list with three items 10, 20, 30, and remove the first.
    cout << "Build a new sequence by inserting 30, 10, 20 (so the sequence\n";
    cout << "is 20, then 10, then 30). Then remove the 20." << endl;
    test.insert(30);
    test.insert(10);
    test.insert(20);
    test.remove_current( );
    if (!correct(test, 2, 0, items2)) return 0;
    test.start( );
    test.remove_current( );
    test.remove_current( );

    // Just for fun, fill up the sequence, and empty it!
    cout << "Just for fun, I'll empty the sequence then fill it up, then\n";
    cout << "empty it again. During this process, I'll try to determine\n";
    cout << "whether any of the sequence's member functions access the\n";
    cout << "array outside of its legal indexes." << endl;
    for (i = 0; i < TESTSIZE; i++)
        test.insert(0);
    for (i = 0; i < TESTSIZE; i++)
        test.remove_current( );

    // All tests passed
    cout << "All tests of this third function have been passed." << endl;
    return YOURPOINTS[3];
}


// **************************************************************************
// int test4( )
//   Performs some tests of the copy constructor.
//   Returns YOURPOINTS[4] if the tests are passed. Otherwise returns 0.
// **************************************************************************
int test4( )
{
    const sequence::size_type TESTSIZE = 30;
    sequence original; // A list that we'll copy.
    double items[2*TESTSIZE];
    sequence::size_type i;

    // Set up the items array to conatin 1...2*TESTSIZE.
    for (i = 1; i <= 2*TESTSIZE; i++)
        items[i-1] = i;

    // Test copying of an empty list. After the copying, we change original.
    cout << "Copy constructor test: for an empty list." << endl;
    sequence copy1(original);
    original.attach(1); // Changes the original list, but not the copy.
    if (!correct(copy1, 0, 0, items)) return 0;

    // Test copying of a list with current item at the tail.
    cout << "Copy constructor test: for a list with cursor at tail." << endl;
    for (i=2; i <= 2*TESTSIZE; i++)
        original.attach(i);
    sequence copy2(original);
    original.remove_current( ); // Delete tail from original, but not the copy
    original.start( );
    original.advance( );
    original.remove_current( ); // Delete 2 from original, but not the copy.
    if (!correct
        (copy2, 2*TESTSIZE, 2*TESTSIZE-1, items)
        )
        return 0;

    // Test copying of a list with cursor near the middle.
    cout << "Copy constructor test: with cursor near middle." << endl;
    original.insert(2);
    for (i = 1; i < TESTSIZE; i++)
        original.advance( );
    // Cursor is now at location [TESTSIZE] (counting [0] as the first spot).
    sequence copy3(original);
    original.start( );
    original.advance( );
    original.remove_current( ); // Delete 2 from original, but not the copy.
    if (!correct
        (copy3, 2*TESTSIZE-1, TESTSIZE, items)
        )
        return 0;

    // Test copying of a list with cursor at the front.
    cout << "Copy constructor test: for a list with cursor at front." << endl;
    original.insert(2);
    original.start( );
    // Cursor is now at the front.
    sequence copy4(original);
    original.start( );
    original.advance( );
    original.remove_current( ); // Delete 2 from original, but not the copy.
    if (!correct
        (copy4, 2*TESTSIZE-1, 0, items)
        )
        return 0;

    // Test copying of a list with no current item.
    cout << "Copy constructor test: for a list with no current item." << endl;
    original.insert(2);
    while (original.is_item( ))
        original.advance( );
    // There is now no current item.
    sequence copy5(original);
    original.start( );
    original.advance( );
    original.remove_current( ); // Delete 2 from original, but not the copy.
    if (!correct
        (copy5, 2*TESTSIZE-1, 2*TESTSIZE, items)
        )
        return 0;

    // All tests passed
    cout << "All tests of this fourth function have been passed." << endl;
    return YOURPOINTS[4];
}


// **************************************************************************
// int test5( )
//   Performs some tests of the assignment operator.
//   Returns YOURPOINTS[5] if the tests are passed. Otherwise returns 0.
// **************************************************************************
int test5( )
{
    const sequence::size_type TESTSIZE = 30;
    sequence original; // A list that we'll copy.
    double items[2*TESTSIZE];
    sequence::size_type i;

    // Set up the items array to conatin 1...2*TESTSIZE.
    for (i = 1; i <= 2*TESTSIZE; i++)
        items[i-1] = i;

    // Test copying of an empty list. After the copying, we change original.
    cout << "Assignment operator test: for an empty list." << endl;
    sequence copy1;
    copy1 = original;
    original.attach(1); // Changes the original list, but not the copy.
    if (!correct(copy1, 0, 0, items)) return 0;

    // Test copying of a list with current item at the tail.
    cout << "Assignment operator test: cursor at tail." << endl;
    for (i=2; i <= 2*TESTSIZE; i++)
        original.attach(i);
    sequence copy2;
    copy2 = original;
    original.remove_current( ); // Delete tail from original, but not the copy
    original.start( );
    original.advance( );
    original.remove_current( ); // Delete 2 from original, but not the copy.
    if (!correct
        (copy2, 2*TESTSIZE, 2*TESTSIZE-1, items)
        )
        return 0;

    // Test copying of a list with cursor near the middle.
    cout << "Assignment operator test: with cursor near middle." << endl;
    original.insert(2);
    for (i = 1; i < TESTSIZE; i++)
        original.advance( );
    // Cursor is now at location [TESTSIZE] (counting [0] as the first spot).
    sequence copy3;
    copy3 = original;
    original.start( );
    original.advance( );
    original.remove_current( ); // Delete 2 from original, but not the copy.
    if (!correct
        (copy3, 2*TESTSIZE-1, TESTSIZE, items)
        )
        return 0;

    // Test copying of a list with cursor at the front.
    cout << "Assignment operator test: with cursor at front." << endl;
    original.insert(2);
    original.start( );
    // Cursor is now at the front.
    sequence copy4;
    copy4 = original;
    original.start( );
    original.advance( );
    original.remove_current( ); // Delete 2 from original, but not the copy.
    if (!correct
        (copy4, 2*TESTSIZE-1, 0, items)
        )
        return 0;

    // Test copying of a list with no current item.
    cout << "Assignment operator test: with no current item." << endl;
    original.insert(2);
    while (original.is_item( ))
        original.advance( );
    // There is now no current item.
    sequence copy5;
    copy5 = original;
    original.start( );
    original.advance( );
    original.remove_current( ); // Deletes 2 from the original, but not copy.
    if (!correct
        (copy5, 2*TESTSIZE-1, 2*TESTSIZE, items)
        )
        return 0;

    cout << "Checking correctness of a self-assignment x = x;" << endl;
    original.insert(2);
    original = original;
    if (!correct
        (original, 2*TESTSIZE-1, 1, items)
        )
        return 0;

    // All tests passed
    cout << "All tests of this fifth function have been passed." << endl;
    return YOURPOINTS[5];
}


// **************************************************************************
// int test6( )
//   Performs some basic tests of insert and attach for the case where the
//   current capacity has been reached.
//   Returns YOURPOINTS[6] if the tests are passed. Otherwise returns 0.
// **************************************************************************
int test6( )
{
    const sequence::size_type TESTSIZE = 30;
    sequence testa, testi;
    double items[2*TESTSIZE];
    sequence::size_type i;

    // Set up the items array to conatin 1...2*TESTSIZE.
    for (i = 1; i <= 2*TESTSIZE; i++)
        items[i-1] = i;

    cout << "Testing to see that attach works correctly when the\n";
    cout << "current capacity is exceeded." << endl;
    for (i = 1; i <= 2*TESTSIZE; i++)
        testa.attach(i);
    if (!correct
        (testa, 2*TESTSIZE, 2*TESTSIZE-1, items)
        )
        return 0;

    cout << "Testing to see that insert works correctly when the\n";
    cout << "current capacity is exceeded." << endl;
    for (i = 2*TESTSIZE; i >= 1; i--)
        testi.insert(i);
    if (!correct
        (testi, 2*TESTSIZE, 0, items)
        )
        return 0;

    // All tests passed
    cout << "All tests of this sixth function have been passed." << endl;
    return YOURPOINTS[7];
}


// **************************************************************************
// int test7( )
//   Tries to find a heap leak in remove_current, the assignment operator or
//   the destructor.
//   Returns YOURPOINTS[7] if no leaks are found. Otherwise returns 0.
// **************************************************************************
int test7( )
{
    const sequence::size_type TESTSIZE = 30;
    sequence test_list;
    sequence empty;
    sequence* list_ptr;
    sequence::size_type base_usage;
    sequence::size_type i;

    cout << "Checking for possible heap leak." << endl;
    cout << "This could occur if remove_current does not correctly release\n";
    cout << "memory, or if the assignment operator does not\n";
    cout << "correctly release memory, or if the destructor does not release\n";
    cout << "the memory of the dynamic array." << endl;

    // Test assignment for a heap leak
    base_usage = memory_used_now;
    for (i = 1; i <= TESTSIZE; i++)
        test_list.insert(i);
    test_list = empty;
    if (memory_used_now != base_usage)
    {
        cout << "    Test failed. Possible heap leak in assignment operator." << endl;
        return 0;
    }

    // Test remove_current for a heap leak
    base_usage = memory_used_now;
    for (i = 1; i <= TESTSIZE; i++)
        test_list.insert(i);
    for (i = 1; i <= TESTSIZE; i++)
        test_list.remove_current( );
    if (memory_used_now != base_usage)
    {
        cout << "    Test failed. Possible heap leak in remove_current." << endl;
        return 0;
    }

    // Test the destructor for a heap leak
    list_ptr = new sequence;
    for (i = 1; i <= TESTSIZE; i++)
        list_ptr->insert(i);
    delete list_ptr;
    if (memory_used_now != base_usage)
    {
        cout << "    Test failed. Possible heap leak in destructor." << endl;
        return 0;
    }

    cout << "No heap leak found." << endl;
    return YOURPOINTS[7];
}


int run_a_test(int number, const char message[], int test_function( ), int max)
{
    int result;

    cout << endl << "START OF TEST " << number << ":" << endl;
    cout << message << " (" << max << " points)." << endl;
    result = test_function( );
    if (result > 0)
    {
        cout << "Test " << number << " got " << result << " points";
        cout << " out of a possible " << max << "." << endl;
    }
    else
        cout << "Test " << number << " failed." << endl;
    cout << "END OF TEST " << number << "." << endl << endl;

    return result;
}


// **************************************************************************
// int main( )
//   The main program calls all tests and prints the sum of all points
//   earned from the tests.
// **************************************************************************
int main( )
{
    int sum = 0;

    cout << "Running " << DESCRIPTION[0] << endl;

    sum += run_a_test(1, DESCRIPTION[1], test1, YOURPOINTS[1]);
    sum += run_a_test(2, DESCRIPTION[2], test2, YOURPOINTS[2]);
    sum += run_a_test(3, DESCRIPTION[3], test3, YOURPOINTS[3]);
    sum += run_a_test(4, DESCRIPTION[4], test4, YOURPOINTS[4]);
    sum += run_a_test(5, DESCRIPTION[5], test5, YOURPOINTS[5]);
    sum += run_a_test(6, DESCRIPTION[6], test6, YOURPOINTS[6]);
    sum += run_a_test(7, DESCRIPTION[7], test7, YOURPOINTS[7]);

    cout << "If you submit this sequence to us now, you will have\n";
    cout << sum << " points out of the " << YOURPOINTS[0];
    cout << " points from this test program.\n";

    return EXIT_SUCCESS;

}
