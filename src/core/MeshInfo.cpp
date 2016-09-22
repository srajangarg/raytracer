#include "MeshInfo.hpp"
#include <sstream>
#include <fstream>

void
TriangleMesh::clear()
{
  for (auto it = triangles.begin(); it != triangles.end(); ++it)
  {
    delete *it;
  }

  for (auto it = vertices.begin(); it != vertices.end(); ++it)
  {
    delete *it;
  }
}

bool
getFirstValue(std::istringstream & ss, int & value)
{
  std::string s;

  if (ss >> s)
  {
    std::istringstream valuestream(s);
    if (valuestream >> value)
      return true;
  }

  return false;
}

bool
TriangleMesh::load(std::string const & path)
{
  clear();

  std::ifstream f(path.c_str());
  if (!f)
  {
    std::cerr << "Mesh: Couldn't load file " << path << std::endl;
    return false;
  }

  std::string line;
  // int normals = 0;

  while (std::getline(f, line))
  {
    std::istringstream linestream(line);
    std::string op;
    linestream >> op;

    if (op[0] == '#')
      continue;

    if (op == "v")
    {
      Vec3 v;
      linestream >> v;
      vertices.push_back(new MeshVertex(v));
    }

    if (op == "f")   // extract a face as a triangle fan
    {
      int first, second, third;

      if (!getFirstValue(linestream, first))
        continue;

      if (!getFirstValue(linestream, second))
        continue;

      while (getFirstValue(linestream, third))
      {
        triangles.push_back(new MeshTriangle(first - 1, second - 1, third - 1));
        second = third;
      }
    }
  }

  return true;
}
