/* ------------------------- match   ------------------------------------------ */
/*                                                                              */
/* Outputs a list when a list of input values matches the creation args.        */
/* Written by Krzysztof Czaja for his cyclone library.                          */
/* Modified to fit into maxlib by Olaf Matthes <olaf.matthes@gmx.de>.           */
/* Get source at http://www.akustische-kunst.org/puredata/maxlib/               */
/*                                                                              */
/* This program is free software; you can redistribute it and/or                */
/* modify it under the terms of the GNU General Public License                  */
/* as published by the Free Software Foundation; either version 2               */
/* of the License, or (at your option) any later version.                       */
/*                                                                              */
/* This program is distributed in the hope that it will be useful,              */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of               */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                */
/* GNU General Public License for more details.                                 */
/*                                                                              */
/* You should have received a copy of the GNU General Public License            */
/* along with this program; if not, write to the Free Software                  */
/* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.  */
/*                                                                              */
/* Based on PureData by Miller Puckette and others.                             */
/*                                                                              */
/* ---------------------------------------------------------------------------- */

/* this is the original copyright notice: */

/* Copyright (c) 1997-2002 Miller Puckette and others.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.  */


#include <string.h>
#include "m_pd.h"

#define MATCH_INISIZE  16  /* LATER rethink */

typedef struct _match
{
    t_object  x_ob;
    int       x_size;    /* as allocated */
    int       x_patlen;  /* as used */
    t_atom   *x_pattern;
    t_atom    x_patini[MATCH_INISIZE];
    int       x_quelen;
    t_atom   *x_queue;
    t_atom    x_queini[MATCH_INISIZE];
    t_atom   *x_queend;
    t_atom   *x_queptr;  /* writing head, post-incremented (oldest-pointing) */
    int       x_;
} t_match;

static t_class *match_class;

/* a caller must check for nrequested > *sizep */
/* returns actual number of atoms: requested (success)
   or a default value of initial size (failure) */
/* the result is guaranteed to be >= min(nrequested, inisize) */
static int match_grow(int nrequested, int *sizep, t_atom **bufp,
		int inisize, t_atom *bufini)
{
    int newsize = *sizep * 2;
    while (newsize < nrequested) newsize *= 2;
    if (*bufp == bufini)
	*bufp = (t_atom *)getbytes(newsize * sizeof(**bufp));
    else
	*bufp = (t_atom *)resizebytes(*bufp, *sizep * sizeof(**bufp),
				      newsize * sizeof(**bufp));
    if (*bufp)
	*sizep = newsize;
    else
    {
	*bufp = bufini;
	nrequested = *sizep = inisize;
    }
    return (nrequested);
}

static void match_clear(t_match *x)
{
    x->x_quelen = 0;
    x->x_queptr = x->x_queue;
}

/* x->x_patlen > 0 is assumed */
/* LATER use a lock to disable reentrant calls.  I do not see any
   purpose of reentering match, but lets CHECKME first... */
static void match_checkin(t_match *x)
{
    int i, patlen = x->x_patlen;
    t_atom *queptr, *pp, *qp;
    if (x->x_queptr >= x->x_queend)
	x->x_queptr = x->x_queue;
    else x->x_queptr++;
    if (x->x_quelen < patlen && ++(x->x_quelen) < patlen)
	return;

    qp = queptr = x->x_queptr;
    for (i = 0, pp = x->x_pattern; i < patlen; i++, pp++)
    {
	if (pp->a_type == A_FLOAT)
	{
	    if (qp->a_type != A_FLOAT || qp->a_w.w_float != pp->a_w.w_float)
		break;
	}
	else if (pp->a_type == A_SYMBOL)
	{
	    if (qp->a_type != A_SYMBOL || qp->a_w.w_symbol != pp->a_w.w_symbol)
		break;
	}
	else if (pp->a_type == A_NULL)
	{
	    if (qp->a_type == A_FLOAT || qp->a_type == A_SYMBOL)
	    {
		/* instantiating a pattern */
		*pp = *qp;
		qp->a_type = A_NULL;
	    }
	    else break;  /* LATER rethink */
	}
	else break;  /* LATER rethink */
	if (qp >= x->x_queend)
	    qp = x->x_queue;
	else qp++;
    }
    if (i == patlen)
    {
	pp = x->x_pattern;
	if (pp->a_type == A_FLOAT)
	{
	    if (patlen == 1)
		outlet_float(((t_object *)x)->ob_outlet, pp->a_w.w_float);
	    else
		outlet_list(((t_object *)x)->ob_outlet, &s_list, patlen, pp);
	}
	else  /* assuming A_SYMBOL (see above) */
	{
	    if (pp->a_w.w_symbol == &s_symbol  /* bypassing typedmess() */
		&& patlen == 2 && pp[1].a_type == A_SYMBOL)
		outlet_symbol(((t_object *)x)->ob_outlet, pp[1].a_w.w_symbol);
	    else
		outlet_anything(((t_object *)x)->ob_outlet, pp->a_w.w_symbol,
				patlen - 1, pp + 1);
	}
	/* CHECKED: no implicit clear (resolving overlapping patterns) */
    }
    /* restoring a pattern */
    for (i = 0, pp = x->x_pattern; i < patlen; i++, pp++)
    {
	if (queptr->a_type == A_NULL)
	{
	    queptr->a_type = pp->a_type;
	    pp->a_type = A_NULL;
	}
	if (queptr >= x->x_queend)
	    queptr = x->x_queue;
	else queptr++;
    }
}

