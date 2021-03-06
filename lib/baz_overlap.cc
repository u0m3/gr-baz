/* -*- c++ -*- */
/*
 * Copyright 2004 Free Software Foundation, Inc.
 * 
 * This file is part of GNU Radio
 * 
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

/*
 * gr-baz by Balint Seeber (http://spench.net/contact)
 * Information, documentation & samples: http://wiki.spench.net/wiki/gr-baz
 */

/*
 * config.h is generated by configure.  It contains the results
 * of probing for features, options etc.  It should be the first
 * file included in your .cc file.
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <baz_overlap.h>
#include <gnuradio/io_signature.h>

#include <stdio.h>

/*
 * Create a new instance of baz_pow_cc and return
 * a boost shared_ptr.  This is effectively the public constructor.
 */
baz_overlap_sptr 
baz_make_overlap (int item_size, int vlen, int overlap, int samp_rate)
{
  return baz_overlap_sptr (new baz_overlap (item_size, vlen, overlap, samp_rate));
}

/*
 * Specify constraints on number of input and output streams.
 * This info is used to construct the input and output signatures
 * (2nd & 3rd args to gr::block's constructor).  The input and
 * output signatures are used by the runtime system to
 * check that a valid number and type of inputs and outputs
 * are connected to this block.  In this case, we accept
 * only 1 input and 1 output.
 */
static const int MIN_IN = 1;	// mininum number of input streams
static const int MAX_IN = 1;	// maximum number of input streams
static const int MIN_OUT = 1;	// minimum number of output streams
static const int MAX_OUT = 1;	// maximum number of output streams

/*
 * The private constructor
 */
baz_overlap::baz_overlap (int item_size, int vlen, int overlap, int samp_rate)
	: gr::block ("overlap",
		gr::io_signature::make(MIN_IN, MAX_IN, item_size),
		gr::io_signature::make(MIN_OUT, MAX_OUT, item_size))
	, d_item_size(item_size)
	, d_vlen(vlen)
	, d_overlap(overlap)
	, d_samp_rate(samp_rate)
{
	float rate = (float)/*samp_rate*/vlen / (float)overlap;
	//set_relative_rate(rate);
	//set_history(overlap);
	set_output_multiple(d_vlen);
	
	fprintf(stderr, "[%s<%i>] item size: %d, vlen: %d, overlap: %d, sample rate: %d\n", name().c_str(), unique_id(), item_size, vlen, overlap, samp_rate);
	fprintf(stderr, "[%s<%i>] rate: %f\n", name().c_str(), unique_id(), rate);
}

/*
 * Our virtual destructor.
 */
baz_overlap::~baz_overlap ()
{
}

void baz_overlap::set_overlap(int overlap)
{
	d_overlap = overlap;
}

void baz_overlap::forecast(int noutput_items, gr_vector_int &ninput_items_required)
{
	//or (size_t i = 0; i < ninput_items_required.size(); ++i)
	//	ninput_items_required[i] = d_item_size*d_vlen;
	
	//fprintf(stderr, "[%s<%i>] forecast: noutput_items: %d\n", name().c_str(), unique_id(), noutput_items);
	
	gr::block::forecast(noutput_items, ninput_items_required);
}

int baz_overlap::general_work(int noutput_items, gr_vector_int &ninput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items)
{
	const void *in = (const void *)input_items[0];
	void *out = (void *)output_items[0];

	/*for (int i = 0; i < noutput_items; i++)
	{
	}*/
	
	//fprintf(stderr, "[%s<%i>] work: noutput_items: %d\n", name().c_str(), unique_id(), noutput_items);
	
	if (noutput_items < d_vlen)
	{
		fprintf(stderr, "[%s<%i>] not enough for work: noutput_items: %d\n", name().c_str(), unique_id(), noutput_items);
		return 0;
	}
	
	memcpy(out, in, d_item_size*d_vlen);
	
	consume_each(d_overlap);
	
	return d_vlen;
}
