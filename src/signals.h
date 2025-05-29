#ifndef SIGNALS_H
#define SIGNALS_H

#include <iostream>
#include <csignal>

namespace Signals {
    volatile inline std::sig_atomic_t signalStatus;
    
    inline void signal_handler(int signal) {
        signalStatus = signal;
    }
}

#endif // SIGNALS_H
