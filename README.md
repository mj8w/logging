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

Singletons are not friendly to unit testing, since they cannot be overridden to perform testing around them - they are truly one instance, and you need multiple instances for unit testing. Also, unit tests can see them as memory leaks since they aren't destroyed. With a global instance in a .cpp file, you can decide to not link in that file and replace it in the unit tests.
    
The method I decided to use here is to make global objects and refer to them directly.
The objects are defined in .cpp files but prototypes in .h files. This should allow
.cpp files to be replaced in unit tests, and can be instantiated locally per test.     
    
The main reason to do this is that when debugging, it might be possible to refer to the global objects, which doesn't work when everything is pointers without a global reference.

## Logging Object
LOG_INFO macro should point at a shared pointer