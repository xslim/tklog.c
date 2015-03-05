//
//  tklog.h
//  Created by Taras Kalapun on 3/3/15.
//

#ifndef __tklog__
#define __tklog__


#include <stdlib.h>
#include <stdio.h>

#if defined(__OBJC__) && (!defined(_tklog_C_MODE))
#  define __tklog_objc_mode
#endif

#define _tklog_default_component "main"

#ifndef log_component
#  define log_component _tklog_default_component
#endif

#ifndef log_component_prefix
#  define log_component_prefix ""
#endif

#ifndef tklog_component
#  define tklog_component log_component_prefix log_component
#endif

// Log levels, prefixed with 'adylcl_v'.
enum _tklog_enum_level_t {
    tklog_vOff = 0,
    
    tklog_vCritical,              // critical situation
    tklog_vError,                 // error situation
    tklog_vWarning,               // warning
    tklog_vInfo,                  // informational message
    tklog_vDebug,                 // coarse-grained debugging information
    tklog_vTrace,                 // fine-grained debugging information
    
    _tklog_level_t_count,
    _tklog_level_t_first = 0,
    _tklog_level_t_last  = _tklog_level_t_count-1
};

// Log level type.
typedef uint32_t _tklog_level_t;


typedef int (*tklog_render_line)(const char *str);


typedef struct {
    void (*render_linec)(const char *component, int level, const char *prefix, const char *str);
    void (*render_line)(const char *str);
} tklog_driver;


typedef struct {
    tklog_driver *driver;
    _tklog_level_t default_log_level;
    char *filepath;
    FILE *file;
    int use_colors;
    char **components;
    uint32_t components_count;
    uint32_t *components_levels;
} tklog_t;

// Public
void tklog_init(char *components[], int count);
void tklog_add_component(const char *name);

void tklog_configure_by_name(const char *name, _tklog_level_t level);
int tklog_component_active(const char *component, _tklog_level_t level);

void tklog_set_render_linec(void (*render_line_ptr)(const char *component, int level, const char *prefix, const char *str));
void tklog_set_render_line(void (*render_line_ptr)(const char *str));

int tklog_set_log_file(const char *filepath);
void tklog_set_enable_colors(int enable);

char *tklog_get_time(void);


void tklog_log_line(const char *component, int level, const char *prefix, const char *str);
void tklog_log_linef(const char *component, int level, const char *func, int line, const char *format, ...);
// Private
void tklog_renderer_linec(const char *component, int level, const char *prefix, const char *str);
void tklog_renderer_line(const char *str);

extern const char * const _tklog_level_header_1[_tklog_level_t_count];  // header with 1 character
extern const char * const _tklog_level_header_1c[_tklog_level_t_count]; // header with 1 character, colored

#ifndef _tklog_filename
#  define _tklog_filename (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#ifdef __tklog_objc_mode
#  define _ady_str @""
#else
#  define _ady_str ""
#endif

#if DEBUG
#  define _tklog_debug_mode 1
#  define _tklog_color 1
#else
#  define _tklog_debug_mode 0
#  define _tklog_color 0
#endif


#define _tklog_prefix "" __func__ ":"__LINE__

#ifdef __tklog_objc_mode
#  define _tklog_c_str(s, ...) [[NSString stringWithFormat:s, ## __VA_ARGS__] cStringUsingEncoding:NSUTF8StringEncoding]
#else
#  define _tklog_c_str(s, ...) s, ## __VA_ARGS__
#endif


#define tklog_log(_component, _level, _format, ...)                             \
    do {                                                                         \
        if (tklog_component_active(_component, _level)) {                       \
            tklog_log_linef(_component, _level, __func__, __LINE__, _tklog_c_str(_format, ## __VA_ARGS__)); \
        } \
    } while (0)


#define tklog_log_if(_component, _level, _predicate, _format, ...)             \
    do {                                                                       \
        if (_predicate) {                                                      \
            tklog_log(_component, _level, _format, ##__VA_ARGS__);             \
        }                                                                      \
    } while (0)



#define tklog_configure(_component, _level) \
    tklog_configure_by_name(log_component_prefix _component, _level)


#define log_critical(...)                                                        \
    tklog_log(tklog_component, tklog_vCritical, _ady_str __VA_ARGS__)

#define log_error(...)                                                           \
    tklog_log(tklog_component, tklog_vError, _ady_str __VA_ARGS__)

#define log_warning(...)                                                         \
    tklog_log(tklog_component, tklog_vWarning, _ady_str __VA_ARGS__)

#define log_info(...)                                                            \
    tklog_log(tklog_component, tklog_vInfo, _ady_str __VA_ARGS__)

#define log_debug(...)                                                           \
    tklog_log(tklog_component, tklog_vDebug, _ady_str __VA_ARGS__)

#define log_trace(...)                                                           \
    tklog_log(tklog_component, tklog_vTrace, _ady_str __VA_ARGS__)

#define log_critical_if(predicate, ...)                                          \
    tklog_log_if(tklog_component, tklog_vCritical, predicate, _ady_str __VA_ARGS__)

#define log_error_if(predicate, ...)                                             \
    tklog_log_if(tklog_component, tklog_vError, predicate, _ady_str __VA_ARGS__)

#define log_warning_if(predicate, ...)                                           \
    tklog_log_if(tklog_component, tklog_vWarning, predicate, _ady_str __VA_ARGS__)

#define log_info_if(predicate, ...)                                              \
    tklog_log_if(tklog_component, tklog_vInfo, predicate, _ady_str __VA_ARGS__)

#define log_debug_if(predicate, ...)                                             \
    tklog_log_if(tklog_component, tklog_vDebug, predicate, _ady_str __VA_ARGS__)

#define log_trace_if(predicate, ...)                                             \
    tklog_log_if(tklog_component, tklog_vTrace, predicate, _ady_str __VA_ARGS__)


#endif /* defined(__Pods__tklog__) */
