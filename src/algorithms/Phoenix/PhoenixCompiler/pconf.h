/* PCONF.H
 *------------------------------------------------------------*
 * Copyright 1988, Fil Alleva and Carnegie Mellon University.
 *------------------------------------------------------------*
 * HISTORY
 */

#ifndef _PCONF_
#define _PCONF_

#include <sys/types.h>

typedef enum {NOTYPE,
	      BYTE, SHORT, INT, LONG,
	      U_BYTE, U_SHORT, U_INT, U_LONG,
	      FLOAT, DOUBLE,
	      BooL, CHAR, STRING,
	      DATA_SRC
} arg_t;

typedef enum {
	SRC_NONE, SRC_HSA, SRC_VQFILE, SRC_CEPFILE, SRC_ADCFILE
} data_src_t;

typedef void* caddr_t;

typedef union _ptr {
    char	*CharP;
    char	*ByteP;
    unsigned char	*UByteP;
    short	*ShortP;
    unsigned short	*UShortP;
    int		*IntP;
    unsigned int	*UIntP;
    long	*LongP;
    unsigned long	*ULongP;
    float	*FloatP;
    double	*DoubleP;
    int		*BoolP;
    char	**StringP;
    data_src_t	*DataSrcP;

} ptr_t;

typedef struct _config {
	char	*LongName;		/* Long Name */
	char	*Doc;			/* Documentation string */
	char	*swtch;			/* Switch Name */
	arg_t	arg_type;		/* Argument Type */
	caddr_t	var;			/* Pointer to the variable */
} config_t;

typedef struct _InternalConfig {
	char	*LongName;		/* Long Name */
	char	*Doc;			/* Documentation string */
	char	*swtch;			/* Switch Name */
	arg_t	arg_type;		/* Argument Type */
	ptr_t	var;			/* Pointer to the variable */
} Config_t;

void quit(int err, char *fmt, char *str);
void pusage (char *prog, Config_t *cp);
int pconf (int argc, char *argv[], Config_t *config_p, char **display, char **geometry,  char *(*GetDefault)());
int ppconf (int argc, char *argv[], Config_t *config_p, char **display, char **geometry, char *(*GetDefault)(), char last);

#endif
