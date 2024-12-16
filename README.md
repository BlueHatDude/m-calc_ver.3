# M-Calculator Version 3

## About
A CLI tool for evaluating mathematical expressions and then printing the result.
The program is written entirely in C with no dependencies and is contained in a
single source file and header file. See implementation examples in
[tests.c](src/tests.c) file. 

## Usage
Type an expression in the form `mcalc3 "{expression}"`. The quotation marks are
important, because without them, your shell may evaluate the expressions before
they are being used as input into `mcalc3`.

## Examples
```
>> mcalc3 "2 + 4"
2 + 4 = 6.0

>> mcalc3 "4 - 2"
4 - 2 = 2.0

>> mcalc3 "0.5 * 4"
0.5 * 4 = 2.0

>> mcalc3 "10 / 3"
10 / 3 = 3.3333

>> mcalc3 "2 ^ 5"
2 ^ 5 = 32.0
```

## Supported Functions
- Basic arithmetic operators: `+`, `-`, `*`, `/`, `^`.
    - `mcalc3` does not support implicit negatives (e.g. `5 + -2`)
    - `mcalc3` does not support implicit multiplication (e.g. `(2+5)(3)`)
- Grouping: `(` and `)`
    - Do not use `[]` or `{}`.

## Important Notes
- `mcalc3` does not support variables (e.g. `2x + 5` is not a valid expression).
- `mcalc3` does not support unicode, and you may encounter problems if you try
to use it. 
- `mcalc3` is finsihed, but it is not a fully featured scientific calculator.
These features will be added in `mcalc4`, because I want to completely rewrite 
this project. 
