#pragma ones

#ifndef Utils_h
#define Utils_h

#include <Arduino.h>
#include <LinkedList.h>
#include <structures\Option.h>

class Utils
{
public:
    static String getNamedOptionValue(LinkedList<Option> *options, String optionName);
    static unsigned int hexToDec(String hexString);
};

#endif