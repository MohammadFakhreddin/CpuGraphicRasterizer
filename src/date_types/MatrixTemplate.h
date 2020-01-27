#pragma once

#include <assert.h>
#include <iostream>

template <typename T>
class _Matrix{
  private:
    int width;
    int height;
    T** cells;
  public:
    ~_Matrix()
    {
      delete cells;
    }
    _Matrix(int width,int height,T cellDefaultValue)
      :
      width(width),
      height(height)
    {
      this->cells = new T*[width];
      int i,j;
      for(i=0;i<width;i++){
        this->cells[i] = new T[height];
        for(j=0;j<height;j++){
          this->cells[i][j] = cellDefaultValue;
        }
      }
    }
    _Matrix(int width,int height,T** initialCellValue)
      :
      width(width),
      height(height)
    {
      this->cells = new T*[width];
      int i,j;
      for(i=0;i<width;i++){
        this->cells[i] = new T[height];
        for(j=0;j<height;j++){
          this->cells[i][j] = initialCellValue[i][j];
        }
      }
      delete initialCellValue;
    }
    _Matrix<T>& operator=(_Matrix<T> rhs){
      width = rhs.width;
      height = rhs.height;
      delete cells;
      cells = new T*[width];
      for(int i=0;i<width;i++){
        cells[i] = new T[height];
        for(int j=0;j<height;j++){
          cells[i][j] = rhs.cells[i][j];
        }
      }
      return *this;
    }
    _Matrix<T> operator+(_Matrix<T> rhs){
      assert(rhs.width==width);
      assert(rhs.height==height);
      T** resultsCell = new T*[width];
      for(int i=0;i<width;i++){
        resultsCell[width] = new T[height];
        for(int j=0;j<height;j++){
          resultsCell[i][j] = rhs.cells[i][j] + cells[i][j];
        }
      }
      _Matrix<T> result(width,height,resultsCell);
      return result;
    }
    _Matrix<T>& operator+=(_Matrix<T> rhs){
      assert(rhs.width==width);
      assert(rhs.height==height);
      for(int i=0;i<width;i++){
        for(int j=0;j<height;j++){
          cells[i][j] += rhs.cells[i][j];
        }
      }
      return *this;
    }
    _Matrix<T> operator-(_Matrix<T> rhs){
      return this + -1*rhs;
    }
    _Matrix<T> operator-=(_Matrix<T> rhs){
      this += -1 * rhs;
      return this;
    }
    _Matrix<T> operator*(_Matrix<T> rhs){
      assert(rhs.height==width);
      auto finalWidth = rhs.width;
      auto finalHeight = height;
      T** resultCells = new T*[finalWidth];
      int i,j,k,p; 
      for(i=0;i<finalWidth;i++){
        resultCells[i] = new T[finalHeight];
         for(j=0;j<finalHeight;j++){
          resultCells[i][j] = 0;
          for(k=0,p=0;k<width;k++,p++){
            resultCells[i][j] += cells[k][j] * rhs.cells[i][p]; 
          }
        }
      }
      _Matrix<T> resultMatrix(finalWidth,finalHeight,resultCells);
      return resultMatrix;
    }
    int getWidth(){
      return width;
    }
    int getHeight(){
      return height;
    }
    void print(){
      std::cout<<"---Printing matrix----"<<std::endl;
      std::cout<<"Width:"<<width<<std::endl;
      std::cout<<"Height:"<<height<<std::endl;
      for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
          std::cout<<" "<<cells[j][i]<<" ";
        }
        std::cout<<std::endl;
      }
      std::cout<<"-----------------------"<<std::endl;
    }
    T get(int x,int y){
      assert(x<width);
      assert(y<height);
      return this->cells[x][y];
    }
    void set(int x,int y,T value){
      assert(x<width);
      assert(y<height);
      this->cells[x][y] = value;
    }
    _Matrix<T> clone(){
      T** cellsCopy = new T*[width];
      int i,j;
      for(i=0;i<width;i++){
        cellsCopy[i] = new T[height];
        for(j=0;j<height;j++){
          cellsCopy[i][j] = cells[i][j];
        }
      }
      return _Matrix<T>(width,height,cellsCopy);
    }
};

using MatrixInt = _Matrix<int>;
using MatrixFloat = _Matrix<float>;
using MatrixDouble = _Matrix<double>;