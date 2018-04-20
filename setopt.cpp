#include <iostream>
#include <string>
#include "urldata.h"

UserDefined::UserDefined(){};

template<typename T>
void UserDefined::setopt(int a, T b) {
    switch (a) {
    case CURLPP_OPT_URL:
        break;
    case CURLPP_OPT_SERVER:
        server = b;
    case CURLPP_OPT_PATH:
        path = b;
    default: std::cerr << "Error: CURLPP_OPT not yet supported" << "\n";
    }
}

template<typename T, typename... Args>
void setopt(int a, T b, Args... args) {
    setopt(a, b);
    setopt(args...);
}
