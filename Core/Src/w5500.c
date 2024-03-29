#include "w5500.h"

static void cs_on(void);
static void cs_off(void);
static uint16_t w5500_get_tx_fsr(uint8_t socket);

uint8_t mac[MAC_ADDRESS_SIZE] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
uint8_t ip_address[IP_ADDRESS_SIZE] = {10, 10, 10, 3};
uint8_t gateway_address[IP_ADDRESS_SIZE] = {10, 10, 10, 1};
uint8_t subnet_mask[IP_ADDRESS_SIZE] = {255, 255, 255, 0};

void w5500_set_addresses(void){
    w5500_write(WIZCHIP_CREG_SHAR_0, WIZCHIP_CREG_BLOCK, mac, sizeof(mac));
    w5500_write(WIZCHIP_CREG_SIPR_0, WIZCHIP_CREG_BLOCK, ip_address, sizeof(ip_address));
    w5500_write(WIZCHIP_CREG_GAR_0, WIZCHIP_CREG_BLOCK, gateway_address, sizeof(gateway_address));
    w5500_write(WIZCHIP_CREG_SUBR_0, WIZCHIP_CREG_BLOCK, subnet_mask, sizeof(subnet_mask));
}

void w5500_reset(void){
    w5500_write_byte(WIZCHIP_CREG_MR, WIZCHIP_CREG_BLOCK, WIZCHIP_CREG_MR_RST);
    while(w5500_read_byte(WIZCHIP_CREG_MR, WIZCHIP_CREG_BLOCK) & WIZCHIP_CREG_MR_RST){}
}

uint8_t w5500_connect(uint16_t port, uint8_t dest_ip_addr[IP_ADDRESS_SIZE]){
    uint8_t statuses[8];

    uint8_t socket = 0;

    for(int i = 0; i < sizeof(statuses); i++){
        w5500_read(WIZCHIP_SREG_SR, WIZCHIP_SREG_BLOCK(i), statuses + i, sizeof(uint8_t));
        if(statuses[i] == WIZCHIP_SREG_SR_CLOSED){
            socket = i;
        }
    }

    w5500_close_socket(socket);
    uint8_t mode_reg = WIZCHIP_SREG_MR_P0;
    w5500_write(WIZCHIP_SREG_MR, WIZCHIP_SREG_BLOCK(socket), &mode_reg, sizeof(mode_reg));

    uint16_t src_port = htons(1024);

    w5500_write(WIZCHIP_SREG_PORT_0, WIZCHIP_SREG_BLOCK(socket), (uint8_t *)&src_port, sizeof(src_port));

    w5500_write_command(WIZCHIP_SREG_CR_OPEN, socket);

    while (w5500_read_byte(WIZCHIP_SREG_SR, WIZCHIP_SREG_BLOCK(socket)) != WIZCHIP_SREG_SR_INIT){
        return 255;
    }

    port = htons(port);

    w5500_write(WIZCHIP_SREG_DIPR_0, WIZCHIP_SREG_BLOCK(socket), dest_ip_addr, IP_ADDRESS_SIZE);
    w5500_write(WIZCHIP_SREG_DPORT_0, WIZCHIP_SREG_BLOCK(socket), (uint8_t *)&port, sizeof(port));
    w5500_write_command(WIZCHIP_SREG_CR_CONNECT, socket);

    uint8_t sr = w5500_read_byte(WIZCHIP_SREG_SR, WIZCHIP_SREG_BLOCK(socket));

    volatile uint8_t ir = w5500_read_byte(WIZCHIP_SREG_IR, WIZCHIP_SREG_BLOCK(socket));

    while (sr != WIZCHIP_SREG_SR_ESTABLISHED)
    {
        if(sr == WIZCHIP_SREG_SR_CLOSED){
            return 255;
        }
        sr = w5500_read_byte(WIZCHIP_SREG_SR, WIZCHIP_SREG_BLOCK(socket));
        ir = w5500_read_byte(WIZCHIP_SREG_IR, WIZCHIP_SREG_BLOCK(socket));
    }
    

    return socket;
}

