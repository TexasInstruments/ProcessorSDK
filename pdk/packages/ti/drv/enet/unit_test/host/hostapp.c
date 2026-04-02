/* ======================================================================
 *   Copyright (C) 2020 Texas Instruments Incorporated
 *
 *   All rights reserved. Property of Texas Instruments Incorporated.
 *   Restricted rights to use, duplicate or disclose this code are
 *   granted through contract.
 *
 *   The program may not be used without the written permission
 *   of Texas Instruments Incorporated or against the terms and conditions
 *   stipulated in the agreement under which this program has been
 *   supplied.
 * ====================================================================
 */

/**
 *  \file     hostapp.c
 *
 *  \brief    This file contains the host-side Eth test application code.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>

#include "../../examples/utils/include/enet_ethutils.h"
#include "../utils/test_common.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define OCTETS_PER_ROW                  8

#define HOST_APP_VERBOSE_OCTETS         16

#define HOST_APP_SOCKET_BUFSIZE         (512 * 1024)

#define ENET_ARRAYSIZE(x)                     (sizeof(x) / sizeof(x[0]))

typedef bool (*TestFunc)(void);

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

typedef struct
{
    int32_t sock;
    char *ifName;
    uint8_t hwAddr[ETH_MAC_ADDR_LEN];
    uint8_t dutAddr[ETH_MAC_ADDR_LEN];
    uint32_t timeout;
    struct ifreq ifIdx;
    struct ifreq ifMac;
    struct sockaddr_ll sockAddr;
    bool verbose;
    bool veryVerbose;
} HostApp;

typedef struct
{
    uint32_t len;
    EthFrame frame;
} TestFrame;

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

void help(const char *me);

int32_t HostApp_open(const char *ifName);

void HostApp_close(void);

void HostApp_send(EthFrame *frame,
                  uint32_t len);

void HostApp_transmit(TestFrame *frames,
                      uint32_t testFramesNum,
                      uint32_t num);

bool HostApp_recv(EthFrame *frame,
                  uint32_t *len);

bool HostApp_receive(uint32_t *frames);

bool HostApp_receiveTput(uint32_t frames);

void HostApp_waitForCmd(uint8_t cmd);

void HostApp_sendCmd(uint8_t cmd);

float HostApp_timeDiff(struct timeval t1,
                       struct timeval t0);

/**
 * \brief Test basic frame reception
 *
 * Basic DUT frame reception test.  The host side performs these operations:
 * - Wait for START command from DUT
 * - Transmit ETH_TEST_ITER_M_COUNT frames
 *
 * \return PASS or FAIL  Test result
 */
bool HostApp_test_0001(void);

/**
 * \brief Test basic frame transmission
 *
 * Basic DUT frame transmission test.  The host side performs these operations:
 * - Wait for START command from DUT
 * - Receive frames until STOP command is detected
 *
 * The number of frames expected to be received is ETH_TEST_ITER_M_COUNT.
 *
 * \return PASS or FAIL  Test result
 */
bool HostApp_test_0002(void);

/**
 * \brief Test external loopback
 *
 * External loopback test.  The host side receives frames from the DUT and
 * sends them back.  The following operations are performed:
 * - Wait for START command from DUT
 * - Loop back all frames received until a STOP command is detected
 *   o One frame is received
 *   o Source and destination MAC addresses are swapped
 *   o The frame is sent back
 *
 * \return PASS or FAIL  Test result
 */
bool HostApp_test_0003(void);

/**
 * \brief Measure DUT transmit throughput
 *
 * Measure DUT transmit throughput over ETH_TEST_ITER_L_COUNT frames.
 * The host performs the following operations:
 * - Wait for START command from DUT
 * - Receive ETH_TEST_ITER_L_COUNT or until the STOP cmd is detected
 * - Compute the elapsed time, packets per second and Mbps
 *
 * \return PASS or FAIL  Test result
 */
bool HostApp_test_0100(void);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

TestFunc testFuncs[] =
{
    &HostApp_test_0001,
    &HostApp_test_0002,
    &HostApp_test_0100,
    // &HostApp_test_0003,
};

HostApp gHostApp =
{
    .dutAddr = {0xf4, 0x84, 0x4c, 0xeb, 0x95, 0x09},
};

EthFrame rxFrame;

CtrlFrame ctrlFrame;

/* Unicast header with dst address of DUT's */
EthFrameHeader hdrUcastCtrl =
{
    .dstMac    = {0xf4, 0x84, 0x4c, 0xeb, 0x95, 0x09},
    .srcMac    = {0x00},   /* to be populated later */
    .etherType = htons(ETHERTYPE_EXPERIMENTAL1),
};

/* Unicast header with dst address to be allowed by DUT's filter */
EthFrameHeader hdrUcastVal =
{
    .dstMac    = {0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f},
    .srcMac    = {0x00},  /* to be populated later */
    .etherType = htons(ETHERTYPE_EXPERIMENTAL2),
};

