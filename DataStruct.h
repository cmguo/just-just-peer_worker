// DataStruct.h

#ifndef _JUST_AGENT_DATA_STRUCT_H_
#define _JUST_AGENT_DATA_STRUCT_H_

#pragma pack(push, 1)

typedef struct _UUID {
    boost::uint32_t  Data1;
    boost::uint16_t Data2;
    boost::uint16_t Data3;
    boost::uint8_t  Data4[8];
} UUID;

#include <string.h>

typedef UUID RID;

const boost::uint16_t BITMAP_SIZE = 50;

const boost::uint16_t UINT8_MAX_VALUE = 256;

const boost::uint16_t LIVEDOWNLOADER_MAX_COUNT = 64;

const boost::uint16_t MAX_P2P_DOWNLOADER_COUNT = 100;


const boost::uint16_t MAX_IP_COUNT = 10;

//////////////////////////////////////////////////////////////////////////
// �����ṹ

struct SPEED_INFO
{
    boost::uint32_t StartTime;               // ��ʼʱ��
    boost::uint32_t TotalDownloadBytes;      // �ܹ����ص��ֽ���
    boost::uint32_t TotalUploadBytes;        // �ܹ��ϴ�ʱ�ֽ���
    boost::uint32_t NowDownloadSpeed;        // ��ǰ�����ٶ� <5s>
    boost::uint32_t NowUploadSpeed;          // ��ǰ�ϴ��ٶ� <5s>
    boost::uint32_t MinuteDownloadSpeed;     // ���һ����ƽ�������ٶ� <60s>
    boost::uint32_t MinuteUploadSpeed;       // ���һ����ƽ���ϴ��ٶ� <60s>
    boost::uint32_t AvgDownloadSpeed;        // ��ʷƽ�������ٶ�
    boost::uint32_t AvgUploadSpeed;          // ��ʷƽ���ϴ��ٶ�

    SPEED_INFO()
    {
        memset(this, 0, sizeof(SPEED_INFO));
    }
};

struct SPEED_INFO_EX : SPEED_INFO
{
    boost::uint32_t RecentDownloadSpeed;     // ��ǰ�����ٶ� <20s>
    boost::uint32_t RecentUploadSpeed;       // ��ǰ�ϴ��ٶ� <20s>
    boost::uint32_t SecondDownloadSpeed;     // ��ǰ1s�������ٶ�
    boost::uint32_t SecondUploadSpeed;       // ��ǰ1s���ϴ��ٶ�

    SPEED_INFO_EX()
    {
        memset(this, 0, sizeof(SPEED_INFO_EX));
    }
};

/*
struct CANDIDATE_PEER_INFO
{
    boost::uint32_t IP;                      // �Լ�����IP
    boost::uint16_t UdpPort;                 // �Լ��ı���UDP Port
    boost::uint16_t TcpPort;                 // �Լ��ı���TCP Port
    boost::uint32_t DetectIP;                // �Լ��ӷ�������õ�̽�� IP
    boost::uint16_t DetectUdpPort;           // �Լ��ӷ�������õ�̽�� UDP Port
    boost::uint32_t StunIP;                  // �Լ�ѡ��� Stun Peer ��IP
    boost::uint16_t StunUdpPort;             // �Լ�ѡ��� Stun Peer �� Port
    boost::uint8_t  Reversed[4];
};
*/
//typedef CANDIDATE_PEER_INFO CANDIDATE_PEER_INFO;

/*
struct TRACKER_INFO
{
    boost::uint16_t Length;
    boost::uint8_t ModNo;
    boost::uint32_t IP;
    boost::uint16_t Port;
    boost::uint8_t Type;
}
*/

struct TRACKER_INFO
{
    boost::uint16_t Length;
    boost::uint8_t ModNo;
    boost::uint32_t IP;
    boost::uint16_t Port;
    boost::uint8_t Type; // 1 UDP

