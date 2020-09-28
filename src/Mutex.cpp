
/***************************************************/
/*! \class Mutex
    \brief STK mutex class.

    This class provides a uniform interface for
    cross-platform mutex use.  On Linux and IRIX
    systems, the pthread library is used. Under
    Windows, critical sections are used.

    by Perry R. Cook and Gary P. Scavone, 1995--2019.
*/
/***************************************************/
#include "Mutex.h"
#ifdef STK_MUTEXT_SUPPORTED
#include "ArdConfig.h"

namespace stk {

Mutex :: Mutex()
{

#if (defined(__OS_IRIX__) || defined(__OS_LINUX__) || defined(__OS_MACOSX__))

  pthread_mutex_init(&mutex_, NULL);
  pthread_cond_init(&condition_, NULL);

#elif defined(__OS_WINDOWS__)

  InitializeCriticalSection(&mutex_);
  condition_ = CreateEvent(NULL,  // no security
                           true,  // manual-reset
                           false, // non-signaled initially
                           NULL); // unnamed
#elif defined(__RTOS__)

     vSemaphoreCreateBinary( mutex_ );
     condition_ = &mutex_;

#endif 
}

Mutex :: ~Mutex()
{
#if (defined(__OS_IRIX__) || defined(__OS_LINUX__) || defined(__OS_MACOSX__))

  pthread_mutex_destroy(&mutex_);
  pthread_cond_destroy(&condition_);

#elif defined(__OS_WINDOWS__)

  DeleteCriticalSection(&mutex_);
  CloseHandle( condition_ );

#elif defined(__RTOS__)
  vSemaphoreDelete(mutex_);

#endif 
}

void Mutex :: lock()
{
#if (defined(__OS_IRIX__) || defined(__OS_LINUX__) || defined(__OS_MACOSX__))

  pthread_mutex_lock(&mutex_);

#elif defined(__OS_WINDOWS__)

  EnterCriticalSection(&mutex_);

#elif defined(__RTOS__)

  xSemaphoreTake( mutex_, ( TickType_t ) portMAX_DELAY ); 

#endif 
}

void Mutex :: unlock()
{
#if (defined(__OS_IRIX__) || defined(__OS_LINUX__) || defined(__OS_MACOSX__))

  pthread_mutex_unlock(&mutex_);

#elif defined(__OS_WINDOWS__)

  LeaveCriticalSection(&mutex_);

#elif defined(__RTOS__)

  xSemaphoreGive( mutex_ );

#endif 
}

void Mutex :: wait()
{
#if (defined(__OS_IRIX__) || defined(__OS_LINUX__) || defined(__OS_MACOSX__))

  pthread_cond_wait(&condition_, &mutex_);

#elif defined(__OS_WINDOWS__)

  WaitForMultipleObjects(1, &condition_, false, INFINITE);

#endif 
}

void Mutex :: signal()
{
#if (defined(__OS_IRIX__) || defined(__OS_LINUX__) || defined(__OS_MACOSX__))

  pthread_cond_signal(&condition_);

#elif defined(__OS_WINDOWS__)

  SetEvent( condition_ );

#endif 
}

} // stk namespace

#endif 
