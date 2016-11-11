/* take input grammar and generate nets

   input: file containing list of nets to compile is first arg
		  dictionary is second argument
   output: <file>.net files
*/

#include <cmath>
#include <cstdio>
#include <string.h>
#include <cstdlib>	  
#include <cctype>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <locale>

extern "C"
{
#include "grammar.h"
#include "pconf.h"
}

using std::wstring;
using std::vector;
using std::unordered_map;

int	TokBufSize=   10000;	/* buffer size to hold char strings */
int	MaxNonTerm=    100000;	/* max number non-terminal pointers */
int	MaxSymbol=     10000;	/* max number of nonterminal symbols */
int	MaxNfa=        150000;	/* max number of nodes in network */
int	MaxSucLink=    500000;	/* max number of arcs in network */

/* Command Line structure */

Gnode *new_nfa();
SucLink *new_nfa_link();

Gnode	*nfa,
	*nfa_ptr,
	*nfa_end;

SucLink *nfa_succ,
	*nfa_suc_ptr,
	*nfa_suc_end;

int	NetNumber;
wstring	NetName;
wstring	NetFeat;
wstring	NetSym;
//wstring	fname;

non_term *non_t;
int num_nt;

wchar_t	*tok_buf,	/* buff for ascii input strings */
	*tok_ptr,
	*tok_buf_end;
vector<wstring> sym;		/* symbol table */
int num_sym;
int last_net;
int num_nets;
int	nt_tok;

std::locale lolo;

unordered_map<wstring, int> wrds;	/* pointers to strings for word numbers */
char	update_dict;
FILE	*fp_out;

wstring grammar_file;
wstring grammar = L".";
wstring netsfile = L"nets";
wstring dictfile = L"base.dic";

int globalDictionaryCount = 0;

static void rewrite( SucLink *arc, wchar_t * rul_buf);
static void alloc_sp ();
static void net_gen (wchar_t *filename, FILE *fp_out);
static void write_net();
static int get_right(wstring& rule, bool& opt, bool& self, bool& head, unsigned int& type, wstring& feat);
static int add_word(const wstring& s);
static int lookup_word(const wstring& s);
static int find_net(const wstring& s);
static int find_sym(const wstring& s);
static int get_symbol(const wstring& s);
static int blank_line(wchar_t * s);
void processNetDefinition(wchar_t * rule_buf, char& state);
void processRewriteRule(wchar_t * rule_buf, char& state);
void processInclude(wchar_t * rule_buf, char& state);
void processEndOfDefinition(wchar_t * rule_buf, char& state);
void processMacroDefinition(wchar_t * rule_buf, char& state);
bool isStringUppercase(const wchar_t * str);

wstring ConvertToWstring( const std::string str )  
{
	// set locale
	std::locale lolo = std::locale::global(std::locale("Russian"));
	
	std::wstring result;

	size_t bufferLength;
	mbstowcs_s( &bufferLength, NULL, 0, str.c_str(), -1 );
	wchar_t* resultBuffer = (wchar_t*)malloc( bufferLength * sizeof( wchar_t ) );
	memset( resultBuffer, 0, bufferLength );
	
	if( mbstowcs_s( &bufferLength, resultBuffer, bufferLength, str.c_str(), -1 ) ) 
	{
		std::locale::global(lolo);
		return false;
	}

	result = wstring( resultBuffer );
	if( resultBuffer ) 
	{
		free( resultBuffer );
	}

	std::locale::global(lolo);

	return result;
}