    bool operator == (const TRACKER_INFO& n) const
    {
        return 0 == memcmp(this, &n, sizeof(TRACKER_INFO));
    }
    bool operator < (const TRACKER_INFO& n) const
    {
        return memcmp(this, &n, sizeof(TRACKER_INFO)) < 0;
    }
    TRACKER_INFO& operator = (const TRACKER_INFO& tracker)
    {
        if (this != &tracker)
        {
            Length = tracker.Length;
            ModNo = tracker.ModNo;
            IP = tracker.IP;
            Port = tracker.Port;
            Type = tracker.Type;
        }
        return *this;
    }
};

struct STATISTIC_TRACKER_INFO
{
    // ��̬
    TRACKER_INFO TrackerInfo; // ��Tracker�Ļ�����Ϣ

    // ��̬��Ϣ
    boost::uint16_t CommitRequestCount;       // ��Tracker Commit ���������
    boost::uint16_t CommitResponseCount;      // ��Tracker Commit ����Ӧ����
    boost::uint16_t KeepAliveRequestCount;    // ��Tracker KeepAlive ���������
    boost::uint16_t KeepAliveResponseCount;   // ��Tracker KeepAlive ����Ӧ����
    boost::uint16_t ListRequestCount;         // ��Tracker List ���������
    boost::uint16_t ListResponseCount;        // ��Tracker List ����Ӧ����
    boost::uint8_t  LastListReturnPeerCount;  // ��һ�γɹ���List�ķ��ص�Peer��
    boost::uint8_t  IsSubmitTracker;          // �ڸ�Group���Ƿ�Ϊ��ǰѡ��Tracker
    boost::uint8_t  ErrorCode;                // �ϴ�Tracker���صĴ�����
    boost::uint16_t KeepAliveInterval;        // ��һ�δӷ��������ص�Submit���

    STATISTIC_TRACKER_INFO()
    {
        Clear();
    }
    STATISTIC_TRACKER_INFO(const TRACKER_INFO & tracker_info)
    {
        Clear();
        TrackerInfo = tracker_info;
    }
    void Clear()
    {
        memset(this, 0, sizeof(STATISTIC_TRACKER_INFO));
    }
};

struct STATISTIC_INDEX_INFO
{
    boost::uint32_t IP;
    boost::uint16_t Port;
    boost::uint8_t  Type;
    boost::uint16_t QueryRIDByUrlRequestCount;
    boost::uint16_t QueryRIDByUrlResponseCount;
    boost::uint16_t QueryHttpServersByRIDRequestCount;
    boost::uint16_t QueryHttpServersByRIDResponseCount;
    boost::uint16_t QueryTrackerListRequestCount;
    boost::uint16_t QureyTrackerListResponseCount;
    boost::uint16_t AddUrlRIDRequestCount;
    boost::uint16_t AddUrlRIDResponseCount;
};

//struct PEER_DOWNLOAD_INFO
//{
//    boost::uint8_t  IsDownloading;
//    boost::uint32_t OnlineTime;
//    boost::uint16_t AvgDownload;
//    boost::uint16_t NowDownload;
//    boost::uint16_t AvgUpload;
//    boost::uint16_t NowUpload;
//};

struct PEER_INFO
{
    boost::uint8_t download_connected_count_;
    boost::uint8_t upload_connected_count_;
    boost::uint32_t mine_upload_speed_;
    boost::uint32_t max_upload_speed_;
    boost::uint32_t rest_playable_time_;
    boost::uint8_t lost_rate_;
    boost::uint8_t redundancy_rate_;
};

struct PIECE_INFO_EX
{
    boost::uint16_t BlockIndex;
    boost::uint16_t PieceIndexInBlock;
    boost::uint16_t SubPieceIndexInPiece;

    PIECE_INFO_EX()
    {
        memset(this, 0, sizeof(PIECE_INFO_EX));
    }

