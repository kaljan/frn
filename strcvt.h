#ifndef CFNAME_H
#define CFNAME_H

// Defines ----------------------------------------------------------
// Typedefs ---------------------------------------------------------

// Extern variables -------------------------------------------------
extern int strcvt_errno;

// Prototypes -------------------------------------------------------
char *convert_string(const char *str);
const char * strcvt_strerr(int err);
#endif // CFNAME_H
