dep_etool += [m, rt, dl, thread_dep]

# if sys_windows
#    eina_pub_deps += [evil]
# endif

# if sys_sun
#    eina_deps += [socket]
# endif

inc_etool = [include_directories('.')]
subdir('checksum')
subdir('data')
subdir('debug')
subdir('log')
subdir('memory')
subdir('safe')
subdir('string')
subdir('thread')
subdir('utils')
subdir('win32')

public_sub_headers_checksum = [
   'checksum/eina_crc.h',
   'checksum/eina_sha.h',
]

public_sub_headers_data = [
   'data/eina_abstract_content.h',
   'data/eina_accessor.h',
   'data/eina_array.h',
   'data/eina_clist.h',
   'data/eina_fp.h',
   'data/eina_hash.h',
   'data/eina_inarray.h',
   'data/eina_inline_private.h',
   'data/eina_inlist.h',
   'data/eina_iterator.h',
   'data/eina_list.h',
   'data/eina_matrix.h',
   'data/eina_matrixsparse.h',
   'data/eina_promise_private.h',
   'data/eina_promise.h',
   'data/eina_quad.h',
   'data/eina_quadtree.h',
   'data/eina_quaternion.h',
   'data/eina_rbtree.h',
   'data/eina_rectangle.h',
   'data/eina_refcount.h',
   'data/eina_tiler.h',
   'data/eina_trash.h',
   'data/eina_value_util.h',
   'data/eina_value.h',
   'data/eina_vector.h',
]

public_sub_headers_data_inline = [
   'data/eina_inline_array.x',
   'data/eina_inline_clist.x',
   'data/eina_inline_cpu.x',
   'data/eina_inline_crc.x',
   'data/eina_inline_f16p16.x',
   'data/eina_inline_f32p32.x',
   'data/eina_inline_f8p24.x',
   'data/eina_inline_file.x',
   'data/eina_inline_fp.x',
   'data/eina_inline_hash.x',
   'data/eina_inline_inlist.x',
   'data/eina_inline_list.x',
   'data/eina_inline_lock_barrier.x',
   'data/eina_inline_lock_posix.x',
   'data/eina_inline_log.x',
   'data/eina_inline_mempool.x',
   'data/eina_inline_modinfo.x',
   'data/eina_inline_range.x',
   'data/eina_inline_rbtree.x',
   'data/eina_inline_rectangle.x',
   'data/eina_inline_safepointer.x',
   'data/eina_inline_slice.x',
   'data/eina_inline_str.x',
   'data/eina_inline_stringshare.x',
   'data/eina_inline_tiler.x',
   'data/eina_inline_trash.x',
   'data/eina_inline_unicode.x',
   'data/eina_inline_ustringshare.x',
   'data/eina_inline_util.x',
   'data/eina_inline_value_util.x',
   'data/eina_inline_value.x',
   'data/eina_inline_vector.x',
]

public_sub_headers_debug = [
   'debug/eina_debug_private.h',
   'debug/eina_debug.h',
]

public_sub_headers_log = [
   'log/eina_error.h',
   'log/eina_evlog.h',
   'log/eina_fnmatch.h',
   'log/eina_log.h',
]

public_sub_headers_memory = [
   'memory/eina_alloca.h',
   'memory/eina_binshare.h',
   'memory/eina_cow.h',
   'memory/eina_freeq.h',
   'memory/eina_lalloc.h',
   'memory/eina_mempool.h',
   'memory/eina_mmap.h',
   'memory/eina_slice.h',
]

public_sub_headers_safe = [
   'safe/eina_safepointer.h',
   'safe/eina_safety_checks.h',
]

public_sub_headers_string = [
   'string/eina_binbuf.h',
   'string/eina_share_common.h',
   'string/eina_slstr.h',
   'string/eina_str.h',
   'string/eina_strbuf_common.h',
   'string/eina_strbuf.h',
   'string/eina_stringshare.h',
   'string/eina_tmpstr.h',
   'string/eina_unicode.h',
   'string/eina_ustrbuf.h',
   'string/eina_ustringshare.h',
]

public_sub_headers_thread = [
   'thread/eina_lock.h',
   'thread/eina_thread_queue.h',
   'thread/eina_thread.h',
]

public_sub_headers_utils = [
   'utils/eina_benchmark.h',
   'utils/eina_bezier.h',
   'utils/eina_convert.h',
   'utils/eina_counter.h',
   'utils/eina_cpu.h',
   'utils/eina_file_common.h',
   'utils/eina_file.h',
   'utils/eina_hamster.h',
   'utils/eina_magic.h',
   'utils/eina_module.h',
   'utils/eina_prefix.h',
   'utils/eina_range.h',
   'utils/eina_simple_xml_parser.h',
   'utils/eina_util.h',
   'utils/eina_vpath.h',
   'utils/eina_xattr.h',
]

