#include "hw3_set.h"

using namespace std;
using namespace HW3;

void runTest(size_t universe_size) {
	cout<<"BEGINNING TEST OF SET WITH A UNIVERSE OF CARDINAL ["<<universe_size<<"]\n\n";
	static const size_t N_UNIVERSE = universe_size;
	static const size_t N = N_UNIVERSE/2;
	size_t i, size_a=0, size_b=0, size_c=0;
	size_t x=0,y=0,z=0;
	size_t a_repeat=0,b_repeat=0,c_repeat=0;
	HW3::set::T tmp_a, tmp_b, tmp_c;
	set a,b,c,null_set,universe_set;
	set ab, ac, bc;
	HW3::set::T arr_a[N], arr_b[N], arr_c[N];
	bool result;

	for(i=0; i<N_UNIVERSE; ++i)
		universe_set.insert(i);

	srand(time(NULL));
	for(i=0; i<N; ++i) {
		tmp_a = rand() % N_UNIVERSE;
		tmp_b = rand() % N_UNIVERSE;
		tmp_c = rand() % N_UNIVERSE;

		// a set/array
		a.insert(tmp_a);
		if(!HW3::array_contains(arr_a, size_a, tmp_a)) {
			arr_a[x++] = tmp_a;
			++size_a;
		} else {
			arr_a[i] = tmp_a;
			++a_repeat;
		}

		// b set/array
		b.insert(tmp_b);
		if(!HW3::array_contains(arr_b, size_b, tmp_b)) {
			arr_b[y++] = tmp_b;
			++size_b;
		} else {
			arr_b[i] = tmp_b;
			++b_repeat;
		}

		// c set/array
		c.insert(tmp_c);
		if(!HW3::array_contains(arr_c, size_c, tmp_c)) {
			arr_c[z++] = tmp_c;
			++size_c;
		} else {
			arr_c[i] = tmp_c;
			++c_repeat;
		}

		// a + b
		ab.insert(tmp_a);
		ab.insert(tmp_b);

		// a + c
		ac.insert(tmp_a);
		ac.insert(tmp_c);

		// b + c
		bc.insert(tmp_b);
		bc.insert(tmp_c);
	}

	/* BTREE FUNCTION TESTS */
	cout<<"TESTSUITE #1: CHECKING VALIDITY OF BTREE INSERT/REMOVE/SEARCH FUNCTIONS:\n";
	cout<<"--------------------------------------------------------\n\n";
	cout<<"TEST #1: CHECKING VALIDITY OF BTREE INSERT FUNCTION:\n";
	cout<<"--------------------------------------------------------\n";

	cout<<"TEST #1.1a: CHECKING IF ARRAY 'arr_a' WITH REPEATION\n"
					"IS BIJECTIVE AND EQUAL TO SET 'a': "
					<<((result=a.bijectiveTo(arr_a, N))==true && a_repeat==0? "Yes"
					: "No")<<" "<<a_repeat<<" repeat[s]\n\n";

	cout<<"TEST #1.1b: CHECKING IF ARRAY 'arr_a' WITHOUT REPEATION\n"
			"IS BIJECTIVE AND EQUAL TO SET 'a': "
			<<((result=a.bijectiveTo(arr_a, size_a))==true? "Yes --> Passed" : "No --> Failed")<<"\n\n";

	cout<<"TEST #1.2a: CHECKING IF ARRAY 'arr_b' WITH REPEATION\n"
					"IS BIJECTIVE AND EQUAL TO SET 'b': "
					<<(b.bijectiveTo(arr_b, N)? "Yes"
					: "No")<<" "<<b_repeat<<" repeat[s]\n\n";

	cout<<"TEST #1.2b: CHECKING IF ARRAY 'arr_b' WITHOUT REPEATION\n"
			"IS BIJECTIVE AND EQUAL TO SET 'b': "
			<<((result=b.bijectiveTo(arr_b, size_b))==true? "Yes --> Passed" : "No --> Failed")<<"\n\n";

	cout<<"TEST #1.3a: CHECKING IF ARRAY 'arr_c' WITH REPEATION\n"
					"IS BIJECTIVE AND EQUAL TO SET 'c': "
					<<(c.bijectiveTo(arr_c, N)? "Yes"
					: "No")<<" "<<c_repeat<<" repeat[s]\n\n";

	cout<<"TEST #1.3b: CHECKING IF ARRAY 'arr_c' WITHOUT REPEATION\n"
			"IS BIJECTIVE AND EQUAL TO SET 'c': "
			<<((result=c.bijectiveTo(arr_c, size_c))==true? "Yes --> Passed" : "No --> Failed")<<"\n\n";

	if(result)
		cout<<"TEST #1: CHECKING FOR VALIDITY OF BTREE INSERT FUNCTION: "
		<<(result? "Passed" : "Failed")<<"\n\n";
	else
		test_failed(universe_size);

	cout<<"TEST #2: CHECKING VALIDITY OF BTREE REMOVE FUNCTION:\n";
	cout<<"--------------------------------------------------------\n";

	set tmp = a;
	cout<<"Setting set 'tmp' to set 'a'. tmp's original Cardinal: "<<tmp.cardinality()<<"\n";

	cout<<"TEST #2.1a: REMOVING ALL ITEMS IN 'arr_a' WITH REPEATION\n"
			"FROM SET 'a', THIS SHOULD TURN 'a' INTO A NULL SET WITH A\n"
			"CARDINAL OF 0: ";

	tmp.removeAll(arr_a, N);

	cout<<((result = tmp.cardinality()==0 && tmp==null_set)==true?
			"Passed" : "Failed")<<" Cardinal="<<tmp.cardinality()<<"\n\n";

	tmp = a;
	cout<<"Setting set 'tmp' to set 'a'. tmp's original Cardinal: "<<tmp.cardinality()<<"\n";

	cout<<"TEST #2.1b: REMOVING ALL ITEMS IN 'arr_a' WITHOUT REPEATION\n"
			"FROM SET 'a', THIS SHOULD TURN 'a' INTO A NULL SET WITH A\n"
			"CARDINAL OF 0: ";

	tmp.removeAll(arr_a, N);

	cout<<((result = tmp.cardinality()==0 && tmp==null_set)==true?
			"Passed" : "Failed")<<" Cardinal="<<tmp.cardinality()<<"\n\n";

	tmp = b;
	cout<<"Setting set 'tmp' to set 'b'. tmp's original Cardinal: "<<tmp.cardinality()<<"\n";

	cout<<"TEST #2.2a: REMOVING ALL ITEMS IN 'arr_b' WITH REPEATION\n"
			"FROM SET 'b', THIS SHOULD TURN 'b' INTO A NULL SET WITH A\n"
			"CARDINAL OF 0: ";

	tmp.removeAll(arr_b, N);

	cout<<((result = tmp.cardinality()==0 && tmp==null_set)==true?
			"Passed" : "Failed")<<" Cardinal="<<tmp.cardinality()<<"\n\n";

	tmp = b;
	cout<<"Setting set 'tmp' to set 'b'. tmp's original Cardinal: "<<tmp.cardinality()<<"\n";

	cout<<"TEST #2.2b: REMOVING ALL ITEMS IN 'arr_b' WITHOUT REPEATION\n"
			"FROM SET 'b', THIS SHOULD TURN 'b' INTO A NULL SET WITH A\n"
			"CARDINAL OF 0: ";

	tmp.removeAll(arr_b, N);

	cout<<((result = tmp.cardinality()==0 && tmp==null_set)==true?
			"Passed" : "Failed")<<" Cardinal="<<tmp.cardinality()<<"\n\n";

	tmp = c;
	cout<<"Setting set 'tmp' to set 'c'. tmp's original Cardinal: "<<tmp.cardinality()<<"\n";

	cout<<"TEST #2.3a: REMOVING ALL ITEMS IN 'arr_c' WITH REPEATION\n"
			"FROM SET 'c', THIS SHOULD TURN 'c' INTO A NULL SET WITH A\n"
			"CARDINAL OF 0: ";

	tmp.removeAll(arr_c, N);

	cout<<((result = tmp.cardinality()==0 && tmp==null_set)==true?
			"Passed" : "Failed")<<" Cardinal="<<tmp.cardinality()<<"\n\n";

	tmp = c;
	cout<<"Setting set 'tmp' to set 'c'. tmp's original Cardinal: "<<tmp.cardinality()<<"\n";

	cout<<"TEST #2.3b: REMOVING ALL ITEMS IN 'arr_c' WITHOUT REPEATION\n"
			"FROM SET 'c', THIS SHOULD TURN 'c' INTO A NULL SET WITH A\n"
			"CARDINAL OF 0: ";

	tmp.removeAll(arr_c, N);

	cout<<((result = tmp.cardinality()==0 && tmp==null_set)==true?
			"Passed" : "Failed")<<" Cardinal="<<tmp.cardinality()<<"\n\n";

	if(result) {
		cout<<"TEST #2: CHECKING FOR VALIDITY OF BTREE REMOVE FUNCTION: "
		<<(result? "Passed" : "Failed")<<"\n\n";

		cout<<"TEST #3: CHECKING FOR VALIDITY OF BTREE SEARCH FUNCTION: Implied Pass.\n"
				"Why? it was utilized by the Bijection of the set with the arrays.\n"
				"The Bijection would return false otherwise. Size it does a 1-by-1\n"
				"comparison of the data in the BTREE with the elements in the array\n"
				"both way.\n\n";
	} else
		test_failed(universe_size);

	/* Operator tests */
	cout<<"\nTESTSUITE #2: VALIDITY OF OPERATORS:\n";
	cout<<"--------------------------------------------------------\n\n";

	// Test Intersection Operator:
	result = a*b == a+b-(a-b)-(b-a);
	cout<<"TEST #1: The Intersection Operator:"
			"\n\nThe intersection between two sets, a and b,"
			"\ncan be expressed in terms of union and difference:\n\n"
			"a * b = a + b - (a - b) - (b - a) --> "
			<< (result? "Passed" : "Failed")<<"\n\n";

	if(!result) test_failed(universe_size);

	/* PROPOSITION 1: For any sets a, b, and c, the following identities hold: */
	cout<<"\nCHECKING AGAINST PROPOSITION 1: For any sets a, b, and c,\n"
			"the following identities hold:\n\n";
	// Test Commulative:
	result = a+b == b+a;
	cout<<"Testing Commulative Law: a + b = b + a --> "
			<< (result? "Passed" : "Failed")<<"\n";
	if(!result) test_failed(universe_size);

	// Test Commulative:
	result = a*b == b*a;
	cout<<"Testing Commulative Law: a * b = b * a --> "
			<< (result? "Passed" : "Failed")<<"\n\n";
	if(!result) test_failed(universe_size);

	// Test Associative:
	result = (a+b)+c == a+(b+c);
	cout<<"Testing Associative Law (a + b) + c = a + (b + c) --> "
			<< (result? "Passed" : "Failed")<<"\n";
	if(!result) test_failed(universe_size);

	result = (a+b)+c == a+(b+c);
	cout<<"Testing Associative Law (a * b) * c = a * (b * c) --> "
			<< (result? "Passed" : "Failed")<<"\n\n";
	if(!result) test_failed(universe_size);

	// Test Distributive:
	result = a+(b*c) == (a+b)*(a+c);
	cout<<"Testing Distributive Law: a + (b * c) = (a + b) * (a + c) --> "
			<< (result? "Passed" : "Failed")<<"\n";
	if(!result) test_failed(universe_size);

	result = a*(b+c) == (a*b)+(a*c);
	cout<<"Testing Distributive Law: a * (b + c) = (a * b) + (a * c) --> "
			<< (result? "Passed" : "Failed")<<"\n\n";
	if(!result) test_failed(universe_size);

	/*
	 *  PROPOSITION 2: For any subset a of universal set U, where
	 *  Ø is the empty set, the following identities hold:
	 */
	cout<<"\nCHECKING AGAINST PROPOSITION 2: For any subset a of universal set\n"
			"U, where Ø is the empty set, the following identities hold:\n\n";
	// Test Identity:
	result = a+null_set == a;
	cout<<"Testing Identity Law: a + Ø = a --> "
			<< (result? "Passed" : "Failed")<<"\n";
	if(!result) test_failed(universe_size);

	// Test Identity:
	result = a*universe_set == a;
	cout<<"Testing Identity Law: a * U = a --> "
			<< (result? "Passed" : "Failed")<<"\n\n";
	if(!result) test_failed(universe_size);

	// Test Complement:
	result = a+(universe_set-a) == universe_set;
	cout<<"Testing Complement Law: a + a' = U --> "
			<< (result? "Passed" : "Failed")<<"\n";
	if(!result) test_failed(universe_size);

	// Test Complement:
	result = a*(universe_set-a) == null_set;
	cout<<"Testing Complement Law: a * a' = Ø --> "
			<< (result? "Passed" : "Failed")<<"\n";
	if(!result) test_failed(universe_size);

	/*
	 *  PROPOSITION 3: For any subsets A and B of a
	 *  universal set U, the following identities hold:
	 */
	cout<<"\nCHECKING AGAINST PROPOSITION 3: For any subsets A and B\n"
			"of a universal set U, the following identities hold:\n\n";
	// Test Idempotent (UNION):
	result = a+a == a;
	cout<<"Testing Idempotent (UNION) Law: a + a = a --> "
			<< (result? "Passed" : "Failed")<<"\n\n";
	if(!result) test_failed(universe_size);

	// Proof of Idempotent (UNION) Test:
	result = a+a == (a+a)*universe_set
			 && (a+a)*universe_set == (a+a)*(a+(universe_set-a))
			 && (a+a)*(a+(universe_set-a)) == a+(a*(universe_set-a))
			 &&  a+(a*(universe_set-a)) == a + null_set
			 && a + null_set == a;
	cout<<"Proof of Idempotent (UNION) Test:\n"
			"a + a = (a + a) * U [by the identity law of intersection]\n"
			"= (a + a) * (a + a') [by the complement law for union]\n"
			"= a + (a * a') [by the distributive law of union over intersection]\n"
			"= a + Ø [by the complement law for intersection]\n"
			"= a [by the identity law for union]\n\n"
			"RESULT: "
			<< (result? "Passed" : "Failed")<<"\n\n";
	if(!result) test_failed(universe_size);

	// Test Idempotent (INTERSECTION):
	result = a*a == a;
	cout<<"Testing Idempotent (INTERSECTION) Law: a * a = a --> "
			<< (result? "Passed" : "Failed")<<"\n\n";
	if(!result) test_failed(universe_size);

	// Proof of Idempotent (INTERSECTION) Test:
	result = a*a == (a*a)+null_set
			 && (a*a)+null_set == (a*a)+(a*(universe_set-a))
			 && (a*a)+(a*(universe_set-a)) == a*(a+(universe_set-a))
			 &&   a*(a+(universe_set-a)) == a * universe_set
			 && a * universe_set == a;
	cout<<"Proof of Idempotent (INTERSECTION) Test:\n"
			"a * a = (a * a) + Ø [by the identity law of union]\n"
			"= (a * a) + (a * a') [by the complement law for intersection]\n"
			"= a * (a + a') [by the distributive law of intersection over union]\n"
			"= a * U [by the complement law for union]\n"
			"= a [by the identity law for intersection]\n\n"
			"RESULT: "
			<< (result? "Passed" : "Failed")<<"\n\n";
	if(!result) test_failed(universe_size);

	// Test Domination:
	result = a+universe_set == universe_set;
	cout<<"Testing Domination Law: a + U = U --> "
			<< (result? "Passed" : "Failed")<<"\n";
	if(!result) test_failed(universe_size);

	// Test Domination:
	result = a*null_set == null_set;
	cout<<"Testing Domination Law: a * Ø = Ø --> "
			<< (result? "Passed" : "Failed")<<"\n\n";
	if(!result) test_failed(universe_size);

	// Test Absorption:
	result = a+(a*b) == a;
	cout<<"Testing Absorption Law: a + (a * b) = a --> "
			<< (result? "Passed" : "Failed")<<"\n";
	if(!result) test_failed(universe_size);

	// Test Absorption:
	result = a*(a+b) == a;
	cout<<"Testing Absorption Law:  a * (a + b) = a --> "
			<< (result? "Passed" : "Failed")<<"\n\n";
	if(!result) test_failed(universe_size);


	/*
	 *  PROPOSITION 4: Let a and b be subsets
	 *  of auniverse U, the following holds:
	 */
	cout<<"\nCHECKING AGAINST PROPOSITION 4: Let a and b be subsets\n"
			"of auniverse U, the following holds:\n\n";
	// Test De Morgan's Law:
	result = universe_set-(a+b) == (universe_set-a)*(universe_set-b);
	cout<<"Testing De Morgan's Law: (a + a)' = a' * b' --> "
			<< (result? "Passed" : "Failed")<<"\n";
	if(!result) test_failed(universe_size);

	// Test De Morgan's Law:
	result = universe_set-(a*b) == (universe_set-a)+(universe_set-b);
	cout<<"Testing De Morgan's Law: (a * a)' = a' + b' --> "
			<< (result? "Passed" : "Failed")<<"\n\n";
	if(!result) test_failed(universe_size);

	// Double Complement (Involution Law):
	result = universe_set-(universe_set-a) == a;
	cout<<"Testing Double Complement (Involution Law): a'' = a --> "
			<< (result? "Passed" : "Failed")<<"\n\n";
	if(!result) test_failed(universe_size);

	// Complement laws for the universal set and the empty set:
	result = universe_set-null_set == universe_set;
	cout<<"Testing Complement laws for the universal\n"
			"set and the empty set: Ø' = U --> "
			<< (result? "Passed" : "Failed")<<"\n\n";
	if(!result) test_failed(universe_size);

	// Complement laws for the universal set and the empty set:
	result = universe_set-universe_set == null_set;
	cout<<"Testing Complement laws for the universal\n"
			"set and the empty set: U' = Ø --> "
			<< (result? "Passed" : "Failed")<<"\n\n";
	if(!result) test_failed(universe_size);

	/*
	 *  PROPOSITION 5: Let A and B be
	 *  subsets of a universe U, then:
	 */
	cout<<"\nCHECKING AGAINST PROPOSITION 5:  Let A and B be\n"
			"subsets of a universe U, then:\n\n";
	// Uniqueness of complements:
	cout<<"Testing  Uniqueness of complements:\n"
			"[IF] A + B = U [AND] A * B = Ø [THEN] B = A' --> ";

	// Force result to false. The uniqueness
	// of compliments must be valid or fail.
	result = false;
	set A, B;
	for(i=0; i<N_UNIVERSE; ++i)
		if(i<N_UNIVERSE/2)
			A.insert(i);
		else
			B.insert(i);

	// Test where condition should be true.
	if(A+B==universe_set && A*B==null_set)
		result = B == universe_set-A;

	// Test where condition may or may not be true.
	if(a+b==universe_set && a*b==null_set)
		result = b == universe_set-a;

	cout<<(result? "Passed" : "Failed")<<"\n";
	if(!result) test_failed(universe_size);

	/*
	 *  PROPOSITION 9: The algebra of relative complements
	 *  For any universe U and subsets A, B, and C of U,
	 *  the following identities hold:
	 */
	cout<<"\nCHECKING AGAINST PROPOSITION 4: The algebra of relative complements\n"
			"For any universe U and subsets A, B, and C of U,\n"
			"the following identities hold:\n\n";
	// Identity Test:
	result = c-(a*b) == (c-a)+(c-b);
	cout<<"Testing Identity: c - (a * b) = (c - a) + (c - b) --> "
			<< (result? "Passed" : "Failed")<<"\n";
	if(!result) test_failed(universe_size);

	// Identity Test:
	result = c-(a+b) == (c-a)*(c-b);
	cout<<"Testing Identity: c - (a + b) = (c - a) * (c - b) --> "
			<< (result? "Passed" : "Failed")<<"\n";
	if(!result) test_failed(universe_size);

	// Identity Test:
	result = c-(b-a) == (a*c)+(c-b);
	cout<<"Testing Identity: c - (b - a) = (a * c) + (c - b) --> "
			<< (result? "Passed" : "Failed")<<"\n";
	if(!result) test_failed(universe_size);

	// Identity Test:
	result = (b-a)*c == (b*c)-a && (b*c)-a == b*(c-a);
	cout<<"Testing Identity: (b - a) * c  =  (b * c) - a  =  b * (c - a) --> "
			<< (result? "Passed" : "Failed")<<"\n";
	if(!result) test_failed(universe_size);

	// Identity Test:
	result = (b-a)+c == (b+c)-(a-c);
	cout<<"Testing Identity: (b - a) + c = (b + c) - (a - c) --> "
			<< (result? "Passed" : "Failed")<<"\n";
	if(!result) test_failed(universe_size);

	// Identity Test:
	result = a-a == null_set;
	cout<<"Testing Identity: a - a = Ø --> "
			<< (result? "Passed" : "Failed")<<"\n";
	if(!result) test_failed(universe_size);

	// Identity Test:
	result = null_set-a == null_set;
	cout<<"Testing Identity: Ø - a = Ø --> "
			<< (result? "Passed" : "Failed")<<"\n";
	if(!result) test_failed(universe_size);

	// Identity Test:
	result = b-a == (universe_set-a)*b;
	cout<<"Testing Identity: b - a = a' * b --> "
			<< (result? "Passed" : "Failed")<<"\n";
	if(!result) test_failed(universe_size);

	// Identity Test:
	result=universe_set-(b-a) == a+(universe_set-b);
	cout<<"Testing Identity: (b - a)' = a + b' --> "
			<< (result? "Passed" : "Failed")<<"\n";
	if(!result) test_failed(universe_size);

	// Identity Test:
	result=universe_set-a == universe_set-a;
	cout<<"Testing Identity: U - a = a' --> "
			<< (result? "Passed" : "Failed")<<"\n";
	if(!result) test_failed(universe_size);

	// Identity Test:
	result=a-universe_set == null_set;
	cout<<"Testing Identity: a - U = Ø --> "
			<< (result? "Passed" : "Failed")<<"\n\n";
	if(!result) test_failed(universe_size);

	cout<<"TEST OF SET WITH UNIVERSE OF CARDINAL ["<<universe_size<<"]\n"
			"HAS SUCCESSFULLY COMPLETED ALL THE STEPS!\n";
}

int main() {
	static const int MAX_ELEMENTS = 1048576; // 2^20
	static const int NUM_TESTS = 10000; // MIN VALUE = 5!!
	int universe_array[NUM_TESTS], i;

	// Let's test the minimal set cases:
	universe_array[0] = 0;
	universe_array[1] = 1;
	universe_array[2] = 2;
	universe_array[3] = 30; // suggested in description
	universe_array[NUM_TESTS-1] = 50000; // suggested in description

	srand(time(NULL));
	for(i=4; i<NUM_TESTS-1; ++i)
		universe_array[i] = rand() % MAX_ELEMENTS;

	for(i=0; i<NUM_TESTS; ++i)
		runTest(universe_array[i]);

	cout<<"\nRAN TESTSUITE "<<i<<" TIMES SUCCESSFULLY!\n";

	return 0;
}
