



#include "stm32f4xx.h"
#include <stdio.h>
#include <stdlib.h>

#define TRIG_PIN1 0       // PA0 for Sensor 1 Trigger
#define ECHO_PIN1 1       // PA1 for Sensor 1 Echo
#define TRIG_PIN2 6       // PA2 for Sensor 2 Trigger
#define ECHO_PIN2 7       // PA3 for Sensor 2 Echo
#define RED_LED_PIN1 12   // PB12 for Sensor 1 Red LED
#define YELLOW_LED_PIN1 13 // PB13 for Sensor 1 Yellow LED
#define GREEN_LED_PIN1 14  // PB14 for Sensor 1 Green LED
#define RED_LED_PIN2 8    // PA8 for Sensor 2 Red LED
#define YELLOW_LED_PIN2 11 // PA11 for Sensor 2 Yellow LED
#define GREEN_LED_PIN2 12  // PA12 for Sensor 2 Green LED
#define TEMP_SENSOR_PIN 9  // PB9 for Temperature Sensor
#define PA0_EXTI_LINE (1 << 0) // EXTI Line for PA0
int flagg=0;
void NVIC_Config(void) {
    // Enable the EXTI line interrupt for PA0
    EXTI->IMR |= PA0_EXTI_LINE;          // Unmask the interrupt
    EXTI->FTSR |= PA0_EXTI_LINE;         // Configure for falling edge trigger (or rising edge if you prefer)
    NVIC_EnableIRQ(EXTI0_IRQn);          // Enable EXTI0 IRQ in the NVIC
}

void EXTI0_IRQHandler(void) {
    // This will be called when PA0 interrupt occurs
    if (EXTI->PR & PA0_EXTI_LINE) {      // Check if interrupt was triggered by PA0
        EXTI->PR |= PA0_EXTI_LINE; 
flagg=1;      // Clear the interrupt pending flag
        GPIOA->ODR &= ~((1U << RED_LED_PIN2) | (1U << YELLOW_LED_PIN2)| (1U << GREEN_LED_PIN2));
    }
}


//bluetooth

void USART1_Init(void);
void USART1_Write(char ch);
void USART1_WriteString(const char* str); // Function to send a string



#define FAN_PIN 8          // PB8 for Fan Control
#define TEMP_THRESHOLD 1

uint32_t flag=0;
uint32_t distance1; // Distance from Sensor 1
uint32_t distance2; // Distance from Sensor 2
uint32_t temperature; // Temperature reading

void printit(uint32_t temperature);
void Update_Fan(uint32_t temperature);
void Delay(uint32_t time);
void GPIO_Config(void);
void Timer_Config(void);
void ADC_Config(void);
uint32_t Measure_Distance1(void);
uint32_t Measure_Distance2(void);
uint32_t Read_Temperature(void);
void Update_LED1(uint32_t distance);
void Update_LED2(uint32_t distance);

int main(void) {
    SystemInit();
    GPIO_Config();
    Timer_Config();
    ADC_Config();
    USART1_Init();
	  USART1_Write('a'); 
	NVIC_Config();
    while (1) {
        distance1 = Measure_Distance1();  // Measure distance from Sensor 1
        distance2 = Measure_Distance2();  // Measure distance from Sensor 2
        temperature = Read_Temperature(); // Read temperature
				Update_Fan(temperature); 
        Update_LED1(distance1);
        Update_LED2(distance2);			// Update LEDs for Sensor 1
        // Initialize UART for Bluetooth communication
			int i=1;
         while(i==1){
				  printit(temperature);
					 i=0;
				 }

                   Delay(100);           // Delay between measurements
    }
}

