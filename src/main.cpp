#include <userver/utils/daemon_run.hpp>

#include "components.hpp"

int main(int argc, char* argv[]) {
    return userver::utils::DaemonMain(argc, argv, MakeComponents());
}
