#include <ctime>

#include "tests/Tests.h"

int main() {
    clock_t start_time = clock();
    test();
    clock_t end_time = clock();
    cout << "\nThe run time is: " << (double) (end_time - start_time) / CLOCKS_PER_SEC * 1000 << "ms" << endl;
}
