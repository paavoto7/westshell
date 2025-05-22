# WestShell
WestShell is a hobby and learning project aimed at deepening my understanding of
how shells work. I'm incrementally building it with more features over time.

The code structure and organisation are very much subject to change.
My primary goal was to first get something up and running and refine from there.

## Features
WestShell can currently run external programs with basic argument support.
Commands are parsed by splitting on spaces, so only simple commands (for example
`executable arg1 arg2 ...`) are supported.

Some limitations (possible improvements):
- No support for piping (|)

- No support for quoted arguments ("like this")

- No input/output redirection

- No background processes

### Why C++?
While the program is largely procedural and could be written in C, I chose C++
because I enjoy the language and its many features over C. This project serves
as a way for me to learn more about both shells and modern C++.