/* Unicast header with dst address to be rejected by DUT's filter */
EthFrameHeader hdrUcastInv =
{
    .dstMac    = {0x08, 0x00, 0x28, 0x01, 0xf6, 0x7c},
    .srcMac    = {0x00},  /* to be populated later */
    .etherType = htons(ETHERTYPE_EXPERIMENTAL2),
};

/* Broadcast header */
EthFrameHeader hdrBcast =
{
    .dstMac    = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
    .srcMac    = {0x00},  /* to be populated later */
    .etherType = htons(ETHERTYPE_EXPERIMENTAL1),
};

/* Multicast header */
EthFrameHeader hdrMcast =
{
    .dstMac    = {0x01, 0x00, 0x5e, 0x0d, 0x0e, 0x0f},
    .srcMac    = {0x00},  /* to be populated later */
    .etherType = htons(ETHERTYPE_EXPERIMENTAL2),
};

/* Test control (broadcast) header */
EthFrameHeader hdrCtrl =
{
    .dstMac    = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
    .srcMac    = {0x00},  /* to be populated later */
    .etherType = htons(ETHERTYPE_EXP_CONTROL),
};

static const struct option long_options[] =
{
    {"timeout", 1, 0, 't'},           /* time-out */
    {"iface", 1, 0, 'i'},             /* interface number */
    {"verbose", 0, 0, 'v'},           /* verbose output */
    {"very-verbose", 0, 0, 'V'},      /* very verbose output */
    {"help", 0, 0, 'h'},              /* show help */
};

static const char short_options[] = "t:p:i:vh";

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int main(int argc,
         char *argv[])
{
    struct timeval timeout;
    const char *action;
    const char *me = argv[0];
    char *ifName   = "eth0";
    bool status;
    long int val;
    int32_t option;
    int32_t c;
    int32_t i, iteration;
    int32_t ret;

    gHostApp.verbose     = BFALSE;
    gHostApp.veryVerbose = BFALSE;
    gHostApp.timeout     = 10;

    while (1)
    {
        c = getopt_long(argc, argv, short_options, long_options, &option);
        if (c == -1)
        {
            break;
        }

        switch (c)
        {
            case 't':
                gHostApp.timeout = strtol(optarg, NULL, 0);
                break;

            case 'i':
                ifName = optarg;
                break;

            case 'v':
                gHostApp.verbose = BTRUE;
                break;

            case 'V':
                gHostApp.verbose     = BTRUE;
                gHostApp.veryVerbose = BTRUE;
                break;

            case 'h':
                help(me);
                return 0;

            default:
                printf("Invalid option %d\n", option);
                return -EINVAL;
        }
    }

    ret = HostApp_open(ifName);
    if (ret)
    {
        printf("Failed to open and init raw socket: %d\n", ret);
        return ret;
    }

    /* Wait for DUT */
    for (iteration = 0; iteration < SANITY_APP_NUM_ITERATION; iteration++)
    {
        printf("Waiting for DUT..\n");

        HostApp_waitForCmd(CTRL_FRAME_CMD_DUT_READY);

        for (i = 0; i < ENET_ARRAYSIZE(testFuncs); i++)
        {
            printf("\n\n-----------------------------------------------------------\n");
            status = testFuncs[i]();
            printf("Test Result: %s\n", status ? "Pass" : "Fail");
            printf("-----------------------------------------------------------\n");
        }
    }

    HostApp_close();

    return 0;
}

void help(const char *me)
{
    printf("Usage: %s TEST [OPTION]...\n"
           "TEST                    throughput or count\n"
           "-h, --help              help\n"
           "-i, --iface=#           interface number (i.e. 0 for eth0)"
           "-t, --timeout=#         time-out (in secs)\n"
           "-v, --verbose           verbose output\n\n"
           "Note: It's suggested to increase the number of receive memory\n"
           "  $ sudo sh -c \"echo 917504 > /proc/sys/net/core/rmem_max\"\n"
           "  $ %s count -t 20\n\n",
           me, me);
}

uint32_t received;

void sigint_handler(int sig)
{
    printf("Received: %d\n", received);
}

