
# Concerto Graphics

Concerto Graphics is a 3D renderer that uses the Vulkan graphics API.

It is part of the Concerto suite of software components, which also includes Concerto Core, Concerto Engine, and Concerto Editor.

It is essentially developed for learning purposes.

## Dependencies

Concerto Graphics depends on Concerto Core, which provides a set of common functionality used by all of the components in the Concerto suite.
## Installation

Install [xmake](https://xmake.io/#/)

Clone this repo 

```bash
git clone git@github.com:ArthurVasseur/ConcertoGraphics.git
```

## Build

If you want to build a debug version of the project : 

```bash
xmake config -m debug
```

or

```bash
xmake config -m release
```

Build the project with `xmake`.

```bash
xmake
```

Creating cmake project files.

```bash
xmake project -k cmakelists
```

Creating Visual Studio project files.

```bash
xmake project -k vsxmake
