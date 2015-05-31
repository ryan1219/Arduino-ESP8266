#include <Arduino.h>
//need to include Arduino standard lib, in main sketch, it is included automaticlly
#include "StringModule.h"
//Connect cpp with header,cpp and header file should be saved in the same folder.

//s and key are internal private variable
//String string;
//String key; 

//constructor
StringModule::StringModule(String base)
{
  string = base;
  key="";
}
//
String StringModule::getString(){
  return string;
}
void StringModule::setString(String newString){
  string=newString;
}
String StringModule::getKey(){
  return key;
}
void StringModule::setKey(String newKey){
  key=newKey;
}

void StringModule::ClearString(){
  string="";
}
  
//boolean Contains(String search);
boolean StringModule::Contains(String search) {
  int endP = search.length();
  int maxP = string.length() - endP;

  for (int i = 0; i <= maxP; i++) {
    if (search.equalsIgnoreCase(string.substring(i, endP)))
      return true;

    endP++;
  }

  return false;
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
