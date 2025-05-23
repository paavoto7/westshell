#include "shell_env.h"

#include <unistd.h>
#include <pwd.h>

ShellEnv::ShellEnv() {
    // Get required information about the user
    struct passwd* pw = getpwuid(geteuid());
    user = pw->pw_name;
    homeDir = pw->pw_dir;

    mainPid = getpid();

    // Needed to be done here as we need the home dir
    history = History(homeDir);
}
