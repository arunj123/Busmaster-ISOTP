/* This file is generated by BUSMASTER */
/* VERSION [1.2] */
/* BUSMASTER VERSION [3.2.2] */
/* PROTOCOL [CAN] */

/* Start BUSMASTER include header */
#include <Windows.h>
#include <CANIncludes.h>
/* End BUSMASTER include header */


/* Start BUSMASTER global variable */

#define TX_INI_FC_WAIT_TIME 127
//#define TP_MIN_WAIT_TIME    50

typedef uint32_t CanId_t; /* Type for representing CanID, both
                             extented and standard */
typedef uint32_t EcuId_t;

typedef uint8_t TpExtAdr_t;

typedef enum TP_TYPE {
    TP_TYPE_STANDARD = 0,   /* Standard tp with message based Identifier */
    TP_TYPE_EXTENTED,  /* Extented tp with additonal address as first byte */
} TpType_t;

typedef enum TPFRAME {
    SINGLE_FRAME   = 0x00,
    FIRST_FRAME    = 0x10,
    CONSICUTIVE    = 0x20,
    FLOW_CONTROL   = 0x30,
    FRAME_MASK     = 0xF0,
    FRAME_LEN_MASK = 0x0F
} TpFrame_t;

typedef enum TP_STATE {
    TP_INACTIVE = 0,
    TP_RX_ACTIVE,
    TP_TX_ACTIVE,
} TpState_t;

typedef enum TP_FC_TYPE {
    TP_FC_CTS         = 0x00, /* Continue to sent */
    TP_WAIT           = 0x01,
    TP_ABORT_OVERFLOW = 0x02,
    TP_FC_TYPE_MASK   = 0x0F,
} TpFcType_t;

typedef enum TP_RX_STATE {
    TP_RX_INACTIVE = 0,
    TP_RX_FC_PENDING,
    TP_RX_CF_WAIT,
} TpRxState_t;

typedef enum TP_TX_STATE {
    TP_TX_INACTIVE = 0,
    TP_TX_SATRT_PENDING,
    TP_TX_FC_WAIT,
    TP_TX_CF_SEND_WAIT,

} TpTxState_t;

typedef struct TP_RXINFO {
    TpRxState_t state;
} TpRxInfo_t;

typedef struct TP_TXINFO {
    TpTxState_t state;
    uint16_t BlockSize;
    uint16_t BlockCnt;
    uint8_t SeparationTime;
} TpTxInfo_t;

typedef struct CAN_MSG_BOX {
    bool isAvailable;
    uint8_t data[8];
} MsgBox_t;

typedef struct ISOTP {
    const TpType_t tpType;   /* To identify if the Tp is standard or an extented one */
    const TpExtAdr_t tpExAdr;/* Extended address used by transport protocol          */

    uint32_t timeCnt;        /* Time counter (in ms). Used by both Rx and Tx process */
    uint32_t WaitTime;
 
    uint16_t len;            /* Total length of the data, read/sent on First Frame   */
    uint16_t dPtr;           /* Data pointer, points to next position to send/reciev */
    uint16_t CFcnt;          /* To track the consicutive frams, received/transmited  */

    uint8_t data[4095];      /* Buffer to keep the data. Max size => (4+8)bits => 4K */
    TpState_t state;         /* Represents the current state of Transport Layer      */

    TpRxInfo_t rx;
    TpTxInfo_t tx;
} ISOTp_t;

typedef struct CANINFO_t {
    CanId_t TxMsgId;   /* Can Message identifier for TP transmission           */
    CanId_t RxMsgId;   /* Can Message identifier for TP reception              */
    bool isExtended;   /* If the Message Ids used are extented or not          */
} CanBusInfo_t;

typedef struct ECU {
    const CanBusInfo_t CanM; /* Information about the can message identifiers used   */

    ISOTp_t tp;              /* Transport layer related data for the ecu             */
    
    MsgBox_t msgBox;         /* Message box, received messages are kept here         */

} ECU_t;

