#include "app_main.h"

#if (I2C_DRV_USED == I2C_DRV_HARD)
void app_i2c_init() {
    drv_i2c_gpio_set(I2C_PIN_GROUP);
    drv_i2c_master_init(I2C_CLOCK);
}

int8_t scan_i2c_addr(uint8_t address) {
    if ((reg_clk_en0 & FLD_CLK0_I2C_EN)==0)
        app_i2c_init();
    u8 r = reg_i2c_speed;
    reg_i2c_speed = (u8)(CLOCK_SYS_CLOCK_HZ/(4*100000)); // 100 kHz
    reg_i2c_id = (u8) address;
    reg_i2c_ctrl = FLD_I2C_CMD_START | FLD_I2C_CMD_ID | FLD_I2C_CMD_STOP;
    while (reg_i2c_status & FLD_I2C_CMD_BUSY);
    reg_i2c_speed = r;
    return ((reg_i2c_status & FLD_I2C_NAK)? 0 : address);
}
#elif (I2C_DRV_USED == I2C_DRV_SOST)

#if I2C_CLOCK > 200000
#define I2C_TCLK_US 10 // *0.625 us (in 1/16 us)  I2C ~ 222222 Hz
#else
#define I2C_TCLK_US 32 // *0.625 us (in 1/16 us)
#endif

_attribute_ram_code_sec_ void sleep_us16(unsigned int us16) {
    unsigned int t = reg_system_tick;
    while((unsigned int)(reg_system_tick - t) < us16) {
    }
}

static void soft_i2c_start(void) {
    gpio_set_output_en(I2C_SCL, 0); // SCL set "1"
    gpio_set_output_en(I2C_SDA, 0); // SDA set "1"
    sleep_us16(I2C_TCLK_US);
    gpio_set_output_en(I2C_SDA, 1); // SDA set "0"
    sleep_us16(I2C_TCLK_US);
    gpio_set_output_en(I2C_SCL, 1); // SCL set "0"
    //sleep_us16(10);
}

static void soft_i2c_stop(void) {
    gpio_set_output_en(I2C_SDA, 1); // SDA set "0"
    sleep_us16(I2C_TCLK_US);
    gpio_set_output_en(I2C_SCL, 0); // SCL set "1"
    sleep_us16(I2C_TCLK_US);
    gpio_set_output_en(I2C_SDA, 0); // SDA set "1"
}

static int soft_i2c_wr_byte(unsigned char  b) {
    int ret, i = 8;
    while(i--) {
        sleep_us16(I2C_TCLK_US/2);
        if(b & 0x80)
            gpio_set_output_en(I2C_SDA, 0); // SDA set "1"
        else
            gpio_set_output_en(I2C_SDA, 1); // SDA set "0"
        sleep_us16(I2C_TCLK_US/2);
        gpio_set_output_en(I2C_SCL, 0); // SCL set "1"
        sleep_us16(I2C_TCLK_US);
        gpio_set_output_en(I2C_SCL, 1); // SCL set "0"
        b <<= 1;
    }
    sleep_us16(I2C_TCLK_US/2);
    gpio_set_output_en(I2C_SDA, 0); // SDA set "1"
    sleep_us16(I2C_TCLK_US/2);
    gpio_set_output_en(I2C_SCL, 0); // SCL set "1"
    sleep_us16(I2C_TCLK_US);
    ret = gpio_read(I2C_SDA);
    gpio_set_output_en(I2C_SCL, 1); // SCL set "0"
    return ret;
}

static unsigned char soft_i2c_rd_byte(int ack) {
    int i = 8;
    unsigned char ret = 0;
    gpio_set_output_en(I2C_SDA, 0); // SDA set "1"
    while(i--) {
        sleep_us16(I2C_TCLK_US);
        gpio_set_output_en(I2C_SCL, 0); // SCL set "1"
        sleep_us16(I2C_TCLK_US);
        ret <<= 1;
        if(gpio_read(I2C_SDA)) {
            ret |= 1;
        }
        gpio_set_output_en(I2C_SCL, 1); // SCL set "0"
    }
    sleep_us16(I2C_TCLK_US/2);
    gpio_set_output_en(I2C_SDA, ack); // SDA set "0"
    sleep_us16(I2C_TCLK_US/2);
    gpio_set_output_en(I2C_SCL, 0); // SCL set "1"
    sleep_us16(I2C_TCLK_US);
    gpio_set_output_en(I2C_SCL, 1); // SCL set "0"
    gpio_set_output_en(I2C_SDA, 0); // SDA set "1"
    return ret;
}


