/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef HL_MUTEX_HPP
#define HL_MUTEX_HPP

/********************  HEADERS  *********************/
#include <pthread.h>

/*******************  NAMESPACE  ********************/
namespace hl
{

/*********************  CLASS  **********************/
class Mutex
{
	public:
		Mutex(void);
		~Mutex(void);
		void lock(void);
		void unlock(void);
	private:
		pthread_mutex_t mutex;
};

}

#endif //HL_MUTEX_HPP
