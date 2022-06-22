//
// Created by zhaohuiqiang on 2021/12/28.
//
#include "tests/Tests.h"

int main(int argc, const char *argv[]) {
    StopWatch watch;
    ParamsTool::fromArgs(argc, argv);
    test();
    cout << "\nThe run time is: " << watch.ShowTickMills() << endl;
}
