#include "signals.h"

#include <signal.h>

void Signals::registerHandler(int sig, void (*handler)(int), int flags) {
    struct sigaction sa {};
    sa.sa_handler = handler;
    sa.sa_flags = flags;
    sigemptyset(&sa.sa_mask);
    sigaction(sig, &sa, nullptr);
}
