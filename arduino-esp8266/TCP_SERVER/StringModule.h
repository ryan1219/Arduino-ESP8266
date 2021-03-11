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
          
	  //fuctions
	  boolean Contains(String search);
      void ReplaceBase(String base);
      void StoreKey(String input);
      String Delimitation(char delimiter);
  private:
	  String s;
      String key;
};

#endif
