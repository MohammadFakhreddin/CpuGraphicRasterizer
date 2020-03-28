#ifndef VectorTemplate_class
#define VectorTemplate_class

#include "../utils/log/Logger.h"

//TODO Add N factor as argument

template <typename T>
class _Vec2d {
private:
	T x;
	T y;
public:
	_Vec2d() = default;
	_Vec2d(T x, T y)
		:
		x(x),
		y(y)
	{}
	const T getX() const {
		return this->x;
	};
	const T getY() const {
		return this->y;
	};
	void setX(T x) {
		this->x = x;
	}
	void setY(T y) {
		this->y = y;
	}
	//TODO Find a better solution because sqrt is a slow function
	const double size() const {
		return sqrt(this->x * this->x + this->y * this->y);
	}
	_Vec2d<double> hat() {
		const double size = this->size();
		return _Vec2d<double>(double(this->x) / size, double(this->y) / size);
	}
	template <typename A>
	_Vec2d& operator+=(const _Vec2d<A>& rhs) {
		this->x += T(rhs.getX());
		this->y += T(rhs.getY());
		return *this;
	}
	template <typename A>
	_Vec2d<T> operator+(const _Vec2d<A>& rhs) {
		return _Vec2d<T>(this->x + T(rhs.getX()), this->y + T(rhs.getY()));
	}
	template <typename A>
	_Vec2d<T> operator-(_Vec2d<A>& rhs) {
		return _Vec2d<T>(this->x - T(rhs.getX()), this->y - T(rhs.getY()));
	}
	template <typename A>
	_Vec2d<T>& operator-=(_Vec2d<A>& rhs) {
		this->x -= T(rhs.getX()); 
		this->y -= T(rhs.getY());
	}
	template <typename A>
	_Vec2d<T>& operator*=(const A value) {
		this->x *= T(value);
		this->y *= T(value);
		return *this;
	}
	template <typename A>
	_Vec2d<T> operator*(const A value){
		return _Vec2d<T>(
			this->x * T(value),
			this->y * T(value)
		);
	}
	template <typename A>
	_Vec2d<T>& operator=(const _Vec2d<A>& rhs) {
		this->x = T(rhs.x);
		this->y = T(rhs.y);
		return *this;
	}
	template <typename A>
	explicit operator _Vec2d<A>() const {
		return _Vec2d<A>(A(this->x), A(this->y));
	}
	void print(){
		Logger::log("Vector ---------------------");
		Logger::log("X:"+std::to_string(x));
		Logger::log("Y:"+std::to_string(y));
		Logger::log("End ------------------------");
	}
};

using Vec2DInt = _Vec2d<int>;
using Vec2DFloat = _Vec2d<float>;
using Vec2DDouble = _Vec2d<double>;

template <typename T>
class _Vec3d {
private:
	T x;
	T y;
	T z;
public:
	_Vec3d() = default;
	_Vec3d(T x, T y, T z)
		:
		x(x),
		y(y),
		z(z)
	{}
	const T getX() const {
		return this->x;
	};
	const T getY() const {
		return this->y;
	};
	const T getZ() const {
		return this->z;
	};
	template <typename A>
	void setX(A x) {
		this->x = T(x);
	}
	template <typename A>
	void setY(A y) {
		this->y = T(y);
	}
	template <typename A>
	void setZ(A z) {
		this->z = T(z);
	}
	const double size() const {
		return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
	}
	_Vec3d<double> hat() {
		const double size = this->size();
		return _Vec3d<double>(double(this->x) / size, double(this->y) / size, double(this->z) / size);
	}
	template <typename A>
	_Vec3d<T>& operator+=(const _Vec3d<A>& rhs) {
		this->x += T(rhs.getX());
		this->y += T(rhs.getY());
		this->z += T(rhs.getZ());
		return *this;
	}
	template <typename A>
	_Vec3d<T> operator+(const _Vec3d<A>& rhs) {
		return _Vec3d<T>(this->x + T(rhs.getX()), this->y + T(rhs.getY()), this->z + T(rhs.getZ()));
	}
	template <typename A>
	_Vec3d<T>& operator-=(const _Vec3d<A>& rhs) const {
		this->x -= T(rhs.getX()); 
		this->y -= T(rhs.getY()); 
		this->z -= T(rhs.getZ());
	}
	template <typename A>
	_Vec3d<T> operator-(const _Vec3d<A>& rhs) const {
		return _Vec3d<T>(this->x - T(rhs.getX()), this->y - T(rhs.getY()), this->z - T(rhs.getZ()));
	}
	template <typename A>
	_Vec3d<T>& operator*=(const A value) {
		this->x *= T(value);
		this->y *= T(value);
		this->z *= T(value);
		return *this;
	}
	template <typename A>
	_Vec3d<T> operator*(const A value) {
		return _Vec3d<T>(
			this->x * T(value),
			this->y * T(value),
			this->z * T(value)
		);
	}
	template <typename A>
	_Vec3d<T>& operator=(const _Vec3d<A>& rhs) {
		this->x = T(rhs.x);
		this->y = T(rhs.y);
		this->z = T(rhs.z);
		return *this;
	}
	template <typename A>
	explicit operator _Vec3d<A>() const {
		return _Vec3d<A>(A(this->x), A(this->y), A(this->z));
	}
	template <typename A,typename B>
	_Vec3d<T>& crossProduct(_Vec3d<A>& vec1,_Vec3d<B>& vec2){
		x = T(vec1.y) * T(vec2.z) - T(vec1.z) * T(vec2.y),
		y = T(vec1.z) * T(vec2.x) - T(vec1.x) * T(vec2.z),
		z = T(vec1.x) * T(vec2.y) - T(vec1.y) * T(vec2.x);
		return *this;
	}
	template <typename A>
	T dotProduct(_Vec3d<A>& rhs){
		return (this->x * T(rhs.x)) + (this->y * T(rhs.y)) + (this->z * T(rhs.z));
	}
	template <typename A>
	_Vec3d<T> crossProduct(_Vec3d<A>& rhs){
		_Vec3d<T> result;
		result.crossProduct(this,rhs);
		return result;
	}
	template <typename A>
	_Vec3d<T>& multiplyOneByOne(_Vec3d<A>& rhs){
		this->x *= rhs.x;
		this->y *= rhs.y;
		this->z *= rhs.z;
		return *this;
	}
	void print(){
		Logger::log(
			"Vector ---------------------"
			"X:"+std::to_string(x)+
			"Y:"+std::to_string(y)+
			"Z:"+std::to_string(z)
		);
	}
};

using Vec3DInt = _Vec3d<int>;
using Vec3DFloat = _Vec3d<float>;
using Vec3DDouble = _Vec3d<double>;

#endif