// WestShell

#include "shell.h"
#include "signals.h"

// Creates shell object and calls its run method
int main() {
    // Sets up SIGCHILD, SIGINT and SIG
    Signals::registerHandler(SIGCHLD, Signals::childSignalHandler, SA_RESTART | SA_NOCLDSTOP);
    Signals::registerHandler(SIGINT, Signals::signalHandler);
    Signals::registerHandler(SIGQUIT, Signals::signalHandler);

    Shell shell;
    int exitCode = shell.run();

    return exitCode;
}
