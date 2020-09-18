/*
  (1) opening a file
  (2) building representation of file
  (3) reading in a camera object
  (4) adding a camera object to the world
  (5) reading in a light source
  (6) adding a light sourcee to the world
  (7) reading / storing a definition
  (8) reading in an object
  (9) applying a definition
  (10) adding an object to the world

  ?? adding to YAML scene information
  ?? adding to YAML reptiton info (for animations)

compile with: g++ --std=c++11 -pedantic -Wall -I/home/ghuber/bin/include main.cpp -o main.o /home/ghuber/bin/lib/libyaml-cpp.a
*/

#include <fstream>
#include <iostream>
#include <string>

#include "parser.h"
/*
tag = camera, light, plane, sphere, cube, cone, cylinder, name
op = add, define, image
 - op : tag
*/
int main()
{
  std::string fileN = std::string("./cover.yaml");

  parser yamlParser;
  yamlParser.fname(fileN);

  yamlParser.openFile();

  yamlParser.parse();

  yamlParser.closeFile();

  

  return 0;
}