int main( int argc, char *argv[])
{
	int 	net_num, j;
	wchar_t	net_name[LABEL_LEN];
	FILE	*fp, *fp_nets;

	lolo = std::locale::global(std::locale("Russian"));

	for (int i = 1; i < argc; i++) 
	{
		if(std::string(argv[i]) == "-f")
		{
			grammar_file = ConvertToWstring(std::string(argv[i + 1]));
		}
	}

	/* malloc space for data structures */
	alloc_sp ();

	nfa_ptr= nfa;
	nfa_suc_ptr= nfa_succ;
	num_nt = 0;

	/* leave null symbol for net 0 */
	sym.push_back(L"*");
	num_sym= 1;

	/* remove dictionary */
	wchar_t p_fname[200];

	swprintf(p_fname, 200, L"%s/%s", grammar.c_str(), dictfile.c_str());

	if(_wfopen_s(&fp, p_fname, L"w, ccs=UTF-8"))
	{
		fwprintf(stderr, L"ERROR: can't create %s\n", p_fname);
		exit(-1);
	}
	fclose (fp);

	/* open file with list of nets */
	swprintf(p_fname, 200, L"%s/%s", grammar.c_str(), netsfile.c_str());

	if(_wfopen_s(&fp_nets, p_fname,  L"r, ccs=UTF-8")) 
	{
		fwprintf(stderr, L"ERROR: can't open %s\n", p_fname);
		exit(-1);
	}

	/* read net names into symbol table */
	wchar_t p_netname[200];
	for(net_num = 1; fwscanf(fp_nets, L"%s", net_name) == 1; net_num++) 
	{
		/* add braces to net name */
		swprintf(p_netname, 200, L"[%s]", net_name);
		/* check for duplicate net names */
		for(j=1; j < num_sym; j++ ) 
		{
			if( !wcsncmp(sym[j].c_str(), p_netname, 200) ) 
			{
				fwprintf(stderr, L"Duplicate net name %s\n", p_netname);
				exit(-1);
			}
		}

		wstring netname(p_netname);

		if(netname[netname.length() - 1] == L'\n')
		{
			netname = netname.substr(0, netname.length() - 1);
		}

		sym.push_back(netname);
		num_sym = sym.size();
	}
	last_net= num_sym-1;
	num_nets= net_num-1;

	/* open output .net file */
	swprintf(p_fname, 200, L"%s/%s.net", grammar.c_str(), grammar_file.c_str());
	if(_wfopen_s(&fp_out, p_fname, L"w, ccs=UTF-8"))
	{
		wprintf(L"can't open %s\n", p_fname);
		exit(1);
	}

	/* write number of nets at start of file */
	fwprintf(fp_out, L"Number of Nets= %d\n", num_nets);

	/* use input .gra file with same stem name */
	swprintf(p_fname, 200, L"%s/%s.gra", grammar.c_str(), grammar_file.c_str());

	/* read grammar rules and compile networks */
	net_gen(p_fname, fp_out);

	fclose(fp_out);

	if( update_dict ) 
	{
		/* rewrite dictionary */
		swprintf(p_fname, 200, L"%s/%s", grammar.c_str(), dictfile.c_str());
		if(_wfopen_s(&fp, p_fname, L"w, ccs=UTF-8")) 
		{
			wprintf(L"open %s failed.\n", dictfile.c_str());
			exit(-1);
		}

		for(auto wrd_p = wrds.begin(); wrd_p != wrds.end(); wrd_p++) 
		{
			fwprintf(fp, L"%s %d\n", wrd_p->first.c_str(), wrd_p->second);
		}
		fclose(fp);
	}

	std::locale::global(lolo);

	return(1);
}

static void reset()
{
	num_sym= last_net +1;
	tok_ptr= tok_buf;
	nfa_ptr= nfa;
	nfa_suc_ptr= nfa_succ;
	num_nt = 0;
}

static void add_nt( int token, SucLink *arc )
{
	if( num_nt >= MaxNonTerm ) 
	{
		wprintf(L"Table overflow, MaxNonTerm= %d\n", MaxNonTerm);
		exit(1);
	}
	non_t[num_nt].tok= token;
	non_t[num_nt].rw= 0;
	non_t[num_nt++].arc= arc;
}

static void alloc_sp ()
{
  /* allocate space for non-term array */
  if( !(non_t = (non_term *) calloc(MaxNonTerm, sizeof(non_term))) ) 
  {
    wprintf(L"not enough memory to allocate non-term array\n");
    exit(1);
  }
  /* allocate space for nfa */
  if( !(nfa= (Gnode *) calloc(MaxNfa, sizeof(Gnode))) ) {
	wprintf(L"not enough memory to allocate nfa\n");
	exit(1);
  }
  nfa_end= nfa + MaxNfa;
  if( !(nfa_succ= (SucLink *) malloc(MaxSucLink * sizeof(SucLink))) ) {
	wprintf(L"not enough memory to allocate nfa_succ\n");
	exit(1);
  }
  nfa_suc_end= nfa_succ + MaxSucLink;
}

