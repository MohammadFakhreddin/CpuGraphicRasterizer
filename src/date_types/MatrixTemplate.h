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
    _Matrix<T> operator+(_Matrix<T> rhs){
      assert(rhs.width==width);
      assert(rhs.height==height);
      T** resultsCell = new T*[width];
      for(int i=0;i<width;i++){
        resultsCell[i] = new T[height];
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
      assert(rhs.width==height);
      T** resultCells = new T*[width];
      for(int i=0;i<width;i++){
        resultCells[i] = new T[rhs.height];
        for(int j=0;j<height;j++){
          resultCells[i][j] = 0;
          for(int k=0;k<width;k++){
            for(int p=0;p<rhs.height;p++){
              resultCells[i][j] += cells[k][j] + rhs.cells[i][p]; 
            }
          }
        }
      }
      _Matrix<T> resultMatrix(width,rhs.height,resultCells);
      return resultMatrix;
    }
    _Matrix<T>& operator*=(_Matrix<T> rhs){
      assert(rhs.width==height);
      T** resultCells = new T[width][rhs.height];
      for(int i=0;i<width;i++){
        resultCells[i] = new T[rhs.height];
        for(int j=0;j<height;j++){
          resultCells[i][j] = 0;
          for(int k=0;k<width;k++){
            for(int p=0;p<rhs.height;p++){
              resultCells[i][j] += cells[k][j] + rhs.cells[i][p]; 
            }
          }
        }
      }
      this->cells = resultCells;
      this->height = rhs.height;
      return this;
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
      for(int i=0;i<width;i++){
        for(int j=0;j<height;j++){
          std::cout<<" "<<cells[i][j]<<" ";
        }
        std::cout<<std::endl;
      }
      std::cout<<"-----------------------"<<std::endl;
    }
};

using MatrixInt = _Matrix<int>;
using MatrixFloat = _Matrix<float>;
using MatrixDouble = _Matrix<double>;