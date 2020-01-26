#pragma once
template <typename T>
class _Vec2d {
private:
	T x;
	T y;
public:
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
	const double size() const {
		return sqrt(this->x * this->x + this->y * this->y);
	}
	_Vec2d<double> hat() {
		const double size = this->size();
		return _Vec2d<double>(double(this->x) / size, double(this->y) / size);
	}
	_Vec2d& operator+=(const _Vec2d<T>& rhs) {
		this->x += rhs.getX();
		this->y += rhs.getY();
		return *this;
	}
	_Vec2d<T> operator+(const _Vec2d<T>& rhs) {
		return _Vec2d<T>(this->x + rhs.getX(), this->y + rhs.getY());
	}
	_Vec2d<T> operator-(const _Vec2d<T>& rhs) const {
		return _Vec2d<T>(this->x - rhs.getX(), this->y - rhs.getY());
	}
	double operator*(const _Vec2d<T>& rhs) {
		return double(this->x) * double(rhs.getX()) + double(this->y) * double(rhs.getY());
	}
	_Vec2d<T>& operator*(const int value) {
		this->x *= value;
		this->y *= value;
		return *this;
	}
	_Vec2d<T>& operator=(const _Vec2d<T>& rhs) {
		this->x = rhs.x;
		this->y = rhs.y;
		return *this;
	}
	template <typename A>
	explicit operator _Vec2d<A>() const {
		return _Vec2d<A>(A(this->x), A(this->y));
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
	void setX(T x) {
		this->x = x;
	}
	void setY(T y) {
		this->y = y;
	}
	void setZ(T z) {
		this->z = z;
	}
	const double size() const {
		return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
	}
	_Vec3d<double> hat() {
		const double size = this->size();
		return _Vec3d<double>(double(this->x) / size, double(this->y) / size, double(this->z) / size);
	}
	_Vec3d& operator+=(const _Vec3d<T>& rhs) {
		this->x += rhs.getX();
		this->y += rhs.getY();
		this->z += rhs.getZ();
		return *this;
	}
	_Vec3d<T> operator+(const _Vec3d<T>& rhs) {
		return _Vec2d<T>(this->x + rhs.getX(), this->y + rhs.getY(), this->z + rhs.getZ());
	}
	_Vec3d<T> operator-(const _Vec3d<T>& rhs) const {
		return _Vec3d<T>(this->x - rhs.getX(), this->y - rhs.getY(), this->z - rhs.getZ());
	}
	double operator*(const _Vec3d<T>& rhs) {
		return double(this->x) * double(rhs.getX()) + double(this->y) * double(rhs.getY() + double(this->z) * double(this->z));
	}
	_Vec3d<T>& operator*(const int value) {
		this->x *= value;
		this->y *= value;
		this->z *= value;
		return *this;
	}
	_Vec3d<T>& operator=(const _Vec3d<T>& rhs) {
		this->x = rhs.x;
		this->y = rhs.y;
		this->z = rhs.z;
		return *this;
	}
	template <typename A>
	explicit operator _Vec3d<A>() const {
		return _Vec3d<A>(A(this->x), A(this->y), A(this->z));
	}
};

using Vec3DInt = _Vec3d<int>;
using Vec3DFloat = _Vec3d<float>;
using Vec3DDouble = _Vec3d<double>;
