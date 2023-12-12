#include "main.h"

void init_tim_gpio(void);
void init_tim(int period);

int main(void)
{
    init_tim_gpio();
    init_tim(1000);

    while (1)
    {
        for (int duty = 0; duty <= 1000; duty += 10)
        {
            TIM1->CCR4 = duty;
            TIM1->CCR3 = duty;
            TIM1->CCR1 = duty;
            delay_ms(10);
        }

        for (int duty = 1000; duty >= 0; duty -= 10)
        {
            TIM1->CCR4 = duty;
            TIM1->CCR3 = duty;
            TIM1->CCR1 = duty;
            delay_ms(10);
        }
    }
}

void init_tim_gpio(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOEEN;

    GPIOE->MODER |= GPIO_MODER_MODER13_1 | GPIO_MODER_MODER14_1 | GPIO_MODER_MODER15_1;
    GPIOE->MODER &= ~(GPIO_MODER_MODER13_0 | GPIO_MODER_MODER14_0 | GPIO_MODER_MODER15_0);

    GPIOE->AFR[1] |= (2 << ((13 - 8) * 4)); // PE13 in AFR[1]
    GPIOE->AFR[1] |= (2 << ((14 - 8) * 4)); // PE14 in AFR[1]
    GPIOE->AFR[1] |= (2 << ((15 - 8) * 4)); // PE15 in AFR[1]
}

void init_tim(int period)
{
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

    TIM1->CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2;
    TIM1->CCMR2 |= TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2;
    TIM1->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;

    TIM1->CCMR2 |= TIM_CCMR2_OC3PE | TIM_CCMR2_OC4PE;
    TIM1->CCMR1 |= TIM_CCMR1_OC1PE;

    TIM1->CR1 |= TIM_CR1_ARPE;
    TIM1->ARR = period;

    TIM1->BDTR |= TIM_BDTR_MOE;
    TIM1->CCER |= TIM_CCER_CC3E | TIM_CCER_CC4E | TIM_CCER_CC1E;

    TIM1->CR1 |= TIM_CR1_CEN;
}

void delay_ms(int ms)
{
    for (volatile int i = 0; i < ms * 800; ++i)
    {
        __NOP();
    }
}
