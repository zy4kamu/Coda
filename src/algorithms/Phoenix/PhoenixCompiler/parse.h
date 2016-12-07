/**
 *
 * .. invisible:
 *     _   _ _____ _     _____ _____
 *
 *    | | | |  ___| |   |  ___/  ___|
 *
 *    | | | | |__ | |   | |__ \ `--.
 *
 *    | | | |  __|| |   |  __| `--. \
 *
 *    \ \_/ / |___| |___| |___/\__/ /
 *
 *     \___/\____/\_____|____/\____/
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License
 *
 */

#include "grammar.h"
#include "pconf.h"

#define	PATH_TREE_DEPTH	50	/* max call-nets in single rewrite rule */
#define HIST_LEN	5

typedef unsigned short	Id;

typedef struct framedef {
	Id	n_slot;		/* number of slots in form */
	Id	*slot;		/* net numbers for slots */
	char	**prompt;	/* propmt for slot */
} FrameDef;

typedef struct gram
{
    FrameDef	*frame_def;		/* nets used in each form */
    char	**frame_name;		/* frame names */
    int		num_frames;		/* number of frames read in */
    char	**labels;		/* names of nets */
	char	**num_feats;
    Gnode	**Nets;			/* pointers to heads of nets */
    int		num_nets;		/* number of nets read in */
    char	**wrds;			/* pointers to strings for words */
    int		num_words;		/* number of words in lexicon */
    int		*node_counts;		/* number of nodes in each net */
    char	*leaf;			/* concept leaf flags */
    unsigned char	*priorities;	/* priorities for nets */
    int		max_pri;		/* max net priority level */
    char	*sym_buf;		/* strings for words and names */
	char	**feats;
} Gram;


/* cells of chart */
typedef struct edge {
    Id			net;
    Id			sw;
    Id			ew;
    Id			score;
    char		nchld;
    struct edge		**chld;
    struct edge		*link;
} Edge;


/* temporary structure used in matching nets to create edges */
typedef struct buf {
	Gnode	*state;		/* current state */
	Id	net;		/* number of top-level net */
	Id	sw;		/* number of top-level net */
	Id	ew;		/* number of top-level net */
	Id	score;		/* number of top-level net */
	char	nchld;
	Edge	*chld[PATH_TREE_DEPTH];	/* pointers to sub-trees (children) */
	int	word_pos;	/* extended flag */
}Path;



/* structure for linking edges into chart */
typedef struct edge_link {
	int	sw;
	struct edge_link *link;
	Edge *edge;
}EdgeLink;


typedef struct frame_id {
	unsigned short id;
	unsigned short count;
} Fid;

typedef struct seq_node {
	Edge *edge;
	unsigned short	n_frames;	/* frame count for path */
	unsigned short	n_act;	/* number of active frames for path */
	Fid	*frame_id;
	struct seq_node *back_link;
	struct seq_node *link;
	unsigned short *pri;
} SeqNode;

typedef struct seq_cell {
	Id score;
	Id n_slots;
	Id n_frames;
	SeqNode *link;
} SeqCell;

typedef struct frame_node {
	int n_frames;
	int frame;
	SeqNode *slot;
	struct frame_node *bp;
	struct frame_node *link;
} FrameNode;

/* functions */

void read_frames(char *dir, char * frames_file,  Gram *gram, char **sb_start, char ** sym_buf_end);
Gram *read_grammar(char *dir, char * dict_file, char * grammar_file, char * frames_file, char * priority_file);
void read_nets(char *net_file, Gram *gram, char ** sb_start, char * sym_buf_end);
int read_dict(char *dir, char * dict_file, Gram *gram, char **sb_start, char *sym_buf_end);
int find_word(char *s, Gram *gram);


