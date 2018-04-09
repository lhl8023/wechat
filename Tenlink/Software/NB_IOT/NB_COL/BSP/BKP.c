#include "BKP.h"

void BKP_Init()
{
    u32 i;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE);
    u16 BKP_Data = BKP_ReadBackupRegister(BKP_DR1);
    if(BKP_Data == 0x1111)
    {
        for(i = 1500000;i>0;i--);
        GPIOA->ODR |= GPIO_Pin_1;  
        for(i = 1500000;i>0;i--);
        GPIOA->ODR &= ~GPIO_Pin_1;
        for(i = 1500000;i>0;i--);
        GPIOA->ODR |= GPIO_Pin_1; 
        for(i = 1500000;i>0;i--);
        GPIOA->ODR &= ~GPIO_Pin_1; 
        for(i = 1500000;i>0;i--);
        GPIOA->ODR |= GPIO_Pin_1;
        BKP_WriteBackupRegister(BKP_DR1, 0x0000);
        while(BKP_ReadBackupRegister(BKP_DR1))
        {
            for(i = 1500000;i>0;i--);
            GPIOA->ODR |= GPIO_Pin_1;  
            for(i = 1500000;i>0;i--);
            GPIOA->ODR &= ~GPIO_Pin_1;
            for(i = 1500000;i>0;i--);
            GPIOA->ODR |= GPIO_Pin_1; 
            for(i = 1500000;i>0;i--);
            GPIOA->ODR &= ~GPIO_Pin_1; 
            for(i = 1500000;i>0;i--);
            GPIOA->ODR |= GPIO_Pin_1;
        }
    }
    else
    {
        BKP_WriteBackupRegister(BKP_DR1, 0x1111);
        for(i = 3000000;i>0;i--);
        GPIOA->ODR &= ~GPIO_Pin_1;
        for(i = 3000000;i>0;i--);
        GPIOA->ODR |= GPIO_Pin_1;  
        for(i = 3000000;i>0;i--);
        GPIOA->ODR &= ~GPIO_Pin_1;
        for(i = 3000000;i>0;i--);
        GPIOA->ODR |= GPIO_Pin_1;  
        for(i = 3000000;i>0;i--);
        GPIOA->ODR &= ~GPIO_Pin_1;
        __set_FAULTMASK(1);      // 关闭所有中端
        NVIC_SystemReset();// 复位
    }
}