#include "CST816.h"

#define TOUCH_OFFSET_Y 15
#define REVERSE 1

CST816_Info CST816_Instance;

iic_bus_t CST816_dev =
	{
		.IIC_SDA_PORT = GPIOB,
		.IIC_SCL_PORT = GPIOB,
		.IIC_SDA_PIN = LL_GPIO_PIN_7,
		.IIC_SCL_PIN = LL_GPIO_PIN_6,
};

/**
 * @brief CST816 GPIO口初始化
 * @param None
 * @retval None
 */
void CST816_GPIO_Init(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

	/* 初始化复位引脚 */
	GPIO_InitStruct.Pin = TOUCH_RST_PIN;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	LL_GPIO_Init(TOUCH_RST_PORT, &GPIO_InitStruct);

	LL_GPIO_SetOutputPin(TOUCH_RST_PORT, TOUCH_RST_PIN);

	/* 初始化中断引脚,不用中断的话屏蔽掉 */

	/* 初始化I2C引脚 */
	IICInit(&CST816_dev);

	/* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
	// IICStop(&CST816_dev);
}

/**
 * @brief CST816初始化
 * @param None
 * @retval None
 */
void CST816_Init(void)
{
	CST816_GPIO_Init();
	CST816_Config_AutoSleepTime(5);
}

/**
 * @brief 读取触摸屏单个寄存器的数据
 * @param reg：寄存器地址
 * @retval 返回寄存器存储的数据
 */
uint8_t CST816_IIC_ReadREG(uint8_t addr)
{
	return IIC_Read_One_Byte(&CST816_dev, Device_Addr, addr);
}

/**
 * @brief 向触摸屏的寄存器写入数据
 * @param addr：寄存器地址
 * @param dat:  写入的数据
 * @retval 返回寄存器存储的数据
 */
void CST816_IIC_WriteREG(uint8_t addr, uint8_t dat)
{
	IIC_Write_One_Byte(&CST816_dev, Device_Addr, addr, dat);
}

/**
 * @brief 触摸屏复位
 * @param None
 * @retval None
 */
void CST816_RESET(void)
{
	TOUCH_RST_0;
	delay_ms(10);
	TOUCH_RST_1;
	delay_ms(100);
}

/**
 * @brief 读取触摸屏在触摸时的坐标值
 * @param None
 * @retval None（数据存储在CST816_Instance结构体中）
 */
void CST816_Get_XY_AXIS(void)
{
	uint8_t DAT[4];
	IIC_Read_Multi_Byte(&CST816_dev, Device_Addr, XposH, 4, DAT);
	CST816_Instance.X_Pos = ((DAT[0] & 0x0F) << 8) | DAT[1];				  //(temp[0]&0X0F)<<4|
	CST816_Instance.Y_Pos = ((DAT[2] & 0x0F) << 8) | DAT[3] + TOUCH_OFFSET_Y; //(temp[2]&0X0F)<<4|
#if REVERSE
	CST816_Instance.X_Pos = 239 - CST816_Instance.X_Pos;
	CST816_Instance.Y_Pos = 279 - CST816_Instance.Y_Pos;
#endif
}

/**
 * @brief 读取触摸屏的手指触摸个数,0xFF为睡眠
 * @param None
 * @retval 返回芯片ID
 */
uint8_t CST816_Get_FingerNum(void)
{
	return CST816_IIC_ReadREG(FingerNum);
}

/**
 * @brief 读取触摸屏的芯片ID
 * @param None
 * @retval 返回芯片ID
 */
uint8_t CST816_Get_ChipID(void)
{
	return CST816_IIC_ReadREG(ChipID);
}

/**
 * @brief 使能连续动作（连续左右滑动，连续上下滑动，双击）
 * @param mode：模式(5种)
 * @retval None
 * 注    意：使能连续动作会增加响应时间
 */
void CST816_Config_MotionMask(uint8_t mode)
{
	CST816_IIC_WriteREG(MotionMask, mode);
}

/**
 * @brief 规定time内无触摸，自动进入低功耗模式
 * @param time：时间(s)
 * @retval None
 */
void CST816_Config_AutoSleepTime(uint8_t time)
{
	CST816_IIC_WriteREG(AutoSleepTime, time);
}

/**
 * @brief 进入睡眠，无触摸唤醒功能
 * @param None
 * @retval None
 */
void CST816_Sleep(void)
{
	CST816_IIC_WriteREG(SleepMode, 0x03);
}

/**
 * @brief 唤醒
 * @param None
 * @retval None
 */
void CST816_Wakeup(void)
{
	CST816_RESET();
}

/**
 * @brief 手势检测滑动分区角度控制。Angle=tan(c)*10 c为以x轴正方向为基准的角度。
 * @param x_right_y_up_angle：角度值
 * @retval None
 */
void CST816_Config_MotionSlAngle(uint8_t x_right_y_up_angle)
{
	CST816_IIC_WriteREG(MotionSlAngle, x_right_y_up_angle);
}

/**
 * @brief 正常快速检测周期配置函数。
 *		  此值会影响到LpAutoWakeTime和AutoSleepTime。
 *		  单位10ms，可选值：1～30。默认值为1。
 * @param Period：周期值
 * @retval None
 */
void CST816_Config_NorScanPer(uint8_t Period)
{
	if (Period >= 30)
		Period = 30;
	CST816_IIC_WriteREG(NorScanPer, Period);
}

/**
 * @brief 中断低脉冲输出宽度配置函数
 * @param Period：周期值
 * @retval None
 */
void CST816_Config_IrqPluseWidth(uint8_t Width)
{
	if (Width >= 200)
		Width = 200;
	CST816_IIC_WriteREG(IrqPluseWidth, Width);
}

/**
 * @brief 功耗扫描唤醒门限配置函数。越小越灵敏。默认值48
 * @param TH：门限值
 * @retval None
 */
void CST816_Config_LpScanTH(uint8_t TH)
{
	CST816_IIC_WriteREG(LpScanTH, TH);
}
