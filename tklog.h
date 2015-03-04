//
//  tklog.h
//  Created by Taras Kalapun on 3/3/15.
//

#ifndef __tklog__
#define __tklog__


#include <stdlib.h>

#if defined(__OBJC__) || (!defined(_tklog_C_MODE))
#   define __tklog_objc_mode
#endif

#define _tklog_default_component "main"
#ifndef log_component
#define log_component _tklog_default_component
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
    void (*render_line)(const char *str);
    void (*render_component)(const char *str);
} tklog_driver;


typedef struct {
    tklog_driver *driver;
    _tklog_level_t default_log_level;
//    int use_colors;
    char **components;
    uint32_t components_count;
    uint32_t *components_levels;
} tklog_t;

// Public
void tklog_init(char *components[], uint32_t count);
void tklog_add_component(const char *name);

void tklog_configure_by_name(const char *name, _tklog_level_t level);

void tklog_log_line(const char *str);

void tklog_vlog_component(const char *component, _tklog_level_t level, const char *format, va_list args);
void tklog_log_component(const char *component, _tklog_level_t level, const char *format, ...);

void tklog_logf(const char *component,
                uint32_t level,
                const char *filename,
                uint32_t line,
                const char *function,
                const char *format, ...);

#ifndef _tklog_filename
#   define _tklog_filename                                                       \
    (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#ifdef __tklog_objc_mode
#define tklog_log(_component, _level, _format, ...)                             \
    tklog_logf(_component, _level, _tklog_filename, __LINE__, __PRETTY_FUNCTION__, [[NSString stringWithFormat:_format, ## __VA_ARGS__] cStringUsingEncoding:NSUTF8StringEncoding])
#else
#define tklog_log(_component, _level, _format, ...)                             \
    tklog_logf(_component, _level, _tklog_filename, __LINE__, __PRETTY_FUNCTION__, _format, ## __VA_ARGS__)
#endif

#define tklog_log_if(_component, _level, _predicate, _format, ...)             \
    do {                                                                       \
        if (_predicate) {                                                      \
            tklog_log(_component, _level, _format, ##__VA_ARGS__);             \
        }                                                                      \
    } while (0)


#define log_critical(...)                                                        \
    tklog_log(log_component, tklog_vCritical, __VA_ARGS__)

#define log_error(...)                                                           \
    tklog_log(log_component, tklog_vError, __VA_ARGS__)

#define log_warning(...)                                                         \
    tklog_log(log_component, tklog_vWarning, __VA_ARGS__)

#define log_info(...)                                                            \
    tklog_log(log_component, tklog_vInfo, __VA_ARGS__)

#define log_debug(...)                                                           \
    tklog_log(log_component, tklog_vDebug, __VA_ARGS__)

#define log_trace(...)                                                           \
    tklog_log(log_component, tklog_vTrace, __VA_ARGS__)

#define log_critical_if(predicate, ...)                                          \
    tklog_log_if(log_component, tklog_vCritical, predicate, __VA_ARGS__)

#define log_error_if(predicate, ...)                                             \
    tklog_log_if(log_component, tklog_vError, predicate, __VA_ARGS__)

#define log_warning_if(predicate, ...)                                           \
    tklog_log_if(log_component, tklog_vWarning, predicate, __VA_ARGS__)

#define log_info_if(predicate, ...)                                              \
    tklog_log_if(log_component, tklog_vInfo, predicate, __VA_ARGS__)

#define log_debug_if(predicate, ...)                                             \
    tklog_log_if(log_component, tklog_vDebug, predicate, __VA_ARGS__)

#define log_trace_if(predicate, ...)                                             \
    tklog_log_if(log_component, tklog_vTrace, predicate, __VA_ARGS__)

void tklog_set_render_line(void (*render_line_ptr)(const char *str));


// Private
void tklog_renderer_line(const char *str);
void tklog_render_component(const char *component, _tklog_level_t level, const char *str);


#endif /* defined(__Pods__tklog__) */
