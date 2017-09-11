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
            virtual bool startup(
                boost::system::error_code & ec);

            virtual bool shutdown(
                boost::system::error_code & ec);

        public:
            void SubmitPeerLog(std::string const & dac);
            boost::system::error_code Set(const char* ,const int);
        private:
            void handle_timer(
                boost::system::error_code const & ec);

        private:
            bool prepare_interface();
            bool start_peer();

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
            bool needStartPeer_;
#ifndef JUST_STATIC_BIND_PEER_LIB
            framework::library::Library lib_;
            // whether to load peer so
            bool needLoadPeer_;
            // peer so name, only used when shouldLoadPeer_ is true
            std::string peerLibName_;
            // peer symbol name, used when shouldLoadPeer_ is true and peerLibName_ not empty
            std::string sym_name_;
#endif
            NETINTERFACE ipeer_;
            
        };

    } // namespace peer_worker
} // namespace just

#endif // _JUST_peer_worker_WORKER_MODULE_H_