    PIECE_INFO_EX(
        boost::uint16_t block_index, 
        boost::uint16_t piece_index_in_block, 
        boost::uint16_t sub_piece_index_in_piece)
        : BlockIndex(block_index)
        , PieceIndexInBlock(piece_index_in_block)
        , SubPieceIndexInPiece(sub_piece_index_in_piece)
    {
    }
};

struct CANDIDATE_PEER_INFO
{
    boost::uint32_t IP;
    boost::uint16_t UdpPort;
    boost::uint16_t TcpPort;
    boost::uint32_t DetectIP;
    boost::uint16_t DetectUdpPort;
    boost::uint32_t StunIP;
    boost::uint16_t StunUdpPort;
    boost::uint8_t  PeerNatType;
    boost::uint8_t  UploadPriority;
    boost::uint8_t  IdleTimeInMins;
    boost::uint8_t  Reversed[1];

    bool operator ==(const CANDIDATE_PEER_INFO& n) const
    {
        return 0 == memcmp(this, &n, sizeof(CANDIDATE_PEER_INFO));
    }

    /*
    bool operator < (const CANDIDATE_PEER_INFO& n) const
    {
        return true;
    }*/

    CANDIDATE_PEER_INFO()
    {
        IP = 0;
        UdpPort = 0;
        TcpPort = 0;
        DetectIP = 0;
        DetectUdpPort = 0;
        StunIP = 0;
        StunUdpPort = 0;
        PeerNatType = -1;
        UploadPriority = 0;
        IdleTimeInMins = 0;
        Reversed[0] = 0;
    }
};

//////////////////////////////////////////////////////////////////////////
// AppStopʱͳ���ϴ�

//////////////////////////////////////////////////////////////////////////
// StatisticModule �ṹ
//   �����ڴ���: PPVIDEO_<PID>
struct STASTISTIC_INFO
{
    // �ٶ������Ϣ
    SPEED_INFO SpeedInfo;                    // ���� �ֽ��� �� �ٶ�

    // IP �����Ϣ
    CANDIDATE_PEER_INFO LocalPeerInfo;      // �Լ� �Ļ���IP�˿���Ϣ
    boost::uint8_t  LocalIpCount;           // ����ͨ��API ��õı���IP�� 
    boost::uint32_t LocalIPs[MAX_IP_COUNT]; // ����ͨ��API ��õ�IP; (�������)

    // �Լ����������Ϣ
    boost::uint32_t LocalPeerVersion;       // �Լ��ں˰汾��
    boost::uint8_t  LocalPeerType;          // �Լ���Peer����

    // TrackerServer�����Ϣ
    boost::uint8_t  TrackerCount;           // Tracker�ĸ���
    boost::uint8_t  GroupCount;             // Group ��, Ҳ��MOD�ĳ���
    STATISTIC_TRACKER_INFO TrackerInfos[UINT8_MAX_VALUE];    // Tracker, (�������)

    // IndexServer �����Ϣ
    STATISTIC_INDEX_INFO StatisticIndexInfo;    // IndexServer�������Ϣ

    // P2PDownloader �����Ϣ
    boost::uint8_t  P2PDownloaderCount;         // �ж��ٸ�����P2P���ص���Դ
    RID    P2PDownloaderRIDs[UINT8_MAX_VALUE];  // ����P2P���ص���ԴRID; ���Ϊ GUID_NULL ��ʾ��; (������)

    // DownloadDriver �����Ϣ
    boost::uint8_t  DownloadDriverCount;                // �������ص���Ƶ
    boost::uint32_t DownloadDriverIDs[UINT8_MAX_VALUE]; // �������ص���Ƶ ������ID; ���Ϊ 0 ��ʾ������; (������)

    //ֹͣʱ�����ϴ����
    boost::uint32_t TotalP2PDownloadBytes;
    boost::uint32_t TotalOtherServerDownloadBytes;

