#include <stdio.h>

#define HELP_IMPLEMENTATION
#define WRAP_ON_ANY_CHAR
#include "help.h"

int main(int argc, char* argv[]) {

    // Example of printing WITH arguments
    printf("\n------------------------------------------------------------\n");

    const struct option long_options[] = {
	{"help"			, no_argument, 0,	'h'},
	{"a"			, no_argument, 0,	'a'},
	{"b"			, no_argument, 0,	'b'},
	{"c"			, no_argument, 0,	'c'},
	{"char"			, required_argument, 0,	'r'},
	{"number"		, required_argument, 0, 'n'},
	{"no_arg_example"	, no_argument, 0,       'e'},
	{0, 0, 0, 0}
    };

    const struct desc_information arg_desc[] = {
	{ "Prints the letter A."				, OPTION	, "",						'a' },\
	{ "Prints the letter B."				, OPTION	, "",						'b' },\
	{ "Prints a number of your choosing!"			, OPTION	, "<NUMBER>",					'n' },\
	{ "Prints this help menu!"				, OPTION	, "",						'h' },\
	{ "You can still mix custom arguments in with options!"	, CUSTOM	, "CUSTOM:" },\
	{ "\n  CoOooOoler args:"				, HEADING},\
	{ 0, 55, "OFFS" }, /*So cool that we need to add more space!*/ \
	{ "Prints the letter C."				, OPTION	, "",						'c' },\
	{ "Prints a character from an integer!"			, OPTION	, "<NUMBER THAT TURNS INTO A CHARACTER>",	'r' },\
	{ "Print an example help menu that doesn't use any argument options."	, OPTION, "",	'e' },\
	{ 0, 20, "OFFS" }, \
	{ "\nCommands: (Purely for the example)"		, HEADING},\
	{ "Package-manager-esque build command."		, CUSTOM	, "build, b" },\
	{ "Package-manager-esque upgrade command."		, CUSTOM	, "upgrade, u" },\
	{ "Package-manager-esque uninstall command."		, CUSTOM	, "uninstall" },\
	{ "Check if these commands exist (they don't)"		, CUSTOM	, "check, c" },\
	{ "\n\nhelp.h Made by Silenceman (eestsoftware@gmail.com)", HEADING	, "NOCOL" },\
	{ "Github: https://github.com/eestsoftware/help.h", HEADING	, "NOCOL" },\
	{0, 0, 0, 0},\
    };

    char opt;
    if (argc <= 1) {
	fprintf(stderr, "An argument is required.\n\n");
	print_help(arg_desc, long_options, argv[0], "[EXAMPLE OPTIONS] [COMMANDS, MAYBE]");
	return 1;
    }
    while ((opt = getopt_long(argc, argv, "habcer:n:", long_options, NULL)) != -1) {
	switch(opt) {
	case 'a':
	    printf("A\n");
	    break;
	case 'b': 
	    printf("B\n");
	    break;
	case 'c': 
	    printf("C\n");
	    break;
	case 'n':
	    int num = atoi(optarg);
	    printf("%d\n", num);
	    break;
	case 'r':
	    int char_num = atoi(optarg);
	    printf("%c\n", char_num);
	    break;
	case 'h':
	default:
	    print_help(arg_desc, long_options, argv[0], "[EXAMPLE OPTIONS] [COMMANDS, MAYBE]");
	    return 0;
	    break;
	case 'e':
	    // Example of printing WITHOUT using any arguments.

	    const struct desc_information no_arg_desc[] = {
		{ "Examples:"							, HEADING},\
		{ "Desc"							, CUSTOM	, "Title" },\
		{ "123"								, CUSTOM	, "Foo" },\
		{ "456"								, CUSTOM	, "Bar" },\
		{ "789"								, CUSTOM	, "Buzz" },\
		{ "\n   Subheading:"						, HEADING},\
		{ "Oh boy!"							, CUSTOM	, "Hey man" },\
		{ "\n   Subheading blank:"					, HEADING	, "NOCOL" },\
		{ 0								, 10		, "OFFS" },\
		{ "\n   This section will have a smaller offset:"		, HEADING},\
		{ "Sets the offset with a strange reverse asm-like syntax."	, CUSTOM	, "OFFS" },\
		{ "Blank-line"							, CUSTOM	, "TITLE" },\
		{ "Multi-line example, Multi-line example, Multi-line example, Multi-line example, Multi-line example, Multi-line example, Multi-line example, Multi-line example, Multi-line example, Multi-line example, Multi-line example, Multi-line example, Multi-line example, Multi-line example, Multi-line example, Multi-line example, Multi-line example, Multi-line example, Multi-line example, Multi-line example",  CUSTOM, "EXMP:" },\
		{ ""								, 20		, "WRAP" },\
		{ "\n   This section will have smaller line length:"		, HEADING},\
		{ "Sets the max length until wrap with the same syntax as OFFS.", CUSTOM	, "WRAP" },\
		{ "\n   Here's an extreme example of both used together with the WRAP_ON_ANY_CHAR macro enabled:"	, HEADING	, "" },\
		{ 0								, 60		, "OFFS" },\
		{ 0								, 5		, "WRAP" },\
		{ "Woooooooooooooooooooooooo ooooooooooooooooooooooooooooooooooooooooooo!", CUSTOM	, "TEST" },\
		{ 0								, 70		, "WRAP" },\
		{ 0								, 30		, "OFFS" },\
		{ "Default value for OFFS is 30, and default value for WRAP is 70.", CUSTOM	, "Note:" },\
		{ "By default wrap only puts linebreaks on spaces. WRAP_ON_ANY_CHAR macro changes it to wrap on any char.", CUSTOM	, "Note 2:" },\
		{ 0, 0, 0, 0 },\
	    };

	    print_help(no_arg_desc, NULL, NULL, NULL);
	    break;
	}
    }
    return 0;
}