int32_t HostApp_open(const char *ifName)
{
    struct timeval timeout;
    uint32_t tout = 10;
    uint32_t size;
    uint32_t i;
    int32_t ret;

    /* Open a raw socket */
    ret = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (ret > 0)
    {
        gHostApp.sock = ret;
    }
    else
    {
        printf("Open: failed to open raw socket: %s\n", strerror(errno));
        ret = -errno;
        return ret;
    }

    /* Set a timeout */
    timeout.tv_sec  = tout;
    timeout.tv_usec = 0;
    ret             = setsockopt(gHostApp.sock, SOL_SOCKET, SO_RCVTIMEO,
                                 (const char *)&timeout, sizeof(timeout));
    if (ret)
    {
        printf("Open: failed to set timeout: %s\n", strerror(errno));
        goto open_err;
    }

    /* Increase the socket buffer size */
    size = HOST_APP_SOCKET_BUFSIZE;
    ret  = setsockopt(gHostApp.sock, SOL_SOCKET, SO_RCVBUF,
                      &size, sizeof(size));
    if (ret)
    {
        printf("Open: failed to socket buffer size: %s\n", strerror(errno));
        goto open_err;
    }

    /* Get interface index */
    memset(&gHostApp.ifIdx, 0, sizeof(struct ifreq));
    strncpy(gHostApp.ifIdx.ifr_name, ifName, IFNAMSIZ - 1);
    ret = ioctl(gHostApp.sock, SIOCGIFINDEX, &gHostApp.ifIdx);
    if (ret)
    {
        printf("Open: failed to get interface index: %s\n", strerror(errno));
        goto open_err;
    }

    /* Get interface MAC address */
    memset(&gHostApp.ifMac, 0, sizeof(struct ifreq));
    strncpy(gHostApp.ifMac.ifr_name, ifName, IFNAMSIZ - 1);
    ret = ioctl(gHostApp.sock, SIOCGIFHWADDR, &gHostApp.ifMac);
    if (ret)
    {
        printf("Open: failed to get interface MAC address: %s\n", strerror(errno));
        goto open_err;
    }

    /* Save host's MAC address */
    for (i = 0; i < ETH_MAC_ADDR_LEN; i++)
    {
        gHostApp.hwAddr[i] = gHostApp.ifMac.ifr_hwaddr.sa_data[i];
    }

    gHostApp.sockAddr.sll_ifindex = gHostApp.ifIdx.ifr_ifindex;
    gHostApp.sockAddr.sll_halen   = ETH_ALEN;
    memset(gHostApp.sockAddr.sll_addr, 0, ETH_MAC_ADDR_LEN);

    printf("Interface    : %s\n", ifName);
    printf("MAC address  : %02x:%02x:%02x:%02x:%02x:%02x\n",
           gHostApp.hwAddr[0] & 0xFF, gHostApp.hwAddr[1] & 0xFF,
           gHostApp.hwAddr[2] & 0xFF, gHostApp.hwAddr[3] & 0xFF,
           gHostApp.hwAddr[4] & 0xFF, gHostApp.hwAddr[5] & 0xFF);
    printf("\n\n");

    /* Update test packet headers with host's MAC address */
    memcpy(hdrUcastCtrl.srcMac, gHostApp.hwAddr, ETH_MAC_ADDR_LEN);
    memcpy(hdrUcastVal.srcMac, gHostApp.hwAddr, ETH_MAC_ADDR_LEN);
    memcpy(hdrUcastInv.srcMac, gHostApp.hwAddr, ETH_MAC_ADDR_LEN);
    memcpy(hdrBcast.srcMac, gHostApp.hwAddr, ETH_MAC_ADDR_LEN);
    memcpy(hdrMcast.srcMac, gHostApp.hwAddr, ETH_MAC_ADDR_LEN);
    memcpy(hdrCtrl.srcMac, gHostApp.hwAddr, ETH_MAC_ADDR_LEN);
    memcpy(&ctrlFrame.hdr, &hdrCtrl, ETH_HDR_LEN);

    return 0;

open_err:
    close(gHostApp.sock);
    gHostApp.sock = -1;
    ret           = -errno;
    return ret;
}

void HostApp_close(void)
{
    close(gHostApp.sock);
    gHostApp.sock = -1;
}

void HostApp_send(EthFrame *frame,
                  uint32_t len)
{
    ssize_t sent;

    sent = sendto(gHostApp.sock,
                  frame, len, 0,
                  (struct sockaddr *)&gHostApp.sockAddr,
                  sizeof(struct sockaddr_ll));

    if (sent < 0)
    {
        printf("Send: failed to transmit packet: %s\n", strerror(errno));
    }
    else if (sent != len)
    {
        printf("Send: short transmission (req %d, got %d)\n", len, (int)sent);
    }
}

void HostApp_transmit(TestFrame *frames,
                      uint32_t testFramesNum,
                      uint32_t num)
{
    TestFrame *frame;
    uint32_t sent = 0;

    while (BTRUE)
    {
        frame = &frames[sent % testFramesNum];

        HostApp_send(&frame->frame, frame->len);

        if (gHostApp.veryVerbose)
        {
            HostappUtils_printFrame(&frame->frame, HOST_APP_VERBOSE_OCTETS);
        }

        usleep(1000);

        if (++sent >= num)
        {
            break;
        }
    }
}

bool HostApp_recv(EthFrame *frame,
                  uint32_t *len)
{
    ssize_t bytes;
    bool status = PASS;

    memset(frame, 0, sizeof(EthFrame));

    bytes = recv(gHostApp.sock, frame, sizeof(EthFrame), 0);
    if (bytes < 0)
    {
        printf("Recv: failed to receive data over raw socket: %s\n", strerror(errno));
        *len   = 0;
        status = FAIL;
    }
    else
    {
        *len = bytes;
    }

    return status;
}

