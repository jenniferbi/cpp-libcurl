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
    Easy myhandle;
    myhandle.setOpt(CURLPP_OPT_HOST, "www.columbia.edu",
      CURLPP_OPT_PATH, "/cu/bulletin/uwb/sel/COMS_Fall2018.html",
      CURLPP_OPT_WRITEFN, foo);

    myhandle.perform();
    return 0;
}
