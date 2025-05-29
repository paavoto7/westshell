// WestShell
#include <iostream>
#include <csignal>

#include "shell.h"
#include "signals.h"

// Creates shell object and calls its run method
int main() {
    signal(SIGINT, Signals::signal_handler);
    signal(SIGQUIT, Signals::signal_handler);

    Shell shell;
    int exitCode = shell.run();

    return exitCode;
}
