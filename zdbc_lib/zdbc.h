#ifndef _Z_DBC_H_
#define _Z_DBC_H_

#ifdef WIN32
    #define FUNCCALL __stdcall
#else
    #define FUNCCALL
#endif

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

typedef unsigned int     uint32;
typedef unsigned int     DBCHandle;
typedef unsigned char    uint8;
typedef unsigned short   uint16;
typedef unsigned long long uint64;

// error code
typedef uint8 ERR_CODE;
#define ERR_SUCCESS            0
#define ERR_FAILED             1
#define ERR_MULTI_TRANSMITTING 2                //��֡���䷢����
#define ERR_NOT_SUPPORT        3                //���ܲ�֧��, ��δ����

#define INVALID_DBC_HANDLE        -1            // ��Ч��DBC���

#define _MAX_FILE_PATH_           260           // ��ļ�·��
#define _DBC_NAME_LENGTH_         127           // ���������
#define _DBC_COMMENT_MAX_LENGTH_  200           // ע�������
#define _DBC_UNIT_MAX_LENGTH_     10            // ��λ�����
#define _DBC_SIGNAL_MAX_COUNT_    256           // һ����Ϣ���е��źŵ������Ŀ

// multiplexer type
#define MUTIPLEXER_NONE     0                   // ��ʹ�ø�����
#define MUTIPLEXER_M_VALUE  1                   // �����źţ������������ص�ֵΪmultiplexer_valueʱ�����ź���Ч
#define MUTIPLEXER_M_SWITCH 2                   // ���������أ�һ��DBC��Ϣֻ����һ���ź�Ϊ����

// CAN type
#define FT_CAN   0                              //CAN֡
#define FT_CANFD 1                              //CANFD֡

struct DBCSignal
{
    uint32 nStartBit;                               // ��ʼλ
    uint32 nLen;                                    // λ����
    double nFactor;                                 // ת������
    double nOffset;                                 // ת��ƫ�� ʵ��ֵ=ԭʼֵ*nFactor+nOffset
    double nMin;                                    // ��Сֵ
    double nMax;                                    // ���ֵ
    //double nValue;                                  // ʵ��ֵ, ����ʹ�ã���Ϊͨ�� ZDBC_CalcActualValue ����
    uint64 nRawValue;                               // ԭʼֵ
    bool is_signed;                                 // �Ƿ��з�������
    bool is_motorola;                               // �Ƿ�Ħ��������ʽ
    uint8 multiplexer_type;                         // ���������ͣ�see 'multiplexer type' above
    uint8 val_type;                                 // 0:integer, 1:float, 2:double
    uint32 multiplexer_value;                       // ����������ֵΪ��ֵʱ�ź���Ч
    char unit[_DBC_UNIT_MAX_LENGTH_+1];             // ��λ
    char strName[_DBC_NAME_LENGTH_+1];              // ����
    char strComment[_DBC_COMMENT_MAX_LENGTH_+1];    // ע��
    char strValDesc[_DBC_NAME_LENGTH_+1];           // ֵ����
};

struct DBCMessage
{
    uint32 nSignalCount;                            // �ź�����
    uint32 nID;                                     // ��ϢID
    uint32 nSize;                                   // ��Ϣռ���ֽ���Ŀ
    double nCycleTime;                              // ��������
    uint8 nExtend;                                  // 1:��չ֡, 0:��׼֡
    DBCSignal vSignals[_DBC_SIGNAL_MAX_COUNT_];     // �źż���
    char strName[_DBC_NAME_LENGTH_+1];              // ����
    char strComment[_DBC_COMMENT_MAX_LENGTH_+1];    // ע��
};

struct DBCNetworkNode
{
    char strName[_DBC_NAME_LENGTH_+1];              // ����ڵ�����
    uint32 stationAddr;                             // ��ַ
};

// ����֡�ṹ����
#define ZDBC_FRAME_TYPE_ZLGCAN      0               // ʹ��zlgcan���can_frame ���� canfd_frame       
#define ZDBC_FRAME_TYPE_CONTROLCAN  1               // ʹ��ControlCAN��VCI_CAN_OBJ

