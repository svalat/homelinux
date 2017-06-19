/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef HL_COLOR_HPP
#define HL_COLOR_HPP

/********************  HEADERS  *********************/
#include <config.h>
#include <string>

/*******************  NAMESPACE  ********************/
namespace hl
{

/********************  MACRO  ***********************/
#ifdef ENABLE_COLORS
	#define COLOR_NORMAL   "\033[0m"
	#define COLOR_RED      "\033[31m"
	#define COLOR_GREEN    "\033[32m"
	#define COLOR_YELLOW   "\033[33m"
	#define COLOR_BLUE     "\033[34m"
	#define COLOR_MAGENTA  "\033[35m"
	#define COLOR_CYAN     "\033[36m"
	#define COLOR_WHITE    "\033[37m"
#else //ENABLE_COLORS
	#define COLOR_NORMAL   ""
	#define COLOR_RED      ""
	#define COLOR_GREEN    ""
	#define COLOR_YELLOW   ""
	#define COLOR_BLUE     ""
	#define COLOR_MAGENTA  ""
	#define COLOR_CYAN     ""
	#define COLOR_WHITE    ""
#endif //ENABLE_COLORS

/*********************  STRUCT  *********************/
struct Colors
{
	/**
	 * Basic function to color a string
	 * @param value String to colot
	 * @param color Color to use.
	**/
	static std::string colored(const std::string & value,const char * color) {
		return color + value + COLOR_NORMAL;
	};

	/**
	 * Colored in red.
	 * @param value String to color
	**/
	static std::string red(const std::string & value) {return colored(value,COLOR_RED);};
	
	/**
	 * Colored in green.
	 * @param value String to color
	**/
	static std::string green(const std::string & value) {return colored(value,COLOR_GREEN);};
	
	/**
	 * Colored in yellow.
	 * @param value String to color
	**/
	static std::string yellow(const std::string & value) {return colored(value,COLOR_YELLOW);};
	
	/**
	 * Colored in blue.
	 * @param value String to color
	**/
	static std::string blue(const std::string & value) {return colored(value,COLOR_BLUE);};
	
	/**
	 * Colored in magenta.
	 * @param value String to color
	**/
	static std::string magenta(const std::string & value) {return colored(value,COLOR_MAGENTA);};
	
	/**
	 * Colored in cyan.
	 * @param value String to color
	**/
	static std::string cyan(const std::string & value) {return colored(value,COLOR_CYAN);};
	
	/**
	 * Colored in white.
	 * @param value String to color
	**/
	static std::string white(const std::string & value) {return colored(value,COLOR_WHITE);};
};

}

#endif //HL_COLOR_HPP
