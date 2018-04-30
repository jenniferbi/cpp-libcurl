/********************************************************
 *               __   __  _____  __                     *
 *              / /  / / / / _ \/ /                     *
 *             / _ \/ /_/ / , _/ /__                    *
 *            /_//_/\____/_/|_/____/                    *
 *                                                      *
 * This file is a threaded example for how to use hurl  *
 * with threads and a writeback function.               *
 *                                                      *
 ********************************************************/

#include <stdio.h>
#include "handle.h"
#include "urldata.h"

int main(void)
{
    auto foo = [](const unsigned char * s, size_t size)
        {
            cout << "*************************************\n";
            cout << size << " received\n";
            cout << "*************************************\n";
            return 0;
        };
    Multi myhandle;
    myhandle.setOpt(CURLPP_OPT_URL, "http://www.stroustrup.com/Bjarne2018.jpg",
                    CURLPP_OPT_THREAD_POOL, 10,
                    CURLPP_OPT_WRITEFN, foo);
    myhandle.perform();

    return 0;
}
