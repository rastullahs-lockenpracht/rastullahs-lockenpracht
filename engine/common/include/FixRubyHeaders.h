/**
 * Einige Undefines, für unfreundliche defines in den Ruby-Headers.
 * Dort werden viele C-Funktionen überdeckt, die in Ogre aber benötigt werden.
 * Diese Datei muss nach einem Ruby-Header inkludiert werden.
 */
#ifndef __FIXRUBYHEADERS_H__
#define __FIXRUBYHEADERS_H__

#undef close
#undef bind
#undef select
#undef shutdown
#undef snprintf
#undef vsnprintf
#undef Sleep
#undef sleep

#endif
