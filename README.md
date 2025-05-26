# WestShell
WestShell is a hobby and learning project aimed at deepening my understanding of
how shells work. I'm incrementally building it with more features over time.

## Getting started
### Requirements
- C++17
- Make

The project currently has no external dependencies beyond the C++ standard library and some parts of the C standard library. It uses `unistd.h` for system calls, so it requires a POSIX-compliant system.

### Building
```
git clone https://github.com/paavoto7/westshell.git
cd westshell
make
./westshell
```
⚠️ WestShell does not currently generate a settings file automatically.
[See instructions below.](#settings-file)

## Features
The project can currently run external programs with basic argument support.
Commands are parsed by splitting on spaces, so only simple commands (for example
`executable arg1 arg2 ...`) are supported.

### Builtins
Current builtin commands include:
- cd
- echo
- history
- hash

### Planned features
- Piping ( | )
- Quoted arguments ("like this")
- Input/output redirection
- History navigation (with arrow keys)
- Background processes

The code structure and organisation are very much subject to change.
My primary goal is to first to implement the core functionality and refine from there.

## Settings file
You can configure WestShell using a `.westshell.ini` file placed in your home directory.
Each line should follow the format:
`key=value`
- No spaces are allowed around the `=` sign
- he primary colour is set by *primaryColour* and secondary by *secondaryColour*.

Values must match the colour variable names specified in [`style.h`](src/style.h).

### Why C++?
While the program is largely procedural and could be written in C, I chose C++
because I enjoy the language and its many features over C. This project serves
as a way for me to learn more about both shells and modern C++.