// WorkerModule.cpp

#include "just/peer_worker/Common.h"
#include "just/peer_worker/WorkerModule.h"
#include "just/peer_worker/ClientStatus.h"

#include <just/dac/DacInfoWorker.h>

#include <peer/Name.h>

#include <framework/process/Process.h>
#include <framework/filesystem/Path.h>
#include <framework/logger/Logger.h>
#include <framework/logger/StreamRecord.h>
#include <framework/system/LogicError.h>
using namespace framework::timer;

#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
using namespace boost::system;


FRAMEWORK_LOGGER_DECLARE_MODULE_LEVEL("just.peer_worker.WorkerModule", framework::logger::Debug);

#define PEER_BUFFER_SIZE 45

static just::peer_worker::WorkerModule* g_workerModule = NULL;
static void SubmitStopLog(boost::uint32_t n, std::string const & dac)
{
    g_workerModule->SubmitPeerLog(dac);
}

namespace just
{
    namespace peer_worker
    {

        WorkerModule::WorkerModule(
            util::daemon::Daemon & daemon)
            : just::common::CommonModuleBase<WorkerModule>(daemon, "WorkerModule")
#ifndef JUST_DISABLE_DAC
            , dac_(util::daemon::use_module<just::dac::DacModule>(daemon))
#endif
            , portMgr_(util::daemon::use_module<just::common::PortManager>(daemon))
            , port_(9000)
            , buffer_size_(PEER_BUFFER_SIZE)
            , timer_(io_svc())
            , needStartPeer_(false)
        #ifndef JUST_STATIC_BIND_PEER_LIB
            , needLoadPeer_(false)
            , peerLibName_("")
            , sym_name_("TS_XXXX")
        #endif
        {
            g_workerModule = this;
            daemon.config().register_module("WorkerModule")
            #ifndef JUST_STATIC_BIND_PEER_LIB
                << CONFIG_PARAM_NAME_RDWR("peer_lib_name", peerLibName_)
                << CONFIG_PARAM_NAME_RDWR("need_load_peer", needLoadPeer_)
                << CONFIG_PARAM_NAME_RDWR("sym_name", sym_name_)
            #endif
                << CONFIG_PARAM_NAME_RDWR("need_start_peer", needStartPeer_)
                << CONFIG_PARAM_NAME_RDWR("buffer_size", buffer_size_)
                << CONFIG_PARAM_NAME_RDWR("port", port_);
            LOG_DEBUG("[WorkerModule] port " << port_);
            memset(&ipeer_, 0, sizeof(ipeer_));
        }

        WorkerModule::~WorkerModule()
        {
        }

        bool WorkerModule::startup(
            error_code & ec)
        {
            if (!prepare_interface()){
                LOG_ERROR("[startup] prepare_interface failed");
                return false;
            }
            bool ret = start_peer();
            if (ret) {
                timer_.expires_from_now(Duration::seconds(1));
                timer_.async_wait(boost::bind(&WorkerModule::handle_timer, this, _1));
            }
            return ret;
        }

        void WorkerModule::SubmitPeerLog(
            std::string const & msg)
        {
#ifndef JUST_DISABLE_DAC
            dac_.submit(just::dac::DacPeerStatInfo(msg));
#endif
        }

        bool WorkerModule::shutdown(
            error_code & ec)
        {
            timer_.cancel(ec);
            stop_peer();
            return !ec;
        }

        void WorkerModule::handle_timer(
            boost::system::error_code const & ec)
        {
            if (!get_daemon().is_started())
                return;
            timer_.expires_from_now(Duration::seconds(1));
            timer_.async_wait(boost::bind(&WorkerModule::handle_timer, this, _1));
#ifndef JUST_CONTAIN_PEER_WORKER
            check_process();
#endif            
            update_stat();
        }

        bool WorkerModule::prepare_interface()
        {
        #ifdef JUST_STATIC_BIND_PEER_LIB
            TS_XXXX(&ipeer_);
            return true;
        #else
            if (!needLoadPeer_){
                return true;
            }

            if (needLoadPeer_ && peerLibName_.empty()){
                LOG_ERROR("[prepare_interface] shouldLoadPeer_ is true, but peerLibName_ is empty");
                return false;
            }
            error_code ec = lib_.open(peerLibName_);
            if (ec){
                LOG_ERROR("[prepare_interface] open " << peerLibName_ << ", failed, " << ec.message());
                return ec;
            }

            typedef void (PEER_API *LPTS_XXXX)(LPNETINTERFACE );
            LPTS_XXXX ts = (LPTS_XXXX)lib_.symbol(sym_name_);
            if (ts){
                ts(&ipeer_);
                LOG_DEBUG("[prepare_interface] find symbol " << sym_name_);
            } else {
                LOG_ERROR("[prepare_interface]Failed to find symbol: " << sym_name_ << ", peerLibName_ " << peerLibName_);
                return false;
            }

            return true;
        #endif
        }

