#ifndef __W5500_H
#define __W5500_H

#include <stdint.h>
#include "main.h"

#define WIZCHIP_CREG_BLOCK          0x00 	//< Common register block
#define WIZCHIP_SREG_BLOCK(N)       (1+4*N) //< Socket N register block
#define WIZCHIP_TXBUF_BLOCK(N)      (2+4*N) //< Socket N Tx buffer address block
#define WIZCHIP_RXBUF_BLOCK(N)      (3+4*N) //< Socket N Rx buffer address block

#define WIZCHIP_CREG_GAR_0          0x0001
#define WIZCHIP_CREG_GAR_1          (WIZCHIP_CREG_GAR_0 + 1)
#define WIZCHIP_CREG_GAR_2          (WIZCHIP_CREG_GAR_1 + 1)
#define WIZCHIP_CREG_GAR_3          (WIZCHIP_CREG_GAR_2 + 1)

#define WIZCHIP_CREG_SUBR_0          0x0005
#define WIZCHIP_CREG_SUBR_1          (WIZCHIP_CREG_SUBR_0 + 1)
#define WIZCHIP_CREG_SUBR_2          (WIZCHIP_CREG_SUBR_1 + 1)
#define WIZCHIP_CREG_SUBR_3          (WIZCHIP_CREG_SUBR_2 + 1)

#define WIZCHIP_CREG_SHAR_0         0x0009
#define WIZCHIP_CREG_SHAR_1         (WIZCHIP_CREG_SHAR_0 + 1)
#define WIZCHIP_CREG_SHAR_2         (WIZCHIP_CREG_SHAR_1 + 1)
#define WIZCHIP_CREG_SHAR_3         (WIZCHIP_CREG_SHAR_2 + 1)
#define WIZCHIP_CREG_SHAR_4         (WIZCHIP_CREG_SHAR_3 + 1)
#define WIZCHIP_CREG_SHAR_5         (WIZCHIP_CREG_SHAR_4 + 1)

#define WIZCHIP_CREG_SIPR_0         0x000F
#define WIZCHIP_CREG_SIPR_1         (WIZCHIP_CREG_SIPR_0 + 1)
#define WIZCHIP_CREG_SIPR_2         (WIZCHIP_CREG_SIPR_1 + 1)
#define WIZCHIP_CREG_SIPR_3         (WIZCHIP_CREG_SIPR_2 + 1)

#define WIZCHIP_CREG_IR             0x0015

#define WIZCHIP_SREG_MR             0x0000

#define WIZCHIP_SREG_CR             0x0001

#define WIZCHIP_SREG_IR             0x0002

#define WIZCHIP_SREG_SR             0x0003

#define WIZCHIP_SREG_SR_CLOSED      0x00
#define WIZCHIP_SREG_SR_INIT        0x13
#define WIZCHIP_SREG_SR_LISTEN      0x14
#define WIZCHIP_SREG_SR_ESTABLISHED 0x17
#define WIZCHIP_SREG_SR_CLOSE_WAIT  0x1C
#define WIZCHIP_SREG_SR_UDP         0x22
#define WIZCHIP_SREG_SR_MACRAW      0x42
#define WIZCHIP_SREG_SR_SYNSENT     0x15
#define WIZCHIP_SREG_SR_SYNRECV     0x16
#define WIZCHIP_SREG_SR_FIN_WAIT    0x18
#define WIZCHIP_SREG_SR_CLOSING     0x1A
#define WIZCHIP_SREG_SR_TIME_WAIT   0x1B
#define WIZCHIP_SREG_SR_LAST_ACK    0x1D

#define WIZCHIP_SREG_PORT_0         0x0004
#define WIZCHIP_SREG_PORT_1         0x0005

#define WIZCHIP_SREG_DHAR_0         0x0006
#define WIZCHIP_SREG_DHAR_1         (WIZCHIP_SREG_DHAR_0 + 1)
#define WIZCHIP_SREG_DHAR_2         (WIZCHIP_SREG_DHAR_1 + 1)
#define WIZCHIP_SREG_DHAR_3         (WIZCHIP_SREG_DHAR_2 + 1)
#define WIZCHIP_SREG_DHAR_4         (WIZCHIP_SREG_DHAR_3 + 1)
#define WIZCHIP_SREG_DHAR_5         (WIZCHIP_SREG_DHAR_4 + 1)

#define WIZCHIP_SREG_DIPR_0         0x000C
#define WIZCHIP_SREG_DIPR_1         (WIZCHIP_SREG_DIPR_0 + 1)
#define WIZCHIP_SREG_DIPR_2         (WIZCHIP_SREG_DIPR_1 + 1)
#define WIZCHIP_SREG_DIPR_3         (WIZCHIP_SREG_DIPR_2 + 1)

#define WIZCHIP_SREG_DPORT_0        0x0010
#define WIZCHIP_SREG_DPORT_1        (WIZCHIP_SREG_DPORT_0 + 1)

#define WIZCHIP_SREG_MSSR_0         0x0012
#define WIZCHIP_SREG_MSSR_1         (WIZCHIP_SREG_MSSR_0 + 1)

#define WIZCHIP_SREG_TOS            0x0015

#define WIZCHIP_SREG_TTL            0x0016

#define WIZCHIP_SREG_RXBUF_SIZE     0x001E

#define WIZCHIP_SREG_TXBUF_SIZE     0x001F

#define WIZCHIP_SREG_TX_FSR_0       0x0020
#define WIZCHIP_SREG_TX_FSR_1       (WIZCHIP_SREG_TX_FSR_0 + 1)

#define WIZCHIP_SREG_TX_RD_0        0x0022
#define WIZCHIP_SREG_TX_RD_1        (WIZCHIP_SREG_TX_RD_0 + 1)

#define WIZCHIP_SREG_TX_WR_0        0x0024
#define WIZCHIP_SREG_TX_WR_1        (WIZCHIP_SREG_TX_WR_0 + 1)

#define WIZCHIP_SREG_RX_RSR_0       0x0026
#define WIZCHIP_SREG_RX_RSR_1       (WIZCHIP_SREG_RX_RSR_0 + 1)

#define WIZCHIP_SREG_RX_RD_0        0x0028
#define WIZCHIP_SREG_RX_RD_1        (WIZCHIP_SREG_RX_RD_0 + 1)

#define WIZCHIP_SREG_RX_WR_0        0x002A
#define WIZCHIP_SREG_RX_WR_1        (WIZCHIP_SREG_RX_WR_0 + 1)

#define WIZCHIP_SREG_IMR            0x002C

#define WIZCHIP_SREG_FRAG_0         0x002D
#define WIZCHIP_SREG_FRAG_1         (WIZCHIP_SREG_FRAG_0 + 1)

#define WIZCHIP_SREG_KPALVTR        0x002F

#define MAC_ADDRESS_SIZE            6
#define IP_ADDRESS_SIZE             4

#define WIZCHIP_CREG_VERSIONR       0x0039

void w5500_write(uint16_t address, uint8_t block, uint8_t *data, size_t len);
void w5500_read(uint16_t address, uint8_t block, uint8_t *data, size_t len);
uint8_t w5500_get_version(void);
void w5500_set_addresses(void);
uint8_t w5500_connect(uint16_t port);

#endif