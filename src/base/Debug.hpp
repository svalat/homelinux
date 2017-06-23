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

#ifndef HL_DEBUG_HPP
#define HL_DEBUG_HPP

/********************  HEADERS  *********************/
#include <stdexcept>
#include <map>
#include "FormattedMessage.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/********************  STRUCT  ***********************/
class Error : public std::runtime_error
{
	public:
		Error(const std::string & message) : runtime_error(message.c_str()) {};
};

/*********************  ENUM  ***********************/
/**
 * @brief Define the debug message levels.
**/
enum DebugLevel
{
	/** For assertions, but your might use standard assert() instead. **/
	MESSAGE_ASSERT,
	/** Used to print some debug informations. **/
	MESSAGE_DEBUG,
	/** Used to print some informations. **/
	MESSAGE_INFO,
	/** Used for normal printing of formatted messages. **/
	MESSAGE_NORMAL,
	/** Used to print warnings. **/
	MESSAGE_WARNING,
	/** Used to print errors. **/
	MESSAGE_ERROR,
	/** Used to print fatal error and abort(). **/
	MESSAGE_FATAL,
	/** Used to send errors via exception **/
	MESSAGE_THROW
};

/*********************  TYPES  **********************/
/**
 * Permit to enable of disable debug messages by category.
**/
typedef std::map<std::string,bool> DebugCategoryMap;

/*********************  CLASS  **********************/
/**
 * @brief Base class to manage errors.
 * 
 * The debug class provide a short way to format error, debug or warning messages and
 * manage error final state (abort, exception...).
 * 
@code{.cpp}
Debug("My message as ard %1 and %2 !",MESSAGE_FATAL).arg(value1).arg(value2).end();
@endcode
**/
class Debug : public FormattedMessage
{
	public:
		Debug(const char * format,const char * file,int line,DebugLevel level = MESSAGE_DEBUG, const char * category = NULL);
		Debug(const char * format,DebugLevel level = MESSAGE_DEBUG,const char * category = NULL);
		virtual ~Debug(void);
		virtual void end(void);
		static void enableCat(const std::string & cat);
		static void enableAll(void);
		static bool showCat(const char * cat);
		static void showList(void);
		static void setVerbosity(const std::string & value);
		static const DebugCategoryMap* getCatMap( void );
	protected:
		/** Category (only for DEBUG level), NULL otherwise. **/
		const char * cat;
		/** Store the message level for end() method. **/
		DebugLevel level;
		/** Store file location to inform user in end() method. **/
		std::string file;
		/** Store line to inform user in end() method. **/
		int line;
		/** To check is message is emitted at exit (only for debug purpose). **/
		bool emitted;
		/** map to enable or disable messages depending on their category. **/
		static DebugCategoryMap * catMap;
		/** Store the maxmium number of chat of categories. **/
		static int catMaxWidth;
		/** Store current rank to prefix output **/
		static int rank;
};

/*********************  CLASS  **********************/
/**
 * Provide the same interface than Debug class but do nothing in implementation.
 * It is used to ignore debug message at compile time if NDEBUG macro is enabled.
**/
class DebugDummy
{
	public:
		DebugDummy(const char * format,const char * file,int line,DebugLevel level = MESSAGE_DEBUG, const char * category = NULL){};
		DebugDummy(const char * format,DebugLevel level = MESSAGE_DEBUG,const char * category = NULL){};
		static void enableCat(const std::string & cat){};
		static void enableAll(void){};
		static bool showCat(const char * cat){return false;};
		static void showList(void){};
		static void setVerbosity(const std::string & value){};
		//from message class
		template <class T> DebugDummy & arg(const T & value) {return *this;};
		DebugDummy & arg(const std::string & value) {return *this;};
		DebugDummy & arg(const char * value) {return *this;};
		DebugDummy & argStrErrno(void) {return *this;};
		DebugDummy & argUnit(unsigned long value,const char * unit = "") {return *this;};
		std::string toString(void) const {return "";};
		void toStream(std::ostream & out) const {};
		void end(void){};
};

/********************  MACROS  **********************/
/**
 * Helper to get current location (file and line).
**/
#define HL_CODE_LOCATION __FILE__,__LINE__

/*******************  FUNCTION  *********************/
/** Short function to print debug static messages without arguments. **/
inline Debug debug(const char * format, const char * cat)   {return Debug(format,MESSAGE_DEBUG,cat);  }
/** Short function to print warning static messages without arguments. **/
inline Debug warning(const char * format) {return Debug(format,MESSAGE_WARNING);}
/** Short function to print error static messages without arguments. **/
inline Debug error(const char * format)   {return Debug(format,MESSAGE_ERROR);  }
/** Short function to print fatal static messages without arguments. **/
inline Debug fatal(const char * format)   {return Debug(format,MESSAGE_FATAL);  }

/********************  MACROS  **********************/
#define HL_FATAL(x)     HL_FATAL_ARG(x).end()
#define HL_DEBUG(cat,x) HL_DEBUG_ARG((cat),(x)).end()
#define HL_ERROR(x)     HL_ERROR_ARG(x).end()
#define HL_WARNING(x)   HL_WARNING_ARG(x).end()
#define HL_MESSAGE(x)   HL_MESSAGE_ARG(x).end()
#define HL_INFO(x)      HL_INFO_ARG(x).end()
#define HL_THROW(x)     HL_THROW_ARG(x).end()

/********************  MACROS  **********************/
#define HL_FATAL_ARG(x)     hl::Debug(x,HL_CODE_LOCATION,hl::MESSAGE_FATAL    )
#define HL_ERROR_ARG(x)     hl::Debug(x,HL_CODE_LOCATION,hl::MESSAGE_ERROR    )
#define HL_WARNING_ARG(x)   hl::Debug(x,HL_CODE_LOCATION,hl::MESSAGE_WARNING  )
#define HL_MESSAGE_ARG(x)   hl::Debug(x,HL_CODE_LOCATION,hl::MESSAGE_NORMAL   )
#define HL_INFO_ARG(x)      hl::Debug(x,HL_CODE_LOCATION,hl::MESSAGE_INFO     )
#define HL_THROW_ARG(x)     hl::Debug(x,HL_CODE_LOCATION,hl::MESSAGE_THROW    )
#ifdef NDEBUG
	#define HL_DEBUG_ARG(cat,x) hl::DebugDummy(x,HL_CODE_LOCATION,hl::MESSAGE_DEBUG,cat)
#else
	#define HL_DEBUG_ARG(cat,x) hl::Debug(x,HL_CODE_LOCATION,hl::MESSAGE_DEBUG,cat)
#endif

/********************  MACROS  **********************/
#define assume(check,message) do { if (!(check)) HL_FATAL(message); } while(0)
#define assumeArg(check,message) if (!(check)) HL_FATAL_ARG(message)
#define assumeThrow(check,message) do { if (!(check)) HL_THROW(message); } while(0)
#define assumeThrowArg(check,message) if (!(check)) HL_THROW_ARG(message)

/********************  MACROS  **********************/
#define HL_TO_STRING(x) #x
#ifdef NDEBUG
	#define HL_ASSERT(x)      do{} while(0)
#else
	#define HL_ASSERT(x)      do{ if (!(x)) hl::Debug(HL_TO_STRING(x),HL_CODE_LOCATION,hl::MESSAGE_ASSERT).end(); } while(0)
#endif

}

#endif //HL_DEBUG_HPP