static void net_gen (wchar_t *filename, FILE *fp_out)
{
	FILE	*fp_in;
	char	state = 'n';

	wchar_t rule_buf[LINE_LEN]; 

	/* open grammar file */

	if(_wfopen_s( &fp_in, filename, L"r, ccs=UTF-8"))
	{
		wprintf(L"ERROR: Can't open grammar file %s\n", filename);
		return;
	}

	/* generate non-deterministic FSA by re-writing non-terminals */

	/* read each line from gra file */
	while( fgetws(rule_buf, LINE_LEN, fp_in) ) 
	{
		if( blank_line(rule_buf) ) 
		{
			continue;
		}

		/* include file */
		if( !wcsncmp(rule_buf, L"#incl", 5) ) 
		{
			processInclude(rule_buf, state);
		}

		/* comment */
		else if( *rule_buf == '#' ) 
		{
			continue;
		}

		/* net definition end */
		else if( *rule_buf == ';' ) 
		{
			processEndOfDefinition(rule_buf, state);
		}

		/* net name, begin net definition */
		else if( *rule_buf == '[' ) 
		{
			processNetDefinition(rule_buf, state);
		}

		/* rewrite rule */
		else if( isspace( (int) *rule_buf, lolo) ) 
		{
			processRewriteRule(rule_buf, state);
		}

		/* macro definition */
		else if( *rule_buf < 128 && isStringUppercase(rule_buf)) 
		{
			processMacroDefinition(rule_buf, state);
		}

		/* error */
		else {
			wprintf(L"ERROR: bad format: %s\n", rule_buf);
		}
	}

	fclose(fp_in);
}

