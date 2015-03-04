//
//  tklog.c
//  Created by Taras Kalapun on 3/3/15.
//

#include "tklog.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

static tklog_t *_tklog_instance = NULL;
static _tklog_level_t _tklog_default_level = tklog_vError;


const char * const _tklog_level_header_1[_tklog_level_t_count] = {
    "-",
    "C",
    "E",
    "W",
    "I",
    "D",
    "T"
};

#define COLORS_ESCAPE "\033["
#define COLORS_RESET  COLORS_ESCAPE ";"
#define COLORS_RED COLORS_ESCAPE "fg255,0,0;"
#define COLORS_YELLOW COLORS_ESCAPE "fg255,255,0;"
#define COLORS_PURPLE COLORS_ESCAPE "fg255,0,255;"
#define COLORS_BLUE COLORS_ESCAPE "fg0,0,255;"
#define COLORS_GREEN COLORS_ESCAPE "fg0,255,0;"

const char * const _tklog_level_header_1c[_tklog_level_t_count] = {
    "-",
    COLORS_RED     "C" COLORS_RESET,
    COLORS_RED     "E" COLORS_RESET,
    COLORS_YELLOW  "W" COLORS_RESET,
    COLORS_GREEN   "I" COLORS_RESET,
    COLORS_PURPLE  "D" COLORS_RESET,
    COLORS_BLUE    "T" COLORS_RESET
};

int _tklog_component_position(tklog_t *log, const char *name) {
    uint32_t count = log->components_count;
    for (uint32_t c = 0; c < count; c++) {
        if (strcmp(name, log->components[c]) == 0) {
            return c;
        }
    }
    return -1;
}

int _tklog_has_component(tklog_t *log, const char *name) {
    uint32_t count = log->components_count;
    for (uint32_t c = 0; c < count; c++) {
        if (strcmp(name, log->components[c]) == 0) {
            return 1;
        }
    }
    return 0;
}

int _tklog_add_component(tklog_t *log, const char *name) {
    
    
    if (log->components_count == 0 || !log->components) {
        log->components_count++;
        log->components = (char**)malloc(sizeof(char*));
        log->components_levels = malloc(sizeof(uint32_t));
    } else {
        
        if (_tklog_has_component(log, name)) {
            return -1;
        }
        
        log->components_count++;
        log->components = (char**)realloc(log->components, log->components_count * sizeof(char*));
        log->components_levels = realloc(log->components_levels, log->components_count * sizeof(uint32_t));
    }
    
    uint32_t current = log->components_count -1;
    log->components[current] = (char*)malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(log->components[current], name);
    log->components_levels[current] = log->default_log_level;
    return current;
}

tklog_t *
tklog_instance(void) {
    if (!_tklog_instance) {
        _tklog_instance = malloc(sizeof(tklog_t));
        
        _tklog_instance->components_count = 0;
        _tklog_instance->components = NULL;
        _tklog_instance->components_levels = NULL;
        
        _tklog_instance->default_log_level = _tklog_default_level;
        
        _tklog_instance->driver = malloc(sizeof(tklog_driver));
        _tklog_instance->driver->render_line = &tklog_renderer_line;
        
        _tklog_add_component(_tklog_instance, _tklog_default_component);
    }
    
    return _tklog_instance;
}


void tklog_init(char *components[], uint32_t count) {
    if (_tklog_instance) {
        // error
        return;
    }
    
    tklog_t *log = tklog_instance();
    
    for (int c = 0; c < count; c++) {
        _tklog_add_component(log, components[c]);
    }
}

void tklog_add_component(const char *name) {
    tklog_t *log = tklog_instance();
    _tklog_add_component(log, name);
}


void tklog_log_line(const char *str) {
    tklog_t *log = tklog_instance();
    if (log->driver->render_line) {
        log->driver->render_line(str);
    }
}

void tklog_vlog_component(const char *component, _tklog_level_t level, const char *format, va_list args) {
    tklog_t *log = tklog_instance();
    
    int pos = _tklog_component_position(log, component);
    if (pos < 0) {
        // Add if not there
        pos = _tklog_add_component(log, component);
    }
    
    _tklog_level_t component_level = log->components_levels[pos];
    if (component_level < level) {
        return;
    }
    
    char *str;
    vasprintf(&str, format, args);
    
    char *buf;
    
#define log_printf_format "%s %s: %s"
#if DEBUG
#define log_printf_level _tklog_level_header_1c[level]
#else
#define log_printf_level _tklog_level_header_1[level]
#endif
    
    asprintf(&buf, log_printf_format, log_printf_level, component, str);
    tklog_log_line(buf);
    free(buf);
    
    free(str);
}

void tklog_log_component(const char *component, _tklog_level_t level, const char *format, ...) {
    va_list args;
    va_start(args, format);
    tklog_vlog_component(component, level, format, args);
    va_end(args);
}


void tklog_logf(const char *component,
                uint32_t level,
                const char *filename,
                uint32_t line,
                const char *function,
                const char *format, ...)
{
    char *buf;
    asprintf(&buf, "%s:%d:%s: %s", filename, line, function, format);

    va_list args;
    va_start(args, format);
    tklog_vlog_component(component, level, buf, args);
    va_end(args);
    
    free(buf);
}

// Configures the given log level for the given log component(s).


// tklog_configure_by_name("*", tklog_vTrace);
void tklog_configure_by_name(const char *name, _tklog_level_t level) {

    tklog_t *log = tklog_instance();
    uint32_t count = log->components_count;

    // no text given, quit
    if (name == NULL || name[0] == '\0') {
        return; //0
    }

    // unsupported level, clip to last level
    if (level > _tklog_level_t_last) {
        level = _tklog_level_t_last;
    }
//
    // configure the components
    size_t text_len = strlen(name);
    if (name[text_len-1] == '*') {
        // text ends with '*', wildcard suffix was specified
        
        if (text_len == 1) log->default_log_level = level;
        
        text_len--;
        for (uint32_t c = 0; c < count; c++) {
            if (strncmp(name, log->components[c], text_len) == 0) {
                log->components_levels[c] = level;
            }
        }
    } else {
        // no wildcard suffix was specified
        int pos = _tklog_component_position(log, name);
        if (pos < 0) {
            pos = _tklog_add_component(log, name);
        }
        log->components_levels[pos] = level;
    }
}


void tklog_set_render_line(void (*render_line_ptr)(const char *str)) {
    tklog_t *log = tklog_instance();
    log->driver->render_line = render_line_ptr;
}

void tklog_renderer_line(const char *str) {
    printf("%s\n", str);
}
