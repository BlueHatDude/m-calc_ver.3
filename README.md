# m-calc_ver.3

A CLI tool for evaluating mathematical expressions and then printing the result. The program is written entirely in C with no dependencies and is contained in a single source file and header file. 

## Usage
Type an expression in the form `mcalc3 "{expression}"`. The quotation marks are important, because without them, your shell may evaluate the expressions before they are being used as input into `mcalc3`.

## Examples
```
>> mcalc3 "2 + 4"
2 + 4 = 6.0000
>> mcalc3 "4 - 2"
4 - 2 = 2.0000
>> mcalc3 "0.5 * 4"
0.5 * 4 = 2
>> mcalc3 "10 / 3"
10 / 3 = 3.3333
>> mcalc3 "2 ^ 5"
2 ^ 5 = 32
>> mcalc3 "sqrt(25)"
sqrt(25) = 5
>> mcalc3 "cos(PI/3)" "sin(PI/3)"
cos(PI/3) = 0.5
sin(PI/3) = 0.8660
```