#define NUM_ECUS 1
ECU_t ecus[NUM_ECUS] = {
    {{0x7B0, 0x7B8, false}, {TP_TYPE_EXTENTED, 0x40,} },
};
/* End BUSMASTER global variable */


/* Start BUSMASTER Function Prototype  */
GCC_EXTERN void GCC_EXPORT OnTimer_Cyclic_1ms_Tp_1( );
GCC_EXTERN void GCC_EXPORT OnBus_Connect();
GCC_EXTERN void GCC_EXPORT OnBus_Disconnect();
GCC_EXTERN void GCC_EXPORT OnMsg_All(STCAN_MSG RxMsg);
void Utils_TP_Cyclic_per_ecu(ECU_t *e);
void Utils_GetUdsTpRxNewHandle(ECU_t *e, uint8_t *rxData);
void Utils_TP_HandleRx(ECU_t *e);
void Utils_TxHandleCF(ECU_t *e);
uint16_t Utils_GetUdsTpSFMaxLen(ECU_t *e, uint8_t *f);
uint16_t Utils_GetUdsTpRxLen(ECU_t *e, uint8_t *f, TpFrame_t ft);
TpFrame_t Utils_GetUdsTpFrameType(ECU_t *e, uint8_t *f);
TpFcType_t Utils_GetUdsTpFCType(ECU_t *e, uint8_t *f);
uint8_t Utils_GetUdsTpFCSTmin(ECU_t *e, uint8_t *f);
uint16_t Utils_GetUdsTpFCBSize(ECU_t *e, uint8_t *f);
void Utils_TxHandleFC(ECU_t *e);
unsigned int Utils_GetSFMaxLen(ECU_t *e);
void Utils_StartTpTx(ECU_t *e);
void Utils_ClearTpData(ECU_t *e);
bool Utils_SendTpResponse(EcuId_t ecu, uint8_t *data, uint16_t size);
GCC_EXTERN void GCC_EXPORT OnKey_s(unsigned char KeyValue);
/* End BUSMASTER Function Prototype  */

/* Start BUSMASTER Function Wrapper Prototype  */
/* End BUSMASTER Function Wrapper Prototype  */


