#pragma once

#include <assert.h>
#include <iostream>
#include <vector>
#include <memory>

template <typename T>
class _Matrix{
  private:
    int width;
    int height;
    std::vector<std::vector<T>> cells;
  public:
    _Matrix(int width,int height,T cellDefaultValue)
      :
      width(width),
      height(height)
    {
      int i,j;
      for(i=0;i<width;i++){
        cells.emplace_back(std::vector<T>());
        for(j=0;j<height;j++){
          cells.at(i).emplace_back(cellDefaultValue);
        }
      }
    }
    _Matrix(int width,int height,std::vector<std::vector<T>> initialCellValue)
      :
      width(width),
      height(height)
    {
      this->cells = initialCellValue;
    }
    _Matrix<T>& operator=(_Matrix<T> rhs){
      width = rhs.width;
      height = rhs.height;
      cells.erase(cells.begin(),cells.end());
      for(int i=0;i<width;i++){
        cells.emplace_back(std::vector<T>());
        for(int j=0;j<height;j++){
          cells.at(i).emplace_back(rhs.cells.at(i).at(j));
        }
      }
      return *this;
    }
    _Matrix<T> operator+(_Matrix<T> rhs){
      assert(rhs.width==width);
      assert(rhs.height==height);
      std::vector<std::vector<T>> resultsCell;
      for(int i=0;i<width;i++){
        resultsCell.emplace_back(std::vector<T>());
        for(int j=0;j<height;j++){
          resultsCell.at(i).emplace_back(rhs.cells.at(i).at(j) + cells.at(i).at(j));
        }
      }
      _Matrix<T> result(width,height,resultsCell);
      return result;
    }
    _Matrix<T> operator*(_Matrix<T> rhs){
      assert(rhs.height==width);
      auto finalWidth = rhs.width;
      auto finalHeight = height;
      std::vector<std::vector<T>> resultCells;
      int i,j,k,p; 
      for(i=0;i<finalWidth;i++){
        resultCells.emplace_back(std::vector<T>());
         for(j=0;j<finalHeight;j++){
          resultCells.at(i).emplace_back(0);
          for(k=0,p=0;k<width;k++,p++){
            resultCells.at(i).at(j) += cells.at(k).at(j) * rhs.cells.at(i).at(p); 
          }
        }
      }
      _Matrix<T> resultMatrix(finalWidth,finalHeight,resultCells);
      return resultMatrix;
    }
    _Matrix<T>& operator*=(_Matrix<T> rhs){
      this = this * rhs;
      return *this;
    }
    bool operator==(_Matrix<T> rhs){
      if(rhs.width!=width || rhs.height!=height){
        return false;
      }
      for(int i=0;i<width;i++){
        for(int j=0;j<height;j++){
          if(rhs.get(i,j)!=get(i,j)){
            return false;
          }
        }
      }
      return true;
    }
    bool operator!=(_Matrix<T> rhs){
      return !(this==rhs);
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
          std::cout<<" "<<cells.at(j).at(i)<<" ";
        }
        std::cout<<std::endl;
      }
      std::cout<<"-----------------------"<<std::endl;
    }
    T get(int x,int y){
      assert(x<width);
      assert(y<height);
      return this->cells.at(x).at(y);
    }
    void set(int x,int y,T value){
      assert(x<width);
      assert(y<height);
      this->cells.at(x).at(y) = value;
    }
    _Matrix<T> clone(){
      std::vector<std::vector<T>> cellsCopy;
      int i,j;
      for(i=0;i<width;i++){
        cellsCopy.emplace_back(std::vector<T>());
        for(j=0;j<height;j++){
          cellsCopy.at(i).emplace_back(cells.at(i).at(j));
        }
      }
      return _Matrix<T>(width,height,cellsCopy);
    }
};

using MatrixInt = _Matrix<int>;
using MatrixFloat = _Matrix<float>;
using MatrixDouble = _Matrix<double>;