bool HostApp_receive(uint32_t *frames)
{
    long long total = 0;
    uint32_t len;
    uint32_t i = 0;
    int32_t ret;
    bool forever = BFALSE;
    bool status  = PASS;

    /* Receive packets indefinitely if requested frame count is 0 */
    if (*frames == 0)
    {
        forever = BTRUE;
    }

    /* Receive frames */
    while (BTRUE)
    {
        /* Receive one frame */
        status = HostApp_recv(&rxFrame, &len);
        if (status == FAIL)
        {
            printf("Receive: failed to receive packet: %d\n", ret);
            break;
        }

        if (gHostApp.veryVerbose)
        {
            HostappUtils_printFrame(&rxFrame, HOST_APP_VERBOSE_OCTETS);
        }

        /* Ignore frames with other EtherTypes */
        if ((rxFrame.hdr.etherType != htons(ETHERTYPE_EXPERIMENTAL1)) &&
            (rxFrame.hdr.etherType != htons(ETHERTYPE_EXPERIMENTAL2)) &&
            (rxFrame.hdr.etherType != htons(ETHERTYPE_EXP_CONTROL)) &&
            (rxFrame.hdr.etherType != htons(ETHERTYPE_VLAN_TAG)))
        {
            continue;
        }

        /* Check if it's a STOP cmd */
        if (EthFrame_isStopCmd(&rxFrame))
        {
            if (gHostApp.verbose)
            {
                printf("Receive: STOP command received\n");
            }

            break;
        }

        /* Verify frame content */
        if (EthFrame_isVlanTagged(&rxFrame))
        {
            ret = HostappUtils_checkVlanPayload((VlanDataFramePayload *)rxFrame.payload);
        }
        else
        {
            ret = HostappUtils_checkPayload((DataFramePayload *)rxFrame.payload);
        }

        if (ret != ETH_TEST_PKT_SOK)
        {
            printf("Receive: frame %d is not valid: %d\n", i + 1, ret);
            HostappUtils_printFrame(&rxFrame, len);
            status = FAIL;
            break;
        }

        total += len;
        i++;
        received = i;

        if ((!forever) && (i == *frames))
        {
            break;
        }
    }

    /* Update with the number of frames received */
    *frames = i;

    if (gHostApp.verbose)
    {
        printf("Receive: received %d frames, %lld bytes\n", i, total);
    }

    return status;
}

bool HostApp_receiveTput(uint32_t frames)
{
    struct timeval t0, t1;
    float elapsed;
    float pps;
    float mbps;
    long long total = 0;
    ssize_t bytes;
    uint32_t i;

    /* Receive frames until STOP cmd is detected */
    for (i = 0; i < frames; i++)
    {
        bytes = recv(gHostApp.sock, &rxFrame, sizeof(EthFrame), 0);
        if (bytes < 0)
        {
            printf("ReceiveTput: failed to received data: %s\n", strerror(errno));
            return FAIL;
        }

        /* Get the "start" timestamp only after the first frame has been received */
        if (i == 0)
        {
            gettimeofday(&t0, NULL);
        }

        /* Check if it's a STOP cmd */
        if (EthFrame_isStopCmd(&rxFrame))
        {
            frames = i;
            break;
        }

        total += bytes;
    }

    /* Get the "end" timestamp after DUT is done sending frames */
    gettimeofday(&t1, NULL);

    /* Compute elapsed time, packets per second and Mbps */
    elapsed = HostApp_timeDiff(t1, t0);
    pps     = (float)frames / elapsed;
    mbps    = (float)total * 8 / elapsed / 1000000;

    printf("ReceiveTput: received %d frames in %.2f secs (%.2f frames/s, %.2f Mbps)\n",
           frames, elapsed, pps, mbps);

    return PASS;
}

void HostApp_sendCmd(uint8_t cmd)
{
    EthFrame *frame = (EthFrame *)&ctrlFrame;

    ctrlFrame.payload.cmd = cmd;
    memset(ctrlFrame.payload.data.payload, 0, sizeof(ctrlFrame.payload.data));

    if (gHostApp.verbose)
    {
        switch (cmd)
        {
            case CTRL_FRAME_CMD_PC_READY:
                printf("SendCmd: READY cmd\n");
                break;

            case CTRL_FRAME_CMD_START:
                printf("SendCmd: START cmd\n");
                break;

            case CTRL_FRAME_CMD_STOP:
                printf("SendCmd: STOP cmd\n");
                break;

            default:
                printf("SendCmd: invalid cmd\n");
                break;
        }
    }

    HostApp_send(frame, sizeof(CtrlFrame));
}

