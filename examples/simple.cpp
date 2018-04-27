#include <stdio.h>
#include "handle.h"
#include "urldata.h"
#include <iostream>
#include <fstream>
#include <boost/timer/timer.hpp>
#include <string>


int main(void)
{
    Easy myhandle;
    myhandle.setOpt(CURLPP_OPT_HOST, "images.metmuseum.org",
                    CURLPP_OPT_PATH, "/CRDImages/as/original/DP141263.jpg",
                    CURLPP_OPT_SCHEME, CURLPP_OPT_HTTPS,
                    CURLPP_OPT_SSLCERT, "ca.pem"
        );

    myhandle.perform();
    return 0;
}