void w5500_send(uint8_t socket, uint8_t *data, size_t len){
    uint8_t status=0;
	uint16_t freesize=0;

    do{
        freesize = w5500_get_tx_fsr(socket);
        status = w5500_read_byte(WIZCHIP_SREG_SR, WIZCHIP_SREG_BLOCK(socket));
        if ((status != WIZCHIP_SREG_SR_ESTABLISHED) && (status != WIZCHIP_SREG_SR_CLOSE_WAIT)){
            return;
        }
    }while (freesize < len);

    uint16_t ptr = w5500_read_short(WIZCHIP_SREG_TX_WR_0, WIZCHIP_SREG_BLOCK(socket));
    
    w5500_write(ptr, WIZCHIP_TXBUF_BLOCK(socket), data, len);

    ptr += len;

    w5500_write_short(WIZCHIP_SREG_TX_WR_0, WIZCHIP_SREG_BLOCK(socket), ptr);
    
    w5500_write_command(WIZCHIP_SREG_CR_SEND, socket);

    uint8_t ir = w5500_read_byte(WIZCHIP_SREG_IR, WIZCHIP_SREG_BLOCK(socket));
    while((ir & WIZCHIP_SREG_IR_SEND_OK) != WIZCHIP_SREG_IR_SEND_OK){}
    w5500_write_byte(WIZCHIP_SREG_IR, WIZCHIP_SREG_BLOCK(socket), WIZCHIP_SREG_IR_SEND_OK);
}

void w5500_send_byte(uint8_t socket, uint8_t byte){
    w5500_send(socket, &byte, sizeof(byte));
}

void w5500_close_socket(uint8_t socket){
    uint8_t ir_block = 0xFF;
    w5500_write_command(WIZCHIP_SREG_CR_CLOSE, socket);
    w5500_write(WIZCHIP_SREG_IR, WIZCHIP_SREG_BLOCK(socket), &ir_block, sizeof(ir_block));
}

void w5500_write_command(uint8_t cmd, uint8_t socket){
    w5500_write(WIZCHIP_SREG_CR, WIZCHIP_SREG_BLOCK(socket), &cmd, sizeof(cmd));

    while(w5500_read_byte(WIZCHIP_SREG_CR, WIZCHIP_SREG_BLOCK(socket))){

    }
}

uint8_t w5500_get_version(void){
    uint8_t version;
    w5500_read(WIZCHIP_CREG_VERSIONR, WIZCHIP_CREG_BLOCK, &version, sizeof(version));
    return version;
}

void w5500_write(uint16_t address, uint8_t block, uint8_t *data, size_t len){
    cs_on();
    uint8_t addr1 = address >> 8;
    HAL_SPI_Transmit(&hspi2, &addr1, 1, 1000);
    uint8_t addr2 = address & 0xFF;
    HAL_SPI_Transmit(&hspi2, &addr2, 1, 1000);
    block <<= 3;
    block |= 0b100;
    block &=~0b11;
    block |= 0b00;
    HAL_SPI_Transmit(&hspi2, &block, 1, 1000);
    HAL_SPI_Transmit(&hspi2, data, len, 1000);
    cs_off();
}

void w5500_read(uint16_t address, uint8_t block, uint8_t *data, size_t len){
    cs_on();
    uint8_t addr1 = address >> 8;
    HAL_SPI_Transmit(&hspi2, &addr1, 1, 1000);
    uint8_t addr2 = address & 0xFF;
    HAL_SPI_Transmit(&hspi2, &addr2, 1, 1000);
    block <<= 3;
    block &=~0b100;
    block &=~0b011;
    block |= 0b000;
    HAL_SPI_Transmit(&hspi2, &block, 1, 1000);
    HAL_SPI_Receive(&hspi2, data, len, 1000);
    cs_off();
}

uint8_t w5500_read_byte(uint16_t address, uint8_t block){
    uint8_t data;

    w5500_read(address, block, &data, sizeof(data));

    return data;
}

uint16_t w5500_read_short(uint16_t address, uint8_t block){
    uint16_t data;

    w5500_read(address, block, (uint8_t *)&data, sizeof(data));

    data = ntohs(data);

    return data;
}

void w5500_write_short(uint16_t address, uint8_t block, uint16_t data){

    data = ntohs(data);

    w5500_write(address, block, (uint8_t *)&data, sizeof(data));
}

void w5500_write_byte(uint16_t address, uint8_t block, uint8_t data){
    w5500_write(address, block, &data, sizeof(data));
}

static void cs_on(void){
    HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET);
}

static void cs_off(void){
    HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);
}

static uint16_t w5500_get_tx_fsr(uint8_t socket){
    uint16_t val, prev;

    prev = w5500_read_short(WIZCHIP_SREG_TX_FSR_0, WIZCHIP_SREG_BLOCK(socket));
    //TODO optimise
    while(1){
        val = w5500_read_short(WIZCHIP_SREG_TX_FSR_0, WIZCHIP_SREG_BLOCK(socket));
        if(val == prev){
            return val;
        }

        prev = val;
    }

}