void HostApp_waitForCmd(uint8_t cmd)
{
    CtrlFrame *frame = (CtrlFrame *)&rxFrame;
    ssize_t bytes;

    printf("Waiting for DUT command\n");

    /* Receive frames until a command frame is detected */
    while (BTRUE)
    {
        /* Receive one frame */
        memset(frame, 0, sizeof(EthFrame));
        bytes = recv(gHostApp.sock, frame, sizeof(EthFrame), 0);
        if (bytes < 0)
        {
            if (errno == EAGAIN)
            {
                continue;
            }
            else
            {
                /* Exit the application since host and DUT are no longer in sync */
                printf("WaitForCmd: errno %d: %s\n", errno, strerror(errno));
                exit(1);
            }
        }

        /* Check if it's a control frame and take action depending on the type */
        if ((frame->hdr.etherType == ntohs(ETHERTYPE_EXP_CONTROL)) &&
            (frame->payload.cmd == cmd))
        {
            switch (cmd)
            {
                case CTRL_FRAME_CMD_START:
                    if (gHostApp.verbose)
                    {
                        printf("WaitForCmd: START cmd received\n");
                    }

                    break;

                case CTRL_FRAME_CMD_DUT_READY:
                default:
                    memcpy(gHostApp.dutAddr, frame->hdr.srcMac, ETH_MAC_ADDR_LEN);
                    memcpy(hdrUcastCtrl.dstMac, gHostApp.dutAddr, ETH_MAC_ADDR_LEN);
                    printf("DUT detected: %02x:%02x:%02x:%02x:%02x:%02x\n",
                           gHostApp.dutAddr[0] & 0xFF, gHostApp.dutAddr[1] & 0xFF,
                           gHostApp.dutAddr[2] & 0xFF, gHostApp.dutAddr[3] & 0xFF,
                           gHostApp.dutAddr[4] & 0xFF, gHostApp.dutAddr[5] & 0xFF);
                    break;
            }

            break;
        }
    }

    printf("DUT command received\n");
}

float HostApp_timeDiff(struct timeval t1,
                       struct timeval t0)
{
    struct timeval temp;
    float elapsed;

    if ((t1.tv_usec - t0.tv_usec) < 0)
    {
        temp.tv_sec  = t1.tv_sec - t0.tv_sec - 1;
        temp.tv_usec = 1000000UL + t1.tv_usec - t0.tv_usec;
    }
    else
    {
        temp.tv_sec  = t1.tv_sec - t0.tv_sec;
        temp.tv_usec = t1.tv_usec - t0.tv_usec;
    }

    elapsed = (float)temp.tv_sec + (float)temp.tv_usec / 1000000;

    return elapsed;
}

bool HostApp_test_0001(void)
{
    uint32_t iterations = ETH_TEST_ITER_M_COUNT;
    TestFrame frames[1];
    DataFramePayload *payload;
    uint16_t len;
    uint32_t i;
    int32_t ret;
    bool status = PASS;

    printf("Test_0001: START\n");

    /* Unicast packet with DUT's MAC address */
    frames[0].len = 100;
    payload       = (DataFramePayload *)frames[0].frame.payload;
    memcpy(&frames[0].frame.hdr, &hdrUcastCtrl, ETH_HDR_LEN);

    /* Wait for DUT to start the test when it's ready */
    HostApp_waitForCmd(CTRL_FRAME_CMD_START);

    /* Transmit one frame per iteration */
    for (i = 0; i < iterations; i++)
    {
        if (gHostApp.verbose)
        {
            printf("Test_0001: iteration: %d of %d\n", i + 1, iterations);
        }

        /* Unicast frame with DUT's MAC address */
        len = frames[0].len - ETH_HDR_LEN;
        ret = HostappUtils_fillPayload(payload, i % ETH_TEST_NUM_TYPES, len);
        if (ret)
        {
            printf("Test_0001: failed to create test packet: %d\n", ret);
            status = FAIL;
            break;
        }

        /* Transmit one frame */
        HostApp_transmit(frames, ENET_ARRAYSIZE(frames), 1);
    }

    /* Indicate to the DUT that the test is complete */
    HostApp_sendCmd(CTRL_FRAME_CMD_STOP);

    printf("Test_0001: transmitted %d of %d frames\n", i, iterations);
    printf("Test_0001: END\n");

    return status;
}

bool HostApp_test_0002(void)
{
    uint32_t recvNum = 0;
    bool st;
    bool status = PASS;

    printf("Test_0002: START\n");

    /* Wait for DUT to start the test when it's ready */
    HostApp_waitForCmd(CTRL_FRAME_CMD_START);

    /* Receive packets indefinitely until STOP cmd is detected */
    st = HostApp_receive(&recvNum);
    if (st == FAIL)
    {
        printf("Test_0002: failed while receiving packets\n");
        status = FAIL;
    }

    /* Check that all packets were received */
    if (recvNum != ETH_TEST_ITER_M_COUNT)
    {
        printf("Test_0002: received frame count mismatch (exp=%d, got=%d)\n",
               ETH_TEST_ITER_M_COUNT, recvNum);
        status = FAIL;
    }

    printf("Test_0002: received %d of %d frames\n", recvNum, ETH_TEST_ITER_M_COUNT);
    printf("Test_0002: END\n");

    return status;
}