// DBCЭ������
#define PROTOCOL_J1939              0               // J1939Э��
#define PROTOCOL_OTHER              1               // ����Э��, ���DBC�ļ���δָ����Ĭ��Ϊ��Э��

struct FileInfo
{
    char strFilePath[_MAX_FILE_PATH_+1]; // dbc�ļ�·��������·����ʹ��GB2312����
    uint8 merge;                         // 1:��������е�����, ��֧�ּ��ض���ļ�;0�����ԭ��������
};

//ֵ�뺬���, ����3 "Not Supported" 2 "Error" 1 "Not Defined"
struct ValDescPair
{
    double value;
    char desc[_DBC_NAME_LENGTH_+1];
};

/*
����˵������ûʵ�ʷ������ݹ���, ֻ�ṩ���͵�����,
          ��������Ҫʵ�ָûص���������ʵ�����ݷ��͡�
��    ��[in]��ctx-�ص�������������
��    ��[in]��pObj-Ҫ���͵�����
�� �� ֵ��true���ͳɹ�
*/
typedef bool (FUNCCALL *OnSend)(void* ctx, void* pObj);

/*
����˵��������CANFD֡�Ļص�
��    ��[in]��ctx-�ص�������������
��    ��[in]��pObj-Ҫ���͵�����
�� �� ֵ��true���ͳɹ�
*/
typedef bool (FUNCCALL *OnSendCANFD)(void* ctx, void* pObj);

/*
����˵������֡������ɺ�ص���Ҳ��ZDBC_Analyse��֡��Ϣ������ɺ�Ļص���
��    ��[in]��ctx-�ص�������������
��    ��[in]��pMsg-���ص�DBC��Ϣ
��    ��[in]��data-��֡��ʵ������
��    ��[in]��nLen-��֡���ݵ��ֽ���Ŀ
��    ��[in]��nDirection-��֡�Ĵ��䷽�� 1:����, 0:����
�� �� ֵ��void
*/
typedef void (FUNCCALL *OnMultiTransDone)(void* ctx, DBCMessage* pMsg, uint8* data, uint16 nLen, uint8 nDirection);

/*
����˵����ZDBC_AsyncAnalyse �첽������ɺ�Ļص�
��    ��[in]��ctx-�ص�������������
��    ��[in]��pMsg-���ص�DBC��Ϣ
��    ��[in]��extraData-�������ʱ����ı�ʶ
*/
typedef void (FUNCCALL *OnAsyncAnalyseDone)(void* ctx, DBCMessage* pMsg, uint64 extraData);

/*
����˵���������ڲ���־����ļ�, Ĭ������־���
��    ��[in]��enable�� 1-������־���, 0-�ر���־���
��    ��[in]��filepath ��־�ļ�·��, enableΪ0ʱ�ɴ�NULL
*/
EXTERN_C  void FUNCCALL ZDBC_SetLogFile( uint8 enable, const char* filepath );

/*
����˵������������֡�ṹ�����ͣ�Ĭ��ΪZDBC_FRAME_TYPE_ZLGCAN
��    ��[in]��type ����֡�ṹ���ͣ�ZDBC_FRAME_TYPE_ZLGCAN �� ZDBC_FRAME_TYPE_CONTROLCAN
*/
EXTERN_C  void   FUNCCALL ZDBC_SetFrameType( uint32 type );
EXTERN_C  uint32 FUNCCALL ZDBC_GetFrameType();

/*
����˵������ʼ������ģ�顣
��    ��[in]��disableMultiSend-�Ƿ�رն�֡���ͣ� 0-���رգ������̴߳����֡���ͽ���; 1-�رգ���֧�ֶ�֡����Ϣ����
��    ��[in]��enableAsyncAnalyse-�Ƿ����첽������ 0-��������ZDBC_AsyncAnalyse�ӿ���Ч��1-����, �����߳̽�������Ϣ
�� �� ֵ��INVALID_DBC_HANDLE:��ʼ��ʧ��
          ����ֵ:��ʼ���ɹ�
          �����ֵ������ʹ��
*/
EXTERN_C  DBCHandle FUNCCALL ZDBC_Init(uint8 disableMultiSend = 0, uint8 enableAsyncAnalyse = 0);

