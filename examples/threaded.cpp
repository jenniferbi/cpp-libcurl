#include <stdio.h>
#include "handle.h"
#include "urldata.h"

int main(void)
{

    Multi myhandle;
    myhandle.setOpt(CURLPP_OPT_URL, "wwww.columbia.edu/cu/bulletin/uwb/");
    myhandle.perform();

    return 0;
}