bool HostApp_test_0003(void)
{
    uint32_t len;
    uint32_t cnt = 0;
    bool status;

    printf("Test_0003: START\n");

    /* Wait for DUT to start the test when it's ready */
    HostApp_waitForCmd(CTRL_FRAME_CMD_START);

    /* Loop frames back to the DUT until STOP cmd is received */
    while (BTRUE)
    {
        if (gHostApp.verbose)
        {
            printf("Test_0003: iteration %d\n", cnt + 1);
        }

        /* Receive one frame */
        len    = sizeof(rxFrame);
        status = HostApp_recv(&rxFrame, &len);
        if (status == FAIL)
        {
            printf("Test_0003: failed to receive frame\n");
            HostApp_sendCmd(CTRL_FRAME_CMD_STOP);
            break;
        }

        if (gHostApp.verbose)
        {
            printf("Test_0003: frame received (%d bytes)\n", len);
            if (gHostApp.veryVerbose)
            {
                HostappUtils_printFrame(&rxFrame, HOST_APP_VERBOSE_OCTETS);
            }
        }

        /* End the test if STOP cmd is received */
        if (EthFrame_isStopCmd(&rxFrame))
        {
            break;
        }

        /* Swap src and dst MAC addresses */
        memcpy(rxFrame.hdr.dstMac, rxFrame.hdr.srcMac, ETH_MAC_ADDR_LEN);
        memcpy(rxFrame.hdr.srcMac, gHostApp.hwAddr, ETH_MAC_ADDR_LEN);

        if (gHostApp.verbose)
        {
            printf("Test_0003: frame to be sent (%d bytes)\n", len);
            if (gHostApp.veryVerbose)
            {
                HostappUtils_printFrame(&rxFrame, HOST_APP_VERBOSE_OCTETS);
            }
        }

        /* Send the frame back */
        HostApp_send(&rxFrame, len);
        cnt++;

        if (gHostApp.verbose)
        {
            printf("Test_0003: iteration %d complete\n", cnt);
        }
    }

    printf("Test_0003: looped back %d frames\n", cnt);
    printf("Test_0003: END\n");

    return status;
}

bool HostApp_test_0004(void)
{
    uint32_t iterations = ETH_TEST_ITER_M_COUNT / 2;
    TestFrame frames[2];
    DataFramePayload *payload;
    uint16_t len;
    uint32_t i;
    int32_t ret;
    bool status = PASS;

    printf("Test_0004: START\n");

    /* Unicast packet with DUT's MAC address */
    frames[0].len = 100;
    memcpy(&frames[0].frame.hdr, &hdrUcastCtrl, ETH_HDR_LEN);

    /* Unicast packet with MAC address to be rejected by DUT's filter */
    frames[1].len = 200;
    memcpy(&frames[1].frame.hdr, &hdrUcastInv, ETH_HDR_LEN);

    /* Wait for DUT to start the test when it's ready */
    HostApp_waitForCmd(CTRL_FRAME_CMD_START);

    /* Transmit two frames per iteration */
    for (i = 0; i < iterations; i++)
    {
        if (gHostApp.verbose)
        {
            printf("Test_0004: iteration: %d of %d\n", i + 1, iterations);
        }

        /* Unicast packet with DUT's MAC address */
        payload = (DataFramePayload *)frames[0].frame.payload;
        len     = frames[0].len - ETH_HDR_LEN;
        ret     = HostappUtils_fillPayload(payload, i % ETH_TEST_NUM_TYPES, len);
        if (ret)
        {
            printf("Test_0004: failed to create test packet: %d\n", ret);
            status = FAIL;
            break;
        }

        /* Unicast packet with MAC address to be rejected by DUT's filter */
        payload = (DataFramePayload *)frames[1].frame.payload;
        len     = frames[1].len - ETH_HDR_LEN;
        ret     = HostappUtils_fillPayload(payload, i % ETH_TEST_NUM_TYPES, len);
        if (ret)
        {
            printf("Test_0004: failed to create test packet: %d\n", ret);
            status = FAIL;
            break;
        }

        /* Transmit the two packets */
        HostApp_transmit(frames, ENET_ARRAYSIZE(frames), ENET_ARRAYSIZE(frames));
    }

    /* Indicate to the DUT that the test is complete */
    HostApp_sendCmd(CTRL_FRAME_CMD_STOP);

    printf("Test_0004: completed %d of %d iterations\n", i, iterations);
    printf("Test_0004: END\n");

    return status;
}

