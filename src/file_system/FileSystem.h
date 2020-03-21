#ifndef FileSystem_header
#define FileSystem_header

#include <string>
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

#include "./../Constants.h"
#include "../utils/log/Logger.h"
#include "../libs/tiny_obj_loader/tiny_obj_loader.h"
#include "../utils/operators/Operators.h"
#include "../utils/log/Logger.h"
#include "../data_types/MatrixTemplate.h"
#include "../3d_shape/surface/base_surface/BaseSurface.h"
#include "../3d_shape/Shape3d.h"
#include "../3d_shape/surface/color_surface/ColorSurface.h"
#ifdef __DESKTOP__
#include "./../libs/stb_image/open_gl_stb_image.h"
#endif
#ifdef __ANDROID__
#include "./../../android/cpp/AndroidEnvironment.h"
#endif
#ifdef __IOS__
#include "./../../ios/ThreeDimentionalCube/ThreeDimentionalCube/IPhoneHelperAbstraction.h"
#endif

class FileSystem{
public:
  static unsigned char * loadTexture(
    std::string textureAddress,
    int* width,
    int* height,
    int* numberOfChannels
  ){
    Logger::log("Loading image:"+textureAddress);
    #if defined(__DESKTOP__)
      return stbi_load(textureAddress.c_str(), width, height, numberOfChannels, STBI_rgb);
    #elif defined(__ANDROID__)
        return AndroidEnvironment::getInstance()->loadImage(
                textureAddress,
                width,
                height,
                numberOfChannels
        );
    #elif defined(__IOS__)
      return IPhoneHelperAbstraction::getInstance()->callObjectiveCToLoadImage(
        textureAddress,
        width,
        height,
        numberOfChannels
      );
    #else
    #   error "loadObject failed Unhandled platform"
    #endif
    return nullptr;
  }
  static std::unique_ptr<Shape3d> loadObjectWithColor(
    std::string filename,
    Vec3DFloat color
  ){
    assert(color.getX()>=0.0f && color.getX()<=1.0f);
    assert(color.getY()>=0.0f && color.getY()<=1.0f);
    assert(color.getZ()>=0.0f && color.getZ()<=1.0f);

    Logger::log("Loading 3d object with name:"+filename);

    std::ifstream* file; 
    
    #if defined(__DESKTOP__)
      file = new std::ifstream(filename);
    #elif defined(__ANDROID__)
    //TODO
    #elif defined(__IOS__)
    //TODO
    #else
    #   error "loadObject failed Unhandled platform"
    #endif
    //Checking if ifstream is filled
    assert(file);

    bool isCounterClockWise = false;
    {//Check if normal vectors are reverse
      std::string firstLine;
      std::getline(*file,firstLine);
      Operators::toLowerCase( firstLine );
      if(firstLine.find("ccw") != std::string::npos){
        isCounterClockWise = true;
      }
    }

    if(isCounterClockWise){
      Logger::log("3d object file is counter clock wise");
    }else{
      Logger::log("3d object file is clock wise");
    }

    std::vector<MatrixFloat> vertices;
    std::vector<BaseSurface*> indices;
    {//Parsing .obj file using tinyObj library
      tinyobj::attrib_t attributes;
      std::vector<tinyobj::shape_t> shapes;
      std::string error;
      auto isSuccess = tinyobj::LoadObj(&attributes,&shapes,nullptr,&error,file);
      // check for errors
      if( !error.empty() && error.substr( 0,4 ) != "WARN" )
      {
        Logger::exception("LoadObj returned error:" + error + " File:" + filename);
        return nullptr;
      }
      if( !isSuccess )
      {
        Logger::exception("LoadObj returned false  File:" + filename);
        return nullptr;
      }
      if( shapes.size() == 0u )
      {
        Logger::exception("LoadObj object file had no shapes  File:" + filename);
        return nullptr;
      }
      {//Loading verticies
      // extract vertex data
      // attrib.vertices is a flat std::vector of floats corresponding
      // to vertex positions, laid out as xyzxyzxyz... etc.
        assert(attributes.vertices.size()%3==0);
        //Reserving space before allocating
        vertices.reserve(attributes.vertices.size()/3u);
        for(auto i=0;i<attributes.vertices.size();i+=3){
          vertices.emplace_back(
            MatrixFloat(3,1,std::vector<std::vector<float>>{
              std::vector<float>{attributes.vertices[i+0]},
              std::vector<float>{attributes.vertices[i+1]},
              std::vector<float>{attributes.vertices[i+2]}
            })
          );
        }
      }
      {//Loading mesh
      // extract index data
      // obj file can contain multiple meshes, we assume just 1
        const auto& mesh = shapes[0].mesh;
      // mesh contains a std::vector of num_face_vertices (uchar)
      // and a flat std::vector of indices. If all faces are triangles
      // then for any face f, the first index of that faces is [f * 3n]
        indices.reserve( mesh.indices.size()/3u );
        for(auto faceIndex = 0; faceIndex < mesh.num_face_vertices.size() ; faceIndex++){
          //Because we only support triangles currently we check if contains vertices or not
          if(mesh.num_face_vertices[faceIndex]!= 3){
            Logger::exception("Number of face vertices cannot be other than "+ std::to_string(mesh.num_face_vertices[faceIndex]));
          }
          //Loading mesh indices into indices vector
          if(isCounterClockWise==false){
            indices.emplace_back(new ColorSurface(
              mesh.indices[faceIndex * 3 + 0].vertex_index,
              mesh.indices[faceIndex * 3 + 1].vertex_index,
              mesh.indices[faceIndex * 3 + 2].vertex_index,
              color.getX(),
              color.getY(),
              color.getZ()
            ));
          }else{
            indices.emplace_back(new ColorSurface(
              mesh.indices[faceIndex * 3 + 2].vertex_index,
              mesh.indices[faceIndex * 3 + 1].vertex_index,
              mesh.indices[faceIndex * 3 + 0].vertex_index,
              color.getX(),
              color.getY(),
              color.getZ()
            ));
          }
        }
      }
    }
    
    delete file;

    return std::make_unique<Shape3d>(
      vertices,
      indices
    );
  }
};

#endif