#ifndef FileSystem_header
#define FileSystem_header

#include <string>
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

#ifdef __DESKTOP__
#include "./../libs/stb_image/open_gl_stb_image.h"
#endif
#ifdef __ANDROID__
#include "./../../android/cpp/AndroidEnvironment.h"
#endif
#ifdef __IOS__
#include "./../../ios/ThreeDimentionalCube/ThreeDimentionalCube/IPhoneHelperAbstraction.h"
#endif
#include "../3d/shape/Shape3d.h"
#include "../utils/operators/Operators.h"
#include "../libs/tiny_obj_loader/tiny_obj_loader.h"
#include "../libs/mini_ball/Miniball.h"

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
  //TODO Retrun array of points , normals and textures instead
  static std::unique_ptr<Shape3d> loadObject(
    std::string filename,
    Constants::LightPrecision lightPercision,
    Texture* texture,
    bool requireCentralizing,
    Shape3d::NormalType normalType,
    bool useFileTextureCoordinates
  ){
    
    Logger::log("Loading 3d object with name:" + filename);

    std::ifstream* file; 
    //TODO We have two options
    //1- Copy all android files into a folder and then run everything using c++ which helps to avoid working with that
    //2- Write everything using that painful android buffer but it can be faster
    #if defined(__DESKTOP__)
      
      file = new std::ifstream(filename);
      
    #elif defined(__ANDROID__)
      
      auto data = AndroidEnvironment::getInstance()->loadText(filename);
      assert(data);
      std::string temporaryFileName = AndroidEnvironment::getInstance()->generateFileAbsolutePath("/cube-temp-file.obj");
      //We need to write it into a file to be accessible to ifstream
      //TODO Refactor this part
      FILE* temporaryFile = fopen(temporaryFileName.c_str(),"w");
      assert(temporaryFile);
      fputs(reinterpret_cast<const char *>(data), temporaryFile);
      fflush(temporaryFile);
      fclose(temporaryFile);

      file = new std::ifstream(temporaryFileName);
      
    #elif defined(__IOS__)
      
      auto resourceAddress = IPhoneHelperAbstraction::getInstance()->callObjectiveCToGetPathNameForResource(filename, "obj");
      file = new std::ifstream(resourceAddress);
      
    #else
    #   error "loadObject failed Unhandled platform"
    #endif
    //Checking if file is filled
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

    std::vector<Matrix3X1Float> vertices;
    std::vector<std::unique_ptr<Surface>> indices;
    std::vector<Matrix3X1Float> normals;
    std::vector<Matrix2X1Float> textureCoordinates;
    {//Parsing .obj file using tinyObj library
      tinyobj::attrib_t attributes;
      std::vector<tinyobj::shape_t> shapes;
      std::string error;
      auto isSuccess = tinyobj::LoadObj(&attributes, &shapes, nullptr, &error, file);
      // check for errors
      if (!error.empty() && error.substr(0, 4) != "WARN")
      {
        Logger::exception("LoadObj returned error:" + error + " File:" + filename);
        return nullptr;
      }
      if (!isSuccess)
      {
        Logger::exception("LoadObj returned false  File:" + filename);
        return nullptr;
      }
      if (shapes.size() == 0u)
      {
        Logger::exception("LoadObj object file had no shapes  File:" + filename);
        return nullptr;
      }
      {//Loading verticies
      // extract vertex data
      // attrib.vertices is a flat std::vector of floats corresponding
      // to vertex positions, laid out as xyzxyzxyz... etc.
        assert(attributes.vertices.size() % 3 == 0);

        //Reserving space before allocating
        vertices.reserve(attributes.vertices.size() / 3u);
        for (unsigned int i = 0; i < attributes.vertices.size(); i += 3) {
          vertices.emplace_back(
            attributes.vertices[i + 0u],
            attributes.vertices[i + 1u],
            attributes.vertices[i + 2u]
          );
        }

      }
      {//Normals
        //TODO Auto generate normals when size is zero instead of asking
        if (normalType == Shape3d::NormalType::fileDefault) {
          assert(attributes.normals.size() % 3 == 0);
          normals.reserve(attributes.normals.size() / 3u);
          for (unsigned int i = 0; i < attributes.normals.size(); i += 3) {
            normals.emplace_back(
              attributes.normals[i + 0u],
              attributes.normals[i + 1u],
              attributes.normals[i + 2u]
            );
          }
        }
      }
      {//Texture coordinates
        //TODO Test this part
        if (useFileTextureCoordinates) {
          assert(attributes.texcoords.size() % 2 == 0);
          textureCoordinates.reserve(attributes.texcoords.size() / 2u);
          for (unsigned int i = 0; i < attributes.texcoords.size(); i += 2) {
            textureCoordinates.emplace_back(
              attributes.texcoords[i + 0u],
              attributes.texcoords[i + 1u]
            );
          }
        }
      }
      {//Loading mesh
      // extract index data
      // obj file can contain multiple meshes, we assume just 1
        const auto& mesh = shapes[0].mesh;

        unsigned short edge1Index = 2u;
        unsigned short edge2Index = 1u;
        unsigned short edge3Index = 0u;
        //My implementation is counter clock wise so I need to rotate before rendering
        if (isCounterClockWise == true) {
          edge1Index = 0u;
          edge2Index = 1u;
          edge3Index = 2u;
        }

        float edge1TexturePointX = 0.0f;
        float edge1TexturePointY = 0.0f;
        float edge2TexturePointX = 0.0f;
        float edge2TexturePointY = 0.0f;
        float edge3TexturePointX = 0.0f;
        float edge3TexturePointY = 0.0f;
        // mesh contains a std::vector of num_face_vertices (uchar)
        // and a flat std::vector of indices. If all faces are triangles
        // then for any face f, the first index of that faces is [f * 3n]
        indices.reserve(mesh.indices.size() / 3u);
        for (unsigned int faceIndex = 0; faceIndex < mesh.num_face_vertices.size(); faceIndex++) {
          //Because we only support triangles currently we check if contains vertices or not
          if (mesh.num_face_vertices[faceIndex] != 3) {
            Logger::exception("Number of face vertices cannot be other than " + std::to_string(mesh.num_face_vertices[faceIndex]));
          }
          unsigned long edgeIndicies[3] = {
            (unsigned long)mesh.indices[faceIndex * 3u + edge1Index].vertex_index,
            (unsigned long)mesh.indices[faceIndex * 3u + edge2Index].vertex_index,
            (unsigned long)mesh.indices[faceIndex * 3u + edge3Index].vertex_index
          };
          //Loading mesh indices into indices vector
          indices.emplace_back(std::make_unique<Surface>(
            lightPercision,
            texture,
            edgeIndicies
          ));
          if (normalType == Shape3d::NormalType::fileDefault) {
            indices.back()->setNormalIndex(0, mesh.indices[faceIndex * 3u + edge1Index].normal_index);
            indices.back()->setNormalIndex(1, mesh.indices[faceIndex * 3u + edge2Index].normal_index);
            indices.back()->setNormalIndex(2, mesh.indices[faceIndex * 3u + edge3Index].normal_index);
          }
          if (useFileTextureCoordinates) {
            edge1TexturePointX = textureCoordinates.at(mesh.indices[faceIndex * 3u + edge1Index].texcoord_index).get(0, 0);
            edge1TexturePointY = textureCoordinates.at(mesh.indices[faceIndex * 3u + edge1Index].texcoord_index).get(1, 0);
            indices.back()->setTextureCoordinates(0, edge1TexturePointX, edge1TexturePointY);

            edge2TexturePointX = textureCoordinates.at(mesh.indices[faceIndex * 3u + edge2Index].texcoord_index).get(0, 0);
            edge2TexturePointY = textureCoordinates.at(mesh.indices[faceIndex * 3u + edge2Index].texcoord_index).get(1, 0);
            indices.back()->setTextureCoordinates(1, edge2TexturePointX, edge2TexturePointY);

            edge3TexturePointX = textureCoordinates.at(mesh.indices[faceIndex * 3u + edge3Index].texcoord_index).get(0, 0);
            edge3TexturePointY = textureCoordinates.at(mesh.indices[faceIndex * 3u + edge3Index].texcoord_index).get(1, 0);
            indices.back()->setTextureCoordinates(2, edge3TexturePointX, edge3TexturePointY);
          }
        }
      }
    }
    if (normalType != Shape3d::NormalType::fileDefault) {
      normals = Shape3d::generateNormals(indices, vertices, normalType);
    }
    Logger::log("Reading from object file is successful");    
    delete file;
    if(requireCentralizing){
      Logger::log("Going to normalize center point");
      {//Centralizing vertices to make them be more friendly for transformation and scale
        // used to enable miniball to access vertex pos info
        // solve the minimum bounding sphere
        Miniball::Miniball<VertexAccessor> mb( 3,vertices.cbegin(),vertices.cend() );
        // get center of min sphere
        // result is a pointer to float[3]
        const auto pc = mb.center();
        Matrix3X1Float center;
        center.setX(*pc);
        center.setY(*std::next(pc));
        center.setZ(*std::next(pc, 2));
        
        // adjust all vertices so that center of minimal sphere is at 0,0
        for( auto& vertex : vertices )
        {
          vertex.minus(center);
        }
      }
      Logger::log("Centralizing 3dShape was successful,Creating shape3d");
    }

#ifdef __ANDROID__
    remove(temporaryFileName.c_str());
#endif // __ANDROID__

    return std::make_unique<Shape3d>(
      vertices,
      indices,
      normals
    );
  }
private:
  struct VertexAccessor
  {
    // iterator type for iterating over vertices
    typedef std::vector<Matrix3X1Float>::const_iterator Pit;
    // it type for iterating over components of vertex
    // (pointer is used to iterate over members of class here)
    typedef const float* Cit;
    // functor that miniball uses to get element iter based on vertex iter
    Cit operator()(Pit it) const
    {
      float* elements = new float[3];
      elements[0] = it->getX();
      elements[1] = it->getY();
      elements[2] = it->getZ();
      return elements;
    }
  };
};

#endif
