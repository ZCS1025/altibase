/*	$NetBSD: refresh.h,v 1.5 2003/08/07 16:44:33 agc Exp $	*/

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
 *	@(#)refresh.h	8.1 (Berkeley) 6/4/93
 */

/*
 * el.refresh.h: Screen refresh functions
 */
#ifndef _h_el_refresh
#define	_h_el_refresh

#include "histedit.h"

typedef struct {
	coord_t	r_cursor;	/* Refresh cursor position	*/
	int	r_oldcv;	/* Vertical locations		*/
	int	r_newcv;
} el_refresh_t;

protected void	re_putc(EditLine *, int, int);
protected void	re_clear_lines(EditLine *);
protected void	re_clear_display(EditLine *);
protected void	re_refresh(EditLine *);
protected void	re_refresh_cursor(EditLine *);
protected void	re_fastaddc(EditLine *);
protected void	re_goto_bottom(EditLine *);

#endif /* _h_el_refresh */
