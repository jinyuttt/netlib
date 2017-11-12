#ifndef UTIL_H
#define UTIL_H
 class Util
{
public:
    Util () {}
   static long getACK()
   {
         return 300;
   }
   static long getNAK()
   {
   return 300;
   }
   static long getLost()
   {
     return 300;
   }
   static long getKeepAlive()
   {
    return 60*1000;
   }
   static long getKeepHeart()
   {
       return 3*60*1000;
   }
  static int getBufSie()
  {
      return 10000;//10M
  }


};
#endif // Util_H