void GPIO_Config(void) {
    // Enable GPIOA and GPIOB clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN;
   

    // Configure PA0 and PA2 as outputs for Sensor Triggers
    GPIOA->MODER |= (1U << (TRIG_PIN1 * 2)) | (1U << (TRIG_PIN2 * 2));
    GPIOA->OTYPER &= ~((1U << TRIG_PIN1) | (1U << TRIG_PIN2));
    GPIOA->OSPEEDR |= (3U << (TRIG_PIN1 * 2)) | (3U << (TRIG_PIN2 * 2));
    GPIOA->PUPDR &= ~((3U << (TRIG_PIN1 * 2)) | (3U << (TRIG_PIN2 * 2)));

    // Configure PA1 and PA3 as inputs for Sensor Echoes
    GPIOA->MODER &= ~((3U << (ECHO_PIN1 * 2)) | (3U << (ECHO_PIN2 * 2)));
    GPIOA->PUPDR &= ~((3U << (ECHO_PIN1 * 2)) | (3U << (ECHO_PIN2 * 2)));

    // Configure PB12, PB13, PB14 as outputs for Sensor 1 LEDs
    GPIOB->MODER |= (1U << (RED_LED_PIN1 * 2)) | (1U << (YELLOW_LED_PIN1 * 2)) | (1U << (GREEN_LED_PIN1 * 2));
    GPIOB->OTYPER &= ~((1U << RED_LED_PIN1) | (1U << YELLOW_LED_PIN1) | (1U << GREEN_LED_PIN1));
    GPIOB->OSPEEDR |= (3U << (RED_LED_PIN1 * 2)) | (3U << (YELLOW_LED_PIN1 * 2)) | (3U << (GREEN_LED_PIN1 * 2));
    GPIOB->PUPDR &= ~((3U << (RED_LED_PIN1 * 2)) | (3U << (YELLOW_LED_PIN1 * 2)) | (3U << (GREEN_LED_PIN1 * 2)));

    // Configure PA10, PA11, PA12 as outputs for Sensor 2 LEDs
    GPIOA->MODER |= (1U << (RED_LED_PIN2 * 2)) | (1U << (YELLOW_LED_PIN2 * 2)) | (1U << (GREEN_LED_PIN2 * 2));
    GPIOA->OTYPER &= ~((1U << RED_LED_PIN2) | (1U << YELLOW_LED_PIN2) | (1U << GREEN_LED_PIN2));
    GPIOA->OSPEEDR |= (3U << (RED_LED_PIN2 * 2)) | (3U << (YELLOW_LED_PIN2 * 2)) | (3U << (GREEN_LED_PIN2 * 2));
    GPIOA->PUPDR &= ~((3U << (RED_LED_PIN2 * 2)) | (3U << (YELLOW_LED_PIN2 * 2)) | (3U << (GREEN_LED_PIN2 * 2)));

    // Configure PB9 as analog input for Temperature Sensor
    GPIOB->MODER |= (3U << (TEMP_SENSOR_PIN * 2)); // Set to analog mode
    GPIOB->PUPDR &= ~(3U << (TEMP_SENSOR_PIN * 2));
		
		
		//for the fan
		
		GPIOB->MODER |= (1U << (FAN_PIN * 2)); // Set PB8 as output
    GPIOB->OTYPER &= ~(1U << FAN_PIN);     // Set PB8 as push-pull
    GPIOB->OSPEEDR |= (3U << (FAN_PIN * 2)); // High speed for PB8
    GPIOB->PUPDR &= ~(3U << (FAN_PIN * 2)); // No pull-up/down for PB8
		
		
		
}

void Timer_Config(void) {
	
		RCC->CR |= RCC_CR_HSION;
    while (!(RCC->CR & RCC_CR_HSIRDY)) {}

    /* Set HSI as system clock */ // this clk is 16mhz
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_HSI;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI) {}

    /* Configure flash latency */
     FLASH->ACR = FLASH_ACR_LATENCY_0WS;
      RCC->APB2ENR |= RCC_APB2ENR_USART1EN;//APB2 Peripheral Clock (PCLK2): Up to 84 MHz

    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;//APB1 Peripheral Clock (PCLK1): Up to 42 MHz
    TIM2->PSC = 84 - 1;
    TIM2->ARR = 0xFFFFFFFF;
    TIM2->CR1 |= TIM_CR1_CEN;
}

void ADC_Config(void) {
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;   // Enable ADC1 clock
    ADC1->CR2 = 0;                        // Reset CR2 to 0
    ADC1->SQR3 = 17;                      // Channel 17 for PB9
    ADC1->CR2 |= ADC_CR2_ADON;            // Enable ADC
}

uint32_t Read_Temperature(void) {
    ADC1->CR2 |= ADC_CR2_SWSTART;         // Start conversion
    while (!(ADC1->SR & ADC_SR_EOC));     // Wait for end of conversion
    return ADC1->DR;                      // Return ADC data register value
}

void Delay(uint32_t time) {
    time *= 1000;
    while (time--) {
        __NOP();
    }
}

uint32_t Measure_Distance1(void) {
    uint32_t start, end, pulse_width;
EXTI->IMR &= ~PA0_EXTI_LINE;    // Mask interrupt for PA0
	
    GPIOA->ODR &= ~(1U << TRIG_PIN1);
    Delay(2);
    GPIOA->ODR |= (1U << TRIG_PIN1);
    Delay(10);
    GPIOA->ODR &= ~(1U << TRIG_PIN1);
EXTI->IMR |= PA0_EXTI_LINE; //unmask
    while (!(GPIOA->IDR & (1U << ECHO_PIN1)));
    start = TIM2->CNT;
    while (GPIOA->IDR & (1U << ECHO_PIN1));
    end = TIM2->CNT;

    pulse_width = (end > start) ? (end - start) : (0xFFFFFFFF - start + end);
    return (pulse_width * 0.0343) / 2;
}

