#ifndef CFNAME_H
#define CFNAME_H

// Defines ----------------------------------------------------------
// Typedefs ---------------------------------------------------------
typedef enum _t_fnf_error {
	CFN_NOERROR = 0,
	CFN_BAD_PINTER = -1,
	CFN_BAD_STRING = -2,
	CFN_ALLOC_ERROR = -3
} cfname_error;

// Extern variables -------------------------------------------------
// Prototypes -------------------------------------------------------
int prepare_new_file_name(char *fname, char ** nfname);

#endif // CFNAME_H
