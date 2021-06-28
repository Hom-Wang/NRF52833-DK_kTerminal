/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    dw1000.c
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include <string.h>
#include "dw1000.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/

/* Prototypes ------------------------------------------------------------------------------*/

#define dwt_getsystemstatus()           dwt_read32bitreg(SYS_STATUS_ID)
#define dwt_setsystemstatus(__STATUS)   dwt_write32bitreg(SYS_STATUS_ID, __STATUS)

uint64_t dwt_getrxtimestamp64(void);
int32_t dwt_getrespondertime(uint32_t *txtime, uint64_t *rxtimestamp, uint64_t *txtimestamp);
int32_t dwt_getinitiatortime(uint32_t *txtimestamp, uint32_t *rxtimestamp);

/* Declaration of static functions. */
static void resp_msg_set_ts(uint8_t *ts_field, const uint64_t ts);
static void resp_msg_get_ts(uint8_t *ts_field, uint32_t *ts);

/* Functions -------------------------------------------------------------------------------*/

//enum
//{
//    // tx rx mode
//    UWB_IMMEDIATE         = 0x00,
//    UWB_DELAYED           = 0x01,
//    UWB_RANGING           = 0x80,

//    // tx mode
//    UWB_TX_RESPONSE       = 0x02,

//    // rx mode
//    UWB_RX_ON_DLY_ERR     = 0x02,
//    UWB_RX_NO_SYNC_PTRS   = 0x04,
//    UWB_RX_CONTINUE       = 0x40,
//};


//uint32_t uwb_recvpacket(uint8_t *packet, uint16_t lens, uint32_t mode)
//{
//    uint16_t frameLens;
//    uint32_t status;
//    if ((mode & UWB_RX_CONTINUE) != UWB_RX_CONTINUE)
//    {
//        dwt_rxenable(mode);
//    }
//    do
//    {
//        status = dwt_getsystemstatus();
//    }
//    while (!(status & (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR)));

//    if (status & SYS_STATUS_RXFCG)
//    {
//        dwt_setsystemstatus(SYS_STATUS_RXFCG);
//        frameLens = dwt_read32bitreg(RX_FINFO_ID) & RX_FINFO_RXFL_MASK_1023;
//        if (frameLens <= lens)
//        {
//            dwt_readrxdata(packet, frameLens, 0);
//            return KS_OK;
//        }
//        else
//        {
//            return KS_ERROR;
//        }
//    }
//    else
//    {
//        /* Clear RX error events in the DW1000 status register. */
//        dwt_setsystemstatus(SYS_STATUS_ALL_RX_ERR);
//        /* Reset RX to properly reinitialise LDE operation. */
//        dwt_rxreset();
////        dwt_setsystemstatus(SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR);
////        dwt_rxreset();
//        return KS_TIMEOUT;
//    }
//}

//uint32_t uwb_sendpacket(uint8_t *packet, uint16_t lens, uint32_t mode)
//{
//    uint32_t status;
//    uint32_t ranging_enable = (mode & UWB_RANGING);
//    dwt_setsystemstatus(SYS_STATUS_TXFRS);
//    dwt_writetxdata(lens, packet, 0);
//    dwt_writetxfctrl(lens, 0, ranging_enable);
//    // dwt_starttx(DWT_START_TX_DELAYED);
//    status = dwt_starttx(mode & 0x03);
//    if ((status == KS_OK) && ((mode & DWT_TX_DELAYED) == DWT_TX_DELAYED))
//    {
//        do
//        {
//            status = dwt_getsystemstatus();
//        }
//        while (!(status & SYS_STATUS_TXFRS));
//        dwt_setsystemstatus(SYS_STATUS_TXFRS);
//    }
//    return status;
//}

#define RNG_DELAY_MS 1000
/* Frames used in the ranging process. See NOTE 3 below. */
static uint8 tx_poll_msg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'W', 'A', 'V', 'E', 0xE0, 0, 0};
static uint8 rx_resp_msg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'V', 'E', 'W', 'A', 0xE1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
/* Length of the common part of the message (up to and including the function code, see NOTE 3 below). */
#define ALL_MSG_COMMON_LEN 10
/* Indexes to access some of the fields in the frames defined above. */
#define ALL_MSG_SN_IDX 2
#define RESP_MSG_POLL_RX_TS_IDX 10
#define RESP_MSG_RESP_TX_TS_IDX 14
#define RESP_MSG_TS_LEN 4