static void match_float(t_match *x, t_float f)
{
    if (x->x_patlen)
    {
	SETFLOAT(x->x_queptr, f);
	match_checkin(x);
    }
}

static void match_symbol(t_match *x, t_symbol *s)
{
    if (s && s != &s_ && x->x_patlen)
    {
	SETSYMBOL(x->x_queptr, s);
	match_checkin(x);
    }
}

/* LATER gpointer */

static void match_list(t_match *x, t_symbol *s, int ac, t_atom *av)
{
    while (ac--)
    {
	if (av->a_type == A_FLOAT) match_float(x, av->a_w.w_float);
	else if (av->a_type == A_SYMBOL) match_symbol(x, av->a_w.w_symbol);
	av++;
    }
}

static void match_anything(t_match *x, t_symbol *s, int ac, t_atom *av)
{
    match_symbol(x, s);
    match_list(x, 0, ac, av);
}

static void match_set(t_match *x, t_symbol *s, int ac, t_atom *av)
{
    if (ac)  /* CHECKED */
    {
	t_atom *pp;
	t_symbol *ps_nn;
	int newlen = ac * 2;
	if (newlen > x->x_size)
	{
	    newlen = match_grow(newlen, &x->x_size, &x->x_pattern,
				 MATCH_INISIZE * 2, x->x_patini);
	    if (newlen == MATCH_INISIZE * 2)
	    {
		x->x_queue = x->x_queini;
		ac = MATCH_INISIZE;
	    }
	    else x->x_queue = x->x_pattern + x->x_size / 2;
	}
	x->x_patlen = ac;
	x->x_queend = x->x_queue + ac - 1;
	match_clear(x);  /* CHECKED */
	memcpy(x->x_pattern, av, ac * sizeof(*x->x_pattern));
	pp = x->x_pattern;
	ps_nn = gensym("nn");
	while (ac--)
	{
	    if (pp->a_type == A_SYMBOL && pp->a_w.w_symbol == ps_nn)
		pp->a_type = A_NULL;
	    pp++;
	}
    }
}

static void match_free(t_match *x)
{
    if (x->x_pattern != x->x_patini)
	freebytes(x->x_pattern, 2 * x->x_size * sizeof(*x->x_pattern));
}

static void *match_new(t_symbol *s, int ac, t_atom *av)
{
    t_match *x = (t_match *)pd_new(match_class);
    x->x_size = MATCH_INISIZE * 2;
    x->x_patlen = 0;
    x->x_pattern = x->x_patini;
    x->x_queue = x->x_queini;
    /* x->x_queend is not used unless x->x_patlen > 0,
       LATER consider chosing a more defensive way... */
    outlet_new((t_object *)x, &s_anything);
    match_clear(x);
    match_set(x, 0, ac, av);
    return (x);
}

#ifndef MAXLIB
void match_setup(void)
{
    match_class = class_new(gensym("match"), (t_newmethod)match_new,
		(t_method)match_free, sizeof(t_match), 0, A_GIMME, 0);
#else
void maxlib_match_setup(void)
{
    match_class = class_new(gensym("maxlib_match"), (t_newmethod)match_new,
		(t_method)match_free, sizeof(t_match), 0, A_GIMME, 0);
	class_addcreator((t_newmethod)match_new, gensym("match"), A_GIMME, 0);
#endif
    class_addfloat(match_class, match_float);
    class_addsymbol(match_class, match_symbol);
    class_addlist(match_class, match_list);
    class_addanything(match_class, match_anything);
    class_addmethod(match_class, (t_method)match_set, gensym("set"), A_GIMME, 0);
    class_addmethod(match_class, (t_method)match_clear, gensym("clear"), 0);
#ifndef MAXLIB
    class_sethelpsymbol(match_class, gensym("help-match.pd"));
#else
    class_sethelpsymbol(match_class, gensym("maxlib/help-match.pd"));
#endif
}
