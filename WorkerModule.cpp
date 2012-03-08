// WorkerModule.cpp

#include "ppbox/peer_worker/Common.h"
#include "ppbox/peer_worker/WorkerModule.h"

#include "ppbox/peer_worker/PPConfig.h"
//#include "ppbox/peer_worker/Utils.h"

#include <ppbox/demux/DemuxerModule.h>

#include <peer/Name.h>

#include <framework/filesystem/Path.h>
#include <framework/logger/LoggerStreamRecord.h>
#include <framework/system/LogicError.h>
using framework::logger::Logger;

#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
using namespace boost::system;



FRAMEWORK_LOGGER_DECLARE_MODULE_LEVEL("WorkerModule", 0);

namespace ppbox
{
    namespace peer_worker
    {

        WorkerModule::WorkerModule(
            util::daemon::Daemon & daemon)
            : ppbox::common::CommonModuleBase<WorkerModule>(daemon, "WorkerModule")
            , port_(9000)
            , timer_(io_svc())
        {
            memset(&ipeer_, 0, sizeof(ipeer_));
            daemon.config().register_module("vod_proxy")
                (MaxPeerConnection_,new MaxPeerConnection(*this))
                (MaxBufferMemory_,new MaxBufferMemory(*this))
                (PreAllocSubPieceCount_,new PreAllocSubPieceCount(*this))
                (EnableUdpChecksum_,new EnableUdpChecksum(*this))
                (MaxPeerTunnel_,new MaxPeerTunnel(*this))
                (MaxPeerConnectorCount_,new MaxPeerConnectorCount(*this));
        }

        WorkerModule::~WorkerModule()
        {
        }

        error_code WorkerModule::startup()
        {
            error_code ec = start_peer();
            if (!ec) {
                timer_.expires_from_now(Duration::seconds(1));
                timer_.async_wait(boost::bind(&WorkerModule::handle_timer, this, _1));
            }
            return ec;
        }

		
		boost::system::error_code WorkerModule::Set(const char* pKey,const int i)
		{
			std::cout<<"WorkerModule:"<<pKey<<" "<<i<<std::endl;
			return boost::system::error_code();
		}


        void WorkerModule::shutdown()
        {
            boost::system::error_code ec;
            timer_.cancel(ec);
            stop_peer();
        }

        void WorkerModule::handle_timer(
            boost::system::error_code const & ec)
        {
            if (!get_daemon().is_started())
                return;
            timer_.expires_from_now(Duration::seconds(1));
            timer_.async_wait(boost::bind(&WorkerModule::handle_timer, this, _1));
            update_stat();
        }

