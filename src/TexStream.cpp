/**
 * \file TexStream.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * Full author contact details are available in file CREDITS.
 *
 * Inspired by Dietmar Kuehl's prefix iostreams found on
 * http://www.inf.uni-konstanz.de/~kuehl/
 */

#include <config.h>

#include "TexStream.h"
#include "TexRow.h"

#include <iostream>
#include <streambuf>

namespace lyx {

////////////////////////////////////////////////////////////////
//
// TexStreamBuffer
//
////////////////////////////////////////////////////////////////


class TexStreamBuffer : public TexStreamBase
{
public:
	TexStreamBuffer(TexStreamBase * sbuf, TexRow * texrow);
	int line() const { return line_; }
	int column() const { return column_; }

protected:
	int overflow(int);
	int sync();

private:
	TexStreamBase * sbuf_; 
	TexRow * texrow_;
	int column_;
	int line_;
};


TexStreamBuffer::TexStreamBuffer(TexStreamBase *sb, TexRow * texrow)
  : sbuf_(sb), texrow_(texrow), line_(0)
{
	setp(0, 0);
	setg(0, 0, 0);
}

int TexStreamBuffer::overflow(int c)
{
	if (c == '\n') {
		++line_;
		column_ = 0;
	} else {
		++column_;
	}
	return c;
}


int TexStreamBuffer::sync()
{
	sbuf_->pubsync();
	return 0;
}

  
////////////////////////////////////////////////////////////////
//
// TexStream
//
////////////////////////////////////////////////////////////////

TexStream::TexStream(TexStreamBase * sbuf, TexRow * texrow)
		: std::basic_ostream<char_type>(sbuf_ = new TexStreamBuffer(sbuf, texrow))
{}


TexStream::~TexStream()
{
	delete sbuf_;
}


int TexStream::line() const
{
	return sbuf_->line();
}


////////////////////////////////////////////////////////////////
//
// Test
//
////////////////////////////////////////////////////////////////

#if 0

int main(int argc, char *argv[])
{
	TexStream out(cout.rdbuf());
	char c;
	while (cin) {
		if (cin.get(c))
			out.put(c);
	}
	cout << "line count: " << out.line() << endl;

	return 0;
}

#endif

}

