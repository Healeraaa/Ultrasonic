#include "Lcd_init.h"
#include "SPIHard.h"
#include "user_dma.h"
#include "PWM1.h"
#include "delay.h"


void MyGPIO_SetPin(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
    WRITE_REG(GPIOx->BSRR, PinMask);
    while ((READ_BIT(GPIOx->ODR, PinMask) == (PinMask)) == 0){}
    return;
}

void MyGPIO_ReSetPin(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
    WRITE_REG(GPIOx->BSRR, (PinMask << 16));
    while ((READ_BIT(GPIOx->ODR, PinMask) == (PinMask)) == 1){}
    return;
}




/**
 * @brief  对LCD的GPIO引脚进行初始化
 * @param  None
 * @retval None
 */
void LCD_GPIO_Init(void)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    // 三个GPIO口都是GPIOE上的，故只开一个GPIOE的时钟
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE);

    LL_GPIO_ResetOutputPin(RES_PORT, RES_PIN);
    GPIO_InitStruct.Pin = RES_PIN;                        // 设置要初始化的引脚为 RES_PIN
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;           // 设置引脚模式为输出模式
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH; // 设置输出速率为最高速率
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL; // 设置输出类型为推挽输出（推挽模式：高电平时拉高，低电平时拉低）
    LL_GPIO_Init(RES_PORT, &GPIO_InitStruct);
    LL_GPIO_SetOutputPin(RES_PORT, RES_PIN);

    LL_GPIO_ResetOutputPin(DC_PORT, DC_PIN);
    GPIO_InitStruct.Pin = DC_PIN;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    LL_GPIO_Init(DC_PORT, &GPIO_InitStruct);
    LL_GPIO_SetOutputPin(DC_PORT, DC_PIN);

    LL_GPIO_ResetOutputPin(CS_PORT, CS_PIN);
    GPIO_InitStruct.Pin = CS_PIN;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    LL_GPIO_Init(CS_PORT, &GPIO_InitStruct);
    LL_GPIO_SetOutputPin(CS_PORT, CS_PIN);
}

/**
 * @brief  LCD串行数据写入函数
 * @param  data 要写入的8bit数据
 * @retval None
 */
void LCD_Write_Bus(uint8_t data)
{
    // SPI1_Transmit8(data);
    SPI1_Transmit8_Time(data,1);
}

/**
 * @brief  LCD写入数据
 * @param  data 要写入的8bit数据
 * @retval None
 */
void LCD_WR_DATA8(uint8_t data)
{
    LCD_Write_Bus(data);
}

/**
 * @brief  LCD写入数据
 * @param  data 要写入的16bit数据
 * @retval None
 */
void LCD_WR_DATA(uint16_t Data)
{
    SPI1_SetDataWidth(LL_SPI_DATAWIDTH_16BIT);
    // SPI1_Transmit16(Data);
    SPI1_Transmit16_Time(Data,1);
    SPI1_SetDataWidth(LL_SPI_DATAWIDTH_8BIT);
}

/**
 * @brief  LCD写入命令
 * @param  data 要写入的8bit数据
 * @retval None
 */
void LCD_WR_REG(uint8_t data)
{
    LCD_DC_Reset(); // 写命令
    // delay_ms(1);
    LCD_Write_Bus(data);
    LCD_DC_Set(); // 写数据
    // delay_ms(1);
}

/**
 * @brief  设置起始和结束地址
 * @param   x1,x2 设置列的起始和结束地址
            y1,y2 设置行的起始和结束地址
 * @retval None
 */
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    LCD_WR_REG(0x2a); // 列地址设置
    LCD_WR_DATA(x1);
    LCD_WR_DATA(x2);
    LCD_WR_REG(0x2b); // 行地址设置
    LCD_WR_DATA(y1);
    LCD_WR_DATA(y2);
    LCD_WR_REG(0x2c); // 储存器写
}


/**
 * @brief  LCD调节背光
 * @param  dc 占空比,5%~100%
 * @retval None
 */
