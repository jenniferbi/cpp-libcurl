/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2016, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 * Copied from CURL. Compiledin C++ to test out SWIRL
 *
 ***************************************************************************/
/* <DESC>
 * Very simple HTTP GET
 * </DESC>
 */
#include <stdio.h>
#include <curl/curl.h>
#include <iostream>
#include <fstream>
#include <boost/timer/timer.hpp>
#include <string>

int main(int argc, char** argv)
{
    //std::string url_name= "https://images.metmuseum.org/CRDImages/as/original/DP141263.jpg";
    std::string url_name (argv[1]);
    std::size_t pos = url_name.find_last_of("/");
    std::string filename="./results/simple_c_";
    filename.append(url_name.substr(pos+1));
    filename.append(".csv");

    const char *c_url_name = url_name.c_str();

    std::ofstream output_file {filename, std::ios_base::app};
    // Not sure if we want to print out all these time values
    boost::timer::auto_cpu_timer t(output_file, "%w\n");

    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, c_url_name);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    return 0;
}
