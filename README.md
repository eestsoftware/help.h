# help.h

Very simple header-only library for formatting CLI help output. Taken and made more extensible from [one of my previous projects](https://github.com/eestsoftware/titris).
Documentation found in the comment inside of the `help.h` file.

## Quick Start

Check [example.c](./example.c) for a basic example of what you can do with it.

```console
$ cc -o example ./example.c
$ ./example
```

Output:

```sh
------------------------------------------------------------
An argument is required.

Usage: ./example [EXAMPLE OPTIONS] [COMMANDS, MAYBE]

Options:
   -a, --a                    Prints the letter A.
   -b, --b                    Prints the letter B.
   -n, --number <NUMBER>      Prints a number of your choosing!
   -h, --help                 Prints this help menu!
    CUSTOM:                   You can still mix custom arguments in with options!

  CoOooOoler args:
   -c, --c                                             Prints the letter C.
   -r, --char <NUMBER THAT TURNS INTO A CHARACTER>     Prints a character from an integer!
   -e, --no_arg_example                                Print an example help menu that doesn't use any argument options.

Commands: (Purely for the example)
    build, b        Package-manager-esque build command.
    upgrade, u      Package-manager-esque upgrade command.
    uninstall       Package-manager-esque uninstall command.
    check, c        Check if these commands exist (they don't)
    

help.h Made by Silenceman (eestsoftware@gmail.com)
Github: https://github.com/eestsoftware/help.h
```
