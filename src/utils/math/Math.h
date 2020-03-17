class Math {
public:
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