static void rewrite( SucLink *arc, wchar_t * rule_buf )
{
	int tok, tok1;
	SucLink	*alt,
		*new_arc,
		*link,
		*start;

	Gnode	*state,
		*from_state;

	bool opt, opt1,
	 self, self1,
	 head, head1,
	 prev_self;

	unsigned int type, type1;

	wstring feat, feat1;

	wstring tokLine(rule_buf);

	if(tokLine[tokLine.length() - 1] == L'\n')
	{
		tokLine = tokLine.substr(0, tokLine.length() - 1);
	}

	tokLine = tokLine.substr(tokLine.find_first_of(L'(') + 1);

	from_state= 0;
	start= 0;
	prev_self=1;

	if( (tok= get_right(tokLine, opt, self, head, type, feat)) < 0 ) 
	{
		return;
	}

	/* while more tokens remaining */
	while( (tok1=get_right(tokLine, opt1, self1, head1, type1, feat1)) >= 0 ) 
	{
		/* create arc for token */
		alt= new_nfa_link();
		alt->succ.tok= tok;
		alt->succ.head= head;
		*(alt->succ.feats) = 0;
		if(!feat.empty())
		{
			wcscpy(alt->succ.feats, feat.c_str());
		}

		alt->succ.call_net= 0;
		alt->nt= 0;
		alt->link= 0;

		if( type == 1) 
		{
			/* if non_term token */
			alt->nt= 1;
			add_nt( tok, alt );
		}
		else if( type == 2 )
		{
			/* if pre_term token */
			alt->succ.call_net= tok;
		}

		if( self && opt ) 
		{
			/* if start or prev state contained self trans add null arc */
			if ( !from_state || prev_self ) 
			{
				/* create next state */
				state= new_nfa();

				/* add null arc to state */
				new_arc= new_nfa_link();
				new_arc->succ.tok= 0;
				new_arc->succ.head= 0;
				new_arc->succ.call_net= 0;
				new_arc->succ.state= state;
				new_arc->nt= 0;

				if( from_state ) 
				{
					new_arc->link= (SucLink *) from_state->succ;
					from_state->succ= (struct gsucc *) new_arc;
				}

				/* if first arc from start node */
				if( !start ) start= new_arc;

				/* add self transition */
				alt->succ.state= state;
					alt->link= (SucLink *) state->succ;
					state->succ= (struct gsucc *) alt;

			}
			else 
			{
			/* add self transition */
				alt->succ.state= from_state;
				alt->link= (SucLink *) from_state->succ;
				from_state->succ= (struct gsucc *) alt;
			}
		}
		else 
		{
			/* if first arc from start node */
			if( !start ) start= alt;

			/* create next state */
			state= new_nfa();
			/* point arc at state */
			alt->succ.state= state;

			/* if not start of rule, link in to state */
			if( from_state ) {
				alt->link= (SucLink *) from_state->succ;
				from_state->succ= (struct gsucc *) alt;
			}

			if( opt ) 
			{
				/* add null arc to next state */
				new_arc= new_nfa_link();
				new_arc->succ.tok= 0;
				new_arc->succ.head= 0;
				new_arc->succ.call_net= 0;
					new_arc->succ.state= state;
				new_arc->nt= 0;
				new_arc->link= alt->link;
				alt->link= new_arc;
			}
				if( self ) 
				{
					/* add self-transition */
					new_arc= new_nfa_link();
					*new_arc= *alt;
					if( type == 1) 
					{
						/* if non_term token */
						add_nt( tok, new_arc );
					}
					new_arc->link= (SucLink *) state->succ;
					state->succ= (struct gsucc *) new_arc;
				}
		}
		from_state= state;
		prev_self= self;
		tok= tok1;
		feat = feat1;
		opt= opt1;
		self= self1;
		type= type1;
		head = head1;
	
	}

	/* create ending arc */
	alt= new_nfa_link();
	if( !start ) start= alt;
	alt->succ.tok= tok;
	alt->succ.head= head;
	*(alt->succ.feats) = 0;
	if(!feat.empty())
	{
		wcscpy(alt->succ.feats, feat.c_str());
	}
	alt->succ.call_net= 0;
	alt->succ.state= arc->succ.state;
	alt->nt= 0;
	alt->link= 0;

	if( type == 1) 
	{
		/* if non_term token */
		alt->nt= 1;
		add_nt( tok, alt );
	}
	else if( type == 2 ) 
	{
		/* if pre_term token */
		alt->succ.call_net= tok;
	}

	if( self && opt ) 
	{
		/* if prev state contained self transition add null arc */
		if ( !from_state || prev_self ) 
		{
			/* create next state */
			state= new_nfa();

			/* add null arc to state */
			new_arc= new_nfa_link();
			new_arc->succ.tok= 0;
			new_arc->succ.head= 0;
			new_arc->succ.call_net= 0;
				new_arc->succ.state= state;
			new_arc->nt= 0;
			if( from_state ) 
			{
				new_arc->link= (SucLink *) from_state->succ;
				from_state->succ= (struct gsucc *) new_arc;
			}

			/* add self transition */
			alt->succ.state= state;
			alt->link= (SucLink *) state->succ;
			state->succ= (struct gsucc *) alt;
		}
		else 
		{
			/* add self transition */
			alt->succ.state= from_state;
			alt->link= (SucLink *) from_state->succ;
			from_state->succ= (struct gsucc *) alt;
			state= from_state;
		}

		/* create null arc to final state */
		new_arc= new_nfa_link();
		new_arc->succ.tok= 0;
		new_arc->succ.head= 0;
		new_arc->succ.state= arc->succ.state;
		new_arc->succ.call_net= 0;
		new_arc->nt= 0;
		new_arc->link= (SucLink *) state->succ;
		state->succ= (struct gsucc *) new_arc;
	}
	else 
	{
		/* if not start of rule, link in to state */
		if( from_state ) 
		{
			alt->link= (SucLink *) from_state->succ;
			from_state->succ= (struct gsucc *) alt;
		}

		if( self) 
		{
			/* create next state */
			state= new_nfa();
			alt->succ.state= state;

			/* add self transition */
			new_arc= new_nfa_link();
			*new_arc= *alt;
				if( type == 1) add_nt( tok, new_arc );
			new_arc->link= (SucLink *) state->succ;
			state->succ= (struct gsucc *) new_arc;

			/* create null arc to final state */
			new_arc= new_nfa_link();
			new_arc->succ.tok= 0;
			new_arc->succ.head= 0;
				new_arc->succ.state= arc->succ.state;
			new_arc->succ.call_net= 0;
			new_arc->nt= 0;
			new_arc->link= (SucLink *) state->succ;
			state->succ= (struct gsucc *) new_arc;
		}
		else 
		{
			alt->succ.state= arc->succ.state;
			if( opt ) {
			/* if optional, add null arc */
			new_arc= new_nfa_link();
			new_arc->succ.tok= 0;
			new_arc->succ.head= 0;
			new_arc->succ.call_net= 0;
			new_arc->succ.state= alt->succ.state;
			new_arc->nt= 0;
			new_arc->link= alt->link;
			alt->link= new_arc;
			}
		}
	}

	/* link in start arc */
	if( !arc->link ) {
	arc->link= start;
	}
	else {
	for( link= arc->link; link->link; link= link->link) ;
	link->link= start;
	}
	
}

