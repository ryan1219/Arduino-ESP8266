#include <Arduino.h>
//need to include Arduino standard lib, in main sketch, it is included automaticlly
#include "StringModule.h"
//Connect cpp with header,cpp and header file should be saved in the same folder.

//s and key are internal private variable
//String s;
//String key; 

StringModule::StringModule(String base)//constructor
{
  s = base;
}
//boolean Contains(String search);
boolean StringModule::Contains(String search) {
  int endP = search.length();
  int maxP = s.length() - endP;

  for (int i = 0; i <= maxP; i++) {
    if (search.equalsIgnoreCase(s.substring(i, endP)))
      return true;

    endP++;
  }

  return false;
}

//void ReplaceBase(String base);
void StringModule::ReplaceBase(String base)
{
  s = base;
}

void StringModule::StoreKey(String input) /* Store the string need to be delimited */
{
  key=input;
}

String StringModule::Delimitation(char delimiter)
{
  int length = key.length();
  
  for(int i =0; i< length; i++)
  {
    if(delimiter == key.charAt(i))
    {
        String sth = key.substring(0,i);
        key = key.substring(i+1);
        return sth;
    }
  }
  
  return key;
}