/* Buffer to store received response message.
 * Its size is adjusted to longest frame that this example code is supposed to handle. */
#define INIT_RX_BUF_LEN 20
static uint8 rx_buffer[INIT_RX_BUF_LEN];

/* Delay between frames, in UWB microseconds. See NOTE 1 below. */
#define UWB_POLL_TX_TO_RESP_RX_DLY_UUS 140
/* Receive response timeout. See NOTE 5 below. */
#define UWB_RESP_RX_TIMEOUT_UUS 210

/* Speed of light in air, in metres per second. */
#define SPEED_OF_LIGHT 299702547

/* Hold copies of computed time of flight and distance here for reference so that it can be examined at a debug breakpoint. */
static double tof;
static double distance;

/* String used to display measured distance on LCD screen (16 characters maximum). */
char dist_str[16] = {0};



/**
 *  @brief  ex_06a_ss_twr_init
 */
void ex_06a_ss_twr_init( void )
{
    /* Frame sequence number, incremented after each transmission. */
    uint8_t frame_seq_nb = 0;
    uint32_t status;
    uint32_t frameLens;

    /* Set expected response's delay and timeout. See NOTE 1 and 5 below.
     * As this example only handles one incoming frame with always the same delay and timeout, those values can be set here once for all. */
    dwt_setrxaftertxdelay(UWB_POLL_TX_TO_RESP_RX_DLY_UUS);
    dwt_setrxtimeout(UWB_RESP_RX_TIMEOUT_UUS);

    /* Loop forever initiating ranging exchanges. */
    while (1)
    {
        /* Write frame data to DW1000 and prepare transmission. See NOTE 7 below. */
        tx_poll_msg[ALL_MSG_SN_IDX] = frame_seq_nb;
        dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_TXFRS);
        dwt_writetxdata(sizeof(tx_poll_msg), tx_poll_msg, 0); /* Zero offset in TX buffer. */
        dwt_writetxfctrl(sizeof(tx_poll_msg), 0, 1); /* Zero offset in TX buffer, ranging. */

        /* Start transmission, indicating that a response is expected so that reception is enabled automatically after the frame is sent and the delay
         * set by dwt_setrxaftertxdelay() has elapsed. */
        dwt_starttx(DWT_START_TX_IMMEDIATE | DWT_RESPONSE_EXPECTED);

        /* We assume that the transmission is achieved correctly, poll for reception of a frame or error/timeout. See NOTE 8 below. */
        while (!((status = dwt_read32bitreg(SYS_STATUS_ID)) & (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR)))
        { };

        /* Increment frame sequence number after transmission of the poll message (modulo 256). */
        frame_seq_nb++;

        if (status & SYS_STATUS_RXFCG)
        {
            /* Clear good RX frame event in the DW1000 status register. */
            dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_RXFCG);

            /* A frame has been received, read it into the local buffer. */
            frameLens = dwt_read32bitreg(RX_FINFO_ID) & RX_FINFO_RXFLEN_MASK;
            if (frameLens <= INIT_RX_BUF_LEN)
            {
                dwt_readrxdata(rx_buffer, frameLens, 0);
            }

            /* Check that the frame is the expected response from the companion "SS TWR responder" example.
             * As the sequence number field of the frame is not relevant, it is cleared to simplify the validation of the frame. */
            rx_buffer[ALL_MSG_SN_IDX] = 0;
            if (memcmp(rx_buffer, rx_resp_msg, ALL_MSG_COMMON_LEN) == 0)
            {
                uint32_t poll_tx_ts, resp_rx_ts, poll_rx_ts, resp_tx_ts;
                int32_t rtd_init, rtd_resp;
                float clockOffsetRatio;

                /* Retrieve poll transmission and response reception timestamps. See NOTE 9 below. */
                dwt_getinitiatortime(&poll_tx_ts, &resp_rx_ts);
//                poll_tx_ts = dwt_readtxtimestamplo32();
//                resp_rx_ts = dwt_readrxtimestamplo32();

                /* Read carrier integrator value and calculate clock offset ratio. See NOTE 11 below. */
                clockOffsetRatio = dwt_readcarrierintegrator() * (FREQ_OFFSET_MULTIPLIER * HERTZ_TO_PPM_MULTIPLIER_CHAN_2 / 1.0e6) ;

                /* Get timestamps embedded in response message. */
                resp_msg_get_ts(&rx_buffer[RESP_MSG_POLL_RX_TS_IDX], &poll_rx_ts);
                resp_msg_get_ts(&rx_buffer[RESP_MSG_RESP_TX_TS_IDX], &resp_tx_ts);

                /* Compute time of flight and distance, using clock offset ratio to correct for differing local and remote clock rates */
                rtd_init = resp_rx_ts - poll_tx_ts;
                rtd_resp = resp_tx_ts - poll_rx_ts;

                tof = (((double)rtd_init - rtd_resp * (1 - clockOffsetRatio)) / 2.0) * DWT_TIME_UNITS;
                distance = tof * SPEED_OF_LIGHT;

                /* Display computed distance on LCD. */
                sprintf(dist_str, "DIST: %3.2f m", distance);
            }
        }
        else
        {
            /* Clear RX error/timeout events in the DW1000 status register. */
            dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR);

            /* Reset RX to properly reinitialise LDE operation. */
            dwt_rxreset();
        }

        /* Execute a delay between ranging exchanges. */
        delay_ms(RNG_DELAY_MS);
    }
}

