/********************************************************
 *               __   __  _____  __                     *
 *              / /  / / / / _ \/ /                     *
 *             / _ \/ /_/ / , _/ /__                    *
 *            /_//_/\____/_/|_/____/                    *
 *                                                      *
 * This file is the simplest example for how to use hurl *
 * based on simple.c, a simple curl example             *
 *                                                      *
 ********************************************************/

#include <iostream>
#include "handle.h"
#include "urldata.h"

int main(void)
{
    Easy myhandle; // Equivalent to CURL *curl;
                   // CURLcode res;
                   // curl = curl_easy_init();
    // Typesafe setopt function
    myhandle.setOpt(CURLPP_OPT_URL, "http://www.stroustrup.com/Bjarne2018.jpg");

    myhandle.perform(); // res = curl_easy_perform(curl);
    return 0;
}
