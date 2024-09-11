
# bruter

## Description


Bruter is a metaprogramable lightweight language written in C;

*`UNDER HEAVY DEVELOPMENT`*

## Table of Contents

- [Operators](#operators)
- [Types](#types)
- [Usage](#usage)
- [Libraries](#libraries)
- [Reserved](#reserved)

## Operators


- `$` = variable.

- `;` = end.

- `.` = recursion.

## Arg Types


- `@pointer` = starts with @;

- `number` = starts with 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 or -;

- `string` = enclosed by !() or !word;

- `list` = enclosed by ();

- `nil` = nil;

## Usage

    function;
    function ...;
    function @variable; // pass a pointer as argument to function call;
    function variable; // pass the pointed object as argument to function call;
    function (function (function ...));
    function @variable_1 $variable_2 ...;
    set target_variable value; // set the value to target_variable;
    set target_variable (function ...); // set the result object to target_variable;
    set target_variable @(function ...); // set a pointer to the result object to target_variable;

## Libraries -- not implemented yet -- desconsider

bruter libraries are loaded with `using`:

    using library_name;

library search path:

    libr_path/library_name/library_name.br
    libr_path/library_name.br

## Reserved

- `()` = expression/list delimiter
- `!` = string operator
- `@` = pointer operator
- `$` = temporary operator
- `;` = end of command separator

## TO DO

- `variables` = ok
- `types` = ok
- `operators` = ok
- `expressions` = ok
- `math lib` = ok
- `string lib` = ok
- `list lib` = ok
- `auto lib` = a std lib for memory management, not yet
- `conditons lib` = ok
- `loops lib` = working on it
- `functions lib` = not yet
- `libraries lib` = not yet
- `type punning` = not yet
- `expect` = a function that expects a type and returns a boolean, not yet
- `vs-code extension` = deadly outdated, not yet
