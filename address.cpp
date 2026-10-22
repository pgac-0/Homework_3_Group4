#include "address.h"

string address::address_info()
{
    string result = "";

    result += address_line1 + "\n";
    result += address_line2 + "\n";
    result += city + "\n";
    result += state + "\n";
    result += zip;

    return result;
}
//implementation code for address struct here