        error_code WorkerModule::start_peer()
        {
            STARTPARAM start_param;

            boost::filesystem::path exe_dir = framework::filesystem::bin_file().remove_leaf();
            std::string log_path = (exe_dir / "peer.log").file_string();
            //freopen(log_path.c_str(), "w", stderr);
#ifdef PPBOX_STATIC_BIND_PEER_LIB
			error_code ec;
			TS_XXXX(&ipeer_);		
#else
            error_code ec = 
                lib_.open(::peer::name_string());
            if (ec)
                return ec;

            typedef void (PEER_API * LPTS_XXXX)(LPNETINTERFACE );
            LPTS_XXXX ts = (LPTS_XXXX)lib_.symbol("TS_XXXX");
            if (ts) {
                ts(&ipeer_);
            } else {
                LOG_S(Logger::kLevelAlarm, "find symbol TS_XXXX failed");
                return framework::system::logic_error::failed_some;
            }
#endif
            // start param
            //memset(&start_param, 0, sizeof(start_param));
            start_param.szDiskPath[0] = '\0';
            start_param.szConfigPath[0] = '\0';
            start_param.szTestDomain[0] = '\0';
            start_param.szPeerGuid[0] = '\0';
            start_param.uSize = sizeof(start_param);
            start_param.hWnd = 0;
            start_param.aIndexServer[0].cIndexServerType = 1; // udp
            start_param.aIndexServer[0].szIndexDomain[0] = '\0';
            start_param.bUseDisk = 1;
            start_param.usTcpPort = 0;
            start_param.bIsTestCore = 0;
            start_param.bHttpProxyEnabled = 1;
            start_param.bReadOnly = 0;
            start_param.cPeerCatalog = PCAT_PPNX;

            {
                std::string host = "192.168.43.98";
                host = "ppvabs.pplive.com";
                boost::uint16_t port = 6400;
                //
                strncpy(start_param.aIndexServer[0].szIndexDomain, host.c_str(), sizeof(start_param.aIndexServer[0].szIndexDomain)-1);
                start_param.aIndexServer[0].usIndexPort = port;
                LOG_S(Logger::kLevelInfor, "Config: --bootstrap=" << host << ":" << port);
            }

            {
                //boost::uint16_t proxy_port = 80;//configs["proxy-port"].as<boost::uint16_t>();
                start_param.usHttpProxyPort = port_;
                LOG_S(Logger::kLevelInfor, "Config: --http-port=" << port_);
            }

            {
                boost::uint16_t udp_port = 5829;//configs["udp-port"].as<boost::uint16_t>();
                start_param.usUdpPort = udp_port;
                LOG_S(Logger::kLevelInfor, "Config: --udp-port=" << udp_port);
            }

            boost::filesystem::path ph_root = framework::filesystem::temp_path() / "vod";
            try {
                boost::filesystem::create_directories(ph_root);
            } catch (...) {
            }

            if (0)
            {
                boost::filesystem::create_directories(ph_root / "data");
                std::string data_dir = (ph_root / "data").file_string();
                size_t max_length = sizeof(start_param.szDiskPath) - 1;
                if (data_dir.length() >= max_length) {
                    LOG_S(Logger::kLevelInfor, "data directory size too large, size = " << data_dir.length() << ", max = " << max_length);
                    return framework::system::logic_error::invalid_argument;
                }
                strncpy(start_param.szDiskPath, data_dir.c_str(), max_length);
                LOG_S(Logger::kLevelInfor, "Config: --data-dir=" << data_dir);
            }

            {
                boost::filesystem::create_directories(ph_root / "config");
                std::string config_dir = (ph_root / "config").file_string();
                size_t max_length = sizeof(start_param.szConfigPath) - 1;
                if (config_dir.length() >= max_length) {
                    LOG_S(Logger::kLevelInfor, "config directory size too large, size = " << config_dir.length() << ", max = " << max_length);
                    return framework::system::logic_error::invalid_argument;
                }
                strncpy(start_param.szConfigPath, config_dir.c_str(), max_length);
                LOG_S(Logger::kLevelInfor, "Config: --config-dir=" << config_dir);
            }

            if (0)
            {
                std::string data_dir_limit = "2GB";//configs["data-dir-limit"].as<string>();
                boost::uint64_t data_limit = 0;
                start_param.ullDiskLimit = data_limit;
                LOG_S(Logger::kLevelInfor, "Config: --data-dir-limit=" << data_limit);
            }

            {
                bool enable_cache = false; //configs["enable-cache"].as<bool>();
                start_param.bUseCache = enable_cache;
                LOG_S(Logger::kLevelInfor, "Config: --enable-cache=" << enable_cache);
            }

            {
                bool enable_push = false; //configs["enable-push"].as<bool>();
                start_param.bUsePush = enable_push;
                LOG_S(Logger::kLevelInfor, "Config: --enable-push=" << enable_push);
            }

            // start up
            if (ipeer_.Startup != NULL) {
                ipeer_.Startup(&start_param);
            }

            return ec;
        }

        void WorkerModule::stop_peer()
        {
            if (ipeer_.Cleanup != NULL) {
                // peer Startup had execute Clearup already
                LOG_S(Logger::kLevelDebug, "[stop_peer] beg");
                ipeer_.Cleanup();
                LOG_S(Logger::kLevelDebug, "[stop_peer] end");
            }
#ifndef PPBOX_STATIC_BIND_PEER_LIB
            lib_.close();
#endif
        }

        void WorkerModule::update_stat()
        {
            framework::container::List<ppbox::demux::SharedStatistics> * stats = 
                (framework::container::List<ppbox::demux::SharedStatistics> *)shared_memory().get_by_id(SHARED_OBJECT_ID_DEMUX);
            if (!stats)
                return;

            char const * const status_str[] = {
                "stopped", 
                "opening", 
                "opened", 
                "paused",
                "playing", 
                "buffering"
            };

            ppbox::demux::SharedStatistics::pointer stat;
            for (stat = stats->first(); &*stat; stat = stats->next(stat)) {
                ppbox::demux::DemuxerStatistic const & demux_stat = stat->demux_stat;
                if (demux_stat.get_play_type() == ppbox::demux::DemuxerType::vod
                    && demux_stat.is_playing_state()
                    && demux_stat.demux_data().rid != NULL
                    && strlen(demux_stat.demux_data().rid) > 0) {
                        LOG_S(Logger::kLevelDebug2, "demux_stat: " 
                            << " " << demux_stat.demux_data().rid 
                            << " " << status_str[demux_stat.state() < 5 ? demux_stat.state() : 5] 
                            << " " << demux_stat.get_buf_time());
                        ipeer_.SetRestPlayTime(demux_stat.demux_data().rid, strlen(demux_stat.demux_data().rid), demux_stat.get_buf_time());
                }
            }
        }

    } // namespace peer_worker
} // namespace ppbox
