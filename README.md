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

### Classes and inheritance

    var animal = class
    {
        var name
        var speak = function()
        {
            print("%{name} says "%{make_sound()}.\n")
        }
    }
    
    var cat = class(animal)
    {
        var make_sound = function()
        {
            return "meow"
        }
    }
    
    var dog = class(animal)
    {
        var make_sound = function()
        {
            return "woof"
        }
    }
    
    var spot = object(dog)
    {
        name = "Spot"
    }
    
    var fluffy = object(cat)
    {
        name = "Fluffy"
    }
    
    for (var a : [fluffy, spot])
        a.speak()

## Questions that might be frequently asked some time in the future

**Q.** What is the difference between a class and an object?
**A.** A class is like a *blueprint* for an object. An object is an
actual implementation of the blueprint.

A class can not be used as an object, and an object can't be used as
a class.