/**
 *  @brief  ex_06b_ss_twr_resp
 */

/* Frames used in the ranging process. See NOTE 3 below. */
static uint8 rx_poll_msg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'W', 'A', 'V', 'E', 0xE0, 0, 0};
static uint8 tx_resp_msg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'V', 'E', 'W', 'A', 0xE1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
/* Length of the common part of the message (up to and including the function code, see NOTE 3 below). */
#define ALL_MSG_COMMON_LEN 10
/* Index to access some of the fields in the frames involved in the process. */
#define ALL_MSG_SN_IDX 2
#define RESP_MSG_POLL_RX_TS_IDX 10
#define RESP_MSG_RESP_TX_TS_IDX 14
#define RESP_MSG_TS_LEN 4
/* Frame sequence number, incremented after each transmission. */

/* Buffer to store received messages.
 * Its size is adjusted to longest frame that this example code is supposed to handle. */
#define RESP_RX_BUF_LEN 12
static uint8 packet[RESP_RX_BUF_LEN];

/* Delay between frames, in UWB microseconds. See NOTE 1 below. */
#define UWB_POLL_RX_TO_RESP_TX_DLY_UUS  330

void ex_06b_ss_twr_resp( void )
{
    uint8_t frame_seq_nb = 0;

    /* Timestamps of frames transmission/reception.
     * As they are 40-bit wide, we need to define a 64-bit int type to handle them. */
    uint64_t poll_rx_ts;
    uint64_t resp_tx_ts;

    /* Hold copy of status register state here for reference so that it can be examined at a debug breakpoint. */
    uint32_t status;
    uint32_t frameLens;
    uint32_t lens = RX_BUFFER_LEN;

    while (1)
    {
        /* Activate reception immediately. */
        dwt_rxenable(DWT_START_RX_IMMEDIATE);

        /* Poll for reception of a frame or error/timeout. See NOTE 6 below. */
        while (!((status = dwt_getsystemstatus()) & (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_ERR)))
        { };

        if (status & SYS_STATUS_RXFCG)
        {
            /* Clear good RX frame event in the DW1000 status register. */
            dwt_setsystemstatus(SYS_STATUS_RXFCG);
            /* A frame has been received, read it into the local buffer. */
            frameLens = dwt_read32bitreg(RX_FINFO_ID) & RX_FINFO_RXFL_MASK_1023;
            if (frameLens <= lens)
            {
                dwt_readrxdata(packet, frameLens, 0);
            }

            /* Check that the frame is a poll sent by "SS TWR initiator" example.
             * As the sequence number field of the frame is not relevant, it is cleared to simplify the validation of the frame. */
            packet[ALL_MSG_SN_IDX] = 0;
            if (memcmp(packet, rx_poll_msg, ALL_MSG_COMMON_LEN) == 0)
            {
                uint32_t resp_tx_time;
                int ret;

                // Retrieve poll reception timestamp.
                // Compute final message transmission time. See NOTE 7 below.
                // Response TX timestamp is the transmission time we programmed plus the antenna delay.
                dwt_getrespondertime(&resp_tx_time, &poll_rx_ts, &resp_tx_ts);
                dwt_setdelayedtrxtime(resp_tx_time);

                /* Write all timestamps in the final message. See NOTE 8 below. */
                resp_msg_set_ts(&tx_resp_msg[RESP_MSG_POLL_RX_TS_IDX], poll_rx_ts);
                resp_msg_set_ts(&tx_resp_msg[RESP_MSG_RESP_TX_TS_IDX], resp_tx_ts);

                /* Write and send the response message. See NOTE 9 below. */
                tx_resp_msg[ALL_MSG_SN_IDX] = frame_seq_nb;
                dwt_writetxdata(sizeof(tx_resp_msg), tx_resp_msg, 0); /* Zero offset in TX buffer. */
                dwt_writetxfctrl(sizeof(tx_resp_msg), 0, 1); /* Zero offset in TX buffer, ranging. */
                ret = dwt_starttx(DWT_START_TX_DELAYED);

                /* If dwt_starttx() returns an error, abandon this ranging exchange and proceed to the next one. See NOTE 10 below. */
                if (ret == DWT_SUCCESS)
                {
                    /* Poll DW1000 until TX frame sent event set. See NOTE 6 below. */
                    while (!(dwt_getsystemstatus() & SYS_STATUS_TXFRS))
                    { };

                    /* Clear TXFRS event. */
                    dwt_setsystemstatus(SYS_STATUS_TXFRS);

                    /* Increment frame sequence number after transmission of the poll message (modulo 256). */
                    frame_seq_nb++;
                }
            }
        }
        else
        {
            /* Clear RX error events in the DW1000 status register. */
            dwt_setsystemstatus(SYS_STATUS_ALL_RX_ERR);
            /* Reset RX to properly reinitialise LDE operation. */
            dwt_rxreset();
        }
    }
}

