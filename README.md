# cpp-libcurl


## Where important things lie
- lib/connconnect.h
- lib/urldata.h  -  struct Curl_easy
- lib/multihandle.h - struct Curl_multi
- lib/easy.c - CURLcode easy_perform
- lib/setopt.c - CURLcode curl_easy_setopt

## What we want to see changed
[1] Global curl class that performs initialization in constructor and cleanup in destructor. (This means it can be called more than once but that seems problematic given the constant warning to only call global_init once.)
[2] Start by implementing Curl multi. The way to perform a curl_easy_perform is using a curl_multi_handle directly. (Possibly, implementation inheritance to easy if we wanted more than just a multi)
[3] Curl_multi_setopt should be made generic, type safe, and possibly use an initialization list to minimize. At the very least, we might want to regroup options into new data structures.
