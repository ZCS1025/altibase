/*	$NetBSD: el.h,v 1.16 2003/10/18 23:48:42 christos Exp $	*/

/*-
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Christos Zoulas of Cornell University.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)el.h	8.1 (Berkeley) 6/4/93
 */

/*
 * el.h: Internal structures.
 */
#ifndef _h_el
#define	_h_el
/*
 * Local defaults
 */
#define	KSHVI
#define	VIDEFAULT
#define	ANCHOR

#include <stdio.h>
#include <sys/types.h>

#define	EL_BUFSIZ	1024		/* Maximum line size		*/

#define	HANDLE_SIGNALS	0x01
#define	NO_TTY		0x02
#define	EDIT_DISABLED	0x04
#define	UNBUFFERED	0x08

typedef int bool_t;			/* True or not			*/

typedef unsigned char el_action_t;	/* Index to command array	*/

typedef struct coord_t {		/* Position on the screen	*/
	int	h;
	int	v;
} coord_t;

typedef struct el_line_t {
	char	*buffer;		/* Input line			*/
	char	*cursor;		/* Cursor position		*/
	char	*lastchar;		/* Last character		*/
	const char	*limit;		/* Max position			*/
} el_line_t;

/*
 * Editor state
 */
typedef struct el_state_t {
	int		inputmode;	/* What mode are we in?		*/
	int		doingarg;	/* Are we getting an argument?	*/
	int		argument;	/* Numeric argument		*/
	int		metanext;	/* Is the next char a meta char */
	el_action_t	lastcmd;	/* Previous command		*/
	el_action_t	thiscmd;	/* this command 		*/
	char		thisch;		/* char that generated it	*/
} el_state_t;

/*
 * Until we come up with something better...
 */
#define	el_strdup(a)	strdup(a)
#define	el_malloc(a)	malloc(a)
#define	el_realloc(a,b)	realloc(a, b)
#define	el_free(a)	free(a)

#include "tty.h"
#include "prompt.h"
#include "key.h"
#include "el_term.h"
#include "refresh.h"
#include "chared.h"
#include "common.h"
#include "search.h"
#include "hist.h"
#include "map.h"
#include "parse.h"
#include "sig.h"
#include "help.h"
#include "read.h"
#include "local_ctype.h"

struct editline {
	char		 *el_prog;	/* the program name		*/
	FILE		 *el_outfile;	/* Stdio stuff			*/
	FILE		 *el_errfile;	/* Stdio stuff			*/
	int		  el_infd;	/* Input file descriptor	*/
	int		  el_flags;	/* Various flags.		*/
	coord_t		  el_cursor;	/* Cursor location		*/
	char		**el_display;	/* Real screen image = what is there */
	char		**el_vdisplay;	/* Virtual screen image = what we see */
	void		 *el_data;	/* Client data			*/
	el_line_t	  el_line;	/* The current line information	*/
	el_state_t	  el_state;	/* Current editor state		*/
	el_term_t	  el_term;	/* Terminal dependent stuff	*/
	el_tty_t	  el_tty;	/* Tty dependent stuff		*/
	el_refresh_t	  el_refresh;	/* Refresh stuff		*/
	el_prompt_t	  el_prompt;	/* Prompt stuff			*/
	el_prompt_t	  el_rprompt;	/* Prompt stuff			*/
	el_chared_t	  el_chared;	/* Characted editor stuff	*/
	el_map_t	  el_map;	/* Key mapping stuff		*/
	el_key_t	  el_key;	/* Key binding stuff		*/
	el_history_t	  el_history;	/* History stuff		*/
	el_search_t	  el_search;	/* Search stuff			*/
	el_signal_t	  el_signal;	/* Signal handling stuff	*/
	el_read_t	  el_read;	/* Character reading stuff	*/
};

protected int	el_editmode(EditLine *, int, const char **);

#ifdef DEBUG
#define	EL_ABORT(a)	do { \
				fprintf(el->el_errfile, "%s, %d: ", \
					 __FILE__, __LINE__); \
				fprintf a; \
				abort(); \
			} while( /*CONSTCOND*/0);
#else
#define EL_ABORT(a)	abort()
#endif
#endif /* _h_el */
