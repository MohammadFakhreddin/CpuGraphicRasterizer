/*#ifndef MatrixTemplate_class
#define MatrixTemplate_class

#include <assert.h>
#include <vector>
#include <memory>

#include "../utils/log/Logger.h";
//TODO Write unit test for this class
template <typename T>
class _Matrix{
  
private:

    unsigned int width;
    
    unsigned int height;
    
    T** cells;
    
    unsigned int i = 0, j = 0, k = 0, p = 0;
    
    T** reserveCells;
   
public:
    
    template <typename A>
    _Matrix(unsigned int width,unsigned int height,A cellDefaultValue)
      :
      width(width),
      height(height)
    {
      cells = new T*[width];
      reserveCells = new T*[width];
      for(i=0;i<width;i++){
        cells[i] = new T[height];
        reserveCells[i] = new T[height];
        for(j=0;j<height;j++){
          cells[i][j] = cellDefaultValue;
          reserveCells[i][j] = T(0);
        }
      }
    }
    
    template <typename A>
    _Matrix(unsigned int width,unsigned int height,std::vector<std::vector<A>> initialCellValue)
      :
      width(width),
      height(height)
    {
      cells = new T*[width];
      reserveCells = new T*[width];
      for (i = 0; i < width; i++) {
        cells[i] = new T[height];
        reserveCells[i] = new T[height];
        for (j = 0; j < height; j++) {
          cells[i][j] = T(initialCellValue.at(i).at(j));
          reserveCells[i][j] = T(0);
        }
      }
    }
    
    ~_Matrix() {
      if (cells && reserveCells) {
        for (i = 0; i < width; i++) {
          if (cells[i] && reserveCells[i]) {
            delete[] cells[i];
            delete[] reserveCells[i];
          }
        }
        delete[] cells;
        delete[] reserveCells;
      }
    }
    
    template <typename A>
    _Matrix<T>& operator=(_Matrix<A> rhs){
      assert(width = rhs.width && "Assignment no longer supports assigning object with diffrent width or height");
      assert(height = rhs.height && "Assignment no longer supports assigning object with diffrent width or height");
      for(i=0;i<width;i++){
        for(j=0;j<height;j++){
          cells[i][j] = T(rhs.cells[i][j]);
        }
      }
      return *this;
    }
    template <typename A>
    _Matrix<T> operator+(_Matrix<A> rhs){
      assert(rhs.width==width);
      assert(rhs.height==height);
      std::vector<std::vector<T>> resultsCell;
      for(i=0;i<width;i++){
        resultsCell.emplace_back(std::vector<T>());
        for(j=0;j<height;j++){
          resultsCell.at(i).emplace_back(T(rhs.cells[i][j]) + cells[i][j]);
        }
      }
      _Matrix<T> result(width,height,resultsCell);
      return result;
    }
    template <typename A>
    _Matrix<T>& operator+=(_Matrix<A> rhs){
      assert(rhs.width == width);
      assert(rhs.height == height);
      for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
          cells[i][j] += T(rhs.cells[i][j]);
        }
      }
      return *this;
    }
    template <typename A>
    _Matrix<T> operator-(_Matrix<A> rhs){
      assert(rhs.width==width);
      assert(rhs.height==height);
      std::vector<std::vector<T>> resultsCell;
      for(int i=0;i<width;i++){
        resultsCell.emplace_back(std::vector<T>());
        for(int j=0;j<height;j++){
          resultsCell.at(i).emplace_back(cells[i][j] - T(rhs.cells[i][j]));
        }
      }
      _Matrix<T> result(width,height,resultsCell);
      return result;
    }
    template <typename A>
    _Matrix<T>& operator-=(_Matrix<A> rhs){
      assert(rhs.width == width);
      assert(rhs.height == height);
      for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
          cells[i][j] -= T(rhs.cells[i][j]);
        }
      }
      return *this;
    }
    template <typename A>
    _Matrix<T> operator*(_Matrix<A> rhs){
      assert(rhs.height == width);
      auto finalWidth = rhs.width;
      auto finalHeight = height;
      std::vector<std::vector<T>> resultCells;
      for(i=0;i<finalWidth;i++){
        resultCells.emplace_back(std::vector<T>());
         for(j=0;j<finalHeight;j++){
          resultCells.at(i).emplace_back(0);
          for(k=0,p=0;k<width;k++,p++){
            resultCells.at(i).at(j) += cells[k][j] * T(rhs.cells[i][p]); 
          }
        }
      }
      _Matrix<T> resultMatrix(finalWidth,finalHeight,resultCells);
      return resultMatrix;
    }
    template <typename A>
    void operator*=(_Matrix<A> rhs){
      //For multiplition
      assert(rhs.height == width && "For *= height of rhs must be equal to width of lhs,Also final matrix size must be equal to result matrix");
      //For assigning to same matrix
      assert(rhs.width == width && "For *= height of rhs must be equal to width of lhs,Also final matrix size must be equal to result matrix");
      for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
          reserveCells[i][j] = 0;
          for (k = 0, p = 0; k < width; k++, p++) {
            reserveCells[i][j] += cells[k][j] * T(rhs.cells[i][p]);
          }
        }
      }
      for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
          cells[i][j] = reserveCells[i][j];
        }
      }
    }
    bool operator==(_Matrix<T> rhs){
      if(rhs.width!=width || rhs.height!=height){
        return false;
      }
      for(i=0;i<width;i++){
        for(j=0;j<height;j++){
          if(rhs.get(i,j)!=get(i,j)){
            return false;
          }
        }
      }
      return true;
    }
    bool operator!=(_Matrix<T> rhs){
      return !(this->operator==(rhs));
    }
    int getWidth() const{
      return width;
    }
    int getHeight() const{
      return height;
    }
    void print(){
      Logger::log("---Printing matrix----");
      Logger::log("Width:"+std::to_string(width));
      Logger::log("Height:"+std::to_string(height));
      std::string line = "";
      for(i=0;i<height;i++){
        line = "";
        for(j=0;j<width;j++){
          line += " " + std::to_string(cells[j][i]) + " ";
        }
        Logger::log(line);
      }
      Logger::log("-----------------------");
    }
    T get(int x,int y) const {
      assert(x<width && "Get method in MatrixTemplate");
      assert(y<height && "Get method in MatrixTemplate");
      assert(x >= 0 && "Get method in MatrixTemplate");
      assert(y >= 0 && "Get method in MatrixTemplate");
      return this->cells[x][y];
    }
    void set(int x,int y,T value){
      assert(x<width && "Set method in MatrixTemplate");
      assert(y<height && "Set method in MatrixTemplate");
      assert(x >= 0 && "Set method in MatrixTemplate");
      assert(y >= 0 && "Set method in MatrixTemplate");
      this->cells[x][y] = value;
    }
    _Matrix<T> clone(){
      std::vector<std::vector<T>> cellsCopy;
      int i,j;
      for(i=0;i<width;i++){
        cellsCopy.emplace_back(std::vector<T>());
        for(j=0;j<height;j++){
          cellsCopy.at(i).emplace_back(cells[i][j]);
        }
      }
      return _Matrix<T>(width,height,cellsCopy);
    }
};

using MatrixInt = _Matrix<int>;
using MatrixFloat = _Matrix<float>;
using MatrixDouble = _Matrix<double>;

#endif
*/

