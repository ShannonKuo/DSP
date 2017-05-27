/*
 * option.h --
 *	This defines the Option type and the interface to the
 *	Opt_Parse library call that parses command lines.
 *
 * Copyright 1988, 1991 Regents of the University of California
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies.  The University of California
 * makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 *
 * $Header: /home/srilm/devel/misc/src/RCS/option.h,v 1.11 2006/01/05 06:05:17 stolcke Exp $ SPRITE (Berkeley)
 */

#ifndef _OPTION
#define _OPTION

#ifdef __cplusplus
extern "C" {
#endif

#include <cfuncproto.h>

/*
 * An array of option descriptions (type Option) is passed into the
 * routine which interprets the command line.  Each option description
 * includes the key-string that indicates the option, a type for the option,
 * the address of an associated variable, and a documentation message
 * that is printed when the command is invoked with a single argument
 * of '?'
 */

typedef struct Option {
    int		type;		/* Indicates option type;  see below */
    _CONST char	*key;		/* The key string that flags option */
    _VoidPtr	address;	/* Address of variable to modify */
    _CONST char	*docMsg;	/* Documentation message */
} Option;
/*
 * Values for type:
 *
 *	OPT_CONSTANT(val) -	if the flag is present then set the
 *				associated (integer) variable to val.
 *				Val must be a non-negative integer.
 *	OPT_TRUE -		if the flag is present then set the
 *				associated (integer) variable to TRUE (1).
 *	OPT_FALSE -		if the flag is present then set the
 *				associated (integer) variable to FALSE (0).
 *	OPT_INT -		if the flag is present then the next argument
 *				on the command line is interpreted as a
 *				signed integer and that value is assigned to
 *				the options associated variable.
 *	OPT_UINT -		if the flag is present then the next argument
 *				on the command line is interpreted as an
 *				unsigned integer and that value is assigned to
 *				the options associated variable.
 *	OPT_STRING -		if the flag is present then the next argument
 *				on the command line is copied into the string
 *				variable associated with the option.
 *	OPT_REST -		if the flag is present, inhibit processing of
 *				later options, so that they're all returned
 *				to the caller in argv.  In addition, set the
 *				associated variable to the index of the first
 *				of these arguments in the returned argv.
 *				This permits a program to allow a flag to
 *				separate its own options from options it will
 *				pass to another program.
 *	OPT_FLOAT -		if the flag is present then the next argument
 *				on the command line is interpreted as a
 *				"double" and that value is assigned to the
 *				option's associated variable.
 *	OPT_TIME -		if the flag is present then the next argument 
 *				on the command line is interpreted as a date 
 *				and time.  The corresponding time value 
 *				(number of seconds past the epoch) is assigned
 *				to the option's associated variable.
 *	OPT_FUNC -		if the flag is present, pass the next argument
 *				to "address" as a function. The function
 *				should be declared:
 *				    int
 *				    func(optString, arg)
 *					char 	*optString;
 *					char	*arg;
 *			    	Func should return non-zero if the argument
 *				was consumed or zero if not.  "optString" is
 *				the option key string that caused the
 *				function to be called and "arg" is the next
 *				argument (if there is no next argument then
 *				"arg" will be NULL).
 *	OPT_GENFUNC -		if the flag is present, pass the remaining
 *				arguments and the number of arguments to
 *				"address" as a function. The function should
 *				be declared:
 *				    int
 *				    func(optString, argc, argv)
 *					char *optString;
 *					int argc;
 *					char **argv;
 *				and should return the new number of arguments
 *				left in argv.  argv should have been shuffled
 *				to eliminate the arguments func consumed.
 *	OPT_DOC -		a dummy entry. Exists mostly for its
 *				documentation string.  As an additional side
 *				effect, if its key string an argument,
 *				Opt_Parse will treat it like a question mark
 *				(i.e. print out the program's usage and exit).
 */

#define OPT_CONSTANT(val)	((int) val)
#define OPT_FALSE		0
#define OPT_TRUE		1
#define OPT_INT			-1
#define OPT_UINT		-2
#define OPT_STRING		-3
#define OPT_REST		-4
#define OPT_FLOAT		-5
#define OPT_FUNC		-6
#define OPT_GENFUNC		-7
#define OPT_DOC			-8
#define OPT_TIME		-9

/*
 * Flag values for Opt_Parse:
 *
 * OPT_ALLOW_CLUSTERING -	Permit many flags to be clustered under
 *				a single "-".  In otherwords, treat
 *				"foo -abc" the same as "foo -a -b -c".
 * OPT_OPTIONS_FIRST	-	Stop parsing if something other than an
 *				option (starting with a hyphen) is encountered.
 */

#define OPT_ALLOW_CLUSTERING	1
#define OPT_OPTIONS_FIRST	2

/*
 * Exported procedures:
 */

int Opt_Parse _ARGS_ ((int argc, char *argv[], Option *optionArray, 
		       int numOptions, int flags));

void Opt_PrintUsage _ARGS_ ((char *commandName, Option *optionArray,
			     int numOptions));

/*
 * Macro to determine size of option array:
 */

#define Opt_Number(optionArray)	(sizeof(optionArray)/sizeof((optionArray)[0]))

#ifdef __cplusplus
}
#endif

#endif /* _OPTION */