    //������ �������� ʵʱ�����Ϣ
    boost::uint32_t TotalHttpNotOriginalDataBytes;      // ʵʱ ���صĴ����� �ֽ���
    boost::uint32_t TotalP2PDataBytes;                  // ʵʱ P2P���صĴ����� �ֽ���
    boost::uint32_t TotalHttpOriginalDataBytes;         // ʵʱ ԭ�����صĴ����� �ֽ���
    boost::uint32_t CompleteCount;                      // �Ѿ���ɵ�DD��

    boost::uint32_t TotalUploadCacheRequestCount;       // �ܹ����ϴ�Cache������
    boost::uint32_t TotalUploadCacheHitCount;           // �ܹ����ϴ�Cache������

    boost::uint16_t HttpProxyPort;                      // HTTPʵ�ʴ���˿�
    boost::uint32_t MaxHttpDownloadSpeed;               // HTTP��������ٶ�
    boost::uint16_t IncomingPeersCount;                 // �ܹ������Peer����
    boost::uint16_t DownloadDurationInSec;              // �����ܹ�����ʱ��(��)
    boost::uint32_t BandWidth;                          // Ԥ��Ĵ���

    boost::uint32_t  GlobalWindowSize;

    boost::uint16_t  GlobalRequestSendCount;             // ÿ�뷢����������

    boost::uint16_t  MemoryPoolLeftSize;                     // �ڴ��ʣ���С

    // ͳ��BHO��FlOAT�ĵ������
    // protocol::ACTION_COUNT_INFO ActionCountInfo;

    // LiveDownloadDriver �����Ϣ
    boost::uint8_t  LiveDownloadDriverCount;               // �������ص�ֱ����Ƶ
    boost::uint32_t LiveDownloadDriverIDs[LIVEDOWNLOADER_MAX_COUNT];// �������ص�ֱ����Ƶ ������ID; ���Ϊ 0 ��ʾ������; (������)

    boost::uint8_t   Resersed[945 - 4 * LIVEDOWNLOADER_MAX_COUNT];                      // �����ֶ�
};

//////////////////////////////////////////////////////////////////////////
// P2PDownloaderStatistic �ṹ
//   �����ڴ���: P2PDOWNLOADER_<PID>_<RID>
//   NOTE: RID��ʽ{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}
struct PEER_DOWNLOAD_INFO
{
    boost::uint8_t IsDownloading;
    boost::uint32_t OnlineTime;
    boost::uint16_t AvgDownload;
    boost::uint16_t NowDownload;
    boost::uint16_t AvgUpload;
    boost::uint16_t NowUpload;

    bool operator ==(const PEER_DOWNLOAD_INFO& n) const
    {
        return 0 == memcmp(this, &n, sizeof(PEER_DOWNLOAD_INFO));
    }

    void operator =(const PEER_DOWNLOAD_INFO& n) 
    {
        IsDownloading = n.IsDownloading;
        OnlineTime = n.OnlineTime;
        AvgDownload = n.AvgDownload;
        NowDownload = n.NowDownload;
        AvgUpload = n.AvgUpload;
        NowUpload = n.NowUpload;
    }

    PEER_DOWNLOAD_INFO()
    {
        IsDownloading = 0;
        OnlineTime = 0;
        AvgDownload = 0;
        NowDownload = 0;
        AvgUpload = 0;
        NowUpload = 0;
    }
};

