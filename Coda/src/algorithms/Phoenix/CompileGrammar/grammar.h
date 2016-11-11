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

#ifndef COMPILE_GRAMMAR_GRAMMAR_H
#define COMPILE_GRAMMAR_GRAMMAR_H

#include <string>

#define MAX_WRDS	0xFFFF	/* max words in lexicon */
#define LABEL_LEN	200	/* max length of labels */
#define LINE_LEN	10000	/* max length line buffer */

/* grammar state structure */
typedef struct gnode
{
	short	n_suc;		/* number of succs */
	short	final;		/* true if final state */
	struct gsucc	*succ;	/* arcs out */
} Gnode;

/* grammar arc structure */
typedef struct gsucc
{
	int	tok;		/* word number, call_netber or 0 */
	wchar_t feats[200];
	int	call_net;	/* 0 or number of net called */
	Gnode	*state;		/* ptr to successor state */
	bool head;
} Gsucc;

typedef struct suc_link
{
	Gsucc	succ;
	struct suc_link *link;
	int nt;
} SucLink;

struct state_set {
	int state;
	char used;
	struct state_set *next;
};
typedef struct state_set set;

typedef struct {
	int tok;
	SucLink *arc;
	char	rw;	/* has been rewritten flag */
} non_term;

#endif
