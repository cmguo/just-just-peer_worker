// Main.cpp

#include "just/peer_worker/Common.h"
#include "just/peer_worker/Version.h"
#include "just/peer_worker/WorkerModule.h"
#include "just/peer_worker/StatusProxy.h"
#include "just/peer_worker/Interface.h"

//#include <just/common/ConfigMgr.h>
#include <just/common/Debuger.h>
#include <just/common/PortManager.h>

#include <framework/logger/Logger.h>
#include <framework/process/Process.h>
#include <framework/process/SignalHandler.h>

#include <boost/bind.hpp>

#ifdef BOOST_POSIX_API
#include <unistd.h>
#include <signal.h>
#endif

namespace just
{
    namespace peer_worker
    {

        void register_module(util::daemon::Daemon & daemon)
        {
            //util::daemon::use_module<just::common::ConfigMgr>(daemon);
            util::daemon::use_module<just::common::Debuger>(daemon);
            util::daemon::use_module<just::common::PortManager>(daemon);
            util::daemon::use_module<just::peer_worker::WorkerModule>(daemon);
            util::daemon::use_module<just::peer_worker::StatusProxy>(daemon);
        }

    }
}

#ifndef _LIB

int main(int argc, char * argv[])
{
    util::daemon::Daemon my_daemon("peer_worker.conf");
    char const * default_argv[] = {
        "++framework::logger::Stream.0.file=$LOG/peer_worker.log", 
        "++framework::logger::Stream.0.append=true", 
        "++framework::logger::Stream.0.roll=true", 
        "++framework::logger::Stream.0.level=5", 
        "++framework::logger::Stream.0.size=102400", 
    };
    my_daemon.parse_cmdline(sizeof(default_argv) / sizeof(default_argv[0]), default_argv);
    my_daemon.parse_cmdline(argc, (char const **)argv);

    framework::process::SignalHandler sig_handler(
        framework::process::Signal::sig_int, 
        boost::bind(&util::daemon::Daemon::post_stop, &my_daemon), true);

    framework::logger::load_config(my_daemon.config());

    just::common::log_versions();

    just::common::CommonModule & common = 
        util::daemon::use_module<just::common::CommonModule>(my_daemon, "VodWorker");
    common.set_version(just::peer_worker::version());

    just::peer_worker::register_module(my_daemon);

    boost::system::error_code ec;
    my_daemon.start(framework::process::notify_wait, ec);

    return 0;
}

#endif
