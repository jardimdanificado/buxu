
# buxu

  buxu stands for (B)RUTER (U)ni(X) (U)tils, it is a set of tools used to build, run and debug buxu scripts and libraries, specifically on unix-like systems;

  buxu includes, but is not limited to:
  - buxu, a interpreter and repl for bruter representation;
  - bupm, stands for Buxu Package Manager;
  - bucc, a tool to compile buxu c files into buxu-compatible shared libraries, stants for Buxu C Compiler;

*`UNDER HEAVY DEVELOPMENT`*

# Building instructions

  ```c
    gcc -o buxu buxu.c -ldl 
  ```
  does the trick;

# Usage
  
  "buxu" executable is the interpreter, and it also act as repl if no args are passed;
  
    # this will start in the repl;
    ./buxu 

    # run script;
    ./buxu path/to/script.br

    # print help message;
    ./buxu --help
    ./buxu -h

    # print version;
    ./buxu --version
    ./buxu -v

    # preload a library;
    ./buxu --load path/to/library.so
    ./buxu -l path/to/library.so

    # add a path to the search path;
    ./buxu --path path/to/libraries
    ./buxu -p path/to/libraries

    # eval a string;
    ./buxu --eval "print 'hello world'"
    ./buxu -e "print 'hello world'"

  "bucc" executable is the compiler script, its basically a wrapper for the system compiler(default is gcc) with some extra flags;

    # compile a c file;
    ./bucc path/to/file.c

    # compile a c bruter file with a specific output name;
    ./bucc path/to/file.c -o path/to/output.so

    # compile a c bruter file using a specific compiler;
    ./bucc path/to/file.c -cc path/to/compiler

    # compile a c bruter file with debug symbols;
    ./bucc path/to/file.c --debug

    # print help message;
    ./bucc --help
    ./bucc -h

    # print version;
    ./bucc --version
    ./bucc -v

  "bupm" executable is the package manager, it deals with a simple repository system, it can list, install and uninstall packages, and have some user-defined commands like build, clear, update, etc;

    # install a package;
    ./bupm install package_name

    # uninstall a package;
    ./bupm uninstall package_name

    # update a package;
    ./bupm update

    # build a package;
    ./bupm build

    # clear a package;
    ./bupm clear

    # print help message;
    ./bupm --help
    ./bupm -h

# Dynamic libraries

  dynamic libraries functions are built in the cli.c file, while unload is a thing, all dynamic libs are auto-closed at exit;

  | Function    | Description                              | Function Signature                           |
  |-------------|------------------------------------------|----------------------------------------------|
  | `load`   | load a library                           | `void function(string path);`                |
  | `unload` | unload a library                         | `void function(string path);`                |

  # DEPENDENCIES

  you might want to know how BRUTER works:  https://github.com/brutopolis/bruter
  
  you might also want to know how br(bruter representation) works: https://github.com/brutopolis/br