public_sub_headers = public_sub_headers_checksum \
                   + public_sub_headers_data  \
                   + public_sub_headers_data_inline \
                   + public_sub_headers_debug \
                   + public_sub_headers_log \
                   + public_sub_headers_memory \
                   + public_sub_headers_safe \
                   + public_sub_headers_string \
                   + public_sub_headers_thread \
                   + public_sub_headers_utils

public_headers = [
  'Eina.h'
]

src_etool_checksum = files(
   'checksum/eina_crc.c',
   'checksum/eina_sha.c',
)

src_etool_data = files(
   'data/eina_abstract_content.c',
   'data/eina_accessor.c',
   'data/eina_array.c',
   'data/eina_fp.c',
   'data/eina_hash.c',
   'data/eina_inarray.c',
   'data/eina_inlist.c',
   'data/eina_iterator.c',
   'data/eina_list.c',
   'data/eina_matrix.c',
   'data/eina_matrixsparse.c',
   'data/eina_promise.c',
   'data/eina_quad.c',
   'data/eina_quadtree.c',
   'data/eina_quaternion.c',
   'data/eina_rbtree.c',
   'data/eina_rectangle.c',
   'data/eina_tiler.c',
   'data/eina_value_util.c',
   'data/eina_value.c',
)

src_etool_debug = files(
   'debug/eina_debug_bt_file.c',
   'debug/eina_debug_bt.c',
   'debug/eina_debug_chunk.c',
   'debug/eina_debug_cpu.c',
   'debug/eina_debug_thread.c',
   'debug/eina_debug_timer.c',
   'debug/eina_debug.c',
)

src_etool_log = files(
   'log/eina_error.c',
   'log/eina_evlog.c',
   'log/eina_fnmatch.c',
   'log/eina_log.c',
)

src_etool_memory = files(
   'memory/eina_binshare.c',
   'memory/eina_cow.c',
   'memory/eina_freeq.c',
   'memory/eina_lalloc.c',
   'memory/eina_mempool.c',
   'memory/eina_mmap.c',
)

src_etool_safe = files(
   'safe/eina_safepointer.c',
   'safe/eina_safety_checks.c',
)

src_etool_string = files(
   'string/eina_binbuf.c',
   'string/eina_share_common.c',
   'string/eina_slstr.c',
   'string/eina_str.c',
   'string/eina_strbuf_common.c',
   'string/eina_strbuf.c',
   'string/eina_stringshare.c',
   'string/eina_tmpstr.c',
   'string/eina_unicode.c',
   'string/eina_ustrbuf.c',
   'string/eina_ustringshare.c',
)

src_etool_thread = files(
   'thread/eina_lock.c',
   'thread/eina_thread_queue.c',
   'thread/eina_thread.c',
)

src_etool_utils = files(
   'utils/eina_benchmark.c',
   'utils/eina_bezier.c',
   'utils/eina_convert.c',
   'utils/eina_counter.c',
   'utils/eina_cpu.c',
   'utils/eina_file_common.c',
   'utils/eina_hamster.c',
   'utils/eina_magic.c',
   'utils/eina_module.c',
   'utils/eina_prefix.c',
   'utils/eina_simple_xml_parser.c',
   'utils/eina_util.c',
   'utils/eina_vpath_xdg.c',
   'utils/eina_vpath.c',
   'utils/eina_xattr.c',
)

src_etool_win32 = files(
   'win32/eina_file_win32.c',
   'win32/eina_win32_dllmain.c',
   'win32/eina_thread_win32.c'
)

src_etool = files(
   'eina_abi.c',
   'eina_main.c',
)

src_etool += src_etool_checksum \
           + src_etool_data \
           + src_etool_debug \
           + src_etool_log \
           + src_etool_memory \
           + src_etool_safe \
           + src_etool_string \
           + src_etool_thread \
           + src_etool_utils \
           + eina_mp_sources # module

if sys_windows
  src_etool += src_etool_win32
else
  src_etool += files('utils/eina_file_posix.c', 'thread/eina_thread_posix.c')
endif

etool_config = configuration_data()

if get_option('buildtype') == 'debug'
   with_max_log_level = -1
   stringshare_usage  = true
   use_valgrind       = true
   debug_malloc       = true
   debug_threads      = true
   default_mempool    = true
   want_cow_magic     = true
elif get_option('buildtype') == 'release'
   with_max_log_level = 3
   stringshare_usage  = false
   use_valgrind       = false
   debug_malloc       = false
   debug_threads      = false
   default_mempool    = false
   want_cow_magic     = false
else
   with_max_log_level = -1
   stringshare_usage  = false
   use_valgrind       = true
   debug_malloc       = false
   debug_threads      = false
   default_mempool    = false
   want_cow_magic     = false
endif

# efl_config.set('EINA_LOG_LEVEL_MAXIMUM', 3)
efl_config.set('EINA_LOG_LEVEL', 1)

if stringshare_usage
   efl_config.set('EINA_STRINGSHARE_USAGE', 1)
endif

if use_valgrind
   valgrind = dependency('valgrind', required: false)
   if valgrind.found()
     efl_config.set('NVALGRIND', 1)
   else
     efl_config.set('HAVE_VALGRIND', 1)
   endif
   dep_etool += valgrind
