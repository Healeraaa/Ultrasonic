#include "IIC_Soft.h"
#include "delay.h"

/**
 * @brief SDA线输入模式配置
 * @param None
 * @retval None
 */
void SDA_Input_Mode(iic_bus_t *bus)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = bus->IIC_SDA_PIN;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    LL_GPIO_Init(bus->IIC_SDA_PORT, &GPIO_InitStruct);
}

/**
 * @brief SDA线输出模式配置
 * @param None
 * @retval None
 */
void SDA_Output_Mode(iic_bus_t *bus)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = bus->IIC_SDA_PIN;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    LL_GPIO_Init(bus->IIC_SDA_PORT, &GPIO_InitStruct);
}

/**
 * @brief SDA线输出一个位
 * @param val 输出的数据
 * @retval None
 */
void SDA_Output(iic_bus_t *bus, uint16_t val)
{
    if (val)
    {
        WRITE_REG(bus->IIC_SDA_PORT->BSRR, bus->IIC_SDA_PIN);
    }
    else
    {
        WRITE_REG(bus->IIC_SDA_PORT->BSRR, (bus->IIC_SDA_PIN << 16));
    }
}

/**
 * @brief SCL线输出一个位
 * @param val 输出的数据
 * @retval None
 */
void SCL_Output(iic_bus_t *bus, uint16_t val)
{
    if (val)
    {
        WRITE_REG(bus->IIC_SCL_PORT->BSRR, bus->IIC_SCL_PIN);
    }
    else
    {
        WRITE_REG(bus->IIC_SCL_PORT->BSRR, (bus->IIC_SCL_PIN << 16));
    }
}

/**
 * @brief SDA输入一位
 * @param None
 * @retval GPIO读入一位
 */
