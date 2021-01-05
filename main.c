/*
 * Amazon FreeRTOS V1.4.7
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */

/* FreeRTOS includes. */
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#ifdef CY_USE_LWIP
#include "lwip/tcpip.h"
#endif

/* AWS library includes. */
#include "iot_system_init.h"
#include "iot_logging_task.h"
#include "iot_wifi.h"

/* BSP & Abstraction inclues */
#include "cybsp.h"
#include "cy_retarget_io.h"
#include "cyabs_rtos.h"
#include "command_console.h"
#include "iperf_utility.h"
#include "bt_utility.h"
#include "wifi_utility.h"
#include "platform/iot_threads.h"

#define CONSOLE_COMMAND_MAX_PARAMS     (32)
#define CONSOLE_COMMAND_MAX_LENGTH     (85)
#define CONSOLE_COMMAND_HISTORY_LENGTH (10)

const char* console_delimiter_string = " ";
static char command_buffer[CONSOLE_COMMAND_MAX_LENGTH];
static char command_history_buffer[CONSOLE_COMMAND_MAX_LENGTH * CONSOLE_COMMAND_HISTORY_LENGTH];

/* Logging Task Defines. */
#define mainLOGGING_MESSAGE_QUEUE_LENGTH    ( 200 )
#define mainLOGGING_TASK_STACK_SIZE         ( configMINIMAL_STACK_SIZE * 8 )

/* The task delay for allowing the lower priority logging task to print out Wi-Fi
 * failure status before blocking indefinitely. */
#define WIFI_UTIL_WIFI_STATUS_DELAY         pdMS_TO_TICKS( 5000 )

#define INACTIVE_INTERVAL_MS    300u

/* This macro specifies the continuous duration in milliseconds for which the
   network has to be inactive. If the network is inactive for this duaration,
   the MCU will suspend the network stack. Now, the MCU will not need to service
   the network timers which allows it to stay longer in sleep/deepsleep.
*/
#define INACTIVE_WINDOW_MS     200u

#define WIFI_CONNECT           1

#define WIFI_SSID              "SSID NAME"
#define WIFI_PASSWORD          "PASS PHRASE"

#define WIFI_SECURITY          eWiFiSecurityWPA2

#define APP_TASK_STACK_SIZE    (configMINIMAL_STACK_SIZE * 8)
#define APP_TASK_PRIORITY      tskIDLE_PRIORITY

int test_console_command(int argc, char *argv[], tlv_buffer_t** data);

#define TEST_CONSOLE_COMMANDS \
    { (char*) "test_console_command",           test_console_command, 1, NULL, NULL, (char *)"", (char *)"Test Console application" }, \

const cy_command_console_cmd_t test_command_table[] =
{
    TEST_CONSOLE_COMMANDS
    CMD_TABLE_END
};

int test_console_command(int argc, char *argv[], tlv_buffer_t** data)
{
    printf (" test_console_command is called , argc = %d , argv = %s \n", argc, argv[1]);
    return 0;
}

void RunApplicationTask(void * pArgument);

/**
 * @brief Application task startup hook for applications using Wi-Fi. If you are not
 * using Wi-Fi, then start network dependent applications in the vApplicationIPNetorkEventHook
 * function. If you are not using Wi-Fi, this hook can be disabled by setting
 * configUSE_DAEMON_TASK_STARTUP_HOOK to 0.
 */
void vApplicationDaemonTaskStartupHook( void );

/**
 * @brief Connects to Wi-Fi.
 */
static void prvWifiConnect( void );

/**
 * @brief LPA command console thread.
 */
void command_console_thread( void );
#define MAX_SCAN_NUMBER         100
WIFIScanResult_t ScanResults[ MAX_SCAN_NUMBER ];
cy_thread_t wifi_thread_ptr;

/**
 * @brief Initializes the board.
 */
static void prvMiscInitialization( void );

extern int uxTopUsedPriority;

/*-----------------------------------------------------------*/

