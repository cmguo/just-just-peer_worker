// WorkerModule.h

#ifndef _PPBOX_PEER_WORKER_WORKER_MODULE_H_
#define _PPBOX_PEER_WORKER_WORKER_MODULE_H_

#include <peer/peer/Interface.h>

#include <framework/library/Library.h>

#include <framework/timer/TimeTraits.h>

namespace ppbox
{
    namespace peer_worker
    {

        class WorkerModule
            : public ppbox::common::CommonModuleBase<WorkerModule>
        {
        public:
            WorkerModule(
                util::daemon::Daemon & daemon);

            virtual ~WorkerModule();

        public:
            virtual boost::system::error_code startup();

            virtual void shutdown();


		public:
			boost::system::error_code Set(const char* ,const int);
        private:
            void handle_timer(
                boost::system::error_code const & ec);

        private:
            boost::system::error_code start_peer();

            void stop_peer();

            void update_stat();

        private:
            boost::uint16_t port_;

        private:
            clock_timer timer_;
#ifndef PPBOX_STATIC_BIND_PEER_LIB			
            framework::library::Library lib_;
#endif
            NETINTERFACE ipeer_;
        };

    } // namespace peer_worker
} // namespace ppbox

#endif // _PPBOX_peer_worker_WORKER_MODULE_H_