void LCD_Set_Light(uint8_t dc)
{
    if (dc >= 5 && dc <= 100)
        PWM1_SetDutyCycle(dc);
}

/**
 * @brief  LCD关闭背光
 * @param  None
 * @retval None
 */
void LCD_Close_Light(void)
{
    PWM1_SetDutyCycle(0);
    PWM1_Disable();
}

/**
 * @brief  LCD开启背光
 * @param  None
 * @retval None
 */
void LCD_Open_Light(void)
{
    PWM1_Enable();
}

/**
 * @brief  ST7789 SLEEP IN
 * @param  None
 * @retval None
 */
void LCD_ST7789_SleepIn(void)
{
    LCD_WR_REG(0x10);
    delay_ms(100);
}

/**
 * @brief  ST7789 SLEEP OUT
 * @param  None
 * @retval None
 */
void LCD_ST7789_SleepOut(void)
{
    LCD_WR_REG(0x11);
    delay_ms(100);
}

void LCD_Init(void)
{
    DMA_Init();
    SPI1_Init();     // 初始化Hard SPI
    PWM1_Init();     // 初始化PWM输出
    LCD_GPIO_Init(); // 初始化GPIO
    delay_ms(100);
    LCD_CS_Reset();  // chip select

    LCD_RES_Reset(); // 复位
    // PWM1_SetDutyCycle(50);
    delay_ms(100);
    LCD_RES_Set();
    delay_ms(100);

    LCD_WR_REG(0x11);//Sleep Out
    delay_ms(120);
    LCD_WR_REG(0x36);//Memory Data Access Control 内存数据访问控制
    if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x00);
	else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC0);
	else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x70);
	else LCD_WR_DATA8(0xA0);

	LCD_WR_REG(0x3A);//Interface Pixel Format 接口像素格式
	LCD_WR_DATA8(0x05);//Control interface color format：16位/像素

	LCD_WR_REG(0xB2);//Porch Setting
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x33);
	LCD_WR_DATA8(0x33); 

	LCD_WR_REG(0xB7); //Gate Control
	LCD_WR_DATA8(0x35);  

	LCD_WR_REG(0xBB);//VCOMS Setting
	LCD_WR_DATA8(0x19);//0.729V

	LCD_WR_REG(0xC0);//LCM Control
	LCD_WR_DATA8(0x2C);//默认

	LCD_WR_REG(0xC2);//VDV and VRH Command Enable
	LCD_WR_DATA8(0x01);//默认设置

	LCD_WR_REG(0xC3);//VRH Set
	LCD_WR_DATA8(0x12);//4.45+( vcom+vcom offset+0.5vdv )   

	LCD_WR_REG(0xC4);//VDV Set
	LCD_WR_DATA8(0x20);//默认  

	LCD_WR_REG(0xC6); //Frame Rate Control in Normal Mode
	LCD_WR_DATA8(0x0F);//默认 60HZ    

	LCD_WR_REG(0xD0); //Power Control 1
	LCD_WR_DATA8(0xA4);//默认
	LCD_WR_DATA8(0xA1);// AVDD:6.8V;AVCL:-4.8V;VDDS:2.3V

	LCD_WR_REG(0xE0);//Positive Voltage Gamma Control
	LCD_WR_DATA8(0xD0);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x11);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x2B);
	LCD_WR_DATA8(0x3F);
	LCD_WR_DATA8(0x54);
	LCD_WR_DATA8(0x4C);
	LCD_WR_DATA8(0x18);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x0B);
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x23);

	LCD_WR_REG(0xE1);
	LCD_WR_DATA8(0xD0);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x11);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x2C);
	LCD_WR_DATA8(0x3F);
	LCD_WR_DATA8(0x44);
	LCD_WR_DATA8(0x51);
	LCD_WR_DATA8(0x2F);
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x20);
	LCD_WR_DATA8(0x23);

	LCD_WR_REG(0x21); 

	LCD_WR_REG(0x29); 

 
}

