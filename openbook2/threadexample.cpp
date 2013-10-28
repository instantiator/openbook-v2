/** threadexample.cpp - Threading demonstration with two threads. **/

#include "openbook2.h"

// basic constructor - has iterations, has a name
// ==============================================
ThreadExample::ThreadExample(std::string xiThreadName, size_t xiIterations) : mName(xiThreadName), mLoops(xiIterations) {}

// sample run function.
// ====================
void ThreadExample::run()
{
    for (size_t i=0; i < mLoops; i++)
        std::cout << i << " " << mName << "\n";

    std::cout << std::endl;
}


