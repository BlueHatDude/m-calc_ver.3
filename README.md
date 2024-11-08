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

>> mcalc3 "sqrt(25)"
sqrt(25) = 5.0

>> mcalc3 "cos(PI/3)" "ln(e^5)"
cos(PI/3) = 0.5
ln(e^5) = 5.0
```

## Supported Functions
- Basic arithmetic operators: `+`, `-`, `*`, `/`, `^`.
- Trignonometric functions: `sin`, `cos`, `tan`, `arcsin`, `arccos`, `arctan`.
    - inverse functions are not currently implemented.
- Logarithms: `ln` and `log`
- Constants: `pi` and `e`
    - do not use π character. 

## Important Notes
- `mcalc3` does not support variables (e.g. `2x + 5` is not a valid expression).
- `mcalc3` does not support unicode, and you may encounter problems if you try
to use it. 
- `mcalc3` is current a WIP. You will not see proper results if you try to use 
it now. 
