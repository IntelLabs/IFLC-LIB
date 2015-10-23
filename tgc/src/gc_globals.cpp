/*
 * COPYRIGHT_NOTICE_1
 */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// System header files
#include <iostream>

// GC header files
#include "gc_cout.h"
#include "gc_header.h"
#include "gc_v4.h"
#include "remembered_set.h"
#include "block_store.h"
#include "object_list.h"
#include "work_packet_manager.h"
#include "garbage_collector.h"
#include "gc_plan.h"
#include "gc_globals.h"
#include "gcv4_synch.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Gc_Plan *_p_gc_plan = NULL;

char *p_plan_file_name = NULL;

bool verbose_gc = false;
bool stats_gc = false;
bool delta_dynopt = false;
bool delta_verbose = false;
unsigned int delta_cutoff_percent = 5;
bool delta_stat = false;
long max_chunks_ephemeral = 0;
long cur_ephemeral_chunk  = 0;
bool promote_on_escape = false;

//
// If the user's command line doesn't set this, then the
// default from the plan file will.
//
POINTER_SIZE_INT initial_heap_size_bytes = 0;
POINTER_SIZE_INT final_heap_size_bytes = 0;

//
// This remembered set has an entry per loaded class.
// It is used for determining valid vtable pointers
// when examining candidate objects.
//

Hash_Table *p_loaded_vtable_directory = NULL;



Hash_Table *dup_removal_enum_hash_table = NULL;

//
// Global to specify the size differentiating
//
unsigned los_threshold_bytes = 0;


bool garbage_collector_is_initialized = false;


//
// This flag is initially false, and is set to true when the
// ORP is fully initialized. This signals that any stop-the-world
// collections can occur.
//
volatile unsigned orp_initialized = 0;


// 
// This is the table holding all the interior pointers used in any given execution 
// of a GC. It is reused each time a GC is done.

ExpandInPlaceArray<slot_offset_entry> *interior_pointer_table = NULL;
ExpandInPlaceArray<slot_offset_entry> *compressed_pointer_table = NULL;



unsigned long enumeration_time;

GC_Thread_Info *active_thread_gc_info_list = NULL;
#ifdef USE_LOCKCMPEX_FOR_THREAD_LOCK
long volatile active_thread_gc_info_list_lock = 0;
#else 
#ifdef USE_PTHREADS
pthread_mutex_t *active_thread_gc_info_list_lock = NULL;
#else  // USE_PTHREADS
McrtTicketLock  *active_thread_gc_info_list_lock = NULL;
#endif // USE_PTHREADS
#endif

// end file gc\gc_globals.cpp