/*
����˵�����ͷ���Դ, ��DBC_Init���ʹ��
��    ��[in]��hDBC-���, DBC_Load�ķ���ֵ
*/
EXTERN_C  void FUNCCALL ZDBC_Release( DBCHandle hDBC );

/*
����˵��������DBC��ʽ�ļ���
��    ��[in]��hDBC-���, DBC_Load�ķ���ֵ
��    ��[in]��pFileInfo-�ļ���Ϣ, �ο�struct FileInfo
�� �� ֵ��true���سɹ�
*/
EXTERN_C  bool FUNCCALL ZDBC_LoadFile( DBCHandle hDBC, const FileInfo* pFileInfo );

/**
* \brief ����dbc�ļ� Э��
* \param[in]   type   : dbc��Э������, j1939ѡ��PROTOCOL_J1939, ����Э��ѡ��PROTOCOL_OTHER
* \retval �ɹ�����true��ʧ�ܷ���false��
*/
EXTERN_C void FUNCCALL ZDBC_SetProtocol( DBCHandle hDBC, uint8 type);

/**
* \brief ���ؼ��ص�dbc�ļ���Э������, PROTOCOL_J1939, PROTOCOL_OTHER
*/
EXTERN_C uint8 FUNCCALL ZDBC_GetProtocol( DBCHandle hDBC );

/*
����˵������ȡ�ļ��ĵ�һ����Ϣ��
��    ��[in]��hDBC-���, DBC_Load�ķ���ֵ
��    ��[out]��pMsg �洢��Ϣ����Ϣ
�� �� ֵ��true��ʾ�ɹ�
*/
EXTERN_C  bool FUNCCALL ZDBC_GetFirstMessage( DBCHandle hDBC, DBCMessage* pMsg );

/*
����˵������ȡ��һ����Ϣ��
��    ��[in]��hDBC-���, DBC_Load�ķ���ֵ
��    ��[out]��pMsg �洢��Ϣ����Ϣ
�� �� ֵ��true��ʾ�ɹ�
*/
EXTERN_C  bool FUNCCALL ZDBC_GetNextMessage( DBCHandle hDBC, DBCMessage* pMsg );

/*
����˵��������ID��ȡ��Ϣ��
��    ��[in]��hDBC-���, DBC_Load�ķ���ֵ
��    ��[in]��nID-��Ϣ��ID
��    ��[out]��pMsg �洢��Ϣ����Ϣ
�� �� ֵ��true��ʾ�ɹ�
*/
EXTERN_C  bool FUNCCALL ZDBC_GetMessageById( DBCHandle hDBC, uint32 nID, DBCMessage* pMsg );

/*
����˵������ȡ�ļ���Ϣ������
��    ��[in]��hDBC-���, DBC_Load�ķ���ֵ
�� �� ֵ����Ϣ����
*/
EXTERN_C  uint32 FUNCCALL ZDBC_GetMessageCount( DBCHandle hDBC );

/*
����˵��������DBCЭ�����֡����
         ���Ϊ��֡��Ϣ����δ����֡���ݣ�ͨ��OnMultiTransDone�ص������������
��    ��[in]��hDBC-���, DBC_Load�ķ���ֵ
��    ��[in]��pObj ԭʼ֡����, ControlCAN����VCI_CAN_OBJ, zlgcan����can_frame ���� canfd_frame
��    ��[in]��frame_type ֡����, �ο�FT_CAN��FT_CANFD��ControlCAN��֧��CANFD
��    ��[out]��pMsg �������
�� �� ֵ��true��ʾ�ɹ�
*/
EXTERN_C  bool FUNCCALL ZDBC_Analyse( DBCHandle hDBC, const void* pObj, uint8 frame_type, DBCMessage* pMsg );

