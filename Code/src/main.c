#include "stm32f429xx.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"
//#include "event_groups.h"


//void delay(uint32_t delay)	{	for(volatile uint32_t i = 0; i < delay;i++){}	}
//uint8_t ucHeap[ configTOTAL_HEAP_SIZE ];

//********************************************************************************
//private function prototype
static void gpioInit(void);

//********************************************************************************
//tasks handle
TaskHandle_t led1Task, led2Task, led3Task;

//********************************************************************************
//task prototype
static void led1Blink(void);
static void led2Blink(void);
static void led3Blink(void);


//********************************************************************************
//main function
int main (void)
{
	gpioInit();
	
	xTaskCreate((TaskFunction_t)led1Blink, "led1", 24, NULL, 1, &led1Task);	
	xTaskCreate((TaskFunction_t)led2Blink, "led2", 24, NULL, 1, &led2Task);
	xTaskCreate((TaskFunction_t)led3Blink, "led2", 24, NULL, 1, &led3Task);
	
	vTaskStartScheduler();	
	
	while(1) {	}	
}

//********************************************************************************
//
void rccInit(void)
{

}

//********************************************************************************
//gpio init
static void gpioInit(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;																		//enable clock portB
	GPIOB->MODER &= ~(GPIO_MODER_MODE7_0 | GPIO_MODER_MODE14_0);							//clr mode pb7, pb 14
	GPIOB->MODER |= GPIO_MODER_MODE7_0 | GPIO_MODER_MODE14_0;								//setup gp-out mode								
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT7 | GPIO_OTYPER_OT14);										//setup push-pull mode
	GPIOB->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED7 | GPIO_OSPEEDR_OSPEED14);			//low speed 
	//GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED7_0 | GPIO_OSPEEDR_OSPEED14_0;			//medium speed
	
	
	GPIOB->MODER &= ~GPIO_MODER_MODE0_0;																			//clr mode pb7, pb 14
	GPIOB->MODER |= GPIO_MODER_MODE0_0;																			//setup gp-out mode								
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT0;																				//setup push-pull mode
	GPIOB->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED0;																//low speed 
	
//	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;																		//enable clock portA
//	GPIOA->MODER &= ~GPIO_MODER_MODE5_0;																			//clr mode pb7, pb 14
//	GPIOA->MODER |= GPIO_MODER_MODE5_0;																			//setup gp-out mode								
//	GPIOA->OTYPER &= ~GPIO_OTYPER_OT5;																				//setup push-pull mode
//	GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED5;																//low speed 
}



//********************************************************************************
//task led 1
static void led1Blink(void)
{	
	for(;;)
	{		
		GPIOB->BSRR = GPIO_BSRR_BS7;
		vTaskDelay(pdMS_TO_TICKS(250));		
		GPIOB->BSRR = GPIO_BSRR_BR7;
		vTaskDelay(pdMS_TO_TICKS(250));		
	}	
}

//********************************************************************************
//task led 2
static void led2Blink(void)
{	
	for(;;)
	{		
		GPIOB->BSRR = GPIO_BSRR_BS14;
		vTaskDelay(pdMS_TO_TICKS(1000));
		GPIOB->BSRR = GPIO_BSRR_BR14;
		vTaskDelay(pdMS_TO_TICKS(1000));		
	}	
}

//********************************************************************************
//led 3 blink
static void led3Blink(void)
{	
	for(;;)
	{		
		for(volatile uint8_t i = 0; i < 10; i++)
		{
			GPIOB->BSRR = GPIO_BSRR_BS0;
			vTaskDelay(pdMS_TO_TICKS(100));
			GPIOB->BSRR = GPIO_BSRR_BR0;
			vTaskDelay(pdMS_TO_TICKS(100));	
		}

		vTaskDelay(pdMS_TO_TICKS(3000));
		
	}	
}
	

void vAssertCalled( unsigned long ulLine, const char * const pcFileName )
{
//static portBASE_TYPE xPrinted = pdFALSE;
volatile uint32_t ulSetToNonZeroInDebuggerToContinue = 0;

    /* Parameters are not used. */
    ( void ) ulLine;
    ( void ) pcFileName;

    taskENTER_CRITICAL();
    {
        /* You can step out of this function to debug the assertion by using
        the debugger to set ulSetToNonZeroInDebuggerToContinue to a non-zero
        value. */
        while( ulSetToNonZeroInDebuggerToContinue == 0 )
        {
        }
    }
    taskEXIT_CRITICAL();
}

/* configSUPPORT_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize )
{
/* If the buffers to be provided to the Idle task are declared inside this
function then they must be declared static – otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xIdleTaskTCB;
static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task’s
    state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task’s stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
/*———————————————————–*/

/* configSUPPORT_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
application must provide an implementation of vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer,
                                     StackType_t **ppxTimerTaskStackBuffer,
                                     uint32_t *pulTimerTaskStackSize )
{
/* If the buffers to be provided to the Timer task are declared inside this
function then they must be declared static – otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xTimerTaskTCB;
static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
    task’s state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task’s stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configTIMER_TASK_STACK_DEPTH is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