struct P2P_CONNECTION_INFO
{
    UUID PeerGuid;                          // ��Peer��PeerGuid
    SPEED_INFO SpeedInfo;                   // ��Peer ���ٶ���Ϣ
    boost::uint32_t PeerVersion;            // �Է�Peer���ں˰汾��
    boost::uint8_t  PeerType;               // �Է�Peer���ں�����
    CANDIDATE_PEER_INFO PeerInfo;           // ��Peer�� IP��Ϣ
    PEER_DOWNLOAD_INFO PeerDownloadInfo;    // �Է������ ���� ��Ϣ
    boost::uint8_t  BitMap[BITMAP_SIZE];    // �Է���BitMap
    boost::uint32_t RTT_Count;              // Udp���ĸ���
    boost::uint16_t RTT_Now;                // ��ǰ��rtt
    boost::uint16_t RTT_Average;            // �ܹ���ƽ��rtt; �ɻ�������udp�����յ�ʱ��ֲ�ͼ
    boost::uint16_t RTT_Max;                // �յ���udp����rtt���
    boost::uint32_t RTT_Total;              // �ܹ���rtt
    boost::uint16_t ElapseTime;             // ��ǰ��ʱʱ��
    boost::uint8_t  WindowSize;             // ���ڴ�С
    boost::uint8_t  AssignedSubPieceCount;  // ��ǰԤ����SubPiece��
    boost::uint32_t AverageDeltaTime;       // Average Delta Time
    boost::uint32_t SortedValue;            // Sorted Value

    //////////////////////////////////////////////////////////////////////////
    boost::uint8_t  IsRidInfoValid;         // 0-Invalid; 1-Valid
    boost::uint16_t Sent_Count;
    boost::uint16_t Requesting_Count;
    boost::uint16_t Received_Count; 
    boost::uint8_t  Reserved[200-7];        // �����ֶ�

    P2P_CONNECTION_INFO()
    {
        Clear();
    }

    void Clear()
    {
        memset(this, 0, sizeof(P2P_CONNECTION_INFO));
    }
};

struct P2PDOWNLOADER_STATISTIC_INFO
{
    RID ResourceID;                 //��Ӧ��RID
    SPEED_INFO SpeedInfo;

    // ��Դ�����Ϣ
    boost::uint32_t FileLength;
    boost::uint16_t BlockNum;
    boost::uint16_t BlockSize;

    // IPPool��Ϣ
    boost::uint16_t IpPoolPeerCount;        // ��ѡIP    
    boost::uint8_t  ExchangingPeerCount;    // ���ڽ�����Ϣ��IP    
    boost::uint8_t  ConnectingPeerCount;    // �������ӵ�IP     

    // �㷨�����Ϣ
    boost::uint16_t TotalWindowSize;                // �ܴ��ڴ�С
    boost::uint16_t TotalAssignedSubPieceCount;     // ��ǰ��������SubPiece��
    boost::uint16_t TotalUnusedSubPieceCount_;      // �����Subpieces��
    boost::uint16_t TotalRecievedSubPieceCount_;    // �յ���Subpiece��
    boost::uint16_t TotalRequestSubPieceCount_;     // ������Subpiece������
    boost::uint16_t SubPieceRetryRate;              // ������: ���� / �յ�
    boost::uint16_t UDPLostRate;                    // ������: (���� - �յ�) / ����

    boost::uint32_t TotalP2PDataBytes;              // ��ǰP2PDownloader���ص���Ч�ֽ���
    boost::uint16_t FullBlockPeerCount;             // ���Ѿ����ӵ�Peer�У���Դȫ����Peer����

    boost::uint32_t TotalUnusedSubPieceCount;       // �����Subpieces��
    boost::uint32_t TotalRecievedSubPieceCount;     // �յ���Subpiece��
    boost::uint32_t TotalRequestSubPieceCount;      // ������Subpiece������
    boost::uint32_t NonConsistentSize;              // ��ǰ���ص����һ��piece�͵�һ��piece֮��ľ���

    boost::uint8_t Reserved[978];                   // ����

    boost::uint16_t PeerCount;                      // Peer��
    P2P_CONNECTION_INFO P2PConnections[];           // �䳤; (�������)
};

//////////////////////////////////////////////////////////////////////////
// DownloadDriverStatistic �ṹ
//   �����ڴ����� DOWNLOADDRIVER_<PID>_<DownloadDriverID>

struct HTTP_DOWNLOADER_INFO
{
    boost::uint8_t  Url[256];
    boost::uint8_t  ReferUrl[256];
    boost::uint8_t  RedirectUrl[256];       // ����301����302 �ض����Url