int get_right(wstring& rule, bool& opt, bool& self, bool& head, unsigned int& type, wstring& feat)
{
	int w_idx;
	feat = L"";

	/* skip white space */
	rule = rule.substr(rule.find_first_not_of(L' '));

	if(rule[0] == L')' || rule[0] == L'!')
	{
		return -1;
	}

	/* check optional and self-transition flags */
	opt= false;
	self= false;
	head = 0;
	
	if(rule[0] == L'*')
	{
		opt= true;
		rule = rule.substr(1);
	}
	else if(rule[0] == L'+')
	{
		self= true;
		rule = rule.substr(1);
	}

	int c = rule.length();
	/* copy symbol */
	for(int i = 0; i < rule.length(); i++)
	{
		if(rule[i] == L' ' || rule[i] == L'.' || rule[i] == L')' || rule[i] == L'!')
		{
			c = i;
			break;
		}
	}

	wstring token = rule.substr(0, c);
	token = token.substr(token.find_first_not_of(L' '));

	rule =  rule.substr(c);

	rule = rule.substr(rule.find_first_not_of(L' '));

	if(rule[0] == L'.')
	{
		feat = rule.substr(rule.find_first_of(L'{'), rule.find_first_of(L'}'));
		rule = rule.substr(rule.find_first_of(L'}') + 1);
	}

	if(rule[0] == L'!')
	{
		head = 1;
		rule = rule.substr(1);
	}
	else
	{
		head = 0;
	}

	if( token[0] == L'[' ) 
	{
		/* net call */
		int	symi;
		type= 2;
		symi= find_sym(token);

		if( symi == (num_sym-1) ) 
		{
			wprintf(L"WARNING: compiling net %s could not find called net %s\n", NetName.c_str(), token.c_str());
		}
		return( symi );
	}
	else if(token[0] < 127 && isStringUppercase(token.c_str())) 
	{
		/* non-terminal */
		type= 1;
		return( find_sym( token ) );
	}
	else 
	{
		/* word */
		type= 0;
		w_idx= lookup_word(token);
		if( w_idx < 0) 
		{
			w_idx= add_word(token);
			update_dict= 1;
		}
		return( w_idx );
	}
}

int find_sym( const wstring& s )
{
	int i;

	/* if null arc */
	if( s == sym[0])  return(0);

	for(i=sym.size()-1; i >= 0; i-- )
	{
	if( s == sym[i] ) break;
	}
	if( i >=  0 )
		return(i);

	if(num_sym == sym.size())
	{
		sym.push_back(s);
	}
	else
	{
		sym[num_sym] = s;
	}

	return( num_sym++ );
}

Gnode *new_nfa()
{
	if( nfa_ptr >= nfa_end ) {
	wprintf(L"Table overflow, MaxNonTerm= %d\n", MaxNonTerm);
	exit(1);
	}

	nfa_ptr->n_suc = 0;
	nfa_ptr->final = 0;
	nfa_ptr->succ = NULL;

	return( nfa_ptr++);
}

