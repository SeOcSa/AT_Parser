#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "LPC177x_8x.h"
#include "system_LPC177x_8x.h"
#include <retarget.h>


#include <DRV\drv_sdram.h>
#include <DRV\drv_lcd.h>
#include <DRV\drv_uart.h>
#include <DRV\drv_touchscreen.h>
#include <DRV\drv_led.h>
#include <utils\timer_software.h>
#include <utils\timer_software_init.h>
#include "at.h"


uint8_t at_command_simple[20]="AT\r\n";
uint8_t at_command_sms[30] = "AT+CMGS=\"0748708200\"\r\n";
uint8_t send_sms_response[100]="";
uint8_t sms_text[30]="Am trimis mesaj";
uint8_t expected_string_length = 0;
uint8_t first_response[5] = "\r\n>";
uint8_t second_response[5] = "OK\r\n";
						
timer_software_handler_t my_timer_handler; 
timer_software_handler_t handler;

st_answer data_structure;

void timer_callback_1(timer_software_handler_t h)
{
}

void TouchScreenCallBack(TouchResult* touchData)
{
	printf("touched X=%3d Y=%3d\n", touchData->X, touchData->Y);		
	
}

void BoardInit()
{
	timer_software_handler_t init_handler ;
	
	TIMER_SOFTWARE_init_system();
	
	
	DRV_SDRAM_Init();
	
	initRetargetDebugSystem();
	DRV_LCD_Init();
	DRV_LCD_ClrScr();
	DRV_LCD_PowerOn();	
	
	DRV_TOUCHSCREEN_Init();
	DRV_TOUCHSCREEN_SetTouchCallback(TouchScreenCallBack);
	DRV_LED_Init();
	printf ("Hello\n");	
	
//	init_handler = TIMER_SOFTWARE_request_timer();
//	TIMER_SOFTWARE_configure_timer(init_handler , MODE_1, 100, 1);
//	TIMER_SOFTWARE_set_callback(init_handler , timer_callback_1);
//	TIMER_SOFTWARE_start_timer(init_handler );
}


void ExecuteCommand( uint8_t* command);
void SendCommand( uint8_t* command);
void GetCommandResponse();
uint32_t CommandResponseValid();
uint32_t ExtractData();
uint32_t ConvertAsuToDbmw(uint32_t value);
void reset_flags();
void ExtractDataString();

int main(void)
{

	uint8_t ch;
	uint32_t rssi_value_asu;
	uint32_t rssi_value_dbmw;
	int counter = 0;
	
	BoardInit();
	DRV_UART_Configure(UART_3, UART_CHARACTER_LENGTH_8, 115200, UART_PARITY_NO_PARITY, 1, TRUE, 3);
	DRV_UART_Configure(UART_2, UART_CHARACTER_LENGTH_8, 115200, UART_PARITY_NO_PARITY, 1, TRUE, 3);
	my_timer_handler = TIMER_SOFTWARE_request_timer();
	TIMER_SOFTWARE_configure_timer(my_timer_handler, MODE_1, 1000, 1);
	
	TIMER_SOFTWARE_start_timer(my_timer_handler);
	
	handler = TIMER_SOFTWARE_request_timer();
	TIMER_SOFTWARE_configure_timer(handler, MODE_1, 30000, 1);
	
	SendCommand(at_command_simple);
	TIMER_SOFTWARE_Wait(1000);
	
	SendCommand(at_command_simple);
	TIMER_SOFTWARE_Wait(1000);
	
	SendCommand(at_command_simple);
	TIMER_SOFTWARE_Wait(1000);

		
	while(1)
	{
	
		if (TIMER_SOFTWARE_interrupt_pending(my_timer_handler))
		{
				expected_string_length = strlen(first_response);
				ExecuteCommand(at_command_sms);
			
				if(strcmp(send_sms_response,first_response) ==0)
				{
						expected_string_length = strlen(second_response);
						SendCommand(sms_text);
						DRV_UART_SendByte(UART_3,0x1A);
						GetCommandResponse();
						printf("raspuns: %s\n",send_sms_response);
						if(strcmp(send_sms_response,second_response)== 0)
							printf("Mesaj trimis cu succes\n");
						else
							printf("error\n");
				}
				
				if (CommandResponseValid())
				{
						
						ExtractDataString();
					
				}
				TIMER_SOFTWARE_clear_interrupt(my_timer_handler);
				reset_flags();
			}
		
	}
	
	return 0; 
}


void ExecuteCommand(uint8_t* command){
	 SendCommand(command);
	 GetCommandResponse(command); 
}

void SendCommand(uint8_t* command){
		DRV_UART_FlushRX(UART_3);
	  DRV_UART_FlushTX(UART_3);
	  DRV_UART_Write(UART_3, command, strlen(command));
}

void GetCommandResponse(){
	 uint8_t ch;
	 uint8_t ready = 0;
		int cnt=0;
	memset(send_sms_response, 0, strlen(send_sms_response));
	
	
	 TIMER_SOFTWARE_reset_timer(handler);
	 TIMER_SOFTWARE_start_timer(handler);
	//set_sms_flag(TRUE);
	 while ((!TIMER_SOFTWARE_interrupt_pending(handler) && ready ==0)){
		 
		 while (DRV_UART_BytesAvailable(UART_3) > 0){
				 DRV_UART_ReadByte(UART_3, &ch);
				send_sms_response[cnt++]=ch;
					//at_parser(ch, &data_structure);
					
				 //if (data_structure.success == 2){
						//ready = 1;
				//}
				 if(strlen(send_sms_response) == expected_string_length)
						ready=1;
		 } 
	 }
		send_sms_response[cnt]='\0';
	 //set_sms_flag(FALSE);
}

uint32_t CommandResponseValid(){
		
	return (data_structure.success == 2);
}

uint32_t ExtractData(){
	const char * pchar;
	uint32_t nr;
	pchar = strchr(data_structure.data[0], ':');
	
	 nr = atoi(pchar+1);
	
	return nr;
}


void ExtractDataString(){
	printf("%s", data_structure.data[0]);
}

uint32_t ConvertAsuToDbmw(uint32_t value){
	return (2 * value -113);
}

void reset_flags(){
	set_not_plus_flag(FALSE);
	set_ip_flag(FALSE);
}
