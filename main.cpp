#include <ctime>

#include "test/Test.h"

int main() {
    //random_str_test();
    //rsa_test();
    //bit_op_test();
    clock_t start_time = clock();
    /*string res;
    do {
    res = ot_test();
    } while(res == "def");*/
    ot_test();
    clock_t end_time = clock();
    cout << "The run time is: " << (double) (end_time - start_time) / CLOCKS_PER_SEC * 1000 << "ms" << endl;
}
