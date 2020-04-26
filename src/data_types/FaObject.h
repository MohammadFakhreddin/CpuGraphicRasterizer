#ifndef FaObject_class
#define FaObject_class

template<typename A>
class FaObject {
public:
    FaObject() = default;
    FaObject(A value):value(value){}
    FaObject& operator=(FaObject& object){
        this->value = object.getValue();
        return *this;
    }
    FaObject& operator=(A value){
        this->value = value;
        return *this;
    }
    template<typename T>
    FaObject& operator=(T value){
        this->value = A(value);
        return *this;
    }
    A value;
};

#endif