bool HostApp_test_0005(void)
{
    TestFrame frames[2];
    DataFrame *frame;
    uint32_t num = ETH_TEST_ITER_M_COUNT;
    uint16_t len;
    bool status = PASS;

    printf("Test_0005: START\n");

    len   = frames[0].len = 1500;
    frame = (DataFrame *)&frames[0].frame;
    memcpy(&frame->hdr, &hdrUcastCtrl, ETH_HDR_LEN);
    HostappUtils_fillPayload(&frame->payload,
                             ETH_TEST_TYPE_PATTERN_1,
                             len - ETH_HDR_LEN);

    len   = frames[1].len = 1500;
    frame = (DataFrame *)&frames[1].frame;

    /* Part 1: DUT's filter has been reset. Only packets with DUT's address
     *         should be accepted */
    memcpy(&frame->hdr, &hdrUcastInv, ETH_HDR_LEN);
    HostappUtils_fillPayload(&frame->payload,
                             ETH_TEST_TYPE_PATTERN_2,
                             len - ETH_HDR_LEN);

    HostApp_waitForCmd(CTRL_FRAME_CMD_START);
    HostApp_transmit(frames, ENET_ARRAYSIZE(frames), num);
    HostApp_sendCmd(CTRL_FRAME_CMD_STOP);

    /* Part 2: An unicast address has been added to the DUT's filter. Packets
     *         with new address as well as DUT's address should be accepted */
    memcpy(&frame->hdr, &hdrUcastVal, ETH_HDR_LEN);
    HostappUtils_fillPayload(&frame->payload,
                             ETH_TEST_TYPE_PATTERN_3,
                             len - ETH_HDR_LEN);

    HostApp_waitForCmd(CTRL_FRAME_CMD_START);
    HostApp_transmit(frames, ENET_ARRAYSIZE(frames), num);
    HostApp_sendCmd(CTRL_FRAME_CMD_STOP);

    /* Part 3: A multicast address has been added to the DUT's filter. Packets
     *         with the multicast address as well as DUT's address should be
     *         accepted */
    memcpy(&frame->hdr, &hdrMcast, ETH_HDR_LEN);
    HostappUtils_fillPayload(&frame->payload,
                             ETH_TEST_TYPE_PATTERN_2,
                             len - ETH_HDR_LEN);

    HostApp_waitForCmd(CTRL_FRAME_CMD_START);
    HostApp_transmit(frames, ENET_ARRAYSIZE(frames), num);
    HostApp_sendCmd(CTRL_FRAME_CMD_STOP);

    /* Part 4: The multicast address has been removed from the DUT's filter.
     *         Only packets with DUT's address should be accepted */
    memcpy(&frame->hdr, &hdrMcast, ETH_HDR_LEN);
    HostappUtils_fillPayload(&frame->payload,
                             ETH_TEST_TYPE_PATTERN_3,
                             len - ETH_HDR_LEN);

    HostApp_waitForCmd(CTRL_FRAME_CMD_START);
    HostApp_transmit(frames, ENET_ARRAYSIZE(frames), num);
    HostApp_sendCmd(CTRL_FRAME_CMD_STOP);

    /* Part 5: DUT's filter is open, all packets should be accepted */
    memcpy(&frame->hdr, &hdrUcastInv, ETH_HDR_LEN);
    HostappUtils_fillPayload(&frame->payload,
                             ETH_TEST_TYPE_PATTERN_4,
                             len - ETH_HDR_LEN);

    HostApp_waitForCmd(CTRL_FRAME_CMD_START);
    HostApp_transmit(frames, ENET_ARRAYSIZE(frames), num);
    HostApp_sendCmd(CTRL_FRAME_CMD_STOP);

    /* Part 5: DUT's filter has been reset. Only packets with DUT's address
     *         should be accepted */
    memcpy(&frame->hdr, &hdrUcastInv, ETH_HDR_LEN);
    HostappUtils_fillPayload(&frame->payload,
                             ETH_TEST_TYPE_PATTERN_2,
                             len - ETH_HDR_LEN);

    HostApp_waitForCmd(CTRL_FRAME_CMD_START);
    HostApp_transmit(frames, ENET_ARRAYSIZE(frames), num);
    HostApp_sendCmd(CTRL_FRAME_CMD_STOP);

    printf("Test_0005: END\n");

    return PASS;
}

bool HostApp_test_0006(void)
{
    uint32_t recvNum = 0;
    bool status;

    printf("Test_0006: START\n");

    /* Wait for DUT to start the test when it's ready */
    HostApp_waitForCmd(CTRL_FRAME_CMD_START);

    /* Receive packets indefinitely until STOP cmd is detected */
    status = HostApp_receive(&recvNum);
    if (status == FAIL)
    {
        printf("Test_0006: failed while receiving packets\n");
    }

    /* Check that all packets were received */
    if (recvNum != ETH_TEST_ITER_M_COUNT)
    {
        printf("Test_0006: received frame count mismatch (exp=%d, got=%d)\n",
               ETH_TEST_ITER_M_COUNT, recvNum);
        status = FAIL;
    }

    printf("Test_0006: received %d of %d frames\n", recvNum, ETH_TEST_ITER_M_COUNT);
    printf("Test_0006: END\n");

    return status;
}

