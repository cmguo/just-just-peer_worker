// SharedStatus.h

#ifndef _JUST_PEER_WORKER_SHARED_STATUS_H_
#define _JUST_PEER_WORKER_SHARED_STATUS_H_

namespace just
{
    namespace peer_worker
    {

        class SharedStatus
        {
        public:
            struct Block
            {
                void const * ptr;
                boost::uint32_t size;
                template <typename Archive>
                void serialize(Archive & ar)
                {
                    ar & size;
                }
            };

        public:
            SharedStatus();

            ~SharedStatus();

        public:
            boost::system::error_code get_memory_blocks(
                std::map<std::string, Block> & blocks);

        private:
            struct Cache;
            Cache * cache_;
        };

    } // namespace peer_worker
} // namespace just

#endif // _JUST_peer_worker_SHARED_STATUS_H_
