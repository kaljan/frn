#ifndef CFNAME_H
#define CFNAME_H

// Defines ----------------------------------------------------------
// Typedefs ---------------------------------------------------------
typedef enum _t_fnf_error {
	FNF_NOERROR = 0,
	FNF_BAD_PINTER = -1,
	FNF_BAD_STRING = -2,
	FNF_ALLOC_ERROR = -3
} fnf_error;

// Extern variables -------------------------------------------------
// Prototypes -------------------------------------------------------
int prepare_new_file_name(char *fname, char ** nfname);

#endif // CFNAME_H
