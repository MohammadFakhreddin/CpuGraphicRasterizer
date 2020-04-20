#ifndef MatrixTemplate_class
#define MatrixTemplate_class

#include <assert.h>
#include <vector>
#include <memory>
#include <cmath>

#include "../utils/log/Logger.h"
#include "../utils/math/Math.h"

template <typename T>
class _Matrix {
private:
  unsigned int width;
  unsigned int height;
  unsigned int i = 0;
  unsigned int j = 0;
  unsigned int k = 0;
  unsigned int matrixSize;
  unsigned int rowValue = 0;
  //TODO Vectors are a bit slow try to replace them in future
  //Atlease replace them with 1D equavalant that is much faster
  T* cells;
  T* placeholderCells;

public:

  _Matrix()
    :
    width(0),
    height(0),
    matrixSize(0),
    cells(nullptr),
    placeholderCells(nullptr)
  {};

  //For safty and clarity of code delete all operators and use methods instead
  template <typename A>
  _Matrix(const unsigned int paramWidth,const unsigned int paramHeight,const A& cellDefaultValue)
    :
    width(paramWidth),
    height(paramHeight),
    matrixSize(paramWidth * paramHeight),
    cells(new T[matrixSize]{ cellDefaultValue }),
    placeholderCells(new T[matrixSize])
  {}

  //TODO Change vector to 1D array
  template <typename A>
  _Matrix(const unsigned int paramWidth, const unsigned int paramHeight, const std::vector<std::vector<A>>& initialCellValue)
    :
    width(paramWidth),
    height(paramHeight),
    matrixSize(paramWidth * paramHeight),
    cells(new T[matrixSize]),
    placeholderCells(new T[matrixSize])
  {
    assert(initialCellValue.size() == width);
    for (i = 0; i < width; i++) {
      assert(initialCellValue[i].size() == height);
      rowValue = i * height;
      for (j = 0; j < height; j++) {
        cells[rowValue + j] = initialCellValue[i][j];
      }
    }
  }

  ~_Matrix() {
    delete[] cells;
    delete[] placeholderCells;
  }

  template <typename A>
  _Matrix<A>& operator=(const _Matrix<A>& rhs) = delete;

  template <typename A>
  _Matrix(const _Matrix<A>& other) = delete;

  _Matrix(_Matrix&& other) noexcept {
    cells = new T[other.matrixSize];
    placeholderCells = new T[other.matrixSize];
    width = other.width;
    height = other.height;
    matrixSize = other.matrixSize;
    std::memcpy(cells, other.cells, matrixSize * sizeof(T));
  }; // move constructor

  _Matrix& operator=(const _Matrix& other) = delete;// copy assignment
  
  _Matrix& operator=(_Matrix&& other) = delete; // move assignment
 
  void assign(const _Matrix<T>& rhs) {
    assert(width == rhs.width);
    assert(height == rhs.height);
    std::memcpy(cells, rhs.cells, matrixSize * sizeof(T));
  }

  template <typename A>
  void reset(const unsigned int paramWidth, const unsigned int paramHeight, const A& cellDefaultValue)
  {
    if (cells) {
      delete[] cells;
    }
    if (placeholderCells) {
      delete[] placeholderCells;
    }
    width = paramWidth;
    height = paramHeight;
    matrixSize = paramWidth * paramHeight;
    cells = new T[matrixSize]{ cellDefaultValue };
    placeholderCells = new T[matrixSize];
  }

  template <typename A>
  void sum(const _Matrix<A>& rhs) {
    assert(rhs.getWidth() == width);
    assert(rhs.getHeight() == height);
    for (i = 0; i < matrixSize; i++) {
      cells[i] += T(rhs.getDirect(i));
    }
  }

  template <typename A>
  void operator+=(_Matrix<A> rhs) = delete;

  template <typename A>
  void operator+(_Matrix<A> rhs) = delete;


  template <typename A>
  void minus(const _Matrix<A>& rhs) {
    assert(rhs.getWidth() == width);
    assert(rhs.getHeight() == height);
    for (i = 0; i < matrixSize; i++) {
      cells[i] -= T(rhs.getDirect(i));
    }
  }

