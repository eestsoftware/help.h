// help.h -- v0.1.0 -- simple help output formatting tool - public domain - Silenceman, Nov 2026
//
// USAGE
// 
// In *ONE* source file, put 
//    # define HELP_IMPLEMENTATION
//    #include "help.h"
//
// Other source files should just include help.h
//
// MACRO API
//
//    - HELP_COLOR_HEADING - Set ANSI color code of headings. Default: "\x1b[32m"	(Green)
//    - HELP_COLOR_OPTION - Set ANSI color code of options. Default: "\x1b[36m"		(Cyan)
//    - HELP_COLOR_ARG - Set ANSI color code of arguments. Default: "\x1b[34m"		(Blue)
//    - HELP_COLOR_DESC - Set ANSI color code of descriptions. Default: "\x1b[0m"	(None)
//
//    - MAX_DESC - Set maximum amount of descriptions checked in an array of descriptions.
//                 This is done so its more "memory-safe" if someone forgets to put a sentinel
//                 in the array.
//
//    - WRAP_ON_ANY_CHAR - If defined, then the description will wrap onto a new line on any character.
//                         By default, the description only wraps at a space ' ' character.
//
// ABOUT
//
// This is meant to be used in conjnuction with GNU <getopt.h> for printing a clean-looking help output that
// covers each flag in the program and optional arguments for it. 
//
// This was made out of frustration when trying to manually allign help ouput using nothing but printf statements
// and emacs simple C mode. So instead, I cooked up a simple boilerplate formatter for decent-looking help outputs.
// Supports colors, custom "options", customizable arguments, custom headings, adjustable offset/line length, whatever.
//
// Fairly extensible too, came up with some sort of an Assembly-Esque syntax for it. May be a bit overcomplicated,
// but it gets the job done.
// 
// BASIC EXAMPLES
//
// See example.c for a more in-depth example that covers pretty much every case.
// Compile it with `cc -o example ./example.c` and run it for yourself.
//
// But for a simpler usecase, you can do something like this:
// 
//  From <getopt.h>:
//  const struct option options[] = {    // This is the program's arg options (Put a \ at the end of each line to cancel out the linebreak)
//	{"help"			, no_argument, 0,	'h'},\
//	{"a"			, no_argument, 0,	'a'},\
//	{"b"			, required_argument, 0,	'b'},\
//	{0, 0, 0, 0},\
//  };
//
//   const struct desc_information arg_desc[] = {     // Description for each argument
//     { "Print a help menu.",  OPTION, "", 'h'},\
//     { "Print the letter A.", OPTION, "", 'a'},\
//     { "Print the letter B alongside something else.", OPTION, "<SOMETHING ELSE>", 'b'},\
//     {0, 0, 0, 0},\ // Don't forget the terminating sentinel!
//   };
// struct desc_information is: { Description, TYPE, Argument Name, (Option Flag Character|NULL)
//  
//   print_help(arg_desc, options, argv[0], NULL);
//
//   Arguments 3 and 4 are NULLable, (program name , usage override). They both print the "Usage: .... [...]" text
//   at the beginning of help output. Usage override will override the [..] text after the program name. If NULL,
//   then its left as [OPTIONS].
//   If program name is NULL, don't print Usage text.
//
//
// MORE EXAMPLES
//
//   A header can be defined in the desc_information array like this:
//      {"Commands:", HEADER},\
//
//     You can have a header without color by using the NULL argument with "NOCOL"
//        {"Commands:", HEADER, "NOCOL"},\
//     This is useful if you want a signature at the bottom of your program, or something like that.
//
//
//   A custom option can be defined like this:
//      {"Option Description", CUSTOM, "Option Title:"},\
//
//      Note the fact that it appears reversed, due to the description always coming first. (TODO:: MAYBE change that?)
//
//   A "command" can be defined like this:
//      {0, <NUM>, "WRAP"},\
//      {0, <NUM>, "OFFS"},\
//
//   OFFS changes the offset between the option title and the description.
//   WRAP changes the maximum length of the description before it wraps.
//
// As you can see, due to the fact that the entire description is a simple struct array, and due to me wanting to save space,
// adding commands and other things lead to it having an assembly-like 'experimental' syntax.
//
// I don't know if I like this, and this MMaaay be subject to change later. We'll see.

#ifndef HELP_H_
#define HELP_H_

#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>

#ifndef HELP_COLOR_HEADING
#define HELP_COLOR_HEADING "\x1b[32m"
#endif

#ifndef HELP_COLOR_OPTION
#define HELP_COLOR_OPTION "\x1b[36m"
#endif

#ifndef HELP_COLOR_ARG
#define HELP_COLOR_ARG "\x1b[34m"
#endif

#ifndef HELP_COLOR_DESC
#define HELP_COLOR_DESC	"\x1b[0m"
#endif

#define HELP_COLOR_RESET "\x1b[0m"

#ifndef MAX_DESC
#define MAX_DESC 256
#endif // MAX_DESC

struct desc_information {
    char* desc		;
    unsigned int is_opt	;
    char* arg_name	;
    char opt_char       ;
};

enum opt_override {
    HEADING, OPTION, CUSTOM
};

static void print_help(const struct desc_information* opt_descs, const struct option* options, char *prog_name, char *usage_override);

const static struct option* get_option_from_char(const struct option* options, char opt_char);

#endif // HELP_H_

/////////////////////////

#ifdef HELP_IMPLEMENTATION

