// ======================================================================================
// File         : mutex_win32.c
// Author       : Wu Jie 
// Last Change  : 07/10/2010 | 01:13:16 AM | Saturday,July
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_mutex_t *ex_create_mutex ()
{
	ex_mutex_t *mutex;

	// allocate mutex memory
	mutex = (ex_mutex_t *)ex_malloc_nomng(sizeof(ex_mutex_t));
	if (mutex) {
		// Create the mutex, with initial value signaled
		mutex->id = CreateMutex(NULL, FALSE, NULL);
		if ( !mutex->id ) {
			ex_error("Couldn't create mutex");
			ex_free_nomng(mutex);
			mutex = NULL;
		}
	} else {
        ex_error ("out of memory!");
    }

	return mutex;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_destroy_mutex ( ex_mutex_t *_mutex )
{
	if ( _mutex ) {
		if ( _mutex->id ) {
			CloseHandle(_mutex->id);
			_mutex->id = 0;
		}
		ex_free_nomng(_mutex);
	}
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_mutex_lock ( ex_mutex_t *_mutex )
{
	if ( _mutex == NULL ) {
		ex_error("Passed a NULL mutex");
		return -1;
	}
	if ( WaitForSingleObject(_mutex->id, INFINITE) == WAIT_FAILED ) {
		ex_error("Couldn't wait on mutex");
		return -1;
	}
	return(0);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_mutex_unlock ( ex_mutex_t *_mutex )
{
	if ( _mutex == NULL ) {
		ex_error("Passed a NULL mutex");
		return -1;
	}
	if ( ReleaseMutex(_mutex->id) == FALSE ) {
		ex_error("Couldn't release mutex");
		return -1;
	}
	return(0);
}