  template <typename A>
  void operator-=(_Matrix<A> rhs) = delete;

  template <typename A>
  void operator-(_Matrix<A> rhs) = delete;

  template <typename A>
  void multiply(const _Matrix<A>& rhs) {
    assert(rhs.getHeight() == width);
    assert(rhs.getWidth() == width);
    for (i = 0; i < rhs.getWidth(); i++) {
      for (j = 0; j < height; j++) {
        rowValue = i * height + j;
        placeholderCells[rowValue] = 0;
        for (k = 0; k < width; k++) {
          placeholderCells[rowValue] += cells[k * height + j] * T(rhs.get(i, k));
        }
      }
    }
    std::memcpy(cells, placeholderCells, matrixSize * sizeof(T));
  }

  template<typename A>
  void multiply(const A rhs) {
    for (i = 0; i < matrixSize; i++) {
      cells[i] *= T(rhs);
    }
  }

  template <typename A>
  void operator*=(_Matrix<A> rhs) = delete;

  template <typename A>
  void operator*(_Matrix<A> rhs) = delete;

  bool equal(const _Matrix<T>& rhs) {
    if (rhs.width != width || rhs.height != height) {
      return false;
    }
    for (i = 0; i < width; i++) {
      for (j = 0; j < height; j++) {
        if (rhs.get(i, j) != get(i, j)) {
          return false;
        }
      }
    }
    return true;
  }

  template <typename A>
  bool operator==(_Matrix<A>& rhs) = delete;
  
  bool unEqual(const _Matrix<T>& rhs) {
    return !(this->equal(rhs));
  }

  template <typename A>
  bool operator!=(_Matrix<A>& rhs) = delete;
  
  unsigned int getWidth() const {
    return width;
  }
  
  unsigned int getHeight() const {
    return height;
  }
  
  void print() {
    Logger::log("---Printing matrix----");
    Logger::log("Width:" + std::to_string(width));
    Logger::log("Height:" + std::to_string(height));
    std::string line = "";
    for (i = 0; i < height; i++) {
      line = "";
      for (j = 0; j < width; j++) {
        line += " " + std::to_string(cells[i * height + j]) + " ";
      }
      Logger::log(line);
    }
    Logger::log("-----------------------");
  }
  
  const T& get(const unsigned int& x, const unsigned int& y) const {
    assert(x < width);
    assert(y < height);
    return cells[x * height + y];
  }

  const T& getX() const {
    assert(width > 0);
    assert(height == 1);
    return cells[0];
  }

  const T& getY() const {
    assert(width > 1);
    assert(height == 1);
    return cells[1];
  }

  const T& getZ() const {
    assert(width > 2);
    assert(height == 1);
    return cells[2];
  }

  const T& getR() const {
    assert(width == 3);
    assert(height == 1);
    return cells[0];
  }

  const T& getG() const {
    assert(width == 3);
    assert(height == 1);
    return cells[1];
  }

  const T& getB() const {
    assert(width == 3);
    assert(height == 1);
    return cells[2];
  }

  void setX(const T& value) {
    assert(width > 0);
    assert(height == 1);
    cells[0] = value;
  }

  void setY(const T& value) {
    assert(width > 1);
    assert(height == 1);
    cells[1] = value;
  }

  void setZ(const T& value) {
    assert(width > 2);
    assert(height == 1);
    cells[2] = value;
  }

  void setR(const T& value) {
    assert(width == 3);
    assert(height == 1);
    cells[0] = value;
  }

  void setG(const T& value) {
    assert(width == 3);
    assert(height == 1);
    cells[1] = value;
  }

  void setB(const T& value) {
    assert(width == 3);
    assert(height == 1);
    cells[2] = value;
  }

  const T& getDirect(const unsigned int& index) const {
    assert(index < matrixSize);
    return cells[index];
  }
  
