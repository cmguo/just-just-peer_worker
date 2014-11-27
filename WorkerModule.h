// WorkerModule.h

#ifndef _JUST_PEER_WORKER_WORKER_MODULE_H_
#define _JUST_PEER_WORKER_WORKER_MODULE_H_

#ifndef JUST_DISABLE_DAC
#include <just/dac/DacModule.h>
#endif

#include <just/common/PortManager.h>

#include <peer/Interface.h>

#include <framework/library/Library.h>
#include <framework/timer/TimeTraits.h>

namespace just
{
    namespace peer_worker
    {

        class WorkerModule
            : public just::common::CommonModuleBase<WorkerModule>
        {
        public:
            WorkerModule(
                util::daemon::Daemon & daemon);

            virtual ~WorkerModule();

        public:
            virtual boost::system::error_code startup();

            virtual void shutdown();


        public:
            void SubmitPeerLog(std::string const & dac);
            boost::system::error_code Set(const char* ,const int);
        private:
            void handle_timer(
                boost::system::error_code const & ec);

        private:
            boost::system::error_code start_peer();

            void stop_peer();

            void update_stat();
            
            void check_process();

        private:
#ifndef JUST_DISABLE_DAC
            just::dac::DacModule& dac_;
#endif
            just::common::PortManager& portMgr_;
            boost::uint16_t port_;
            boost::uint8_t buffer_size_;
        private:
            clock_timer timer_;
#ifndef JUST_STATIC_BIND_PEER_LIB
            framework::library::Library lib_;
#endif
            NETINTERFACE ipeer_;
            
        };

    } // namespace peer_worker
} // namespace just

#endif // _JUST_peer_worker_WORKER_MODULE_H_