/* Start BUSMASTER generated function - OnTimer_Cyclic_1ms_Tp_1 */
void OnTimer_Cyclic_1ms_Tp_1( )
{
    uint32_t e;

    for (e = 0; e < NUM_ECUS; e++) {
        Utils_TP_Cyclic_per_ecu(&ecus[e]);
    }
    
}/* End BUSMASTER generated function - OnTimer_Cyclic_1ms_Tp_1 */
/* Start BUSMASTER generated function - OnBus_Connect */
void OnBus_Connect()
{
/* TODO */
}/* End BUSMASTER generated function - OnBus_Connect */
/* Start BUSMASTER generated function - OnBus_Disconnect */
void OnBus_Disconnect()
{
/* TODO */
}/* End BUSMASTER generated function - OnBus_Disconnect */
/* Start BUSMASTER generated function - OnMsg_All */
void OnMsg_All(STCAN_MSG RxMsg)
{
    uint8_t e;
    for (e = 0; e < NUM_ECUS; e++) {
        if (ecus[e].CanM.RxMsgId == RxMsg.id) {
            /* Message ignored, if message box is already full */
            if (false == ecus[e].msgBox.isAvailable) {
                memcpy(ecus[e].msgBox.data, RxMsg.data, 8);
                ecus[e].msgBox.isAvailable = true;
            }
        }
    }
}/* End BUSMASTER generated function - OnMsg_All */
/* Start BUSMASTER generated function - Utils_TP_Cyclic_per_ecu */
void Utils_TP_Cyclic_per_ecu(ECU_t *e)
{
    switch(e->tp.state) {
        case TP_TX_ACTIVE:
            e->tp.timeCnt++;
            switch(e->tp.tx.state) {
                case TP_TX_SATRT_PENDING:
                    Utils_StartTpTx(e);
                break;
                case TP_TX_FC_WAIT:
                    Utils_TxHandleFC(e);
                break;
                case TP_TX_CF_SEND_WAIT:
                    Utils_TxHandleCF(e);
                break;
                default:
                break;
            }
        break;
        case TP_INACTIVE:
            Utils_TP_HandleRx(e);
            break;
        case TP_RX_ACTIVE:
            Utils_TP_HandleRx(e);
        break;

        default:
        break;
    }

    if ((TP_TX_INACTIVE == e->tp.tx.state) && (TP_RX_INACTIVE == e->tp.rx.state)) {
        if (e->tp.state != TP_INACTIVE) {
            e->tp.state = TP_INACTIVE;
            Trace( "Back to inactive state " );
        }
    } 
}/* End BUSMASTER generated function - Utils_TP_Cyclic_per_ecu */
/* Start BUSMASTER generated function - Utils_GetUdsTpRxNewHandle */
void Utils_GetUdsTpRxNewHandle(ECU_t *e, uint8_t *rxData)
{
    /* Get Frame type */
    TpFrame_t ft = Utils_GetUdsTpFrameType(e, rxData);
    uint16_t len = Utils_GetUdsTpRxLen(e, rxData, ft);

    /* Single frame handling */
    if(SINGLE_FRAME == ft) {
        
        if(len <= Utils_GetUdsTpSFMaxLen(e, rxData)) {
            /* RxIndication to upper layer */
            char pd[100];
            Trace("New message Received: " );
            for(int i = 0; i < 8; i++) {
                sprintf(pd + i*3, "%02X ", rxData[i]); 
            }
            Trace(pd);
        }
        else {
            /* Frame ignored */
            Trace("Rx Frame ignored: len = %d for SF", len);
        }
    }

    /* Multi frame handling */
    if(FIRST_FRAME == ft) {
        if(len <= sizeof(e->tp.data)) {
            /* TODO: Update Rx and Tp States, initialize Rx, data to start further reception */

        }
        else {
            /* Frame ignored */
            Trace("Rx Frame ignored: len = %d for FF", len);
        }
    }

    /* Other types of frames are unexpected at this state, hence ignored */
    return;
}/* End BUSMASTER generated function - Utils_GetUdsTpRxNewHandle */
/* Start BUSMASTER generated function - Utils_TP_HandleRx */
void Utils_TP_HandleRx(ECU_t *e)
{
    uint8_t rxData[8];

    if (e->msgBox.isAvailable) {
        
        /* Copy received data and mark, messagebox available */
        memcpy(rxData, e->msgBox.data, sizeof(rxData));
        e->msgBox.isAvailable = false;

        /* Handle received data according to state and frame type */
        switch(e->tp.rx.state) {
            case TP_RX_INACTIVE:
                // Check New Rx message in mail box
                Utils_GetUdsTpRxNewHandle(e, rxData);
            break;
            default:
            break;
        }
    }
}/* End BUSMASTER generated function - Utils_TP_HandleRx */
/* Start BUSMASTER generated function - Utils_TxHandleCF */
void Utils_TxHandleCF(ECU_t *e)
{
    if (e->tp.timeCnt >= e->tp.WaitTime) {
        STCAN_MSG canMsg = STCAN_MSG(0);
        uint8_t canWrIdx = 0;

        /* Reset timecounter */
        e->tp.timeCnt = 0;

        /* Send consicutive frame */
        canMsg.data[canWrIdx++] = (uint8_t)(((uint8_t) CONSICUTIVE) | (e->tp.CFcnt & 0xF) );

        if((e->tp.len - e->tp.dPtr) > 7) {
            memcpy(&canMsg.data[canWrIdx], &e->tp.data[e->tp.dPtr], 7);
            e->tp.tx.BlockCnt++;

            /* if block count reached, wait for FC */
            if(e->tp.tx.BlockCnt >= e->tp.tx.BlockSize) {
                e->tp.tx.state = TP_TX_FC_WAIT;
                e->tp.WaitTime = TX_INI_FC_WAIT_TIME;
                e->msgBox.isAvailable = false; /* Clear any previous messages in messagebox */
            }
            else { /* Else wait for separation time to next send */
                e->tp.WaitTime = e->tp.tx.SeparationTime;
            }
            e->tp.dPtr += 7; /* Increment data pointer to next place */
        }
        else {
            /* Copy only the remaing bytes  */
            memcpy(&canMsg.data[canWrIdx], &e->tp.data[e->tp.dPtr], (e->tp.len - e->tp.dPtr));

            e->tp.tx.state = TP_TX_INACTIVE;
            Trace("Tx Success: Multiframe tx completed successfully");
        }

        /* Incremet number of CFs sent */
        e->tp.CFcnt++;

        /* Send the constructed frame using API */
        canMsg.id = e->CanM.TxMsgId;
        canMsg.isExtended = e->CanM.isExtended;
        canMsg.dlc = 8;
        SendMsg(canMsg);
    }
    return;
}/* End BUSMASTER generated function - Utils_TxHandleCF */
/* Start BUSMASTER generated function - Utils_GetUdsTpSFMaxLen */
uint16_t Utils_GetUdsTpSFMaxLen(ECU_t *e, uint8_t *f)
{
    uint16_t ret;

    if (TP_TYPE_STANDARD == e->tp.tpType) {
        ret = 7;
    }
    else {
        ret = 6;
    }
    return ret;
}/* End BUSMASTER generated function - Utils_GetUdsTpSFMaxLen */
/* Start BUSMASTER generated function - Utils_GetUdsTpRxLen */
uint16_t Utils_GetUdsTpRxLen(ECU_t *e, uint8_t *f, TpFrame_t ft)
{
    uint16_t ret;

    if (TP_TYPE_STANDARD == e->tp.tpType) {
        ret = (uint16_t)(((TpFrame_t)f[0]) & FRAME_LEN_MASK);
    }
    else {
        ret = (uint16_t)(((TpFrame_t)f[1]) & FRAME_LEN_MASK);
    }

    if(FIRST_FRAME == ft) {
        if (TP_TYPE_STANDARD == e->tp.tpType) {
            ret = (ret << 8) | f[1];
        }
        else {
            ret = (ret << 8) | f[2];
        }
    }

    return ret;
}/* End BUSMASTER generated function - Utils_GetUdsTpRxLen */
/* Start BUSMASTER generated function - Utils_GetUdsTpFrameType */
TpFrame_t Utils_GetUdsTpFrameType(ECU_t *e, uint8_t *f)
{
    TpFrame_t ret;

    if (TP_TYPE_STANDARD == e->tp.tpType) {
        ret = (TpFrame_t)(((TpFrame_t)f[0]) & FRAME_MASK);
    }
    else {
        ret = (TpFrame_t)(((TpFrame_t)f[1]) & FRAME_MASK);
    }
    return ret;
}/* End BUSMASTER generated function - Utils_GetUdsTpFrameType */
/* Start BUSMASTER generated function - Utils_GetUdsTpFCType */
TpFcType_t Utils_GetUdsTpFCType(ECU_t *e, uint8_t *f)
{
    TpFcType_t ret;

    if (TP_TYPE_STANDARD == e->tp.tpType) {
        ret = (TpFcType_t)(((TpFcType_t)f[0]) & TP_FC_TYPE_MASK);
    }
    else {
        ret = (TpFcType_t)(((TpFcType_t)f[1]) & TP_FC_TYPE_MASK);
    }
    return ret;
}/* End BUSMASTER generated function - Utils_GetUdsTpFCType */
/* Start BUSMASTER generated function - Utils_GetUdsTpFCSTmin */
uint8_t Utils_GetUdsTpFCSTmin(ECU_t *e, uint8_t *f)
{
    uint8_t ret;
    uint8_t byt;

    if (TP_TYPE_STANDARD == e->tp.tpType) {
        byt = f[2];
    }
    else {
        byt = f[3];
    }

    /* 0xF1 to 0xF9 UF, 100 to 900 microseconds.
     * Equivalent to 0, since atleast 50ms wait
     * by design is present */
    if( byt > 127 ) {
        ret = 0;
    }
    else {
        ret = byt;
    }

    return ret;
}/* End BUSMASTER generated function - Utils_GetUdsTpFCSTmin */
/* Start BUSMASTER generated function - Utils_GetUdsTpFCBSize */
uint16_t Utils_GetUdsTpFCBSize(ECU_t *e, uint8_t *f)
{
    uint16_t ret;
    uint8_t byt;
    if (TP_TYPE_STANDARD == e->tp.tpType) {
        byt = f[1];
    }
    else {
        byt = f[2];
    }

    /* If 0 is received, then never wait for an FC */
    if (0 == byt) {
        ret = 0xFFFF; /* Set to maximum value,
                       * so that FC is never expected */
    }
    else {
        ret = byt;
    }

    return ret;
}/* End BUSMASTER generated function - Utils_GetUdsTpFCBSize */
/* Start BUSMASTER generated function - Utils_TxHandleFC */
void Utils_TxHandleFC(ECU_t *e)
{
    TpFrame_t ft;
    if (e->msgBox.isAvailable) {
        
        /* Read the type of frame from the received message data */
        ft = Utils_GetUdsTpFrameType(e, e->msgBox.data);

        if (FLOW_CONTROL == ft) {
            TpFcType_t fct = Utils_GetUdsTpFCType(e, e->msgBox.data);
            uint8_t STmin = Utils_GetUdsTpFCSTmin(e, e->msgBox.data);
            switch(fct) {
                case TP_FC_CTS:
                    e->tp.WaitTime = 0; /* Send the next CC without waiting */
                    e->tp.tx.SeparationTime = STmin; /* Save STmin for future */
                    e->tp.tx.BlockSize = Utils_GetUdsTpFCBSize(e, e->msgBox.data);
                    e->tp.tx.BlockCnt = 0;

                    e->tp.tx.state = TP_TX_CF_SEND_WAIT;
                    e->tp.timeCnt = 0; /* Reset time counter */
                break;
                case TP_WAIT:
                    e->tp.WaitTime = STmin; /* Wait for another FC */
                    e->tp.timeCnt = 0; /* Reset time counter */
                break;
                default:
                    e->tp.tx.state = TP_TX_INACTIVE;
                    Trace("TX Failed: FC received type = %02X", fct);
                break;
            }
        }
        else {
            Trace("TX: FC expected, but received type %02X and ignored", ft);
        }

        e->msgBox.isAvailable = false; /* Reset the available flag as data is used */
    } 
    else {
        /* Go to inactive after waiting for enough time */
        if (e->tp.timeCnt >= e->tp.WaitTime) {
            e->tp.tx.state = TP_TX_INACTIVE;

            Trace( "TX Failed: FC not received" );
        }
    }
    return;
}/* End BUSMASTER generated function - Utils_TxHandleFC */
/* Start BUSMASTER generated function - Utils_GetSFMaxLen */
unsigned int Utils_GetSFMaxLen(ECU_t *e)
{
    if (TP_TYPE_STANDARD == e->tp.tpType) {
        return 7u;
    }
    else {
        return 6u;
    }
}/* End BUSMASTER generated function - Utils_GetSFMaxLen */
/* Start BUSMASTER generated function - Utils_StartTpTx */
void Utils_StartTpTx(ECU_t *e)
{
    STCAN_MSG canMsg = STCAN_MSG(0);
    uint8_t canWrIdx = 0;
    uint8_t sfLen = Utils_GetSFMaxLen(e);

    if (TP_TYPE_EXTENTED == e->tp.tpType) {
        canMsg.data[canWrIdx++] = e->tp.tpExAdr;
    }

    if (e->tp.len > sfLen) {
        /* Send start frame */
        canMsg.data[canWrIdx++] = (uint8_t)(((uint8_t) FIRST_FRAME) | (e->tp.len >> 8) );
        canMsg.data[canWrIdx++] = (uint8_t)(e->tp.len);
        memcpy(&canMsg.data[canWrIdx], e->tp.data, sfLen);
        e->tp.tx.state = TP_TX_FC_WAIT;
        e->tp.WaitTime = TX_INI_FC_WAIT_TIME;
        e->tp.CFcnt = 1; /* Next CF to be sent is 1 */
        e->tp.dPtr = sfLen;
    }
    else {
        /* Send Single frame */
        canMsg.data[canWrIdx++] = (uint8_t)(((uint8_t)SINGLE_FRAME) | e->tp.len);
        memcpy(&canMsg.data[canWrIdx], e->tp.data, e->tp.len);
        e->tp.tx.state = TP_TX_INACTIVE;
    }

    canMsg.id = e->CanM.TxMsgId;
    canMsg.isExtended = e->CanM.isExtended;
    canMsg.dlc = 8;
    SendMsg(canMsg);

    return;
}/* End BUSMASTER generated function - Utils_StartTpTx */
/* Start BUSMASTER generated function - Utils_ClearTpData */
void Utils_ClearTpData(ECU_t *e)
{
    e->tp.timeCnt = 0;
    e->tp.WaitTime = 0;
 
    e->tp.dPtr = 0;
    e->tp.CFcnt = 0;

    e->tp.state = TP_INACTIVE;

    memset(&e->tp.rx, 0, sizeof(e->tp.rx));
    memset(&e->tp.tx, 0, sizeof(e->tp.tx));
}/* End BUSMASTER generated function - Utils_ClearTpData */
/* Start BUSMASTER generated function - Utils_SendTpResponse */
bool Utils_SendTpResponse(EcuId_t ecu, uint8_t *data, uint16_t size)
{
    bool ret = false;
    if (TP_INACTIVE == ecus[ecu].tp.state) {
        if ( 0 != size ) {
            if( sizeof(ecus[ecu].tp.data) >= size ) {
                /* Clear previous tp states */
                Utils_ClearTpData(&ecus[ecu]);

                /* Copy data */
                memcpy(ecus[ecu].tp.data, data, size);
                ecus[ecu].tp.len = size;

                /* Set appropriate states */
                ecus[ecu].tp.state = TP_TX_ACTIVE;
                ecus[ecu].tp.tx.state = TP_TX_SATRT_PENDING;

                ret = true;
            }
            else {
                Trace ( "Tx Failed: DataLen more than supported %d bytes", sizeof(ecus[ecu].tp.data) );
            }
        }
        else {
            Trace ( "Tx Failed: Datalength 0");
        }
    }
    else {
        Trace ( "Tx Failed: TP already Active" );
    }
    
    return ret;
}/* End BUSMASTER generated function - Utils_SendTpResponse */
/* Start BUSMASTER generated function - OnKey_s */
void OnKey_s(unsigned char KeyValue)
{
    uint16_t i;
    uint8_t txDataResp[4096];
    for (i = 0; i < 4096; i++) txDataResp[i] = (uint8_t)(i + 1);
    Utils_SendTpResponse(0, txDataResp, 0);
    Utils_SendTpResponse(0, txDataResp, 4097);
    //Utils_SendTpResponse(0, txDataResp, 3);
    Utils_SendTpResponse(0, txDataResp, 155);
}/* End BUSMASTER generated function - OnKey_s */
 