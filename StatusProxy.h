// StatusProxy.h

#ifndef _JUST_PEER_WORKER_STATUS_PROXY_H_
#define _JUST_PEER_WORKER_STATUS_PROXY_H_

#include <framework/network/NetName.h>

namespace just
{
    namespace peer_worker
    {

        class StatusProxy
            : public util::daemon::ModuleBase<StatusProxy>
        {
        public:
            StatusProxy(
                util::daemon::Daemon & daemon);

            ~StatusProxy();

        private:
            virtual bool startup(
                boost::system::error_code & ec);

            virtual bool shutdown(
                boost::system::error_code & ec);

        private:
            class ProxyManager;
            class StatusSession;
            framework::network::NetName addr_;
            ProxyManager * mgr_;
        };

    } // namespace peer_worker
} // namespace just

#endif // _JUST_peer_worker_STATUS_PROXY_H_
