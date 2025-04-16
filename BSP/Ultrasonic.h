#ifndef __ULTRASONIC_H
#define __ULTRASONIC_H



#ifdef UltrasonicTX
void UltrasonicTX_Init(void);
void UltrasonicTX_Reset(void);
void UltrasonicTX_Set(void);
uint8_t Ultrasonic_GetTxFlag(void);
void Ultrasonic_GiveTxNum(uint8_t data);
uint8_t Ultrasonic_GetTxNum(void);
void Ultrasonic_Send_Task(void);
#endif

#ifdef UltrasonicRX
void UltrasonicRX_Init(void);
uint8_t UltrasonicRX_Get(void);
uint8_t Ultrasonic_GetRxFlag(void);
uint8_t Ultrasonic_Receive_State(void);
uint8_t Ultrasonic_Receive_Task(void);
#endif

#endif
