/* winf.h

   Copyright 2006, 2007, 2009, 2011, 2013 Red Hat, Inc.

This software is a copyrighted work licensed under the terms of the
Cygwin license.  Please consult the file "CYGWIN_LICENSE" for
details. */

#ifndef _WINF_H
#define _WINF_H

/* Hack for Cygwin processes.  If the Windows command line length gets slightly
   bigger than this value, the stack position is suddenly moved up by 64K for
   no apparent reason, which results in subsequent forks failing.  Since Cygwin
   processes get the full command line as argv array anyway, this only affects
   the maximum command line length of Cygwin applications which nonsensically
   have a WinMain instead of a main entry point or which use GetCommandLine. */
#define MAXCYGWINCMDLEN 30000

#define MAXWINCMDLEN 32767
#define LINE_BUF_CHUNK (MAX_PATH * 2)

class av
{
  char **argv;
  int calloced;
 public:
  int argc;
  bool win16_exe;
  av (): argv (NULL) {}
  av (int ac_in, const char * const *av_in) : calloced (0), argc (ac_in), win16_exe (false)
  {
    argv = (char **) cmalloc_abort (HEAP_1_ARGV, (argc + 5) * sizeof (char *));
    memcpy (argv, av_in, (argc + 1) * sizeof (char *));
  }
  void *operator new (size_t, void *p) __attribute__ ((nothrow)) {return p;}
  ~av ()
  {
    if (argv)
      {
	for (int i = 0; i < calloced; i++)
	  cfree (argv[i]);
	cfree (argv);
      }
  }
  int unshift (const char *what, int conv = 0) __reg2;
  operator char **() {return argv;}
  void all_calloced () {calloced = argc;}
  void replace0_maybe (const char *arg0)
  {
    /* Note: Assumes that argv array has not yet been "unshifted" */
    if (!calloced)
      {
	argv[0] = cstrdup1 (arg0);
	calloced = 1;
      }
  }
  void dup_all ()
  {
    for (int i = calloced; i < argc; i++)
      argv[i] = cstrdup1 (argv[i]);
    calloced = argc;
  }
  int setup (const char *, path_conv&, const char *, int, const char *const *,
	     bool) __reg3;
};

class linebuf
{
 public:
  size_t ix;
  char *buf;
  size_t alloced;
  linebuf () : ix (0), buf (NULL), alloced (0) {}
  ~linebuf () {if (buf) free (buf);}
  void __reg3 add (const char *, int);
  void add (const char *what) {add (what, strlen (what));}
  void prepend (const char *, int);
  void __reg2 finish (bool);
  bool __reg3 fromargv(av&, const char *, bool);;
  operator char *() {return buf;}
};

#endif /*_WINF_H*/
