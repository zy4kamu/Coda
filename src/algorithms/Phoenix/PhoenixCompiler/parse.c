/* parse function
   Take word string as argument
   Generate lattice of parses in structure "parses"
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <malloc.h>
#include <time.h>

#include "parse.h"
#include "pconf.h"

#define NO_SCORE 0xFFFF

/* ********* Command Line Parameters ************** */

/* flags */
int	verbose		=1,
	extract		=0,
	MAX_PARSES	=10,
	USE_HISTORY	=0,
	PROFILE		=0,
	IGNORE_OOV	=1,
	ALL_PARSES	=1,
	BIGRAM_PRUNE	=0,
	MAX_PATHS	=0;	/* if set, limit max paths expanded */


/* files */
char	*grammar_file = NULL,
	*dir = NULL,
	*config_file = NULL,
	*function_wrd_file = NULL;

/* default buffer sizes */
int	EdgeBufSize=	1000,	/* max number of paths in beam */
	ChartBufSize=	40000,	/* max number of paths in beam */
	PeBufSize=	2000,	/* number of Val slots for trees  */
	InputBufSize=	1000,	/* max words in line of input */
	StringBufSize=	50000,	/* max words in line of input */
	SlotSeqLen=	200,	/* max number of slots in a sequence */
	FrameBufSize=	500,	/* buffer for frame nodes */
	SymBufSize=	100000,	/* buffer size to hold char strings */
	ParseBufSize=	200,	/* buffer for parses */
	SeqBufSize=	500,	/* buffer for sequence nodes */
	PriBufSize=	2000,	/* buffer for sequence nodes */
	FidBufSize=	1000;	/* buffer for frame ids */

char	sym1[] = "<s>",		/* default start of utt symbol */
	sym2[] = "</s>",	/* default end of utt symbol */
	*start_sym= sym1,
	*end_sym= sym2;

/* Command Line structure */

config_t conf[] = {
  {"dir",	"dir containing grammar files (frames, base.dic, etc)",
	   "-dir", 	STRING,	(caddr_t) &dir},
  {"grammar",	"file containing compiled grammar nets",
	   "-grammar", 	STRING,	(caddr_t) &grammar_file},
  {"start_sym",	"start of utterance grammar symbol",
	   "-start_sym", 	STRING,	(caddr_t) &start_sym},
  {"end_sym",	"end of utterance grammar symbol",
	   "-end_sym", 	STRING,	(caddr_t) &end_sym},
  {"config_file",	"file containing command line arguments",
	   "-config", 	STRING,	(caddr_t) &config_file},
  {"function_wrd_file",	"file containing function words",
	   "-function_wrd", STRING,	(caddr_t) &function_wrd_file},
  {"verbose",	"verbosity level",
	   "-verbose", 	INT, 	(caddr_t) &verbose},
  {"extract",	"0= print full parse, 1= print extracted elements",
	   "-extract",	INT,		(caddr_t) &extract},
  {"MAX_PARSES",	"maximum number of parses to print",
	   "-MAX_PARSES",	INT,	(caddr_t) &MAX_PARSES},
  {"USE_HISTORY",	"use history in finding best parse",
	   "-USE_HISTORY",	INT,	(caddr_t) &USE_HISTORY},
  {"PROFILE",	"print profile stats",
	   "-PROFILE",	INT,		(caddr_t) &PROFILE},
  {"IGNORE_OOV",	"Ignore oov words in parse",
	   "-IGNORE_OOV",	INT,	(caddr_t) &IGNORE_OOV},
  {"ALL_PARSES",	"Print all parses to text buffer",
	   "-ALL_PARSES",	INT,	(caddr_t) &ALL_PARSES},
  {"BIGRAM_PRUNE",	"1= aggressive pruning in parse search",
	   "-BIGRAM_PRUNE",	INT,	(caddr_t) &BIGRAM_PRUNE},
  {"MAX_PATHS", "Maximum provisional edges expanded before pruning", 
	   "-MAX_PATHS",	INT,	(caddr_t) &MAX_PATHS},
  {"EdgeBufSize", "Maximum edges in chart", 
	   "-EdgeBufSize",	INT,	(caddr_t) &EdgeBufSize},
  {"ChartBufSize", "Maximum edge links in chart", 
	   "-ChartBufSize", INT,	(caddr_t) &ChartBufSize},
  {"PeBufSize", "Number of Val structs for trees", 
	   "-PeBufSize", INT,	(caddr_t) &PeBufSize},
  {"SymBufSize", "Size in bytes of symbol buffer for dictionary", 
	   "-SymBufSize",	INT,	(caddr_t) &SymBufSize},
  {"FrameBufSize", "Maximum number of Frame Nodes in parses", 
	   "-FrameBufSize",	INT,	(caddr_t) &FrameBufSize},
  {"FidBufSize", "Maximum size of Frame Table Structure", 
	   "-FidBufSize",	INT,	(caddr_t) &FidBufSize},
  {"PriBufSize", "Maximum size of Frame Table Structure", 
	   "-PriBufSize",	INT,	(caddr_t) &PriBufSize},
  {"ParseBufSize", "Buffer for set of parses", 
	   "-ParseBufSize",	INT,	(caddr_t) &ParseBufSize},
  {"InputBufSize", "Maximum number of words in line of input", 
	   "-InputBufSize",	INT,	(caddr_t) &InputBufSize},
  {"StringBufSize", "size of buffer for parse substrings", 
	   "-StringBufSize",	INT,	(caddr_t) &StringBufSize},
  {"SeqBufSize", "Maximum size of Seq Table Structure", 
	   "-SeqBufSize",	INT,	(caddr_t) &SeqBufSize},
  {"SlotSeqLen", "Maximum number of slots in a sequence", 
	   "-SlotSeqLen",	INT,	(caddr_t) &SlotSeqLen},
  {NULL, NULL, NULL, NOTYPE, NULL}
  };