uint8_t SDA_Input(iic_bus_t *bus)
{
    if (LL_GPIO_IsInputPinSet(bus->IIC_SDA_PORT, bus->IIC_SDA_PIN) == GPIO_PIN_SET)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief IIC起始信号
 * @param None
 * @retval None
 */
void IICStart(iic_bus_t *bus)
{
    SDA_Output(bus, 1);
    // delay1(DELAY_TIME);
    delay_us(2);
    SCL_Output(bus, 1);
    delay_us(1);
    SDA_Output(bus, 0);
    delay_us(1);
    SCL_Output(bus, 0);
    delay_us(1);
}
/**
 * @brief IIC结束信号
 * @param None
 * @retval None
 */
void IICStop(iic_bus_t *bus)
{
    SCL_Output(bus, 0);
    delay_us(2);
    SDA_Output(bus, 0);
    delay_us(1);
    SCL_Output(bus, 1);
    delay_us(1);
    SDA_Output(bus, 1);
    delay_us(1);
}

/**
 * @brief IIC等待确认信号
 * @param None
 * @retval None
 */
uint8_t IICWaitAck(iic_bus_t *bus)
{
    uint16_t cErrTime = 5;
    SDA_Input_Mode(bus);
    SCL_Output(bus, 1);
    while (SDA_Input(bus))
    {
        cErrTime--;
        delay_us(1);
        if (0 == cErrTime)
        {
            SDA_Output_Mode(bus);
            IICStop(bus);
            return ERROR;
        }
    }
    SDA_Output_Mode(bus);
    SCL_Output(bus, 0);
    delay_us(2);
    return SUCCESS;
}

/**
 * @brief IIC发送确认信号
 * @param None
 * @retval None
 */
void IICSendAck(iic_bus_t *bus)
{
    SDA_Output(bus, 0);
    delay_us(1);
    SCL_Output(bus, 1);
    delay_us(1);
    SCL_Output(bus, 0);
    delay_us(1);
}

/**
 * @brief IIC发送非确认信号
 * @param None
 * @retval None
 */
void IICSendNotAck(iic_bus_t *bus)
{
    SDA_Output(bus, 1);
    delay_us(1);
    SCL_Output(bus, 1);
    delay_us(1);
    SCL_Output(bus, 0);
    delay_us(2);
}

/**
 * @brief IIC发送一个字节
 * @param cSendByte 需要发送的字节
 * @retval None
 */
void IICSendByte(iic_bus_t *bus, unsigned char cSendByte)
{
    uint8_t i = 8;
    while (i--)
    {
        SCL_Output(bus, 0);
        delay_us(2);
        SDA_Output(bus, cSendByte & 0x80);
        delay_us(1);
        cSendByte += cSendByte;
        delay_us(1);
        SCL_Output(bus, 1);
        delay_us(1);
    }
    SCL_Output(bus, 0);
    delay_us(2);
}

/**
 * @brief IIC接收一个字节
 * @param None
 * @retval 接收到的字节
 */
uint8_t IICReceiveByte(iic_bus_t *bus)
{
    uint8_t i = 8;
    uint8_t cR_Byte = 0;
    SDA_Input_Mode(bus);
    while (i--)
    {
        cR_Byte += cR_Byte;
        SCL_Output(bus, 0);
        delay_us(2);
        SCL_Output(bus, 1);
        delay_us(1);
        cR_Byte |= SDA_Input(bus);
    }
    SCL_Output(bus, 0);
    SDA_Output_Mode(bus);
    return cR_Byte;
}
/**
 * @brief IIC发送一个字节到指定设备的指定寄存器
 * @param bus IIC总线结构体指针
 * @param daddr 设备地址
 * @param reg 寄存器地址
 * @param data 要发送的数据
 * @retval 0 成功，1 失败
 */
uint8_t IIC_Write_One_Byte(iic_bus_t *bus, uint8_t daddr, uint8_t reg, uint8_t data)
{
    IICStart(bus);

    IICSendByte(bus, daddr << 1);
    if (IICWaitAck(bus)) // 等待应答
    {
        IICStop(bus);
        return 1;
    }
    IICSendByte(bus, reg);
    IICWaitAck(bus);
    IICSendByte(bus, data);
    IICWaitAck(bus);
    IICStop(bus);
    delay_us(1);
    return 0;
}

/**
 * @brief IIC发送多个字节到指定设备的指定寄存器
 * @param bus IIC总线结构体指针
 * @param daddr 设备地址
 * @param reg 寄存器地址
 * @param length 要发送的数据长度
 * @param buff 要发送的数据缓冲区
 * @retval 0 成功，1 失败
 */
uint8_t IIC_Write_Multi_Byte(iic_bus_t *bus, uint8_t daddr, uint8_t reg, uint8_t length, uint8_t buff[])
{
    uint8_t i;
    IICStart(bus);

    IICSendByte(bus, daddr << 1);
    if (IICWaitAck(bus))
    {
        IICStop(bus);
        return 1;
    }
    IICSendByte(bus, reg);
    IICWaitAck(bus);
    for (i = 0; i < length; i++)
    {
        IICSendByte(bus, buff[i]);
        IICWaitAck(bus);
    }
    IICStop(bus);
    delay_us(1);
    return 0;
}

/**
 * @brief IIC从指定设备的指定寄存器读取一个字节
 * @param bus IIC总线结构体指针
 * @param daddr 设备地址
 * @param reg 寄存器地址
 * @retval 读取到的数据
 */
unsigned char IIC_Read_One_Byte(iic_bus_t *bus, uint8_t daddr, uint8_t reg)
{
    unsigned char dat;
    IICStart(bus);
    IICSendByte(bus, daddr << 1);
    IICWaitAck(bus);
    IICSendByte(bus, reg);
    IICWaitAck(bus);

    IICStart(bus);
    IICSendByte(bus, (daddr << 1) + 1);
    IICWaitAck(bus);
    dat = IICReceiveByte(bus);
    IICSendNotAck(bus);
    IICStop(bus);
    return dat;
}

/**
 * @brief IIC从指定设备的指定寄存器读取多个字节
 * @param bus IIC总线结构体指针
 * @param daddr 设备地址
 * @param reg 寄存器地址
 * @param length 要读取的数据长度
 * @param buff 读取到的数据缓冲区
 * @retval 0 成功，1 失败
 */
uint8_t IIC_Read_Multi_Byte(iic_bus_t *bus, uint8_t daddr, uint8_t reg, uint8_t length, uint8_t buff[])
{
    unsigned char i;
    IICStart(bus);
    IICSendByte(bus, daddr << 1);
    if (IICWaitAck(bus))
    {
        IICStop(bus);
        return 1;
    }
    IICSendByte(bus, reg);
    IICWaitAck(bus);

    IICStart(bus);
    IICSendByte(bus, (daddr << 1) + 1);
    IICWaitAck(bus);
    for (i = 0; i < length; i++)
    {
        buff[i] = IICReceiveByte(bus);
        if (i < length - 1)
        {
            IICSendAck(bus);
        }
    }
    IICSendNotAck(bus);
    IICStop(bus);
    return 0;
}

/**
 * @brief 初始化IIC总线 注意GPIO时钟需要在函数外手动开启
 * @param bus IIC总线结构体指针
 * @retval None
 */
void IICInit(iic_bus_t *bus)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    // bus->CLK_ENABLE();

    GPIO_InitStruct.Pin = bus->IIC_SDA_PIN;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    LL_GPIO_Init(bus->IIC_SDA_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = bus->IIC_SCL_PIN;
    LL_GPIO_Init(bus->IIC_SCL_PORT, &GPIO_InitStruct);
}