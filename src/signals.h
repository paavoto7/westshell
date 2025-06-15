#ifndef SIGNALS_H
#define SIGNALS_H

#include <csignal>

namespace Signals {
    volatile inline std::sig_atomic_t signalStatus = 0;
    volatile inline std::sig_atomic_t childSignalStatus = 0;
    
    // Register a handler by sigaction. Default flag is SA_RESTART.
    void registerHandler(int sig, void (*handler)(int), int flags = SA_RESTART);
    
    inline void signalHandler(int signal) {
        signalStatus = signal;
    }

    inline void childSignalHandler(int signal) {
        childSignalStatus = signal;
    }
}

#endif // SIGNALS_H
