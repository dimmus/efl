#ifndef _ECORE_INPUT_PRIVATE_H
#define _ECORE_INPUT_PRIVATE_H

extern int _core_input_log_dom;

#ifdef ECORE_INPUT_DEFAULT_LOG_COLOR
#  undef ECORE_INPUT_DEFAULT_LOG_COLOR
#endif

#define ECORE_INPUT_DEFAULT_LOG_COLOR EINA_COLOR_BLUE

#ifdef ERR
#  undef ERR
#endif
#define ERR(...) EINA_LOG_DOM_ERR(_core_input_log_dom, __VA_ARGS__)

#ifdef DBG
#  undef DBG
#endif
#define DBG(...) EINA_LOG_DOM_DBG(_core_input_log_dom, __VA_ARGS__)

#ifdef INF
#  undef INF
#endif
#define INF(...) EINA_LOG_DOM_INFO(_core_input_log_dom, __VA_ARGS__)

#ifdef WRN
#  undef WRN
#endif
#define WRN(...) EINA_LOG_DOM_WARN(_core_input_log_dom, __VA_ARGS__)

#ifdef CRI
#  undef CRI
#endif
#define CRI(...) EINA_LOG_DOM_CRIT(_core_input_log_dom, __VA_ARGS__)

int ecore_input_joystick_init(void);
int ecore_input_joystick_shutdown(void);
#endif
