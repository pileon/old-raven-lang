# raven-lang

A generic script language, but made primarily for use in the Raven MUD server.

## Highlights

* Object-oriented
* Everything is first-class
* Dynamic typing
* Syntax similar to C and C++
* Extendable, possible to add syntactic structures

## Examples

### Hello world

    print("Hello world\n")

### Fibonacci series, recursive

    # Recursively calculate a Fibonacci number
    var fib = function(n)
    {
        if (n == 0 || n == 1)
            return n;
            
        return fib(n - 1) + fib(n - 2);
    }
    
    # Print the first ten Fibonacci numbers
    for (i : 1..10)
        print("Fibbonacci for %{i} is %{fib(i)}\n");
