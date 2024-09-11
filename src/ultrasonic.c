#include "ultrasonic.h"

void US_send_pulse()
{
    // Arm EXTI
    NVIC_enable_IRQ(EXTI15_10_IRQn, 2);

    // Send pulse.
    GPIO_write_pin(GPIOA, TRIGGER, 0);
    for (int i = 0; i < 20; i++)
    {
    }
    GPIO_write_pin(GPIOA, TRIGGER, 1);
    for (int i = 0; i < 100; i++)
    {
    }
    GPIO_write_pin(GPIOA, TRIGGER, 0);
}

void US_init(){
    GPIO_Init_t gpio_init_handle;
    timer_init_typedef timer_init_handle;

    // PA9 OUT
    gpio_init_handle.gpio_base = GPIOA;
    gpio_init_handle.mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_handle.speed = GPIO_SPEED_10MHZ;
    gpio_init_handle.pin = TRIGGER;
    GPIO_init(&gpio_init_handle);

    // PA10 IN pull down
    gpio_init_handle.pin = ECHO;
    gpio_init_handle.mode = GPIO_MODE_INPUT_PULL_DOWN;
    GPIO_init(&gpio_init_handle);

    timer_init_handle.timer_base = TIM2;
    timer_init_handle.prescaler = 7;
    timer_init_handle.period = 50000;
    timer_init(&timer_init_handle);
    timer_reset(TIM2);

    // Enable EXTI for PA10.
    EXTI_enable(EXTI_PORTA, ECHO, 1, 1);

}
