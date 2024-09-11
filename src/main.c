#include "rcc.h"
#include "uart.h"
#include "systick.h"
#include "gpio.h"
#include "timer.h"
#include "nvic.h"
#include "printf.h"
#include "ultrasonic.h"

#define BUZZER 4
#define LED 8
#define TRIGGER 9
#define ECHO 10

#define WARNING_DISTANCE 50

void SysClk_init();

UART_Init_t uart_init_handle;
GPIO_Init_t gpio_init_handle;

uint32_t ticks_global = 0;
uint32_t echo_delay = 0;
float distance = 0.0;
uint8_t distance_str[25] = {};

uint8_t message[10];

uint32_t ticks_buzzer = 1;
uint32_t buzzer_period = 0;
uint8_t buzzer_state = 0;

int main()
{
    SysClk_init();

    RCC_USART3_ENABLE();
    RCC_PORTA_ENABLE();
    RCC_PORTB_ENABLE();
    RCC_TIM2_ENABLE();
    RCC_AFIO_ENABLE();

    // PA8 OUT (LED)
    gpio_init_handle.mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_handle.gpio_base = GPIOA;
    gpio_init_handle.pin = LED;
    gpio_init_handle.speed = GPIO_SPEED_10MHZ;
    GPIO_init(&gpio_init_handle);

    // PA4 OUT  (BUZZER)
    gpio_init_handle.pin = BUZZER;
    GPIO_init(&gpio_init_handle);

    // PB10 (TX) AFIO Pull Push
    UART_TX_cfg(GPIOB, 10);

    // PB11 (RX) GPIO Input floating
    UART_RX_cfg(GPIOB, 11);

    // Initialize USART3
    uart_init_handle.baud_rate = 9600;
    uart_init_handle.USART_base = USART3;
    uart_init_handle.direction = UART_DIR_FULL_DUPLEX;
    uart_init_handle.mode = UART_MODE_DMA;
    UART_Init(&uart_init_handle);

    US_init();

    SysTick_interrupt_init(1);

    while (1)
    {
        if (distance <= WARNING_DISTANCE)
        {
            GPIO_write_pin(GPIOA, LED, 1);
        }
        else
        {
            buzzer_state = 0;
            GPIO_write_pin(GPIOA, LED, 0);
            GPIO_write_pin(GPIOA, BUZZER, 0);
        }

        GPIO_write_pin(GPIOA, BUZZER, buzzer_state);
    }

    return 0;
}

void SysTick_Handler()
{
    ticks_global++;
    ticks_buzzer++;

    if (ticks_buzzer % (uint32_t)(10 * distance) == 0)
    {
        buzzer_state ^= 1;
        ticks_buzzer = 0;
    }

    if (ticks_global % 200 == 0)
    {
        US_send_pulse();
    }
}

void EXTI15_10_IRQHandler()
{
    if (GPIO_read_pin(GPIOA, ECHO))
    {
        // At rising edge, start timer.
        TIM2->CR1 |= 1 << 0; // Counter enable.
    }

    else
    {
        // At falling edge, record time and stop timer.
        echo_delay = TIM2->CNT;
        timer_reset(TIM2);
        NVIC_disable_IRQ(EXTI15_10_IRQn);
        distance = (echo_delay) / 58.0;
        sprintf(distance_str, "D: %.2f cm\n", distance);
        UART_printf(USART3, distance_str);
    }

    EXTI_CLEAR_PENDING(10);
    NVIC_CLEAR_PENDING(EXTI15_10_IRQn);
}

void SysClk_init()
{
    RCC->CR |= 1 << 16; // Turn HSE on.
    while (!(RCC->CR & 1 << 17))
    {
        // While HSE not ready, wait.
    }

    RCC->CFGR |= 1;       // Choose HSE as the source.
    RCC->CR &= ~(1 << 0); // Turnoff HSI. (To reduce power consumption).
}

void HardFault_Handler()
{
    RCC_PORTC_ENABLE();
    GPIO_Init_t gpio_init_handle;
    gpio_init_handle.gpio_base = GPIOC;
    gpio_init_handle.mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_handle.pin = 13;
    gpio_init_handle.speed = GPIO_SPEED_10MHZ;
    GPIO_init(&gpio_init_handle);

    while (1)
    {
        GPIO_write_pin(GPIOC, 13, 1);
        SysTick_delay_ms(1000);
        GPIO_write_pin(GPIOC, 13, 0);
        SysTick_delay_ms(1000);
    };
}