/*
����˵��������DBCЭ�����֡����ΪDBCMessage, �첽�����������߳�; ʹ�ô˽ӿ���ҪZDBC_Init�������enableAsyncAnalyse=1��
         ���Ϊ��֡��Ϣ����δ����֡���ݡ�
��    ��[in]��hDBC-���, DBC_Load�ķ���ֵ
��    ��[in]��pObj ԭʼ֡����, ControlCAN����VCI_CAN_OBJ, zlgcan����can_frame ���� canfd_frame
��    ��[in]��frame_type ֡����, �ο�FT_CAN��FT_CANFD��ControlCAN��֧��CANFD
��    ��[in]��extraData ��֡���ݵĸ������ݣ�������ɺ��ص����ش˲���������֡ʱ���
�� �� ֵ��true��ʾ����������гɹ�, ͨ�� ZDBC_SetAsyncAnalyseCallback ���õĻص������������
*/
EXTERN_C  bool FUNCCALL ZDBC_AsyncAnalyse(DBCHandle hDBC, const void* pObj, uint8 frame_type, uint64 extraData);

/*
����˵���������첽�����ص�����, ��֡��Ϣ���֡��Ϣ��ʹ�ô˻ص���
��    ��[in]��hDBC-���, DBC_Load�ķ���ֵ
��    ��[in]��cb �ص�����
��    ��[in]��ctx ������
*/
EXTERN_C  void FUNCCALL ZDBC_SetAsyncAnalyseCallback( DBCHandle hDBC, OnAsyncAnalyseDone cb, void* ctx);

/*
����˵������������Ҫ���øú����ѽ��յ���֡���ݴ�����,
         �漰��֡�������Ҫ��������, �����޷�ʵ���豸������
��    ��[in]��hDBC-���, DBC_Load�ķ���ֵ
��    ��[in]��pObj ԭʼ֡����
��    ��[in]��frame_type ֡���� �ο�FT_CAN FT_CANFD
�� �� ֵ��void
*/
EXTERN_C  void FUNCCALL ZDBC_OnReceive( DBCHandle hDBC, const void* pObj, uint8 frame_type );

/*
����˵��������ʵ�ʷ������ݵĴ�����, �漰���ݷ���ʱ�������ã�ֻ��Ҫ����һ�Ρ�
��    ��[in]��hDBC-���, DBC_Load�ķ���ֵ
��    ��[in]��sender�ص�����ָ��
��    ��[in]��ctx-�ص�������������
�� �� ֵ��void
*/
EXTERN_C  void FUNCCALL ZDBC_SetSender( DBCHandle hDBC, OnSend sender, void* ctx );
EXTERN_C  void FUNCCALL ZDBC_SetCANFDSender( DBCHandle hDBC, OnSendCANFD sender, void* ctx );

/*
����˵�������ô����֡�������ݵĻص�������ֻ��Ҫ����һ��
��    ��[in]��hDBC-���, DBC_Load�ķ���ֵ
��    ��[in]��OnMultiTransDone�ص�����ָ��
��    ��[in]��ctx-�ص�������������
�� �� ֵ��void
*/
EXTERN_C  void FUNCCALL ZDBC_SetOnMultiTransDoneFunc( DBCHandle hDBC, OnMultiTransDone func, void* ctx);
/*
����˵��������DBC��Ϣ��
         zlgcan���֧��CANFD��֡���ͣ���֡ʱֻ��CAN֡���͡�
��    ��[in]��hDBC-���, DBC_Load�ķ���ֵ
��    ��[in]��pMsg Ҫ���͵���Ϣ
��    ��[in]��frame_type ʹ��CAN����CANFD֡���ͣ��ο�FT_CAN��FT_CANFD
�� �� ֵ���ο�//error code
*/
EXTERN_C  ERR_CODE FUNCCALL ZDBC_Send( DBCHandle hDBC, const DBCMessage* pMsg, uint8 frame_type  );