/**
 * @brief Application runtime entry point.
 */
int main( void )
{
    /* Perform any hardware initialization that does not require the RTOS to be
     * running.  */
    prvMiscInitialization();

    /* Create tasks that are not dependent on the Wi-Fi being initialized. */
    xLoggingTaskInitialize( mainLOGGING_TASK_STACK_SIZE,
                            tskIDLE_PRIORITY,
                            mainLOGGING_MESSAGE_QUEUE_LENGTH );

    /* Start the scheduler.  Initialization that requires the OS to be running,
     * including the Wi-Fi initialization, is performed in the RTOS daemon task
     * startup hook. */
    vTaskStartScheduler();

    return 0;
}
/*-----------------------------------------------------------*/

static void prvMiscInitialization( void )
{
    cy_rslt_t result = cybsp_init();
    if (result != CY_RSLT_SUCCESS)
    {
        printf(  "BSP initialization failed \r\n" );
    }
    result = cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE);
    if (result != CY_RSLT_SUCCESS)
    {
        printf( "Retarget IO initializatoin failed \r\n" );
    }
    printf("prvMiscInitialization completed \n");
}
/*-----------------------------------------------------------*/
void vApplicationDaemonTaskStartupHook( void )
{
    /* Need to export this var to elf so that debugger can work properly */
    (void)uxTopUsedPriority;

    /* FIX ME: Perform any hardware initialization, that require the RTOS to be
     * running, here. */

    /* FIX ME: If your MCU is using Wi-Fi, delete surrounding compiler directives to
     * enable the unit tests and after MQTT, Bufferpool, and Secure Sockets libraries
     * have been imported into the project. If you are not using Wi-Fi, see the
     * vApplicationIPNetworkEventHook function. */
    if( SYSTEM_Init() == pdPASS )
    {
#ifdef CY_USE_LWIP
        /* Initialize lwIP stack. This needs the RTOS to be up since this function will spawn
         * the tcp_ip thread.
         */
        tcpip_init(NULL, NULL);
#endif

        /* Connect to the Wi-Fi before running the tests. */
#if WIFI_CONNECT
        prvWifiConnect();
#endif
       command_console_thread();
    }
}

void prvWifiConnect( void )
{
    WIFINetworkParams_t xNetworkParams;
    WIFIReturnCode_t xWifiStatus;
    uint8_t ucTempIp[4] = { 0 };
    cy_rslt_t result;
    uint8_t retry_count = 0;

    xWifiStatus = WIFI_On();

    if( xWifiStatus == eWiFiSuccess )
    {

        configPRINTF( ( "Wi-Fi module initialized. Connecting to AP...\r\n" ) );
    }
    else
    {
        configPRINTF( ( "Wi-Fi module failed to initialize.\r\n" ) );

        /* Delay to allow the lower priority logging task to print the above status.
            * The while loop below will block the above printing. */
        vTaskDelay( WIFI_UTIL_WIFI_STATUS_DELAY );

        while( 1 )
        {
        }
    }

    /* Setup parameters. */
    xNetworkParams.pcSSID = WIFI_SSID;
    xNetworkParams.ucSSIDLength = sizeof( WIFI_SSID ) - 1;
    xNetworkParams.pcPassword = WIFI_PASSWORD;
    xNetworkParams.ucPasswordLength = sizeof( WIFI_PASSWORD ) - 1;
    xNetworkParams.xSecurity = WIFI_SECURITY;
    xNetworkParams.cChannel = 0;
    while(true)
    {
        printf("calling WIFI_ConnectAP \n");
        xWifiStatus = WIFI_ConnectAP( &( xNetworkParams ) );
        if( xWifiStatus == eWiFiSuccess )
        {
            configPRINTF( ( "Wi-Fi Connected to AP. Creating tasks which use network...\r\n" ) );
            xWifiStatus = WIFI_GetIP( ucTempIp );
            if ( eWiFiSuccess == xWifiStatus )
            {
                configPRINTF( ( "IP Address acquired %d.%d.%d.%d\r\n",
                                ucTempIp[ 0 ], ucTempIp[ 1 ], ucTempIp[ 2 ], ucTempIp[ 3 ] ) );
				configPRINTF( ( "Connected to Wi-Fi Network \r\n" ) );
				break;
            }
        }
        else
        {
			retry_count++;
            if (retry_count >= 3)
            {
                configPRINTF(("Exceeded maximum Wi-Fi connection attempts\r\n"));
				break;
            }
            configPRINTF(("Failed to join Wi-Fi network. Retrying...\r\n"));
            continue;
        }
	}

    Iot_CreateDetachedThread(RunApplicationTask, NULL, APP_TASK_PRIORITY, APP_TASK_STACK_SIZE);
}

