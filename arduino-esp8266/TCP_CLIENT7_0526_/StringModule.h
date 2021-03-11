#ifndef StringModule_h
#define StringModule_h
//prevent this header is included mutiple times during compiling
/*header statements are platform independent, cpp file statements are particular to 
Arduino situation. 
*/
#include <Arduino.h>
//need to include Arduino standard lib, in main sketch, it is included automaticlly

class StringModule
{
  public:
  //constructor
  StringModule(String base);
  
  //setter and getter method for string and key
  String getString();
  void setString(String newString);
  String getKey();
  void setKey(String newKey);
  
  //functions
  void ClearString();
  boolean Contains(String search);
  String Delimitation(char delimiter);
  
  private:
  String string;
  String key;
};

#endif
