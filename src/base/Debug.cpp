/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  INFO   ***********************/
/**
 * This file is imported from project MALT developped by Sebastien Valat.
 * at exascale lab / university of versailles and as been improved
 * int lhcb-daqpipe-v2 project at CERN
**/

/********************  HEADERS  *********************/
//as first
#include "config.h"
//standard
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cstring>
//header to implement
#include "Debug.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/********************  MACRO  ***********************/
#define COLOR_NORMAL   "\033[0m"
#define COLOR_RED      "\033[31m"
#define COLOR_GREEN    "\033[32m"
#define COLOR_YELLOW   "\033[33m"
#define COLOR_BLUE     "\033[34m"
#define COLOR_MAGENTA  "\033[35m"
#define COLOR_CYAN     "\033[36m"
#define COLOR_WHITE    "\033[37m"

/********************** CONSTS **********************/
/**
 * String to print names of debug levels.
**/
#ifdef ENABLE_COLOR
	static const char * cstLevelPrefix[] = {COLOR_RED "Assert : ",COLOR_CYAN , COLOR_CYAN,"", COLOR_YELLOW "Warning : ", COLOR_RED"Error : ",COLOR_RED"Fatal : "};
	static const char * cstPostfix = COLOR_NORMAL;
#else
	static const char * cstLevelPrefix[] = {"Assert : ","Debug : ","Info : ","","Warning : ","Error : ","Fatal : "};
	static const char * cstPostfix = "";
#endif

/********************  GLOBALS  *********************/
static DebugCategoryMap ref;
DebugCategoryMap * Debug::catMap = &ref;
int Debug::catMaxWidth = 0;

/*******************  FUNCTION  *********************/
/**
 * Constructor of debug object. Arguments must be setup with arg() method and message printing
 * will be effective when calling the end() method.
 * @param format Define the message format (use %X for aguments where X is the ID of arg).
 * @param file Define the source location of the message emitter.
 * @param line Define the source location of the message emitter.
 * @param level Define the level of debug message.
 * @param cat Optional category of message to filter output.
 * 
@code
Debug msg("this is a test with arg %1 and arg %2 !").arg(10).arg("string").end();
@endcode
**/
Debug::Debug(const char* format, const char* file, int line, DebugLevel level, const char * cat)
	:FormattedMessage(format)
{
	this->cat = cat;
	this->file = file;
	this->line = line;
	this->level = level;
	this->emitted = false;
}

/*******************  FUNCTION  *********************/
/**
 * Short constructor without code location.
**/
Debug::Debug(const char* format, DebugLevel level, const char * category)
	:FormattedMessage(format)
{
	this->cat = category;
	this->file = "??";
	this->line = 0;
	this->emitted = false;
	this->level = level;
}

/*******************  FUNCTION  *********************/
/**
 * Emit the message.
**/
void Debug::end(void)
{
	this->emitted = true;
	std::stringstream buf;
	
	//select
	switch(level) {
		case MESSAGE_DEBUG:
			#ifndef NDEBUG
				if (showCat(cat))
				{
					buf << cstLevelPrefix[level] << '[' << std::left << std::setw(catMaxWidth) << cat << "] " << *this << cstPostfix << std::endl;
					std::cout << buf.str();
				}
			#endif //NDEBUG
			break;
		case MESSAGE_INFO:
		case MESSAGE_NORMAL:
			///if (line != 0)
			//	std::cout << std::endl << cstLevelPrefix[level] << "At " <<  file << ':' << line << " : \n";
			buf << cstLevelPrefix[level] << *this << cstPostfix << std::endl;
			std::cout << buf.str();
			break;
		case MESSAGE_ASSERT:
			#ifdef NDEBUG
				break;
			#else
				if (line != 0)
					buf << std::endl << cstLevelPrefix[level] << "At " <<  file << ':' << line << " : \n";
				buf << cstLevelPrefix[level] << *this << cstPostfix << std::endl;
				std::cout << buf.str();
				abort();
			#endif
		case MESSAGE_WARNING:
		case MESSAGE_ERROR:
			if (line != 0)
				buf << std::endl << cstLevelPrefix[level] << "At " <<  file << ':' << line << " : \n";
			buf << cstLevelPrefix[level] << *this << cstPostfix << std::endl;
			std::cerr << buf.str();
			break;
		case MESSAGE_FATAL:
			if (line != 0)
				buf << std::endl << cstLevelPrefix[level] << "At " <<  file << ':' << line << " : \n";
			buf << cstLevelPrefix[level] << *this << cstPostfix << std::endl;
			std::cerr << buf.str();
			abort();
			break;
	}
}

/*******************  FUNCTION  *********************/
/**
 * Destructor, only to check message emission on debug mode.
**/
Debug::~Debug(void)
{
	assert(emitted);
}

/*******************  FUNCTION  *********************/
/**
 * Enable display of messages from selected categoty.
 * @param cat Select the category of message to display.
**/
void Debug::enableCat ( const std::string& cat )
{
	//setup
	if (catMap == NULL)
		catMap = new DebugCategoryMap;
	
	//set length
	int len = cat.size();
	if (len > catMaxWidth)
		catMaxWidth = len;
	
	//set
	(*catMap)[cat] = true;
}

/*******************  FUNCTION  *********************/
/**
 * Check if the given category has to be shown or not.
 * @param cat Name of the categoty to test.
**/
bool Debug::showCat ( const char* cat )
{
	if (catMap == NULL) {
		int len = strlen(cat);
		if (len > catMaxWidth)
			catMaxWidth = len;
	}

	if (catMap == NULL || cat == NULL) {
		return true;
	} else {
		DebugCategoryMap::const_iterator it = catMap->find(cat);
		if (it == catMap->end()) {
			(*catMap)[cat] = false;
			return false;
		} else {
			return it->second;
		}
	}
}

/*******************  FUNCTION  *********************/
/**
 * Enable all category to be shown by setting catMap to NULL.
**/
void Debug::enableAll ( void )
{
	catMap = NULL;
}

/*******************  FUNCTION  *********************/
/**
 * Can help user by printing all the available categories at execution end.
 * Enabled by using '-v help'.
**/
void Debug::showList ( void )
{
	std::string buffer = "Debug categories :\n";
	if (catMap != NULL) {
		for (DebugCategoryMap::const_iterator it = catMap->begin() ; it != catMap->end() ; ++it) {
			buffer += " - ";
			buffer += it->first;
			buffer += "\n";
		}
	}
	HL_INFO(buffer.c_str());
}

/********************** CONSTS **********************/
/**
 * Enable verbosity for the given categories listed as a string with
 * comma separated values.
 * @param value A string with comma separated categorie names.
**/
void Debug::setVerbosity ( const std::string& value )
{
	std::istringstream iss(value);
	std::string token;
	while(std::getline(iss, token, ',')) {
		if (token == "all" || token == "*")
			enableAll();
		else
			enableCat(token);
	}
}

/********************  GLOBALS  *********************/
const DebugCategoryMap * Debug::getCatMap ( void )
{
	return catMap;
}

}