bool HostApp_test_0007(void)
{
    uint32_t recvNum = 0;
    bool st;
    bool status = PASS;

    printf("Test_0007: START\n");

    /* Wait for DUT to start the test when it's ready */
    HostApp_waitForCmd(CTRL_FRAME_CMD_START);

    /* Receive packets indefinitely until STOP cmd is detected */
    st = HostApp_receive(&recvNum);
    if (st == FAIL)
    {
        printf("Test_0007: failed while receiving packets\n");
        status = FAIL;
    }

    /* Check that all packets were received */
    if (recvNum != ETH_TEST_ITER_M_COUNT)
    {
        printf("Test_0007: received frame count mismatch (exp=%d, got=%d)\n",
               ETH_TEST_ITER_M_COUNT, recvNum);
        status = FAIL;
    }

    printf("Test_0007: received %d of %d frames\n", recvNum, ETH_TEST_ITER_M_COUNT);
    printf("Test_0007: END\n");

    return status;
}

bool HostApp_test_0008(void)
{
    uint32_t recvNum;
    uint16_t len;
    bool st;
    bool status = PASS;

    printf("Test_0008: START\n");

    /* Wait for DUT to start the test when it's ready */
    HostApp_waitForCmd(CTRL_FRAME_CMD_START);

    /* Receive packets indefinitely until STOP cmd is detected */
    for (len = 10; len <= 1500; len += 10)
    {
        recvNum = 0;
        st      = HostApp_receive(&recvNum);
        if (st == FAIL)
        {
            printf("Test_0008: failed while receiving packets\n");
            status = FAIL;
        }

        /* Check that all packets were received */
        if (recvNum != ETH_TEST_ITER_S_COUNT)
        {
            printf("Test_0008: received frame count mismatch (exp=%d, got=%d)\n",
                   ETH_TEST_ITER_S_COUNT, recvNum);
            status = FAIL;
        }
    }

    printf("Test_0008: END\n");

    return status;
}

bool HostApp_test_0009(void)
{
    uint32_t iterations = ETH_TEST_ITER_M_COUNT;
    TestFrame frames[1];
    VlanDataFramePayload *payload;
    uint16_t len;
    uint32_t i;
    int32_t ret;
    bool status = PASS;

    printf("Test_0009: START\n");

    /* Unicast packet with DUT's MAC address */
    frames[0].len = 100;
    payload       = (VlanDataFramePayload *)frames[0].frame.payload;
    memcpy(&frames[0].frame.hdr, &hdrUcastCtrl, ETH_HDR_LEN);
    frames[0].frame.hdr.etherType = htons(ETHERTYPE_VLAN_TAG);

    /* Wait for DUT to start the test when it's ready */
    HostApp_waitForCmd(CTRL_FRAME_CMD_START);

    /* Transmit one frame per iteration */
    for (i = 0; i < iterations; i++)
    {
        if (gHostApp.verbose)
        {
            printf("Test_0009: iteration: %d of %d\n", i + 1, iterations);
        }

        /* Unicast frame with DUT's MAC address */
        len = frames[0].len - ETH_HDR_LEN;
        ret = HostappUtils_fillVlanPayload(payload,
                                           i % ETH_TEST_NUM_TYPES,
                                           len,
                                           ETH_TEST_VLAN_PCP,
                                           ETH_TEST_VLAN_VID,
                                           ETHERTYPE_EXPERIMENTAL1);
        if (ret)
        {
            printf("Test_0009: failed to create test packet: %d\n", ret);
            status = FAIL;
            break;
        }

        /* Transmit one frame */
        HostApp_transmit(frames, ENET_ARRAYSIZE(frames), 1);
    }

    /* Indicate to the DUT that the test is complete */
    HostApp_sendCmd(CTRL_FRAME_CMD_STOP);

    printf("Test_0009: transmitted %d of %d frames\n", i, iterations);
    printf("Test_0009: END\n");

    return status;
}

bool HostApp_test_0010(void)
{
    uint32_t iterations = ETH_TEST_ITER_S_COUNT;
    uint32_t i;
    bool status = PASS;
    bool st;

    printf("Test_0010: START\n");

    /* Wait for DUT to start the test when it's ready */
    HostApp_waitForCmd(CTRL_FRAME_CMD_START);

    /* Transmit and receive packets after DUT's mode change */
    for (i = 0; i < iterations; i++)
    {
        if (gHostApp.verbose)
        {
            printf("Test_0010: iteration: %d\n", i + 1);
        }

        /* Receive packets */
        st = HostApp_test_0002();
        if (st == FAIL)
        {
            printf("Test_0010: failed to receive packets\n");
            status = FAIL;
        }

        /* Transmit packets */
        st = HostApp_test_0001();
        if (st == FAIL)
        {
            printf("Test_0010: failed to transmit packets\n");
            status = FAIL;
        }
    }

    printf("Test_0010: completed %d of %d iterations\n", i, iterations);
    printf("Test_0010: END\n");

    return status;
}

bool HostApp_test_0100(void)
{
    uint32_t frames = 150000;
    bool status;

    printf("Test_0100: START\n");

    /* Wait for DUT to start the test when it's ready */
    HostApp_waitForCmd(CTRL_FRAME_CMD_START);

    /* Receive packets and measure the throughput */
    status = HostApp_receiveTput(frames);
    if (status == FAIL)
    {
        printf("Test_0100: failed to get receive throughput\n");
    }

    printf("Test_0100: END\n");

    return status;
}
