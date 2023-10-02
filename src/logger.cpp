#include <iostream>
#include <string>

int printlog(int errorType, const char* thread, const char* msg) {

    // logging function used throughout the program
    // errorType is used to color the output
    // thread is used to identify where it goes wrong

    if (errorType == 0) {            // default / white
        std::cout << "\033[0;37m[rendrr/" << thread << "] ";
    } else if (errorType == 1) {     // success / green
        std::cout << "\033[0;32m[rendrr/" << thread << "] ";
    } else if (errorType == 2) {     // warning / yellow
        std::cout << "\033[0;33m  [rendrr/" << thread << "] ";
    } else if (errorType == 3) {     // error / red
        std::cout << "\n  \033[0;31m[rendrr/" << thread << "] ";
    }

    std::cout << msg << "\033[0;37m" << std::endl;
    return 0;
}