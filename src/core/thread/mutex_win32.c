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

struct mutex_t { 
    HANDLE id;
};

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

mutex_t* mutex_create ()
{
	mutex_t* mutex;

	// allocate mutex memory
	mutex = (mutex_t*)ex_malloc_nomng(sizeof(mutex_t*));
	if ( mutex ) {
		// Create the mutex, with initial value signaled
		mutex->id = CreateMutex(NULL, FALSE, NULL);
		if ( !mutex->id ) {
			ex_error("Couldn't create mutex");
			ex_free_nomng(mutex);
			mutex = NULL;
		}
	}
	return mutex;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void mutex_destroy ( mutex_t* _mutex )
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

int mutex_lock ( mutex_t* _mutex )
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

int mutex_unlock ( mutex_t* _mutex )
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
