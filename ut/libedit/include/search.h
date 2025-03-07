/*	$NetBSD: search.h,v 1.8 2003/10/18 23:27:36 christos Exp $	*/

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
 *	@(#)search.h	8.1 (Berkeley) 6/4/93
 */

/*
 * el.search.h: Line and history searching utilities
 */
#ifndef _h_el_search
#define	_h_el_search

#include "histedit.h"

typedef struct el_search_t {
	char	*patbuf;		/* The pattern buffer		*/
	size_t	 patlen;		/* Length of the pattern buffer	*/
	int	 patdir;		/* Direction of the last search	*/
	int	 chadir;		/* Character search direction	*/
	char	 chacha;		/* Character we are looking for	*/
	char	 chatflg;		/* 0 if f, 1 if t */
} el_search_t;


protected int		el_match(const char *, const char *);
protected int		search_init(EditLine *);
protected void		search_end(EditLine *);
protected int		c_hmatch(EditLine *, const char *);
protected void		c_setpat(EditLine *);
protected el_action_t	ce_inc_search(EditLine *, int);
protected el_action_t	cv_search(EditLine *, int);
protected el_action_t	ce_search_line(EditLine *, int);
protected el_action_t	cv_repeat_srch(EditLine *, int);
protected el_action_t	cv_csearch(EditLine *, int, int, int, int);

#endif /* _h_el_search */
