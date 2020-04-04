#ifndef MatrixTemplate_class
#define MatrixTemplate_class

#include <assert.h>
#include <vector>
#include <memory>

#include "../utils/log/Logger.h"

template <typename T>
class _Matrix {
private:
  unsigned int width;
  unsigned int height;
  unsigned int i = 0;
  unsigned int j = 0;
  unsigned int k = 0;
  //TODO Vectors are a bit slow try to replace them in future
  std::vector<std::vector<T>> cells;
  std::vector<std::vector<T>> placeholderCells;
public:

  _Matrix() = delete;

  //For safty and clarity of code delete all operators and use methods instead
  template <typename A>
  _Matrix(const unsigned int paramWidth,const unsigned int paramHeight,const A& cellDefaultValue)
    :
    width(paramWidth),
    height(paramHeight)
  {
    for (i = 0; i < width; i++) {
      cells.emplace_back(std::vector<T>());
      placeholderCells.emplace_back(std::vector<T>());
      for (j = 0; j < height; j++) {
        cells.at(i).emplace_back(T(cellDefaultValue));
        placeholderCells.at(i).emplace_back(T(0));
      }
      assert(cells.at(i).size() == placeholderCells.at(i).size());
      assert(cells.at(i).size() == height);
    }
    assert(cells.size() == placeholderCells.size());
    assert(cells.size() == width);
  }
  
  template <typename A>
  _Matrix(const unsigned int paramWidth, const unsigned int paramHeight, const std::vector<std::vector<A>>& initialCellValue)
    :
    width(paramWidth),
    height(paramHeight)
  {
    assert(initialCellValue.size() == width);
    for (i = 0; i < width; i++) {
      assert(initialCellValue.at(i).size() == height);
      cells.emplace_back(std::vector<T>());
      placeholderCells.emplace_back(std::vector<T>());
      for (j = 0; j < height; j++) {
        placeholderCells.at(i).emplace_back(T(0));
        cells.at(i).emplace_back(initialCellValue.at(i).at(j));
      }
      assert(cells.at(i).size() == placeholderCells.at(i).size());
      assert(cells.at(i).size() == height);
    }
    assert(cells.size() == placeholderCells.size());
    assert(cells.size() == width);
  }

  template <typename A>
  void assign(const _Matrix<A>& rhs) {
    assert(width == rhs.getWidth());
    assert(height == rhs.getHeight());
    for (i = 0; i < width; i++) {
      for (j = 0; j < height; j++) {
        cells.at(i).at(j) = T(rhs.get(i, j));
      }
    }
  }

  template <typename A>
  void operator=(_Matrix<A> rhs) = delete;

  template <typename A>
  void sum(const _Matrix<A>& rhs) {
    assert(rhs.getWidth() == width);
    assert(rhs.getHeight() == height);
    for (i = 0; i < width; i++) {
      for (j = 0; j < height; j++) {
        cells.at(i).at(j) += T(rhs.get(i, j));
      }
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
    for (i = 0; i < width; i++) {
      for (j = 0; j < height; j++) {
        cells.at(i).at(j) -= T(rhs.get(i, j));
      }
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
        placeholderCells.at(i).at(j) = 0;
        for (k = 0; k < width; k++) {
          placeholderCells.at(i).at(j) += cells.at(k).at(j) * T(rhs.get(i, k));
        }
      }
    }
    for (i = 0; i < width; i++) {
      for (j = 0; j < height; j++) {
        cells.at(i).at(j) = placeholderCells.at(i).at(j);
      }
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
        line += " " + std::to_string(cells.at(j).at(i)) + " ";
      }
      Logger::log(line);
    }
    Logger::log("-----------------------");
  }
  
  const T& get(const unsigned int& x, const unsigned int& y) const {
    assert(x < width);
    assert(y < height);
    return this->cells.at(x).at(y);
  }
  
  void set(const unsigned int& x, const unsigned int& y, const T& value) {
    assert(x < width);
    assert(y < height);
    this->cells.at(x).at(y) = value;
  }

  _Matrix<T> clone() {
    std::vector<std::vector<T>> cellsCopy;
    for (i = 0; i < width; i++) {
      cellsCopy.emplace_back(std::vector<T>());
      for (j = 0; j < height; j++) {
        cellsCopy.at(i).emplace_back(cells.at(i).at(j));
      }
    }
    return _Matrix<T>(width, height, cellsCopy);
  }

  static void assignAsRotationXMatrix(_Matrix<T>& matrix, const float& degree) {
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

  static void assignAsRoationYMatrix(_Matrix<T>& matrix, const float& degree) {
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

  template<typename A>
  double dotProduct(_Matrix<A>& rhs) {
    assert(this->width == 3 && this->height == 1);
    assert(rhs.width == 3 && rhs.height == 1);
    return 
      (double(this->get(0, 0)) * double(rhs.get(0, 0))) + 
      (double(this->get(1, 0)) * double(rhs.get(1, 0))) + 
      (double(this->get(2, 0)) * double(rhs.get(2, 0)));
  }
  //TODO Write unit tests for project
  template<typename A,typename B>
  void crossPoduct(_Matrix<A>& mat1, _Matrix<B>& mat2) {
    this->set(0, 0, 
      (T(mat1.get(1, 0)) * T(mat2.get(2, 0)))
      - (T(mat1.get(2, 0)) * T(mat2.get(1, 0)))
    );
    this->set(1, 0,
      (T(mat1.get(2, 0)) * T(mat2.get(0, 0)))
      - (T(mat1.get(0, 0)) * T(mat2.get(2, 0)))
    );
    this->set(2, 0
      (T(mat1.get(0, 0)) * T(mat2.get(1, 0)))
      - (T(mat1.get(1, 0)) * T(mat2.get(0, 0)))
    );
  }

};

using MatrixInt = _Matrix<int>;
using MatrixFloat = _Matrix<float>;
using MatrixDouble = _Matrix<double>;

#endif