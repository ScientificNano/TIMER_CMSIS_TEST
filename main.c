#include "stm32f407xx.h"
#define SYSCLOCK 168000000U
typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;//нахуя это нужно?
void RCC_init(void){
RCC->CR|=RCC_CR_HSEON;
while((RCC->CR&(RCC_CR_HSERDY))!=RESET){};
FLASH->ACR&=~FLASH_ACR_PRFTEN;
FLASH->ACR|=FLASH_ACR_PRFTEN;
FLASH->ACR=(((FLASH->ACR) & (~(FLASH_ACR_LATENCY)))|FLASH_ACR_LATENCY_5WS);
RCC->CFGR=(((RCC->CFGR) & ~(RCC_CFGR_HPRE))|RCC_CFGR_HPRE_DIV1);
RCC->CFGR=(((RCC->CFGR) & ~(RCC_CFGR_PPRE1))|RCC_CFGR_PPRE1_DIV4);
RCC->CFGR=(((RCC->CFGR) & ~(RCC_CFGR_PPRE2))|RCC_CFGR_PPRE2_DIV2);
RCC->PLLCFGR=(((RCC->PLLCFGR)& ~(RCC_PLLCFGR_PLLM))|RCC_PLLCFGR_PLLM_2);
RCC->PLLCFGR=(((RCC->PLLCFGR)& ~(RCC_PLLCFGR_PLLN))|RCC_PLLCFGR_PLLN_7|RCC_PLLCFGR_PLLN_5|RCC_PLLCFGR_PLLN_3);//разные способы включения битов со строкой 19
//RCC->PLLCFGR=(((RCC->PLLCFGR)& ~(RCC_PLLCFGR_PLLP))|0x0UL);// очистка битов не тоже самое??
RCC->PLLCFGR&=~RCC_PLLCFGR_PLLP;
RCC->PLLCFGR=(((RCC->PLLCFGR)& ~(RCC_PLLCFGR_PLLSRC))|RCC_PLLCFGR_PLLSRC_HSE);
RCC->PLLCFGR=(((RCC->PLLCFGR)& ~(RCC_PLLCFGR_PLLQ))|0x7000000U);//тактирование usb
RCC->CR|=RCC_CR_PLLON;
while((RCC->CR&(RCC_CR_PLLRDY))!=RCC_CR_PLLRDY){};
RCC->CFGR |= RCC_CFGR_SW_PLL; 					
while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {}
}
void timer_init(){
RCC->APB1ENR=RCC_APB1ENR_TIM2EN;
TIM2->SMCR&=~TIM_SMCR_SMS;
TIM2->CR1=TIM_CR1_CEN;
TIM2->PSC=1679;
TIM2->ARR=99999;
TIM2->DIER|=TIM_DIER_UIE;
NVIC_EnableIRQ(TIM2_IRQn);
}
int main(){
RCC_init();
timer_init();
RCC->AHB1ENR|= RCC_AHB1ENR_GPIOAEN;
GPIOA->MODER|=GPIO_MODER_MODER6_0;
while(1){}
}
void TIM2_IRQHandler(void){
TIM2->SR&= ~TIM_SR_UIF;
GPIOA->ODR^= (1 << 6);
}
