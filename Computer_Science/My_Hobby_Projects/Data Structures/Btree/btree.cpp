#include "hw3_set.h"

namespace HW3 {

bool set::loose_insert(const T &value) {
	int i = 0;

	for(i=0; i<entry_count; ++i)
		if(data[i] == value)
			return false;
		else if(value < data[i])
			break;

	if(is_leaf()) {
		if(i != entry_count)
			memmove(&data[i+1], &data[i], (entry_count-i)*sizeof(T));

		data[i] = value;
		++entry_count;
		return true;
	}

	return subset[i]->loose_insert(value);
}

void set::print() {
    for (int i = 0; i<entry_count; i++) {
        if (!is_leaf()) subset[i]->print();
        std::cout << data[i] << " ";
    }

	if (!is_leaf()) subset[child_count-1]->print();
}

}