SucLink *new_nfa_link()
{
	if( nfa_suc_ptr >= nfa_suc_end ) {
	wprintf(L"Table overflow, MaxSucLink= %d\n", MaxSucLink);
	exit(1);
	}

	nfa_suc_ptr->succ.tok = 0;
	nfa_suc_ptr->succ.state = NULL;
	nfa_suc_ptr->succ.call_net = 0;
	*(nfa_suc_ptr->succ.feats) = 0;
	nfa_suc_ptr->succ.head = 0;
	nfa_suc_ptr->link = NULL;
	nfa_suc_ptr->nt = 0;

	return( nfa_suc_ptr++);
}

void write_net()
{
	Gnode	*state;
	SucLink	*arc;
	int		count,
			tot_arcs;
	int	i;

	/* check to see all nt rewritten */
	for(i=0; i < num_nt; i++) 
	{
		if( !non_t[i].rw ) 
		{
			wprintf(L"WARNING: In <%s> NonTerm %s not rewritten\n",
				NetName.c_str(), sym[ non_t[i].tok ].c_str());
		}
	}

	/* write net name, net number, number of states and concept_flag */
	fwprintf(fp_out, L"%s %d %d %d %s\n", NetName.c_str(), NetNumber, nfa_ptr - nfa, 0, NetFeat.c_str());

	/* generate final net by removing non-terminal arcs */
	tot_arcs= 0;
	for(state= nfa, i=0; state < nfa_ptr; state++, i++) 
	{
	  /* count succs */
		for(arc= (SucLink *)state->succ,count=0; arc; arc= arc->link) 
		{
			if( !arc->nt ) count++;
		}
		fwprintf(fp_out, L"%d  %d %d\n", i, count, state->final);
		for( arc= (SucLink *) state->succ; arc; arc= arc->link ) 
		{
			if( arc->nt ) continue;
			if(*(arc->succ.feats))
			{
				fwprintf(fp_out, L"\t\t%d    %d    %d    %d    %s\n", arc->succ.tok,
					arc->succ.call_net, arc->succ.state - nfa, arc->succ.head, arc->succ.feats);
			}
			else
			{
				fwprintf(fp_out, L"\t\t%d    %d    %d    %d    %s\n", arc->succ.tok,
				arc->succ.call_net, arc->succ.state - nfa,  arc->succ.head,  L"_");
			}
		}
		tot_arcs += count;
	}
	wprintf(L"%s  %d states  %d arcs\n", NetName.c_str(), nfa_ptr - nfa, tot_arcs);

}

int find_net(const wstring& s)   
{
	int i;

	for(i=1; i <= last_net; i++ )
	{
		if(s == sym[i])
		{
			break;
		}
	}

	if( i <=  last_net ) return(i);

	wprintf(L"WARNING: net %s not found\n", s.c_str());
	fflush(stdout);
	return(-1);
}

int lookup_word(const wstring& is)
{
	wstring s = is;
	/* convert to uppercase */
	std::locale lolo = std::locale::global(std::locale("Russian"));
	std::transform(s.begin(), s.end(), s.begin(), ::towupper);
	std::locale::global(lolo);

	/* if unknown word */
	if(wrds.find(is) == wrds.end()) return(-1);

	return(wrds[is]);
}

/* same as lookup_word, but adds word if not found */
int add_word(const wstring& is)
{
	wstring s = is;
	/* convert to uppercase */
	std::locale lolo = std::locale::global(std::locale("Russian"));
	std::transform(s.begin(), s.end(), s.begin(), ::towupper);
	std::locale::global(lolo);

	if(wrds.find(s) == wrds.end())
	{
		wrds[s] = ++globalDictionaryCount;
		return globalDictionaryCount;
	}
	else
	{
		return wrds[s];
	}

	fwprintf(stderr, L"ERROR: Unable to add word %s\n", s.c_str());
	return(-1);
}

int get_symbol(const wstring& rule)
{
	static int last_tok = 0;

	wstring token = rule.substr(rule.find_first_not_of(L' '));

	token = token.substr(0, token.find_first_of(L' '));

	last_tok= find_sym(token);
	return( last_tok );
}