        bool WorkerModule::start_peer()
        {
            if (!needStartPeer_){
                portMgr_.set_port(just::common::vod,port_);
                LOG_DEBUG("[start_peer] do not need to start, set port_ " << port_);
                return true;
            }

            STARTPARAM start_param;
            // start param
            //memset(&start_param, 0, sizeof(start_param));
            start_param.szDiskPath[0] = '\0';
            start_param.szConfigPath[0] = '\0';
            start_param.szPeerGuid[0] = '\0';
            start_param.uSize = sizeof(start_param);
            start_param.hWnd = 0;
            start_param.aIndexServer[0].cIndexServerType = 1; // udp
            start_param.aIndexServer[0].szIndexDomain[0] = '\0';
            start_param.bUseDisk = 1;
            start_param.usTcpPort = 0;
            start_param.submit_log = SubmitStopLog;
            start_param.bHttpProxyEnabled = 1;
            start_param.bReadOnly = 0;
            start_param.memory_pool_size_in_MB = buffer_size_;
            LOG_INFO("Config: --buffer_size:"<<(boost::uint32_t)buffer_size_);
            
            {
                std::string host = "192.168.43.98";
                host = "ppvabs.pplive.com";
                boost::uint16_t port = 6400;
                //
                strncpy(start_param.aIndexServer[0].szIndexDomain, host.c_str(), sizeof(start_param.aIndexServer[0].szIndexDomain)-1);
                start_param.aIndexServer[0].usIndexPort = port;
                LOG_INFO("Config: --bootstrap=" << host << ":" << port);
            }

            {
                //boost::uint16_t proxy_port = 80;//configs["proxy-port"].as<boost::uint16_t>();
                start_param.usHttpProxyPort = port_;
                LOG_INFO("Config: --http-port=" << port_);
            }

            {
                boost::uint16_t udp_port = 5829;//configs["udp-port"].as<boost::uint16_t>();
                start_param.usUdpPort = udp_port;
                LOG_INFO("Config: --udp-port=" << udp_port);
            }

            boost::filesystem::path ph_root = framework::filesystem::temp_path() / "vod";
            try {
                boost::filesystem::create_directories(ph_root);
            } catch (...) {
            }

            if (0)
            {
                boost::filesystem::create_directories(ph_root / "data");
                std::string data_dir = (ph_root / "data").string();
                size_t max_length = sizeof(start_param.szDiskPath) - 1;
                if (data_dir.length() >= max_length) {
                    LOG_INFO("data directory size too large, size = " << data_dir.length() << ", max = " << max_length);
                    return framework::system::logic_error::invalid_argument;
                }
                strncpy(start_param.szDiskPath, data_dir.c_str(), max_length);
                LOG_INFO("Config: --data-dir=" << data_dir);
            }

            {
                boost::filesystem::create_directories(ph_root / "config");
                std::string config_dir = (ph_root / "config").string();
                size_t max_length = sizeof(start_param.szConfigPath) - 1;
                if (config_dir.length() >= max_length) {
                    LOG_INFO("config directory size too large, size = " << config_dir.length() << ", max = " << max_length);
                    return framework::system::logic_error::invalid_argument;
                }
                strncpy(start_param.szConfigPath, config_dir.c_str(), max_length);
                LOG_INFO("Config: --config-dir=" << config_dir);
            }

            if (0)
            {
                std::string data_dir_limit = "2GB";//configs["data-dir-limit"].as<string>();
                boost::uint64_t data_limit = 0;
                start_param.ullDiskLimit = data_limit;
                LOG_INFO("Config: --data-dir-limit=" << data_limit);
            }

            {
                bool enable_push = false; //configs["enable-push"].as<bool>();
                start_param.bUsePush = enable_push;
                LOG_INFO("Config: --enable-push=" << enable_push);
            }

            // start up
            if (ipeer_.Startup != NULL) {
                port_ = ipeer_.Startup(&start_param);
            }
            portMgr_.set_port(just::common::vod,port_);

            return true;
        }

        void WorkerModule::stop_peer()
        {
            if (needStartPeer_) {
                if (ipeer_.Cleanup != NULL) {
                    // peer Startup had execute Clearup already
                    LOG_DEBUG("[stop_peer] beg");
                    ipeer_.Cleanup();
                    LOG_DEBUG("[stop_peer] end");
                }
            }

#ifndef JUST_STATIC_BIND_PEER_LIB
            lib_.close();
#endif
        }

        void WorkerModule::check_process()
        {
#ifndef JUST_CONTAIN_PEER_WORKER
#ifdef __APPLE__
            int id = framework::this_process::parent_id();
            if(1 == id)
             {
                LOG_ERROR("[check_process] Main Process is died");
                exit(0);
             }
#endif
#endif           
        }

        void WorkerModule::update_stat()
        {
            framework::container::List<just::peer_worker::ClientStatus> * stats = 
                (framework::container::List<just::peer_worker::ClientStatus> *)shared_memory().get_by_id(SHARED_OBJECT_ID_DEMUX);
            if (!stats) {
                LOG_ERROR("[update_stat] could not get ClientStatus");
                return;
            }

            just::peer_worker::ClientStatus::pointer stat;
            for (stat = stats->first(); &*stat; stat = stats->next(stat)) {
                std::string current_url = stat->current_url();
                size_t buffer_time = stat->buffer_time();
                LOG_TRACE("client_status: " << " " << current_url << " " << buffer_time);
                if (ipeer_.SetRestPlayTimeByUrl){
                    ipeer_.SetRestPlayTimeByUrl(current_url.c_str(), buffer_time);
                }
            }
        }

    } // namespace peer_worker
} // namespace just
