
# bruter

## Description


Bruter is a metaprogramable lightweight language written in C;

*`UNDER HEAVY DEVELOPMENT`*

## Table of Contents

- [Types](#types)
- [Usage](#usage)
- [Libraries](#libraries)
- [Reserved](#reserved)

## Arg Types

- `number` = starts with 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 or -;

- `string` = enclosed by double quotes;

- `list` = enclosed by ();

- `nil` = nil;

## Usage

    function;
    function ...;
    function variable;
    function (function (function ...));
    function variable_1 variable_2 ...;
    set target_variable value; 
    set target_variable (function ...);

## Libraries -- not implemented yet -- desconsider

bruter libraries are loaded with `using`:

    using library_name;

library search path:

    libr_path/library_name/library_name.br
    libr_path/library_name.br

## Reserved

- `()` = expression/list delimiter
- `""` = string delimiter
- `;` = end of command separator

## TO DO

- `functions` = ok/testing
- `prototypes lib` = testing
- `multiprocessing` = ok on linux, need to write specific code for windows because of fork
- `libraries lib` = +/- check std_include
- `expect` = a function that expects a type and returns a boolean, not yet
- `vs-code extension` = testing
- `reduce allocs` = not yet
