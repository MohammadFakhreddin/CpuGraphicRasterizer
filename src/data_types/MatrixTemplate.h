#ifndef MatrixTemplate_class
#define MatrixTemplate_class

#include <assert.h>
#include <vector>
#include <memory>

#include "../utils/log/Logger.h";

template <typename T>
class _Matrix {
private:
  unsigned int width;
  unsigned int height;
  unsigned int i = 0;
  unsigned int j = 0;
  unsigned int k = 0;
  unsigned int p = 0;
  std::vector<std::vector<T>> cells;
  std::vector<std::vector<T>> placeholderCells;
public:
  template <typename A>
  _Matrix(unsigned int paramWidth,unsigned int paramHeight, A cellDefaultValue)
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
  _Matrix(unsigned int paramWidth, unsigned int paramHeight, std::vector<std::vector<A>> initialCellValue)
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
  _Matrix<T>& operator=(_Matrix<A> rhs) {
    assert(width == rhs.getWidth());
    assert(height == rhs.getHeight());
    cells.erase(cells.begin(), cells.end());
    for (i = 0; i < width; i++) {
      cells.emplace_back(std::vector<T>());
      for (j = 0; j < height; j++) {
        cells.at(i).emplace_back(T(rhs.get(i,j)));
      }
    }
    return *this;
  }
  template <typename A>
  _Matrix<T> operator+(_Matrix<A> rhs) {
    assert(rhs.getWidth() == width);
    assert(rhs.getHeight() == height);
    std::vector<std::vector<T>> resultsCell;
    for (i = 0; i < width; i++) {
      resultsCell.emplace_back(std::vector<T>());
      for (j = 0; j < height; j++) {
        resultsCell.at(i).emplace_back(T(rhs.get(i,j)) + cells.at(i).at(j));
      }
    }
    _Matrix<T> result(width, height, resultsCell);
    return result;
  }
  template <typename A>
  _Matrix<T>& operator+=(_Matrix<A> rhs) {
    assert(rhs.getWidth() == width);
    assert(rhs.getHeight() == height);
    for (i = 0; i < width; i++) {
      for (j = 0; j < height; j++) {
        cells.at(i).at(j) += T(rhs.get(i, j));
      }
    }
    return *this;
  }
  template <typename A>
  _Matrix<T> operator-(_Matrix<A> rhs) {
    assert(rhs.getWidth() == width);
    assert(rhs.getHeight() == height);
    std::vector<std::vector<T>> resultsCell;
    for (i = 0; i < width; i++) {
      resultsCell.emplace_back(std::vector<T>());
      for (j = 0; j < height; j++) {
        resultsCell.at(i).emplace_back(cells.at(i).at(j) - T(rhs.get(i,j)));
      }
    }
    _Matrix<T> result(width, height, resultsCell);
    return result;
  }
  template <typename A>
  _Matrix<T>& operator-=(_Matrix<A> rhs) {
    assert(rhs.getWidth() == width);
    assert(rhs.getHeight() == height);
    for (int i = 0; i < width; i++) {
      for (int j = 0; j < height; j++) {
        cells.at(i).at(j) -= T(rhs.get(i, j));
      }
    }
    return *this;
  }
  template <typename A>
  _Matrix<T> operator*(_Matrix<A> rhs) {
    assert(rhs.getHeight() == width);
    auto finalWidth = rhs.getWidth();
    auto finalHeight = height;
    std::vector<std::vector<T>> resultCells;
    for (i = 0; i < finalWidth; i++) {
      resultCells.emplace_back(std::vector<T>());
      for (j = 0; j < finalHeight; j++) {
        resultCells.at(i).emplace_back(0);
        for (k = 0, p = 0; k < width; k++, p++) {
          resultCells.at(i).at(j) += cells.at(k).at(j) * T(rhs.get(i,p));
        }
      }
    }
    _Matrix<T> resultMatrix(finalWidth, finalHeight, resultCells);
    return resultMatrix;
  }
  template <typename A>
  _Matrix<T>& operator*=(_Matrix<A> rhs) {
    assert(rhs.getHeight() == width);
    assert(rhs.getWidth() == width);
    for (i = 0; i < rhs.getWidth(); i++) {
      for (j = 0; j < height; j++) {
        placeholderCells.at(i).at(j) = 0;
        for (k = 0, p = 0; k < width; k++, p++) {
          placeholderCells.at(i).at(j) += cells.at(k).at(j) * T(rhs.get(i, p));
        }
      }
    }
    for (i = 0; i < width; i++) {
      for (j = 0; j < height; j++) {
        cells.at(i).at(j) = placeholderCells.at(i).at(j);
      }
    }
    return *this;
  }
  bool operator==(_Matrix<T> rhs) {
    if (rhs.width != width || rhs.height != height) {
      return false;
    }
    for (int i = 0; i < width; i++) {
      for (int j = 0; j < height; j++) {
        if (rhs.get(i, j) != get(i, j)) {
          return false;
        }
      }
    }
    return true;
  }
  bool operator!=(_Matrix<T> rhs) {
    return !(this->operator==(rhs));
  }
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
    for (int i = 0; i < height; i++) {
      line = "";
      for (int j = 0; j < width; j++) {
        line += " " + std::to_string(cells.at(j).at(i)) + " ";
      }
      Logger::log(line);
    }
    Logger::log("-----------------------");
  }
  T get(int x, int y) const {
    assert(x < width);
    assert(y < height);
    return this->cells.at(x).at(y);
  }
  void set(int x, int y, T value) {
    assert(x < width);
    assert(y < height);
    this->cells.at(x).at(y) = value;
  }
  _Matrix<T> clone() {
    std::vector<std::vector<T>> cellsCopy;
    int i, j;
    for (i = 0; i < width; i++) {
      cellsCopy.emplace_back(std::vector<T>());
      for (j = 0; j < height; j++) {
        cellsCopy.at(i).emplace_back(cells.at(i).at(j));
      }
    }
    return _Matrix<T>(width, height, cellsCopy);
  }
};

using MatrixInt = _Matrix<int>;
using MatrixFloat = _Matrix<float>;
using MatrixDouble = _Matrix<double>;

#endif