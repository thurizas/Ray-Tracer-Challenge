#include <string>
#include <iostream>
#include <fstream>
#include <vector>

class world;

class parser
{
 public:
  parser();
  parser(const std::string);
  ~parser();

  void fname(std::string fn) { m_fileName = fn;}
  std::string fname() { return m_fileName;}
	
  
  void openFile();
  void closeFile();

  world* parse();

 private:
  std::string                                m_fileName;
  std::vector<std::pair<std::string, void*>> m_symTable;
  std::ifstream                              m_inFile;
  world*                                     m_world;
  bool                                       m_isOpen;

  std::string removeComments(std::string);
  std::string removeRWhitespace(std::string);
  
  void doImage();
  void doAdd();
  void doDefine();
};
  