int blank_line(wchar_t *s)
{
	while( *s && isspace( (int) *s, lolo) ) s++;
	if( !*s ) return(1);
	else return(0);
}

void processNetDefinition(wchar_t * rule_buf, char& state)
{
	int		s_idx;
	SucLink	*arc;
	Gnode	*start_node, *end_node;

	if( state != 'n' ) 
	{
		wprintf(L"ERROR: Unexpected net definition %s\n", rule_buf);
		return;
	}

	/* set net number */
	NetFeat = L"_";
	wchar_t nf_pointer[200];
	wchar_t nn_pointer[200];
	*nf_pointer = *nn_pointer = 0;

	swscanf(rule_buf, L"%[^.]%*[.]%s", nn_pointer, nf_pointer);

	NetName = wstring(nn_pointer);

	if(NetName[NetName.length() - 1] == L'\n')
	{
		NetName = NetName.substr(0, NetName.length() - 1);
	}

	NetFeat = *nf_pointer ? wstring(nf_pointer) : L"_";

	int l = NetName.size();


	NetNumber= find_net( NetName );

	/* replace square brackets with angle brackets */

	NetSym = NetName;
	NetSym[0] = L'<';
	NetSym[NetSym.length() -1] = L'>';

	if(NetSym[NetSym.length() - 1] == L'\n')
	{
		NetSym = NetSym.substr(0, NetSym.length() - 1);
	}

	s_idx= find_sym( NetSym );

	/* create start and end nodes */
	start_node= new_nfa();
	start_node->n_suc= 1;
	start_node->final= 0;
	end_node= new_nfa();
	end_node->n_suc= 0;
	end_node->final= 1;
	
	/* arc between labelled with net name */
	arc= new_nfa_link();
	arc->succ.tok= s_idx;
	arc->succ.head = 0;
	arc->succ.state= end_node;
	arc->succ.call_net= 0;
	arc->link= 0;
	arc->nt= 1;
	start_node->succ= (struct gsucc *) arc;

	add_nt( s_idx, arc );

	nt_tok= get_symbol(NetSym);
	state= 'p';
}

void processRewriteRule(wchar_t * rule_buf, char& state)
{
	if( !wcschr(rule_buf, (int) '(' ) || !wcschr(rule_buf, (int) ')' ) ) 
	{
		wprintf(L"ERROR: Bad format for rule: %s\n", rule_buf);
		return;
	}

	/* rewrite each previous occurence of symbol with rhs */
	for(int nt = 0; nt < num_nt; nt++) 
	{
		if( non_t[nt].tok == nt_tok ) 
		{
			rewrite( non_t[nt].arc, rule_buf);
			/* mark as rewritten */
			non_t[nt].rw++;
		}
	}
	return;
}

void processInclude(wchar_t * rule_buf, char& state)
{
	wchar_t	fname[LABEL_LEN];
	wchar_t	name[LABEL_LEN];

	swscanf(rule_buf, L"#include %s", name);
	swprintf(fname, 200, L"%s/%s", grammar, name);
	net_gen(fname, fp_out);
}

void processEndOfDefinition(wchar_t * rule_buf, char& state)
{
	if( state != 'n' ) 
	{
		/* write out compiled net */
		write_net();
		/* reset structures */
		reset();
		state= 'n';
	}
}

void processMacroDefinition(wchar_t * rule_buf, char& state)
{
	wstring rulStr(rule_buf); 

	if(rulStr[rulStr.length() - 1] == L'\n')
	{
		rulStr = rulStr.substr(0, rulStr.length() - 1);
	}

	if( (nt_tok= get_symbol(rulStr)) < 0) 
		return;
	state= 'e';
}

bool isStringUppercase(const wchar_t * str)
{
	lolo = std::locale::global(std::locale("Russian"));

	if(wstring(str) == L"TO")
		int a =0;

	while(*str != L' ' && *str != L'\n' && *str != L')' && *str != 0)
	{
		if(!isupper( (wchar_t) *str, lolo) && *str != L'\n')
		{
			return false;
		}
		str++;
	}

	std::locale::global(lolo);

	return true;
}
