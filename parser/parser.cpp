#include "parser.h"

parser::parser() : m_fileName(""), m_world(nullptr), m_isOpen(false)
{ }

parser::parser(const std::string fn) : m_fileName(fn), m_world(nullptr), m_isOpen(false)
{ }

parser::~parser()
{
  if(m_isOpen)                  // close the input file
  {
	closeFile();
	m_isOpen = false;
  }
}

void parser::openFile()
{
  if(m_fileName == "")
  {
	// TODO : throw exception no file
  }
  else
  {
	m_inFile = std::ifstream(m_fileName.c_str(), std::ios::in);
	if(!m_inFile.is_open())
	{
	  // TODO : throw exception -- file can't be opened
	}
	else
	{
	  m_isOpen = true;
	}
  }
}


  
void parser::closeFile()
{
  if(m_isOpen)
  {
	m_inFile.close();
	m_isOpen = false;
  }
}



world* parser::parse()
{
  if(m_isOpen)
  {
	std::string strLine;

	while(getline(m_inFile, strLine))
	{
	  strLine = removeComments(strLine);
	  strLine = removeRWhitespace(strLine);
	  
	  if(0 != strLine.length())
	  {
		std::cout << "***" << strLine << "***" << std::endl;
	  }
	}
  }
  else
  {
	// TODO : throw file no opened exception
  }
  // TODO : read line by line
  // TODO : skip full comment lines & empty lines
  // TODO : switch on operation


}


void parser::doImage()
{}

void parser::doAdd()
{}


void parser::doDefine()
{
  
}

std::string parser::removeComments(std::string line)
{

  return line;
}

std::string parser::removeRWhitespace(std::string line)
{

  return line;
}