/*
����˵������ȡ�����źŵ�ֵ�뺬��Ը�����
��    ��[in]��hDBC-���, DBC_Load�ķ���ֵ
��    ��[in]��msg_id, ��ϢID
��    ��[in]��signal_name, �ź���
�� �� ֵ��ֵ�뺬��Ը���
*/
EXTERN_C  uint32 FUNCCALL ZDBC_GetValDescPairCount( DBCHandle hDBC, uint32 msg_id, const char* signal_name);

/*
����˵������ȡֵ�뺬��ԡ�
��    ��[in]��hDBC-���, DBC_Load�ķ���ֵ
��    ��[in]��msg_id, ��ϢID
��    ��[in]��signal_name, �ź���
��    ��[in,out]��pair ����,�����߹���,��СΪDBC_GetValDescPairCount����ֵ
�� �� ֵ��true��ʾ�ɹ�
*/
EXTERN_C  void FUNCCALL ZDBC_GetValDescPair( DBCHandle hDBC, uint32 msg_id, const char* signal_name, ValDescPair* pair);

/*
����˵��������ԭʼ���ݽ���ΪDBCMessage
��    ��[in]��hDBC-���, DBC_Load�ķ���ֵ
��    ��[out]��pMsg �������
��    ��[in]��pObj ֡��������, ControlCAN����VCI_CAN_OBJ, zlgcan����can_frame
��    ��[in]��nCount ԭʼ֡���ݸ���, �������С
�� �� ֵ��true��ʾ�ɹ�
*/
EXTERN_C  bool FUNCCALL ZDBC_Decode( DBCHandle hDBC, DBCMessage* pMsg, const void* pObj, uint32 nCount );

/*
����˵��������DBCMessage����Ϊԭʼ����
��    ��[in]��hDBC-���, DBC_Load�ķ���ֵ
��    ��[out]��pObj �����ԭʼ���ݻ���������, ControlCAN����VCI_CAN_OBJ, zlgcan����can_frame
��    ��[in,out]��pObj ��������С, ����ʱΪʵ��ԭʼ���ݸ���
��    ��[in]��pMsg Ҫ�������Ϣ
�� �� ֵ��true��ʾ�ɹ�
*/
EXTERN_C  bool FUNCCALL ZDBC_Encode( DBCHandle hDBC, void* pObj, uint32* nCount, const DBCMessage* pMsg );

/*
����˵�����ź�ԭʼֵת��Ϊʵ��ֵ
��    ��[in]��sgl �ź�
��    ��[in,out]��rawVal ԭʼֵ, �����ֵ�����źų��ȿɱ�ʾ��Χ���ᱻ�ضϡ�
�� �� ֵ��ʵ��ֵ
*/
EXTERN_C double FUNCCALL ZDBC_CalcActualValue(const DBCSignal* sgl, uint64 *rawVal);

/*
����˵�����ź�ʵ��ֵת��Ϊԭʼֵ
��    ��[in]��sgl �ź�
��    ��[in,out]��actualVal ʵ��ֵ, �����ɱ�ʾ��Χʱ�ᱻ����
�� �� ֵ��ԭʼֵ
*/
EXTERN_C uint64 FUNCCALL ZDBC_CalcRawValue(const DBCSignal* sgl, double *actualVal);

/*
����˵������ȡ����ڵ�����
��    ��[in]��hDBC-���, DBC_Load�ķ���ֵ
�� �� ֵ������ڵ�������
*/
EXTERN_C uint32 FUNCCALL ZDBC_GetNetworkNodeCount(DBCHandle hDBC);

/*
����˵������ȡָ������������ڵ���Ϣ
��    ��[in]��hDBC-���, DBC_Load�ķ���ֵ
��    ��[in]��index λ������
��    ��[out]��node ����ڵ���Ϣ
�� �� ֵ���ɹ���ʧ��
*/
EXTERN_C bool FUNCCALL ZDBC_GetNetworkNodeAt(DBCHandle hDBC, uint32 index, DBCNetworkNode *node);


#endif 
