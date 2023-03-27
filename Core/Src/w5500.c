#include "w5500.h"

static void cs_on(void);
static void cs_off(void);

uint8_t mac[MAC_ADDRESS_SIZE] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
uint8_t ip_address[IP_ADDRESS_SIZE] = {192, 168, 0, 133};
uint8_t gateway_address[IP_ADDRESS_SIZE] = {192, 168, 0, 1};
uint8_t subnet_mask[IP_ADDRESS_SIZE] = {255, 255, 255, 0};

void w5500_set_addresses(void){
    w5500_write(WIZCHIP_CREG_SHAR_0, WIZCHIP_CREG_BLOCK, mac, sizeof(mac));
    w5500_write(WIZCHIP_CREG_SIPR_0, WIZCHIP_CREG_BLOCK, ip_address, sizeof(ip_address));
    w5500_write(WIZCHIP_CREG_GAR_0, WIZCHIP_CREG_BLOCK, gateway_address, sizeof(gateway_address));
    w5500_write(WIZCHIP_CREG_SUBR_0, WIZCHIP_CREG_BLOCK, subnet_mask, sizeof(subnet_mask));
}

uint8_t w5500_connect(uint16_t port){
    uint8_t statuses[8];

    uint8_t socket = 0;

    for(int i = 0; i < sizeof(statuses); i++){
        w5500_read(WIZCHIP_SREG_SR, WIZCHIP_SREG_BLOCK(i), statuses + i, sizeof(uint8_t));
        if(statuses[i] == WIZCHIP_SREG_SR_CLOSED){
            socket = i;
        }
    }

    

    return socket;
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

static void cs_on(void){
    HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET);
}

static void cs_off(void){
    HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);
}