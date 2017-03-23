## Synopsis

PERG is a tiny grep-like utility capable of doing but a tiny subset of what we're used to with grep.

Tested on these platforms:
1. GCC 6.2.1/Arch Linux
2. GCC 4.9.2/Fedora 21
3. AppleClang 8.0.0/Mac OS Sierra (v10.12)

C++14 is required.

## Installation

```
git clone https://github.com/aautushka/perg
mkdir perg/build
cd perg/build && cmake .. && make all
```

## Tests

The project was written with tests in mind. Builds with the Google Test framework.

Running tests:

```
git clone https://github.com/aautushka/perg
mkdir perg/build
cd perg/build && cmake .. 
make tests & &tests/tests
```
