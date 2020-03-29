#ifndef Math_class
#define Math_class 

#include <math.h>

template<typename T>
T constexpr _piTemplate = (T)3.14159265358979323846264338327950288419;

class Math {
public:

    static constexpr float piFloat = _piTemplate<float>;
    static constexpr double piDouble = _piTemplate<double>;

    template<typename A,typename B>
    static A max(A value,B max){
        if(value>max){
            return value;
        }
        return A(max);
    };
    
    template<typename A,typename B>
    static A min(A value,B min){
        if(value<min){
            return value;
        }
        return A(min);
    };

    template<typename A,typename B,typename C>
    static A clamp(A value,B min,C max){
        if(value>max){
            return A(max);
        }
        if(value<min){
            return A(min);
        }
        return value;
    };

    static double sqrt(double x) {
      if (x <= 0)
        return 0;       // if negative number throw an exception?
      int exp = 0;
      x = frexp(x, &exp); // extract binary exponent from x
      if (exp & 1) {      // we want exponent to be even
        exp--;
        x *= 2;
      }
      double y = (1 + x) / 2; // first approximation
      double z = 0;
      while (y != z) {    // yes, we CAN compare doubles here!
        z = y;
        y = (y + x / y) / 2;
      }
      return ldexp(y, exp / 2); // multiply answer by 2^(exp/2)
    }

};

#endif