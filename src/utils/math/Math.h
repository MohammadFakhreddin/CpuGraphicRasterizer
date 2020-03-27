#ifndef Math_class
#define Math_class 

template<typename T>
T constexpr _piTemplate = (T)3.14159265358979323846264338327950288419;

class Math {
public:

    static constexpr float piFloat = _piTemplate<float>;
    static constexpr double piDouble = _piTemplate<double>;

    template<typename A,typename B>
    static A max(A value,B max){
        if(value>max){
            return A(max);
        }
        return value;
    };
    template<typename A,typename B>
    static A min(A value,B max){
        if(value<min){
            return A(min);
        }
        return value;
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

};

#endif