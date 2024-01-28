# PL/M to C

Updated & compiled on modern systems using GCC.

Usage:

```sh
# ./plm2c /pth/to/src.plm
```

This is a PL/M to C converter.  It will take most PL/M code and do a nice
job of converting it to C.  It tries to be intelligent about leaving formatting
and comments intact.  This version supports PL/M-286.  It is something I wrote
about a year ago to convert several meg of source code, and it did a 99.5%
conversion.  I was later handed some source code that it crashed on, and I
extended it to support that code too.

Please keep in mind that this may well not work for your code.  It has
been tested only on a few sets of code, each following its own set of coding
standards.  Also, don't expect a lot of support from me, as my interest in
PL/M is next to none.  I no longer work for the employer for whom I wrote
this converter, but they have given me permission to own and post the sources.
I will, time permitting, collect bug fixes and post patches to the software.
Please mail fixes directly to me, as I may miss any posting of them.  If
anyone is interested in taking over maintenance of this code, please let me
know!

The source code compiles under Unix.  I've compiled it on a Sun-4, a VAX
running Ultrix, and a VAX running Mt. Xinu.  At one time I had a version that
ran under MSDOS, but I can't guarantee it will now.  I've included a makefile
for use with Turbo-C.  You need to define IBMPC to compile it.  What it could
really use is tuning for a large memory model, as in it's present state, it
can only handle small source files.  It could also benefit from a good man
page.

The converter expects source code to be in "proper" format (i.e. proper
use of EXTERNAL declarations, and following of the Intel PL/M grammar as the
converter knows it.)  It has some moderate error-recovery, but may well dump
core if it expects one thing and gets another.

I've included a garbage file test.plm; typeing "plm2c test.plm" should
result in a test.c file that is identical to the provided test.c.out.

See the file CAVEATS.DOC for compatibility issues.
    
Hope you find it useful!<br/>
Robert Ankeney<br/>
April 9, 1991<br/>
...!tektronix!bob@reed.bitnet<br/>