#ifndef MatrixTemplate_class
#define MatrixTemplate_class

#include <assert.h>
#include <vector>
#include <memory>

#include "../utils/log/Logger.h";

template <typename T>
class _Matrix {
private:
  int width;
  int height;
  std::vector<std::vector<T>> cells;
public:
  template <typename A>
  _Matrix(int width, int height, A cellDefaultValue)
    :
    width(width),
    height(height)
  {
    int i, j;
    for (i = 0; i < width; i++) {
      cells.emplace_back(std::vector<T>());
      for (j = 0; j < height; j++) {
        cells.at(i).emplace_back(T(cellDefaultValue));
      }
    }
  }
  template <typename A>
  _Matrix(int width, int height, std::vector<std::vector<A>> initialCellValue)
    :
    width(width),
    height(height)
  {
    this->cells = initialCellValue;
  }
  template <typename A>
  _Matrix<T>& operator=(_Matrix<A> rhs) {
    width = rhs.width;
    height = rhs.height;
    cells.erase(cells.begin(), cells.end());
    for (int i = 0; i < width; i++) {
      cells.emplace_back(std::vector<T>());
      for (int j = 0; j < height; j++) {
        cells.at(i).emplace_back(T(rhs.cells.at(i).at(j)));
      }
    }
    return *this;
  }
  template <typename A>
  _Matrix<T> operator+(_Matrix<A> rhs) {
    assert(rhs.width == width);
    assert(rhs.height == height);
    std::vector<std::vector<T>> resultsCell;
    for (int i = 0; i < width; i++) {
      resultsCell.emplace_back(std::vector<T>());
      for (int j = 0; j < height; j++) {
        resultsCell.at(i).emplace_back(T(rhs.cells.at(i).at(j)) + cells.at(i).at(j));
      }
    }
    _Matrix<T> result(width, height, resultsCell);
    return result;
  }
  template <typename A>
  _Matrix<T>& operator+=(_Matrix<A> rhs) {
    *this = *this + rhs;
    return *this;
  }
  template <typename A>
  _Matrix<T> operator-(_Matrix<A> rhs) {
    assert(rhs.width == width);
    assert(rhs.height == height);
    std::vector<std::vector<T>> resultsCell;
    for (int i = 0; i < width; i++) {
      resultsCell.emplace_back(std::vector<T>());
      for (int j = 0; j < height; j++) {
        resultsCell.at(i).emplace_back(cells.at(i).at(j) - T(rhs.cells.at(i).at(j)));
      }
    }
    _Matrix<T> result(width, height, resultsCell);
    return result;
  }
  template <typename A>
  _Matrix<T>& operator-=(_Matrix<A> rhs) {
    *this = *this - rhs;
    return *this;
  }
  template <typename A>
  _Matrix<T> operator*(_Matrix<A> rhs) {
    assert(rhs.height == width);
    auto finalWidth = rhs.width;
    auto finalHeight = height;
    std::vector<std::vector<T>> resultCells;
    int i, j, k, p;
    for (i = 0; i < finalWidth; i++) {
      resultCells.emplace_back(std::vector<T>());
      for (j = 0; j < finalHeight; j++) {
        resultCells.at(i).emplace_back(0);
        for (k = 0, p = 0; k < width; k++, p++) {
          resultCells.at(i).at(j) += cells.at(k).at(j) * T(rhs.cells.at(i).at(p));
        }
      }
    }
    _Matrix<T> resultMatrix(finalWidth, finalHeight, resultCells);
    return resultMatrix;
  }
  template <typename A>
  _Matrix<T>& operator*=(_Matrix<A> rhs) {
    *this = (*this) * rhs;
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
    return !(this == rhs);
  }
  int getWidth() const {
    return width;
  }
  int getHeight() const {
    return height;
  }
  void print() {
    Logger::log("---Printing matrix----");
    Logger::log("Width:" << width);
    Logger::log("Height:" << height);
    std::string line = "";
    for (int i = 0; i < height; i++) {
      line = "";
      for (int j = 0; j < width; j++) {
        line += " " << cells.at(j).at(i) << " ";
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