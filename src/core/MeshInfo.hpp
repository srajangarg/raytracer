/*
 * MeshInfo.hpp
 *
 * Basic mesh data
 *
 *  Created on: Feb 19, 2009
 *      Author: jima
 *      Modified: sidch
 */

#ifndef __MeshInfo_hpp__
#define __MeshInfo_hpp__

#include "Algebra3.hpp"
#include <vector>

/** Vertex of an Mesh mesh. */
struct MeshVertex
{
  Vec3 pos;  ///< Vertex position.

  /** Constructor. */
  MeshVertex(Vec3 pos) : pos(pos) {}
};

/** Triangle of a mesh. */
struct MeshTriangle
{
  int ind[3];  ///< Vertex indices.

  /** Constructor. */
  MeshTriangle(int i, int j, int k)
  {
    ind[0] = i;
    ind[1] = j;
    ind[2] = k;
  }
};

/** A triangle mesh. */
struct TriangleMesh
{
  std::vector<MeshVertex *>    vertices;   ///< Vertex data.
  std::vector<MeshTriangle *>  triangles;  ///< Triangle data.

  /** Default constructor. */
  TriangleMesh() {}

  /** Construct mesh from a file. */
  TriangleMesh(std::string const & path) { if (!load(path)) throw "Mesh: Could not load file"; }

  /** Destructor. */
  ~TriangleMesh() { clear(); }

  /** Load mesh from a file. */
  bool load(std::string const & path);

  /** Clear mesh data. */
  void clear();
};

#endif  // __MeshInfo_hpp__
