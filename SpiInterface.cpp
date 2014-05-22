/*
 * SPI testing utility (using spidev driver)
 *
 * Copyright (c) 2007  MontaVista Software, Inc.
 * Copyright (c) 2007  Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * Cross-compile with cross-gcc -I/path/to/cross-kernel/include
 */

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <cstring>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <SpiInterface.hpp>

const char*     SpiInterface::device = "/dev/spidev1.0";
uint8_t         SpiInterface::mode   = 0; 
uint8_t         SpiInterface::bits   = 32;
uint32_t        SpiInterface::speed  = 1000000;
uint16_t        SpiInterface::delay  = 0;
                   
SpiInterface::SpiInterface() {
} 

SpiInterface::~SpiInterface() {
} 

void SpiInterface::pabort(const char *s) {
    perror(s);
    abort();
}

uint32_t SpiInterface::transfer(int fd, uint32_t data) {

    uint32_t read=0; 
    printf("\nSent = 0x%04X ", data);
    int ret;
    uint8_t byte1 = 0xFF & (data >> 24); 
    uint8_t byte2 = 0xFF & (data >> 16); 
    uint8_t byte3 = 0xFF & (data >> 8); 
    uint8_t byte4 = 0xFF & (data >> 0); 

    uint8_t tx[] = { byte1, byte2, byte3, byte4 };
    const uint8_t txsize = ARRAY_SIZE(tx); 

    uint8_t rx[txsize] = {0};

    struct spi_ioc_transfer tr; 
    memset( (void *) & tr, 0, sizeof(struct spi_ioc_transfer));
    tr.tx_buf           = (unsigned long) tx; 
    tr.rx_buf           = (unsigned long) rx; 
    tr.len              = txsize; 
    tr.delay_usecs      = delay; 
    tr.speed_hz         = speed; 
    tr.bits_per_word    = bits; 

    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);

    if (ret < 1)
        pabort("can't send spi message");

    for (ret = 0; ret < txsize; ret++) {
        read |= rx[ret] << 8*(txsize-ret-1);
    }
    printf("\nRead = 0x%04X ", read);
    return read; 
}

uint32_t SpiInterface::WriteRead (uint32_t data) {
    int ret = 0;
    int fd;

    printf("\nOpening SPI device %s", device); 
    fd = open(device, O_RDWR);
    if (fd < 0)
        pabort("can't open spi device");

    // set spi mode
    printf("\nSetting SPI Mode 0x%02X: ", mode); 
    ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
    if (ret == -1)
        pabort("can't set spi mode");

    // read back spi mode
    ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
    printf("\nReading SPI Mode 0x%02X: ", mode); 
    if (ret == -1)
        pabort("can't get spi mode");

    // set bits per word
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret == -1)
        pabort("can't set bits per word");

    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (ret == -1)
        pabort("can't get bits per word");

    // max speed hz
    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (ret == -1)
        pabort("can't set max speed hz");

    ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    if (ret == -1)
        pabort("can't get max speed hz");

    printf("spi mode: %d\n", mode);
    printf("bits per word: %d\n", bits);
    printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);

    uint32_t send_data = data;  // implictly cast to uint32_t
    transfer(fd,send_data);

    close(fd);

    return ret;
}