    SPEED_INFO SpeedInfo;

    PIECE_INFO_EX DownloadingPieceEx;       // ������������� PieceEx
    PIECE_INFO_EX StartPieceEx;             // ���ĸ�Subpiece��ʼ����û�Ͽ����ӹ�;
    boost::uint32_t LastConnectedTime;      // �������HTTP���ӵ�ʱ��
    boost::uint32_t LastRequestPieceTime;   // �������PieceEx��ʱ��

    boost::uint16_t LastHttpStatusCode;     // �ϴ�HTTP����ķ���ֵ (����200.206)
    boost::uint16_t RetryCount;             // Http���Դ���
    boost::uint8_t  IsSupportRange;         // �Ƿ�֧��Range

    boost::uint8_t  IsDeath;                // �Ƿ�������
    boost::uint8_t  Resersed[400];          // �����ֶ�

    HTTP_DOWNLOADER_INFO()
    {
        Clear();
    }

    void Clear()
    {
        memset(this, 0, sizeof(HTTP_DOWNLOADER_INFO));
    }
};

struct DOWNLOADDRIVER_STATISTIC_INFO
{
    boost::uint32_t DownloadDriverID;
    SPEED_INFO SpeedInfo;

    boost::uint8_t OriginalUrl[256];
    boost::uint8_t OriginalReferUrl[256];

    RID ResourceID;                         // ��ԴRID (����Ϊȫ0)
    boost::uint32_t FileLength;             // �ļ��ĳ���
    boost::uint32_t BloskSize;              // Block�Ĵ�С
    boost::uint16_t BlockCount;             // Block�ĸ���

    boost::uint32_t TotalHttpDataBytes;     // ����HttpDownloader���ص���Ч�ֽ���
    boost::uint32_t TotalLocalDataBytes;    // ���б����Ѿ����ع�����Ч�ֽ���
    boost::uint8_t FileName[256];           // �ļ���,(TCHAR*)

    boost::uint8_t IsHidden;                // �Ƿ�����(���ڽ�������ʾ����)

    boost::uint8_t SourceType;              // ��ʶ�Ƿ��ǿͻ���
    boost::uint8_t WebUrl[256];             // va �������� WebUrl

    boost::uint8_t StateMachineType;
    boost::uint8_t StateMachineState[14];

    boost::uint32_t PlayingPosition;
    boost::uint32_t DataRate;
    boost::uint8_t http_state;
    boost::uint8_t p2p_state;
    boost::uint8_t timer_using_state;
    boost::uint8_t timer_state;

    boost::uint8_t Resersed[451];           // �����ֶ�

    boost::uint8_t  HttpDownloaderCount;
    HTTP_DOWNLOADER_INFO HttpDownloaders[]; // �䳤; (�������)
};