void RunApplicationTask(void *pArgument)
{
    struct netif *wifi;
    WIFIReturnCode_t xWifiStatus;

    (void)pArgument;

    while (true)
    {
        /* Configures an emac activity callback to the Wi-Fi interface and
         * suspends the network if the network is inactive for a duration of
         * INACTIVE_WINDOW_MS inside an interval of INACTIVE_INTERVAL_MS. The
         * callback is used to signal the presence/absence of network activity
         * to resume/suspend the network stack.
         */
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

/*-----------------------------------------------------------*/
void command_console_thread( void )
{
    WIFIReturnCode_t WiFiStatus;
    uint8_t IpAddr[4] = {0};

    printf("\r\n Command Console App\r\n");

    cy_command_console_cfg_t console_cfg;
    cy_rslt_t result;

#ifndef ENABLE_UART_POLLING
     const cyhal_uart_cfg_t uart_config =
     {
             .data_bits = 8,
             .stop_bits = 1,
             .parity = CYHAL_UART_PARITY_NONE,
             .rx_buffer = rx_buff,
             .rx_buffer_size = 50,
     };

     cyhal_uart_configure(&cy_retarget_io_uart_obj, &uart_config);
     cyhal_uart_register_callback(&cy_retarget_io_uart_obj, &uart_callback, NULL);
     cyhal_uart_event_t interrupt_mask = (cyhal_uart_event_t)(CYHAL_UART_IRQ_RX_NOT_EMPTY | CYHAL_UART_IRQ_RX_DONE
                                                             | CYHAL_UART_IRQ_RX_FULL | CYHAL_UART_IRQ_RX_ERROR );
     cyhal_uart_enable_event(&cy_retarget_io_uart_obj, interrupt_mask, CYHAL_ISR_PRIORITY_DEFAULT, true);
#endif

    console_cfg.serial = &cy_retarget_io_uart_obj;
    console_cfg.line_len = sizeof(command_buffer);
    console_cfg.buffer = command_buffer;
    console_cfg.history_len = CONSOLE_COMMAND_HISTORY_LENGTH;
    console_cfg.history_buffer_ptr = command_history_buffer;
    console_cfg.delimiter_string = console_delimiter_string;
    console_cfg.params_num = CONSOLE_COMMAND_MAX_PARAMS;
    console_cfg.delimiter_string = " ";
    console_cfg.thread_priority    = CY_RTOS_PRIORITY_NORMAL;

    /* Initialize command console library */
    result = cy_command_console_init(&console_cfg);
    if(result != CY_RSLT_SUCCESS)
    {
        printf("Error in command console init.\n");
    }

    result = cy_command_console_add_table ( test_command_table );
    if ( result != CY_RSLT_SUCCESS )
    {
        printf ("Error in adding command console table : %lu \n", result);
    }

    result = wifi_utility_init();

    if ( result != CY_RSLT_SUCCESS )
    {
        printf ("failed to initialize wifi utility, error : %lu \n", result);
    }

    iperf_utility_init(NULL);

    bt_utility_init();

    while(1)
    {
        vTaskDelay(500);
    }

}

/*-----------------------------------------------------------*/

/**
 * @brief User defined Idle task function.
 *
 * @note Do not make any blocking operations in this function.
 */
void vApplicationIdleHook( void )
{
    /* FIX ME. If necessary, update to application idle periodic actions. */

    static TickType_t xLastPrint = 0;
    TickType_t xTimeNow;
    const TickType_t xPrintFrequency = pdMS_TO_TICKS( 5000 );

    xTimeNow = xTaskGetTickCount();

    if( ( xTimeNow - xLastPrint ) > xPrintFrequency )
    {
        xLastPrint = xTimeNow;
    }
}

void vApplicationTickHook()
{
}

/*-----------------------------------------------------------*/

/**
* @brief User defined application hook to process names returned by the DNS server.
*/
#if ( ipconfigUSE_LLMNR != 0 ) || ( ipconfigUSE_NBNS != 0 )
    BaseType_t xApplicationDNSQueryHook( const char * pcName )
    {
        /* FIX ME. If necessary, update to applicable DNS name lookup actions. */

        BaseType_t xReturn;

        /* Determine if a name lookup is for this node.  Two names are given
         * to this node: that returned by pcApplicationHostnameHook() and that set
         * by mainDEVICE_NICK_NAME. */
        if( strcmp( pcName, pcApplicationHostnameHook() ) == 0 )
        {
            xReturn = pdPASS;
        }
        else if( strcmp( pcName, mainDEVICE_NICK_NAME ) == 0 )
        {
            xReturn = pdPASS;
        }
        else
        {
            xReturn = pdFAIL;
        }

        return xReturn;
    }

#endif /* if ( ipconfigUSE_LLMNR != 0 ) || ( ipconfigUSE_NBNS != 0 ) */
/*-----------------------------------------------------------*/

/**
 * @brief User defined assertion call. This function is plugged into configASSERT.
 * See FreeRTOSConfig.h to define configASSERT to something different.
 */
void vAssertCalled(const char * pcFile,
	uint32_t ulLine)
{
    /* FIX ME. If necessary, update to applicable assertion routine actions. */

	const uint32_t ulLongSleep = 1000UL;
	volatile uint32_t ulBlockVariable = 0UL;
	volatile char * pcFileName = (volatile char *)pcFile;
	volatile uint32_t ulLineNumber = ulLine;

	(void)pcFileName;
	(void)ulLineNumber;

	printf("vAssertCalled %s, %ld\n", pcFile, (long)ulLine);
	fflush(stdout);

	/* Setting ulBlockVariable to a non-zero value in the debugger will allow
	* this function to be exited. */
	taskDISABLE_INTERRUPTS();
	{
		while (ulBlockVariable == 0UL)
		{
			vTaskDelay( pdMS_TO_TICKS( ulLongSleep ) );
		}
	}
	taskENABLE_INTERRUPTS();
}
/*-----------------------------------------------------------*/

/**
 * @brief User defined application hook need by the FreeRTOS-Plus-TCP library.
 */
#if ( ipconfigUSE_LLMNR != 0 ) || ( ipconfigUSE_NBNS != 0 ) || ( ipconfigDHCP_REGISTER_HOSTNAME == 1 )
    const char * pcApplicationHostnameHook(void)
    {
        /* FIX ME: If necessary, update to applicable registration name. */

        /* This function will be called during the DHCP: the machine will be registered
         * with an IP address plus this name. */
        return clientcredentialIOT_THING_NAME;
    }

#endif

#ifndef CY_FN_DEFINE
void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                    char * pcTaskName )
{
    configPRINTF( ( "ERROR: stack overflow with task %s\r\n", pcTaskName ) );
    portDISABLE_INTERRUPTS();

    /* Unused Parameters */
    ( void ) xTask;

    /* Loop forever */
    for( ; ; )
    {
    }
}

void vApplicationMallocFailedHook()
{
    configPRINTF( ( "ERROR: Malloc failed to allocate memory\r\n" ) );
    taskDISABLE_INTERRUPTS();

    /* Loop forever */
    for( ; ; )
    {
    }
}
#endif
