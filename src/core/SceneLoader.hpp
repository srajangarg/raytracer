/*
 * SceneLoader.hpp
 *
 *  Created on: Feb 8, 2009
 *      Author: jima
 *      Modified: sidch
 */

#ifndef __SceneLoader_hpp__
#define __SceneLoader_hpp__

#include "Scene.hpp"
#include <map>
#include <vector>

/** This is basically like an elaborate constructor for Scene. */
class SceneLoader
{
  public:
    SceneLoader(Scene & scene, std::string file);
    ~SceneLoader();

  private:
    // make these private; they shouldn't be called
    SceneLoader(SceneLoader const & copy);
    SceneLoader & operator=(SceneLoader const & scene);

    // record locations of newline, to make it easy to convert a position in the file to a line number
    std::vector<int> endlineTable;

    // std::map from group names to groups, to facilitate lookup when parsing
    std::map<std::string, SceneGroup *> groups;
    std::map<std::string, ParametricMaterial *> materials;
    // all the instances in the file
    std::vector<SceneInstance *> instances;

    // a stream for error messages, usually cout
    std::ostream * err;

    // the top level
    SceneInstance * root;

    /* helper functions */
    void buildEndlineTable(std::string filename); // preprocess a file to get the endline table
    void curPos(std::ostream & out, int g); // convert position in file to line number
    void errLine(int at); // write the line number of position 'at' to the error stream
    void cleanAfter(std::vector<ParametricValue *> & vals, unsigned int i); // free memory from a parametric values std::vector (cleans up after getValues)

    /* functions to facilitated reading the file stream */
    std::string getString(std::istream & str); // extract the next std::string of characters (contiguous letters, numbers, and s_)
    std::string getQuoted(std::istream & str); // extract the next quoted std::string
    bool readCommand(std::istream & str, std::string & name); // extract the next command (ie std::string after an open paren)
    bool findOpenParen(std::istream & str); // extract characters until we find an open paren
    int findOpenOrClosedParen(std::istream & str); // extract until we find an opening or a closing paren
    bool findCloseParen(std::istream & str); // extract until we find one more closing paren than opening parens (eg current expression is finished)
    ParametricValue * getValue(std::istream & str); // extract one numeric value
    int getValues(std::istream & str, std::vector<ParametricValue *> & vals); // extract numeric values until we can't find any more, return amount found
    bool getName(std::istream & str, std::string type, std::string & name); // gets a variable name from a command
    void setLightDefaults(SceneGroup * n);
    void setSphereDefaults(SceneGroup * n);
    void setCameraDefaults(SceneGroup * n);
    void setMaterialDefaults(ParametricMaterial * n);

    /* functions for processing each top-level command type */
    bool doInclude(std::istream & str, std::string & name); // process an Include command
    SceneInstance * doI(std::istream & str, std::string & name); // process an I (instance) command
    bool doG(std::istream & str, std::string & name); // process a G (group) command
    bool doRender(std::istream & str, std::string & name); // process a Render command
    bool doCamera(std::istream & str, std::string & name); // process a Camera command
    bool doLight(std::istream & str, std::string & name); // process a Light command
    bool doSphere(std::istream & str, std::string & name); // process a Sphere command
    bool doMaterial(std::istream & str, std::string & name); // process a Material command

    /* the main loading function */
    bool buildScene(std::string filename);
};

#endif  // __SceneLoader_hpp__
