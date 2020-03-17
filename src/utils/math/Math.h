class Math {
public:
    template<typename A>
    static A max(A value,A max){
        if(value>max){
            return max;
        }
        return value;
    };
    template<typename A>
    static A min(A value,A max){
        if(value<min){
            return min;
        }
        return value;
    };
    template<typename A>
    static A clamp(A value,A min,A max){
        if(value>max){
            return max;
        }
        if(value<min){
            return min;
        }
        return value;
    };
};