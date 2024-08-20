

#include "d_time.h"

#include <iostream>
#include <unistd.h>


int main(int argc, char *argv[])
{
    std::cout << "timestamp: "  << d_time_stampms() << std::endl;
    sleep(1);
    std::cout << "timestamp: "  << d_time_stampms() << std::endl;

    return 0;
}
