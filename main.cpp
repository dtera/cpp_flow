#include <ctime>

#include "tests/Tests.h"
#include "tests/HttpTest.h"

int main() {
    //random_str_test();
    //rsa_test();
    //bit_op_test();
    clock_t start_time = clock();
    /*string res;
    do {
    res = ot_test();
    } while(res == "def");*/
    //ot_test();
    /*BaseOTSender<string>::gen_keypair();
    cout << BaseOTSender<string>::get_pub_key() << endl;*/
    httpClientTest();
    httpServerTest();
    clock_t end_time = clock();
    cout << "\nThe run time is: " << (double) (end_time - start_time) / CLOCKS_PER_SEC * 1000 << "ms" << endl;
}
