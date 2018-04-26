#include <stdio.h>
#include "handle.h"
#include "urldata.h"

int main(int argc, char** argv)
{
    std::string url (argv[1]);
    Easy myhandle;
    myhandle.setOpt(CURLPP_OPT_URL, url,
      CURLPP_OPT_TIMEOUT, 1000
        );

    myhandle.perform();
    return 0;
}