uint32_t Measure_Distance2(void) {
    uint32_t start, end, pulse_width;
    GPIOA->ODR &= ~(1U << TRIG_PIN2);
    Delay(2);
    GPIOA->ODR |= (1U << TRIG_PIN2);
    Delay(10);
    GPIOA->ODR &= ~(1U << TRIG_PIN2);

    while (!(GPIOA->IDR & (1U << ECHO_PIN2)));
    start = TIM2->CNT;
    while (GPIOA->IDR & (1U << ECHO_PIN2));
    end = TIM2->CNT;

    pulse_width = (end > start) ? (end - start) : (0xFFFFFFFF - start + end);
    return (pulse_width * 0.0343) / 2;
}


void Update_LED1(uint32_t distance) {
    if (distance < 200) {
			  GPIOB->ODR |= (1U << FAN_PIN);
        GPIOB->ODR |= (1U << RED_LED_PIN1);
        GPIOB->ODR &= ~((1U << YELLOW_LED_PIN1) | (1U << GREEN_LED_PIN1));
    } else if (distance >= 200 && distance <= 1500) {
        GPIOB->ODR |= (1U << YELLOW_LED_PIN1);
        GPIOB->ODR &= ~((1U << RED_LED_PIN1) | (1U << GREEN_LED_PIN1));
    } else {
        GPIOB->ODR |= (1U << GREEN_LED_PIN1);
        GPIOB->ODR &= ~((1U << RED_LED_PIN1) | (1U << YELLOW_LED_PIN1));
    }
}

void Update_LED2(uint32_t distance) {
    if (distance < 500) {
        GPIOA->ODR |= (1U << RED_LED_PIN2);
        GPIOA->ODR &= ~((1U << YELLOW_LED_PIN2) | (1U << GREEN_LED_PIN2));
    } else if (distance >= 500 && distance <= 1500) {
        GPIOA->ODR |= (1U << YELLOW_LED_PIN2);
        GPIOA->ODR &= ~((1U << RED_LED_PIN2) | (1U << GREEN_LED_PIN2));
    } else {
        GPIOA->ODR |= (1U << GREEN_LED_PIN2);
        GPIOA->ODR &= ~((1U << RED_LED_PIN2) | (1U << YELLOW_LED_PIN2));
}
}


void Update_Fan(uint32_t temperature) {
    if (temperature > TEMP_THRESHOLD) {
				flag=1;
			  //GPIOA->ODR |= (1U << RED_LED_PIN2);
        GPIOB->ODR |= (1U << FAN_PIN);   // Turn on fan
    } else {
        GPIOB->ODR &= ~(1U << FAN_PIN);  // Turn off fan
    }
}




void printit(uint32_t temperature) {
    for (int i = 0; i < 100000; i++) {} // Delay

    // Generate a random heart rate between 75 and 80 BPM

    // Prepare message to send
    char message[30];

    sprintf(message, "temperature %d",temperature);
    USART1_WriteString(message);
    
}



void USART1_Init(void) {
    RCC->AHB1ENR |= (1U << 0);  // Enable GPIOA clock
    RCC->APB2ENR |= (1U << 4);  // Enable USART1 clock

    // Configure PA9 (TX) and PA10 (RX) for USART1 in alternate function mode
    GPIOA->MODER &= ~(3U << (9 * 2) | 3U << (10 * 2));
    GPIOA->MODER |= (2U << (9 * 2)) | (2U << (10 * 2)); // Set PA9, PA10 to AF mode
    GPIOA->AFR[1] |= (7U << (1 * 4)) | (7U << (2 * 4)); // Set AF7 for PA9 and PA10 (USART1)

    // Configure USART1 for 9600 baud rate
    USART1->BRR = 0x683; // Assuming 16 MHz clock, BRR for 9600 baud   divide the clock value by our baud rate///not ????
    USART1->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE; // Enable TX, RX, and USART1
}

void USART1_Write(char ch) {
    while (!(USART1->SR & USART_SR_TXE)); // Wait until TX buffer is empty
    USART1->DR = ch;                       // Send character
    while (!(USART1->SR & USART_SR_TC));   // Wait until transmission is complete
}

void USART1_WriteString(const char* str) {
     while (*str) {                         // Loop until the null terminator
        USART1_Write(*str++);             // Send each character
    }
}