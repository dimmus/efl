#ifndef _CORE_EXE_EO_LEGACY_H_
#define _CORE_EXE_EO_LEGACY_H_

#ifndef _CORE_EXE_EO_CLASS_TYPE
#define _CORE_EXE_EO_CLASS_TYPE

typedef Eo Core_Exe;

#endif

#ifndef _CORE_EXE_EO_TYPES
#define _CORE_EXE_EO_TYPES

/** A structure which stores information on lines data from a child process.
 *
 * @ingroup Core_Exe_Event_Data
 */
typedef struct _Core_Exe_Event_Data_Line
{
  char *line; /**< The bytes of a line of buffered data */
  int size; /**< The size of the line buffer in bytes */
} Core_Exe_Event_Data_Line;

/** Ecore exe event data structure
 *
 * @ingroup Core_Exe
 */
typedef struct _Core_Exe_Event_Data
{
  Efl_Object *exe; /**< The handle to the process. FIXME: should actually be
                    * Ecore.Exe, workaround cyclic */
  void *data; /**< The raw binary data from the child process received */
  int size; /**< The size of this data in bytes */
  Core_Exe_Event_Data_Line *lines; /**< An array of line data if line buffered.
                                     * The last one has its line member set to
                                     * @c NULL */
} Core_Exe_Event_Data;

/** Flags for executing a child with its stdin and/or stdout piped back.
 *
 * @ingroup Ecore
 */
typedef enum
{
  CORE_EXE_NONE = 0, /**< No exe flags at all */
  CORE_EXE_PIPE_READ = 1, /**< Exe Pipe Read mask */
  CORE_EXE_PIPE_WRITE = 2, /**< Exe Pipe Write mask */
  CORE_EXE_PIPE_ERROR = 4, /**< Exe Pipe error mask */
  CORE_EXE_PIPE_READ_LINE_BUFFERED = 8, /**< Reads are buffered until a newline
                                          * and split 1 line per
                                          * Core_Exe_Event_Data_Line */
  CORE_EXE_PIPE_ERROR_LINE_BUFFERED = 16, /**< Errors are buffered until a
                                            * newline and split 1 line per
                                            * Core_Exe_Event_Data_Line */
  CORE_EXE_PIPE_AUTO = 32, /**< stdout and stderr are buffered automatically */
  CORE_EXE_RESPAWN = 64, /**< FIXME: Exe is restarted if it dies */
  CORE_EXE_USE_SH = 128, /**< Use /bin/sh to run the command. */
  CORE_EXE_NOT_LEADER = 256, /**< Do not use setsid() to set the executed
                               * process as its own session leader */
  CORE_EXE_TERM_WITH_PARENT = 512, /**< Makes child receive SIGTERM when parent
                                     * dies. */
  CORE_EXE_ISOLATE_IO = 1024 /**< Try and isolate stdin/out and err of the
                               * process so it isn't shared with the parent. */
} Core_Exe_Flags;


#endif



#endif