else
   efl_config.set('NVALGRIND', 1)
   valgrind = []
endif

if debug_malloc
   efl_config.set('EINA_DEBUG_MALLOC', 1)
endif

if want_cow_magic
   efl_config.set('EINA_COW_MAGIC_ON', 1)
endif

if get_option('eina-magic-debug')
   etool_config.set('EINA_MAGIC_DEBUG', '1')
endif

if default_mempool
   etool_config.set('EINA_DEFAULT_MEMPOOL', '1')
endif

if get_option('systemd')
  efl_config.set('HAVE_SYSTEMD', '1')
endif

etool_config.set('EINA_SAFETY_CHECKS', '1')
etool_config.set('EINA_HAVE_THREADS', '1')

if cc.has_header_symbol('pthread.h', 'pthread_barrier_init')
   etool_config.set('EINA_HAVE_PTHREAD_BARRIER', '1')
endif

if sys_linux
   pthread_np_header_file = 'pthread.h'
   pthread_setname_np_api = 'pthread_setname_np'
   pthread_args = '-D_GNU_SOURCE'
else
   pthread_np_header_file = 'pthread_np.h'
   pthread_setname_np_api = 'pthread_set_name_np'
   pthread_args = []
endif

if cc.has_header_symbol(pthread_np_header_file, pthread_setname_np_api, args : pthread_args)
   etool_config.set('EINA_HAVE_PTHREAD_SETNAME', '1')
endif

if cc.has_header_symbol(pthread_np_header_file, 'pthread_attr_setaffinity_np', args : pthread_args)
   etool_config.set('EINA_HAVE_PTHREAD_AFFINITY', '1')
endif

if debug_threads and get_option('debug-threads')
   if cc.has_header_symbol('execinfo.h', 'backtrace')
      etool_config.set('EINA_HAVE_DEBUG_THREADS', '1')
   endif
endif

etool_config.set('EINA_SIZEOF_WCHAR_T', cc.sizeof('wchar_t',  prefix : '#include<wchar.h>'))

uintptr_size = cc.sizeof('uintptr_t',  prefix : '#include<stdint.h>')

etool_config.set('EINA_SIZEOF_UINTPTR_T', uintptr_size)
efl_config.set('SIZEOF_UINTPTR_T', uintptr_size)

if cc.has_header('dirent.h')
   etool_config.set('EINA_CONFIGURE_HAVE_DIRENT_H', '1')
endif

etool_config.set('EINA_ENABLE_LOG', '1')

if cc.has_header_symbol('alloca.h', 'alloca')
   etool_config.set('EINA_HAVE_ALLOCA_H', '1')
endif

if cc.has_header('byteswap.h')
   etool_config.set('EINA_HAVE_BYTESWAP_H', '1')
endif

if cc.has_header_symbol('pthread.h', 'pthread_spin_init')
   etool_config.set('EINA_HAVE_POSIX_SPINLOCK', '1')
endif

if sys_osx
   if cc.has_header_symbol('libkern/OSAtomic.h', 'OSSpinLockTry')
     etool_config.set('EINA_HAVE_OSX_SPINLOCK', 1)
   endif
   if cc.has_header_symbol('mach/task.h', 'semaphore_create')
     etool_config.set('EINA_HAVE_OSX_SEMAPHORE', 1)
   endif
endif

if host_machine.endian() == 'big'
   etool_config.set('EINA_HAVE_WORDS_BIGENDIAN', '1')
endif

etool_config_file = configure_file(
  output: 'etool_config.h',
  configuration: etool_config,
  install_dir: dir_package_include
)

public_headers += etool_config_file

unwind = dependency('libunwind-generic', required: false)
if unwind.found()
   efl_config.set('HAVE_UNWIND', 1)
   dep_etool += unwind
   efl_config.set('EINA_LOG_BACKTRACE_ENABLE', 1)
endif

iconv = dependency('iconv', required: false, method: 'auto')
dep_etool += [iconv]

execinfo = cc.find_library('execinfo', required: false)
dep_etool += [execinfo]

if sys_windows
   bcrypt = cc.find_library('bcrypt', required: true)
   dep_etool += [bcrypt]
endif

lib_etool = library('etool', src_etool,
  c_args : [package_c_args, '-DEINA_BUILD'],
  include_directories : inc_config + inc_etool,
#   dependencies: dep_etool + [eina_deps, evil],
  dependencies: dep_etool,
  install: true,
  version : version
)

etool = declare_dependency(
  include_directories: inc_config + inc_etool,
  dependencies : dep_etool,
  link_with: lib_etool,
)

install_headers(public_headers,
  install_dir : dir_package_include,
)

install_headers(public_sub_headers,
  install_dir : join_paths(dir_package_include, 'etool')
)

package_header_subdirs += 'etool'

dep_etool_one = dep_etool

efl_tools_summary = {'Package cargs': package_c_args}
summary(efl_tools_summary, section: 'EFL tools')