  void set(const unsigned int& x, const unsigned int& y, const T& value) {
    assert(x < width);
    assert(y < height);
    cells[x * height + y] = value;
  }

  void setDirect(const unsigned int& index, const T& value) {
    assert(index < matrixSize);
    cells[index] = value;
  }

  template<typename A, typename B>
  void setXY(const A& x, const B& y) {
    assert(width >= 2);
    assert(height == 1);
    cells[0] = T(x);
    cells[1] = T(y);
  }

  template<typename A, typename B,typename C>
  void setXYZ(const A& x, const B& y,const C& z) {
    assert(width >= 2);
    assert(height == 1);
    cells[0] = T(x);
    cells[1] = T(y);
    cells[2] = T(z);
  }

  static void assignAsRotationXMatrix(_Matrix<T>& matrix, const float& degree) {
    assert(matrix.getWidth() == 3);
    assert(matrix.getHeight() == 3);
    matrix.set(0, 0, 1);
    assert(matrix.get(0, 1) == 0);
    assert(matrix.get(0, 2) == 0);
    assert(matrix.get(1, 0) == 0);
    matrix.set(1, 1, cosf(degree));
    matrix.set(1, 2, sinf(degree));
    assert(matrix.get(2, 0) == 0);
    matrix.set(2, 1, -sinf(degree));
    matrix.set(2, 2, cosf(degree));
  }

  static void assignAsRotationYMatrix(_Matrix<T>& matrix, const float& degree) {
    assert(matrix.getWidth() == 3);
    assert(matrix.getHeight() == 3);
    matrix.set(0, 0, cosf(degree));
    assert(matrix.get(0, 1) == 0);
    matrix.set(0, 2, sinf(degree));
    assert(matrix.get(1, 0) == 0);
    matrix.set(1, 1, 1);
    assert(matrix.get(1, 2) == 0);
    matrix.set(2, 0, -sinf(degree));
    assert(matrix.get(2, 1) == 0);
    matrix.set(2, 2, cosf(degree));
  }

  static void assignAsRotationZMatrix(_Matrix<T>& matrix, const float& degree) {
    matrix.set(0, 0, cosf(degree));
    matrix.set(0, 1, -sinf(degree));
    assert(matrix.get(0, 2) == 0);
    matrix.set(1, 0, sinf(degree));
    matrix.set(1, 1, cosf(degree));
    assert(matrix.get(1, 2) == 0);
    assert(matrix.get(2, 0) == 0);
    assert(matrix.get(2, 1) == 0);
    matrix.set(2, 2, 1);
  }

  static void assignAsRotationXYZMatrix(
    _Matrix<T>& matrix, 
    const float& xDegree, 
    const float& yDegree, 
    const float& zDegree
  ) {
    assert(matrix.getWidth() == 3);
    assert(matrix.getHeight() == 3);
    matrix.set(0, 0, cosf(yDegree) * cosf(zDegree));
    matrix.set(0, 1, cosf(yDegree) * (-sinf(zDegree)));
    matrix.set(0, 2, -sinf(yDegree));
    matrix.set(1, 0, ((-sinf(xDegree)) * sinf(yDegree) * cosf(zDegree)) + (cosf(xDegree) * sinf(zDegree)));
    matrix.set(1, 1, (sinf(xDegree) * sinf(yDegree) * sinf(zDegree)) + (cosf(xDegree) * cosf(zDegree)));
    matrix.set(1, 2, (-sinf(xDegree)) * cosf(yDegree));
    matrix.set(2, 0, (cosf(xDegree) * sinf(yDegree) * cosf(zDegree)) + (sinf(xDegree) * sinf(zDegree)));
    matrix.set(2, 1, (cosf(xDegree) * sinf(yDegree) * (-1 * sinf(zDegree))) + (sinf(xDegree) * cosf(zDegree)));
    matrix.set(2, 2, cosf(xDegree) * cosf(yDegree));
  }

  static void addToTransfromXMatrix(
    _Matrix<T>& matrix,
    const float& x
  ) {
    assert(matrix.getWidth() == 3);
    assert(matrix.getHeight() == 1);
    matrix.setX(matrix.getX() + x);
  }