uint64_t dwt_getrxtimestamp64(void)
{
    uint8_t buf[5];
    uint64_t ts = 0;
    dwt_readrxtimestamp(buf);
    for (int32_t i = 4; i >= 0; i--)
    {
        ts <<= 8;
        ts |= buf[i];
    }
    return ts;
}

int32_t dwt_getrespondertime(uint32_t *txtime, uint64_t *rxtimestamp, uint64_t *txtimestamp)
{
    *rxtimestamp = dwt_getrxtimestamp64();
    *txtime = (*rxtimestamp + (UWB_POLL_RX_TO_RESP_TX_DLY_UUS * UWB_UUS_TO_DWT_TIME)) >> 8;
    *txtimestamp = (((uint64_t)(*txtime & 0xFFFFFFFE)) << 8) + UWB_TX_ANT_DELAY;
    return (*txtimestamp - *rxtimestamp);
}

int32_t dwt_getinitiatortime(uint32_t *txtimestamp, uint32_t *rxtimestamp)
{
    /* Retrieve poll transmission and response reception timestamps. See NOTE 9 below. */
    *txtimestamp = dwt_readtxtimestamplo32();
    *rxtimestamp = dwt_readrxtimestamplo32();
    return (*rxtimestamp - *txtimestamp);
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn final_msg_set_ts()
 *
 * @brief Fill a given timestamp field in the response message with the given value. In the timestamp fields of the
 *        response message, the least significant byte is at the lower address.
 *
 * @param  ts_field  pointer on the first byte of the timestamp field to fill
 *         ts  timestamp value
 *
 * @return none
 */
static void resp_msg_set_ts(uint8_t *ts_field, const uint64_t ts)
{
    int i;
    for (i = 0; i < RESP_MSG_TS_LEN; i++)
    {
        ts_field[i] = (ts >> (i * 8)) & 0xFF;
    }
}
/*! ------------------------------------------------------------------------------------------------------------------
 * @fn resp_msg_get_ts()
 *
 * @brief Read a given timestamp value from the response message. In the timestamp fields of the response message, the
 *        least significant byte is at the lower address.
 *
 * @param  ts_field  pointer on the first byte of the timestamp field to get
 *         ts  timestamp value
 *
 * @return none
 */
static void resp_msg_get_ts(uint8_t *ts_field, uint32_t *ts)
{
    int i;
    *ts = 0;
    for (i = 0; i < RESP_MSG_TS_LEN; i++)
    {
        *ts += ts_field[i] << (i * 8);
    }
}

/*************************************** END OF FILE ****************************************/
