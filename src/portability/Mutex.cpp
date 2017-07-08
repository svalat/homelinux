/*****************************************************
             PROJECT  : homelinux
             VERSION  : 2.0.0-dev
             DATE     : 06/2017
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "Mutex.hpp"

/*******************  NAMESPACE  ********************/
namespace hl
{

/*******************  FUNCTION  *********************/
Mutex::Mutex(void)
{
	pthread_mutex_init(&mutex,NULL);
}

/*******************  FUNCTION  *********************/
Mutex::~Mutex(void)
{
	pthread_mutex_destroy(&mutex);
}

/*******************  FUNCTION  *********************/
void Mutex::lock(void)
{
	pthread_mutex_lock(&mutex);
}

/*******************  FUNCTION  *********************/
void Mutex::unlock(void)
{
	pthread_mutex_unlock(&mutex);
}

}
