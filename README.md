# logging
C++ logging example using fmt library and minimal interface using macros, meaning it is very simple to write the code that uses it and is very powerful.

# Notes
Notes about a variety of subjects that might be relevant 

## About Singletons
I did some research on Singletons, comparing to static variables.
Here are some facts I learned.
- Singletons allow the object to be constructed "lazyily", meaning that the object can be created after main() starts.
    - They aren't required to be. You can make them internally defined as static
- Statics can be global or local, and so can Singletons.
- You can force a class to only be instantiated once with a singleton
    - "restricting" developers like little children