struct LIVE_DOWNLOADDRIVER_STATISTIC_INFO
{
    boost::uint32_t LiveDownloadDriverID;
    SPEED_INFO LiveHttpSpeedInfo;
    SPEED_INFO LiveP2PSpeedInfo;
    SPEED_INFO LiveP2PSubPieceSpeedInfo;
    boost::uint8_t http_state;
    boost::uint8_t p2p_state;
    boost::uint8_t  OriginalUrl[256];              // CDN IP
    boost::uint16_t LastHttpStatusCode;            // �ϴ�HTTP����ķ���ֵ (����200.206)
    boost::uint32_t TotalP2PDataBytes;             // ��ǰP2PDownloader���ص���Ч�ֽ���
    boost::uint32_t TotalRecievedSubPieceCount;    // �յ���Subpiece��
    boost::uint32_t TotalRequestSubPieceCount;     // ������Subpiece������
    boost::uint32_t TotalAllRequestSubPieceCount;     // ������Subpiece������
    boost::uint32_t TotalUnusedSubPieceCount;      // �����Subpieces��
    boost::uint16_t IpPoolPeerCount;               // ��ѡIP
    boost::uint32_t DataRate;                      // ������
    boost::uint16_t CacheSize;                      // �ѻ����С
    boost::uint32_t CacheFirstBlockId;             // �ѻ���ĵ�һƬBlock ID
    boost::uint32_t CacheLastBlockId;              // �ѻ�������һƬBlock ID
    boost::uint32_t PlayingPosition;               // ���ŵ�
    boost::uint32_t LeftCapacity;                  // �ڴ��ʣ��
    boost::int32_t  RestPlayTime;                  // ʣ��ʱ��
    RID       ResourceID;                          // ��ԴRID
    boost::uint8_t  IsPlayingPositionBlockFull; // �����Ƹ�����������һƬBlock�Ƿ���
    boost::uint32_t LivePointBlockId;              // ֱ�����Block ID
    boost::uint32_t DataRateLevel;                  // �����ȼ�
    boost::int32_t P2PFailedTimes;                  // P2P����ʧ�ܵĴ���
    boost::uint8_t HttpSpeedStatus;                 // Http�ٶ�״̬(0, 1, 2�ֱ����fast, checking, slow)
    boost::uint8_t HttpStatus;                      // Http״̬(unknown, good, bad)
    boost::uint8_t P2PStatus;                       // P2P״̬(unknown, good, bad)
    boost::uint32_t JumpTimes;                      // ��Ծ�˶��ٴ�
    boost::uint32_t NumOfChecksumFailedPieces;      // У��ʧ�ܵ�piece����
    boost::uint8_t Is3200P2PSlow;                   // 3200״̬��P2P�Ƿ���
    RID ChannelID;                                  // Ƶ��ID
    boost::uint32_t TotalUdpServerDataBytes;        // ��UdpServer���ص��ֽ���
    boost::uint8_t PmsStatus;                       // 0����������1��������
    boost::uint32_t UniqueID;                       // ��������ID
    SPEED_INFO UdpServerSpeedInfo;                  // UdpServer�ٶ�
    boost::uint8_t IsPaused;                        // �Ƿ���ͣ��0�����ţ�1������ͣ
    boost::uint8_t IsReplay;                        // �Ƿ���ϣ�0�������ϣ�1�������
    boost::uint32_t MissingSubPieceCountOfFirstBlock;  // ��һ��������Block�пյ�SubPiece����
    boost::uint32_t ExistSubPieceCountOfFirstBlock;  // ��һ��������Block�д��ڵ�SubPiece����
    boost::uint32_t P2PPeerSpeedInSecond;        // P2P Peerһ����ٶ�
    boost::uint32_t P2PUdpServerSpeedInSecond;  // UdpServerһ����ٶ�

    boost::uint8_t Reserved[896];

    boost::uint16_t PeerCount;                     // Peer��
    P2P_CONNECTION_INFO P2PConnections[MAX_P2P_DOWNLOADER_COUNT];  // �䳤; (�������)
};

struct PEER_UPLOAD_INFO
{
    PEER_UPLOAD_INFO()
    {
        Clear();
    }

    boost::uint32_t ip;
    boost::uint16_t port;
    boost::uint32_t upload_speed;
    PEER_INFO peer_info;
    boost::uint8_t resersed[126 - sizeof(PEER_INFO)];

    void Clear()
    {
        memset(this, 0, sizeof(PEER_UPLOAD_INFO));
    }
};

struct UPLOAD_INFO
{
    UPLOAD_INFO()
    {
        Clear();
    }

    boost::uint8_t peer_upload_count;
    boost::uint8_t reserve;
    boost::uint32_t upload_speed;
    boost::uint32_t actual_speed_limit;
    boost::uint32_t upload_subpiece_count;
    boost::uint8_t resersed[116];
    PEER_UPLOAD_INFO peer_upload_info[256];

    void Clear()
    {
        memset(this, 0, sizeof(UPLOAD_INFO));
    }
};

#pragma pack(pop)

#endif // _JUST_AGENT_DATA_STRUCT_H_