  static void addToTransfromYMatrix(
    _Matrix<T>& matrix,
    const float& y
  ) {
    assert(matrix.getWidth() == 3);
    assert(matrix.getHeight() == 1);
    matrix.setY(matrix.getY() + y);
  }

  static void addToTransfromZMatrix(
    _Matrix<T>& matrix,
    const float& z
  ) {
    assert(matrix.getWidth() == 3);
    assert(matrix.getHeight() == 1);
    matrix.setZ(matrix.getZ() + z);
  }

  static void addToTransfromXYZMatrix(
    _Matrix<T>& matrix,
    const float& x,
    const float& y,
    const float& z
  ) {
    assert(matrix.getWidth() == 3);
    assert(matrix.getHeight() == 1);
    matrix.setX(matrix.getX() + x);
    matrix.setY(matrix.getY() + y);
    matrix.setZ(matrix.getZ() + z);
  }

  static void addToScaleMatrix(
    _Matrix<T>& matrix, 
    const float& value
  ) {
    assert(matrix.getWidth() == 3);
    assert(matrix.getHeight() == 3);
    matrix.set(0, 0, matrix.get(0, 0) + value);
    assert(matrix.get(0, 1) == 0);
    assert(matrix.get(0, 2) == 0);
    assert(matrix.get(1, 0) == 0);
    matrix.set(1, 1, matrix.get(1, 1) + value);
    assert(matrix.get(1, 2) + value);
    assert(matrix.get(2, 0) + value);
    assert(matrix.get(2, 1) + value);
    matrix.set(2, 2, matrix.get(2, 2) + value);
  }

  template<typename A>
  T dotProduct(const _Matrix<A>& rhs) const {
    assert(this->width == 3 && this->height == 1);
    assert(rhs.getWidth() == 3 && rhs.getHeight() == 1);
    return 
      T((double(this->get(0, 0)) * double(rhs.get(0, 0))) + 
      (double(this->get(1, 0)) * double(rhs.get(1, 0))) + 
      (double(this->get(2, 0)) * double(rhs.get(2, 0))));
  }
  //TODO Write unit tests for project
  template<typename A,typename B>
  void crossProduct(const _Matrix<A>& mat1, const _Matrix<B>& mat2) {
    this->set(0, 0, 
      (T(mat1.get(1, 0)) * T(mat2.get(2, 0)))
      - (T(mat1.get(2, 0)) * T(mat2.get(1, 0)))
    );
    this->set(1, 0,
      (T(mat1.get(2, 0)) * T(mat2.get(0, 0)))
      - (T(mat1.get(0, 0)) * T(mat2.get(2, 0)))
    );
    this->set(2, 0,
      (T(mat1.get(0, 0)) * T(mat2.get(1, 0)))
      - (T(mat1.get(1, 0)) * T(mat2.get(0, 0)))
    );
  }

  template<typename A>
  void hat(_Matrix<A>& matrix) const {
    
    assert(width == 3);
    assert(height == 1);

    assert(matrix.getWidth() == 3);
    assert(matrix.getHeight() == 1);
    
    const A vectorSize = size<A>();
    
    for (unsigned short i = 0; i < 3; i++) {
    
      matrix.set(i, 0, A(this->get(i, 0)) / vectorSize);
    
    }

  }

  template<typename A>
  A squareSize() const {
    assert(width == 3);
    assert(height == 1);
    return A(
      this->get(0, 0) * this->get(0, 0) +
      this->get(1, 0) * this->get(1, 0) +
      this->get(2, 0) * this->get(2, 0)
    );
  }
 
  template<typename A>
  A size() const {
    assert(width == 3);
    assert(height == 1);
    return sqrt(squareSize<A>());
  }

  const unsigned int& getMatrixArraySize() const {
    return matrixSize;
  }

};

using MatrixInt = _Matrix<int>;
using MatrixFloat = _Matrix<float>;
using MatrixDouble = _Matrix<double>;

#endif