/* file parameters */
char	dict_file[LABEL_LEN] =		"base.dic",
	priority_file[LABEL_LEN] =	"NET_PRIORITIES",
	frames_file[LABEL_LEN] =	"forms";



/*** global variables ***/

int	*active_slots,		/* set of slot level nets used in frames */
	*cur_nets,		/* set of nets to be used in this parse */
	num_active,		/* length of cur_nets */
	num_nets,		/* total number of nets read in */
	num_frames,		/* total number of frames read in */
	max_pri;		/* number of priority levels assigned to nets */

Gram	*gram;			/* grammar */

/* parser structures */
int	*script,		/* array of word nums for input line */
	script_len,		/* number of words in script */
	num_seqs,		/* number of current parses */
	start_token,		/* word number of start symbol */
	end_token;		/* word number of end symbol */

char	fun_wrds[MAX_WRDS];	/* flags indicating function words */


/* parser buffers */
SeqNode		**pbuf, **pbuf_end;	/* lattice of final parses */
EdgeLink	**chart;	/* chart of matched nets */
Edge 		*edge_buf,	/* buffer of chart edges */
		*edge_buf_end, 
		*edge_ptr;	/* ptr to next free edge in buf */
EdgeLink	*edge_link_buf,	/* buffer of nodes to link edges in lists */
		*edge_link_end,
		*edge_link_ptr;
Edge		**pe_buf,	/* buffer for trees pointed to by edges */
		**pe_buf_end,
		**pe_buf_ptr;
Edge		**slot_seq;
SeqCell		*seq_end;
SeqNode		*seq_buf,		/* buffer for sequence path tree */
		*seq_buf_ptr,
		*seq_buf_end;
Fid		*fid_buf,		/* buffer for frame path tree */
		*fid_buf_ptr,
		*fid_buf_end;
unsigned short	*pri_buf, *pri_buf_ptr, *pri_buf_end;
char		*pstr_buf,
		*pstr_buf_ptr,
		*pstr_buf_end;

int	first_concept;		/* used by print_extracts() */

int	fr_seq[100];
SeqNode	*sl_seq[100];
SeqNode *parses[500];
int	num_parses,
	hi,
	history[ HIST_LEN];

/**** Static Variables******* */
FrameNode	*frame_buf,		/* buffer for frame path tree */
		*frame_buf_ptr,
		*frame_buf_end,
		**fr_list1,
		**fr_list2;

int		n_slots,	/* number of slots in current parses */
		n_frames;	/* number of frames in current parses */

/* used for segmenting input */
int	last_end = 0,
	brk_pt = 0;

char	print_line[LINE_LEN],
	*print_line_ptr;

/* forward function declarations */
SeqNode		*get_new_slot();
FrameNode	*get_new_frame();
Edge 		*copy_tree();


FrameNode *get_new_frame()
{
	if( frame_buf_ptr == frame_buf_end ) {
	fprintf(stderr, "ERROR: overflow FrameBufSize  %d\n", FrameBufSize);
	return((FrameNode *)0);
	}
	return(frame_buf_ptr++);
}
