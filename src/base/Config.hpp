/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef HL_OPTIONS_HPP
#define HL_OPTIONS_HPP

/********************  HEADERS  *********************/
//must be as first
#include "config.h"
//std
#include <string>
#include <ctime>
//from jsoncpp
#ifdef HAVE_JSON_CPP
	#include <json/json.h>
#endif //HAVE_JSON_CPP


/*******************  NAMESPACE  ********************/
namespace hl
{

/********************  STRUCT  **********************/
/**
 * Structure to store the configuration of HLPIPE.
**/
struct Config
{
};

}

#endif //HL_OPTIONS_HPP