static void print_help(const struct desc_information* opt_descs, const struct option* options, char *prog_name, char* usage_override) {
    if (prog_name != NULL) {
	if (usage_override == NULL) usage_override = "[OPTIONS]";

	printf(HELP_COLOR_HEADING	"Usage:" HELP_COLOR_RESET\
	HELP_COLOR_OPTION		" %s" HELP_COLOR_RESET\
	HELP_COLOR_ARG			" %s" HELP_COLOR_RESET "\n\n"\
	HELP_COLOR_HEADING		"Options:" HELP_COLOR_RESET "\n"\
	, prog_name, usage_override);
    }
    //dsc is seperate for description category formatting
    int whitespace_offset = 30;
    int max_line_len = 70;
    for (int i = 0, dsc = 0; dsc < MAX_DESC; ++i, ++dsc) { 
	if (opt_descs[dsc].desc == NULL && opt_descs[dsc].arg_name == NULL) break; // Sentinel terminated

        if (opt_descs[dsc].desc == NULL) {
	    if (strncmp(opt_descs[dsc].arg_name, "OFFS", 5) == 0) whitespace_offset = opt_descs[dsc].is_opt;
	    if (strncmp(opt_descs[dsc].arg_name, "WRAP", 5) == 0) max_line_len = opt_descs[dsc].is_opt;
	    --i;
	    continue;
	}

	char *splitDesc = strdup(opt_descs[dsc].desc);

	size_t wspace = 10; // This is all the space and - characters in a default formatted opt
	int is_no_col = opt_descs[dsc].arg_name != NULL && strncmp(opt_descs[dsc].arg_name, "NOCOL", 6) == 0;
	if (opt_descs[dsc].is_opt == HEADING) {
	    printf("%s%s" HELP_COLOR_RESET "\n", is_no_col ? HELP_COLOR_RESET : HELP_COLOR_HEADING, splitDesc);
	    --i;
	    free(splitDesc);
	    continue;
	} else if (opt_descs[dsc].is_opt == CUSTOM) { // Custom text formatted like an option.
	    printf(HELP_COLOR_OPTION "    %s" HELP_COLOR_RESET, opt_descs[dsc].arg_name);
	    wspace = 4 + strlen(opt_descs[dsc].arg_name);
	} else if (opt_descs[dsc].is_opt == OPTION) {
	    if (options == NULL || opt_descs[dsc].opt_char == 0) continue;
	    const struct option* help_option = get_option_from_char(options, opt_descs[dsc].opt_char);
	    if (help_option == NULL) continue;
	    printf("%s   -%c, --%s" HELP_COLOR_RESET HELP_COLOR_ARG " %s" HELP_COLOR_RESET,
	    is_no_col ? HELP_COLOR_RESET : HELP_COLOR_OPTION,
	    help_option->val,
	    help_option->name,
	    (help_option->has_arg && !is_no_col) ? opt_descs[dsc].arg_name : "");
	    wspace += strlen(help_option->name);
	}
	int whitespace_offset_offset = (opt_descs[dsc].is_opt == OPTION ? strlen(opt_descs[dsc].arg_name) : 0);
	if (whitespace_offset <= 0) whitespace_offset = 0;
	for (; wspace < whitespace_offset - whitespace_offset_offset; ++wspace) printf(" ");
	printf(HELP_COLOR_DESC);
	size_t dLen = strlen(splitDesc);
	int lineBreak = 0;

	#ifdef WRAP_ON_ANY_CHAR
	int break_points[100];
	for (size_t b = 0; b < 100; ++b) break_points[b] = -1;
	int break_point_idx = 0;
	int break_point_done = 0;
	#endif//WRAP_ON_ANYCHAR

	for (size_t j = 0; dLen >= max_line_len && j < dLen; ++j, ++lineBreak) {
	    #ifdef WRAP_ON_ANY_CHAR
	    if (lineBreak < max_line_len) continue;
	    break_points[break_point_idx] = j;
	    ++break_point_idx;
	    #else
	    if (lineBreak < max_line_len || splitDesc[j] != ' ') continue;
	    splitDesc[j] = '\n';
	    #endif//WRAP_ON_ANYCHAR
	    lineBreak = 0;
	}
	for (size_t j = 0; j < dLen; ++j) {
	    #ifdef WRAP_ON_ANY_CHAR // It replaces the char with a linebreak, so we have to do some more BS to reinsert the missing one.
	    if (break_points[break_point_done] == j) {
		++break_point_done;
		--j;
	    #else
	    if (splitDesc[j] == '\n') {
	    #endif//WRAP_ON_ANYCHAR
		printf("\n"); // instead of padding the string by insreting, do this BS
		for (size_t wspace = 0; wspace < whitespace_offset; ++wspace) printf(" ");
		continue;
	    }
	    printf("%c", splitDesc[j]);
	}
	printf(HELP_COLOR_RESET "\n");
	free(splitDesc);
    }
}

const static struct option* get_option_from_char(const struct option* options, char opt_char) {
    const struct option *ret_opt = NULL;
    for (int i = 0; i < MAX_DESC; ++i) {
        if (options[i].name == NULL) break; //use the sentinel
	if (options[i].val != opt_char) continue;
	ret_opt = &options[i];
	break;
    }
    return ret_opt;
}

#endif //HELP_IMPLEMENTATION

/* MIT License
Copyright (c) 2026 Silenceman <eestsoftware@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
*/
