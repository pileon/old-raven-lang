# raven-lang

A generic script language, but made primarily for use in the Raven MUD server.

## Highlights

* Object-oriented
* Everything is first-class
* Dynamic typing
* Syntax similar to C and C++
* Extensible, possible to add syntactic structures

## Examples

### Hello world

    print("Hello world\n")

### <a name="fibonacci"></a>Fibonacci series, recursive

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

The above example also demonstrates the possibility to have arbitrary
expression in literal string constants.

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

**Q.** What is the difference between a class and an object?<br/>
**A.** A class is like a *blueprint* for an object. An object is an
actual implementation of the blueprint.

A class can not be used as an object, and an object can't be used as
a class. That means you can not use a class in expressions, and you
can't inherit from an object.

**Q.** Does an anonymous function capture its scope?<br/>
**A.** Yes, if you have an anonymous function as part of an expression
it will capture its surrounding scope.

Example:

    var foo = 5
    function(){ print("foo = %{foo}\n") }()

When declaring a named function (like `fib` in the [Fibonacci](#fibonacci) example
above) the scope is not captured.
