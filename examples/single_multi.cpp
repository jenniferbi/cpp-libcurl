/********************************************************
 *               __   __  _____  __                     *
 *              / /  / / / / _ \/ /                     *
 *             / _ \/ /_/ / , _/ /__                    *
 *            /_//_/\____/_/|_/____/                    *
 *                                                      *
 * This file is a threaded example for how to use hurl  *
 * with multihandle. This example spins up 1 thread.    *
 *                                                      *
 ********************************************************/

#include <stdio.h>
#include "handle.h"
#include "urldata.h"

int main(void)
{
    Multi myhandle;
    myhandle.setOpt(CURLPP_OPT_URL, "http://www.stroustrup.com/Bjarne2018.jpg",
                    CURLPP_OPT_THREAD_POOL, 1); // Default value is 2
    myhandle.perform();

    return 0;
}