/* scan_i2c_addr() return: address (=0 - NAK) */
unsigned char scan_i2c_addr(unsigned char address) {
    unsigned char ret = 0;
    soft_i2c_start();
    if(soft_i2c_wr_byte(address) == 0)
        ret = address;
    soft_i2c_stop();
    return ret;
}

/* send_i2c_byte() return: NAK (=0 - send ok) */
int send_i2c_byte(unsigned char i2c_addr, unsigned char cmd) {
    int ret;
    soft_i2c_start();
    ret = soft_i2c_wr_byte(i2c_addr);
    if(ret == 0)
        ret = soft_i2c_wr_byte(cmd);
    soft_i2c_stop();
    return ret;
}

/* send_i2c_word() return: NAK (=0 - send ok) */
int send_i2c_word(unsigned char i2c_addr, unsigned short w) {
    int ret;
    soft_i2c_start();
    ret = soft_i2c_wr_byte(i2c_addr);
    if(ret == 0) {
        ret = soft_i2c_wr_byte((unsigned char)(w));
        if(ret == 0)
            ret = soft_i2c_wr_byte((unsigned char) (w >> 8));
    }
    soft_i2c_stop();
    return ret;
}

/* send_i2c_bytes() return: NAK (=0 - send ok) */
int send_i2c_bytes(unsigned char i2c_addr, unsigned char * dataBuf, size_t dataLen) {
    int ret = 0;
    int size = dataLen;
    unsigned char *p = dataBuf;
    soft_i2c_start();
    ret = soft_i2c_wr_byte(i2c_addr);
    if(ret == 0) {
        while(size--) {
            ret = soft_i2c_wr_byte(*p);
            if(ret)
                break;
            p++;
        }
    }
    soft_i2c_stop();
    return ret;
}

/* read_i2c_byte() return: data (=-1 - read error) */
int read_i2c_byte(unsigned char i2c_addr) {
    int ret = -1;
    soft_i2c_start();
    if(soft_i2c_wr_byte(i2c_addr | 1) == 0)
        ret = soft_i2c_rd_byte(0);
    soft_i2c_stop();
    return ret;
}

/* read_i2c_bytes() return: NAK (=0 - read ok) */
int read_i2c_bytes(unsigned char i2c_addr, unsigned char * dataBuf, int dataLen) {
    int ret = -1;
    int size = dataLen;
    unsigned char *p = dataBuf;
    soft_i2c_start();
    if(soft_i2c_wr_byte(i2c_addr | 1) == 0) {
        while(size--) {
            *p = soft_i2c_rd_byte(size);
            p++;
        }
        ret = 0;
    }
    soft_i2c_stop();
    return ret;
}

/* read_i2cread_i2c_addr_bytes() return: NAK (=0 - read ok) */
int read_i2c_addr_bytes(unsigned char i2c_addr, unsigned char reg_addr, unsigned char * dataBuf, int dataLen) {
    int ret = -1;
    int size = dataLen;
    unsigned char *p = dataBuf;
    soft_i2c_start();
    if(soft_i2c_wr_byte(i2c_addr | 1) == 0) {
        if(soft_i2c_wr_byte(reg_addr) == 0) {
            while(size--) {
                *p = soft_i2c_rd_byte(size);
                p++;
            }
            ret = 0;
        }
    }
    soft_i2c_stop();
    return ret;
}

///* Universal I2C/SMBUS read-write transaction
// * wrlen = 0..127 !
// * return NAK (=0 - ok) */
//int I2CBusUtr(void * outdata, i2c_utr_t * tr, u32 wrlen) {
//    u8 * pwrdata = (u8 *) &tr->wrdata;
//    u8 * poutdata = (u8 *) outdata;
//    u32 cntstart = wrlen - (tr->mode & 0x7f);
//    u32 rdlen = tr->rdlen & 0x7f;
//    int ret;
//    soft_i2c_start();
//    ret = soft_i2c_wr_byte(*pwrdata++);
//    while(ret == 0 && wrlen) {
//        // write data
//        ret = soft_i2c_wr_byte(*pwrdata++);
//
//        if(--wrlen == cntstart && ret == 0) { // + send start & id
//            if(tr->mode & 0x80) {
//                soft_i2c_stop();
//                sleep_us16(I2C_TCLK_US);
//            }
//            soft_i2c_start();
//            ret = soft_i2c_wr_byte(tr->wrdata[0] | 1);
//        }
//    }
//    if(ret == 0) {
//        while(rdlen) {
//            if(--rdlen == 0 && (tr->rdlen & 0x80) == 0)
//                *poutdata = soft_i2c_rd_byte(1);
//            else
//                *poutdata = soft_i2c_rd_byte(0);
//            poutdata++;
//        }
//    }
//    soft_i2c_stop();
//    return ret;
//}

void app_i2c_init() {
}

#endif /* I2C_DRV_USED */
