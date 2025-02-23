/*
 Copyright (c) 2011 Arduino.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

static int _readResolution = 10;
static int _writeResolution = 8;

void analogReadResolution(int res) {
	_readResolution = res;
}

void analogWriteResolution(int res) {
	_writeResolution = res;
}

static inline uint32_t mapResolution(uint32_t value, uint32_t from, uint32_t to) {
	if (from == to)
		return value;
	if (from > to)
		return (value >> (from-to));
	else
		return (value << (to-from));
}

eAnalogReference analog_reference = AR_DEFAULT;

void analogReference(eAnalogReference ulMode)
{
	analog_reference = ulMode;
}

uint32_t analogRead(uint32_t ulPin)
{
#if defined(__M451__) || defined(__M252__) || defined(__M480__)|| defined(__M460__)
  volatile uint32_t ulValue = 0;  

  if(ulPin>ADC_MAX_COUNT || ADC_Desc[ulPin].A==NULL) return 0;  	  
  	  	
	/* Disable the digital input path to avoid the leakage current. */
  GPIO_DISABLE_DIGITAL_PATH(GPIO_Desc[ADC_Desc[ulPin].pintype.num].P,GPIO_Desc[ADC_Desc[ulPin].pintype.num].bit);
   
  ADC_Config(ADC_Desc[ulPin]);
  
  /* Set the ADC internal sampling time, input mode as single-end and enable the A/D converter */
#if defined(__M252__)|| defined(__M480__) ||defined(__M460__)
	EADC_Open(ADC_Desc[ulPin].A, 0);
	EADC_SetExtendSampleTime(EADC, ADC_Desc[ulPin].ch, 6);
#else
	EADC_Open(ADC_Desc[ulPin].A, EADC_CTL_DIFFEN_SINGLE_END);
	EADC_SetInternalSampleTime(EADC, 6);
#endif
	
	/* Configure the sample 0 module for analog input channel 0 and enable ADINT0 trigger source */
	EADC_ConfigSampleModule(EADC,ADC_Desc[ulPin].ch, EADC_ADINT0_TRIGGER, ADC_Desc[ulPin].ch);
	
	/* Clear the A/D ADINT0 interrupt flag for safe */
	EADC_CLR_INT_FLAG(EADC, 0x1);
	
	/* Enable sample module interrupt */
	EADC_ENABLE_SAMPLE_MODULE_INT(EADC, 0, (0x1 << ADC_Desc[ulPin].ch));
	
	/* Trigger sample module to start A/D conversion */
	EADC_START_CONV(EADC, (0x1 << ADC_Desc[ulPin].ch));
	
	while(!(EADC_GET_INT_FLAG(EADC,0x1)));
  
	ulValue=EADC_GET_CONV_DATA(EADC, ADC_Desc[ulPin].ch);
  
 	/* Wait conversion done */
 	while(EADC_GET_DATA_VALID_FLAG(EADC, (0x1 << ADC_Desc[ulPin].ch)) != ( 0x1U << ADC_Desc[ulPin].ch));
	
	/* Disable the sample module interrupt */
	EADC_DISABLE_SAMPLE_MODULE_INT(EADC, 0, (1<<ADC_Desc[ulPin].ch));
	
	ulValue=EADC_GET_CONV_DATA(EADC, ADC_Desc[ulPin].ch);
	ulValue = mapResolution(ulValue, 12, _readResolution);

  // Close ADC
  EADC_Close(ADC_Desc[ulPin].A);

#elif defined(__NUC240__)
  uint32_t ulValue = 0;  

  if(ulPin>ADC_MAX_COUNT || ADC_Desc[ulPin].A==NULL) return;  	  
  	
  ADC_Config(ADC_Desc[ulPin]);
  
  //GPIO_ENABLE_DIGITAL_PATH(GPIO_Desc[ADC_Desc[ulPin].pintype.num].P,GPIO_Desc[ADC_Desc[ulPin].pintype.num].bit);
  
  // Enable channel 0
	ADC_Open(ADC_Desc[ulPin].A, 0, 0, (1<<ADC_Desc[ulPin].ch));
		
	// Power on ADC
	ADC_POWER_ON(ADC_Desc[ulPin].A);

  // Wait for busy of conversion
  while(ADC_IS_BUSY(ADC_Desc[ulPin].A));	
  	
  // Start for conversion	
  ADC_START_CONV(ADC_Desc[ulPin].A);
  		
	// Wait for end of conversion
	while(!ADC_GET_INT_FLAG(ADC_Desc[ulPin].A,ADC_ADF_INT));
	
	// Clear ADC flag
	ADC_CLR_INT_FLAG(ADC_Desc[ulPin].A,ADC_ADF_INT);
	
	// Read the value
	ulValue = ADC_GET_CONVERSION_DATA(ADC_Desc[ulPin].A,ADC_Desc[ulPin].ch);	
	ulValue = mapResolution(ulValue, 12, _readResolution);
  
  //GPIO_DISABLE_DIGITAL_PATH(GPIO_Desc[ADC_Desc[ulPin-1].pintype.num].P,GPIO_Desc[ADC_Desc[ulPin].pintype.num-1].bit);
  
  // Close ADC
  ADC_Close(ADC_Desc[ulPin].A);

#elif defined(__NANO100__) || defined(__NANO1X2__)
  uint32_t ulValue = 0;  

  if(ulPin>ADC_MAX_COUNT || ADC_Desc[ulPin].A==NULL) return;  	  
  	
  ADC_Config(ADC_Desc[ulPin]);
  
  //GPIO_ENABLE_DIGITAL_PATH(GPIO_Desc[ADC_Desc[ulPin].pintype.num].P,GPIO_Desc[ADC_Desc[ulPin].pintype.num].bit);
  
  // Enable channel 
	ADC_Open(ADC_Desc[ulPin].A, ADC_INPUT_MODE_SINGLE_END, ADC_OPERATION_MODE_SINGLE, (1<<ADC_Desc[ulPin].ch));
	
	// Power on ADC
	ADC_POWER_ON(ADC_Desc[ulPin].A);

  // Wait for busy of conversion
  while(ADC_IS_BUSY(ADC_Desc[ulPin].A));	
  	
  // Start for conversion	
  ADC_START_CONV(ADC_Desc[ulPin].A);
  		
	// Wait for end of conversion
	while(!ADC_GET_INT_FLAG(ADC_Desc[ulPin].A,ADC_ADF_INT));
	
	// Clear ADC flag
	ADC_CLR_INT_FLAG(ADC_Desc[ulPin].A,ADC_ADF_INT);
	
	// Read the value
	ulValue = ADC_GET_CONVERSION_DATA(ADC_Desc[ulPin].A,ADC_Desc[ulPin].ch);	
	ulValue = mapResolution(ulValue, 12, _readResolution);
  
  //GPIO_DISABLE_DIGITAL_PATH(GPIO_Desc[ADC_Desc[ulPin-1].pintype.num].P,GPIO_Desc[ADC_Desc[ulPin].pintype.num-1].bit);
  
  // Close ADC
  ADC_Close(ADC_Desc[ulPin].A);

#elif defined(__NUC131__)
  uint32_t ulValue = 0;  

  if(ulPin>ADC_MAX_COUNT || ADC_Desc[ulPin].A==NULL) return;  	  
  	
  ADC_Config(ADC_Desc[ulPin]);
  
  //GPIO_ENABLE_DIGITAL_PATH(GPIO_Desc[ADC_Desc[ulPin].pintype.num].P,GPIO_Desc[ADC_Desc[ulPin].pintype.num].bit);
  
  // Enable channel 0
	ADC_Open(ADC_Desc[ulPin].A, 0, 0, (1<<ADC_Desc[ulPin].ch));
		
	// Power on ADC
	ADC_POWER_ON(ADC_Desc[ulPin].A);

  // Wait for busy of conversion
  while(ADC_IS_BUSY(ADC_Desc[ulPin].A));	
  	
  // Start for conversion	
  ADC_START_CONV(ADC_Desc[ulPin].A);
  		
	// Wait for end of conversion
	while(!ADC_GET_INT_FLAG(ADC_Desc[ulPin].A,ADC_ADF_INT));
	
	// Clear ADC flag
	ADC_CLR_INT_FLAG(ADC_Desc[ulPin].A,ADC_ADF_INT);
	
	// Read the value
	ulValue = ADC_GET_CONVERSION_DATA(ADC_Desc[ulPin].A,ADC_Desc[ulPin].ch);	
	ulValue = mapResolution(ulValue, 12, _readResolution);
  
  //GPIO_DISABLE_DIGITAL_PATH(GPIO_Desc[ADC_Desc[ulPin-1].pintype.num].P,GPIO_Desc[ADC_Desc[ulPin].pintype.num-1].bit);
  
  // Close ADC
  ADC_Close(ADC_Desc[ulPin].A);

#elif defined(__M032BT__)|| defined(__M032KG__)
	uint32_t ulValue = 0;
	
	if ((ulPin >= ADC_MAX_COUNT) || (ADC_Desc[ulPin].A == NULL))
	{
		 return 0;
	}
	
	ADC_Config(ADC_Desc[ulPin]);

	//GPIO_ENABLE_DIGITAL_PATH(GPIO_Desc[ADC_Desc[ulPin].pintype.num].P,GPIO_Desc[ADC_Desc[ulPin].pintype.num].bit);
	// Power on ADC
	ADC_POWER_ON(ADC_Desc[ulPin].A);

	/* Set input mode as single-end, Single mode, and select channel 2 */
	ADC_Open(ADC_Desc[ulPin].A, ADC_ADCR_DIFFEN_SINGLE_END, ADC_ADCR_ADMD_SINGLE, (1 << ADC_Desc[ulPin].ch));
	
	/* Clear the A/D interrupt flag for safe */
	ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT);
	// Wait for busy of conversion
	while (ADC_IS_BUSY(ADC_Desc[ulPin].A));
	// Start for conversion	
	ADC_START_CONV(ADC_Desc[ulPin].A);

	// Wait for end of conversion
	while (!ADC_GET_INT_FLAG(ADC_Desc[ulPin].A, ADC_ADF_INT));

	// Clear ADC flag
	ADC_CLR_INT_FLAG(ADC_Desc[ulPin].A, ADC_ADF_INT);

	// Read the value
	ulValue = ADC_GET_CONVERSION_DATA(ADC_Desc[ulPin].A, ADC_Desc[ulPin].ch);
	ulValue = mapResolution(ulValue, 12, _readResolution);

	//GPIO_DISABLE_DIGITAL_PATH(GPIO_Desc[ADC_Desc[ulPin-1].pintype.num].P,GPIO_Desc[ADC_Desc[ulPin].pintype.num-1].bit);

	// Close ADC
	ADC_Close(ADC_Desc[ulPin].A);
#endif
  return ulValue;	
}

// Right now, PWM output only works on the pins with
// hardware support.  These are defined in the appropriate
// pins_*.c file.  For the rest of the pins, we default
// to digital output.
static uint8_t pinEnabled[PWM_MAX_COUNT]={0};
static uint32_t fixValue[PWM_MAX_COUNT]={0};
void analogWrite(uint32_t ulPin, uint32_t ulValue) {
	
#ifdef USE_BoardToPin
	if(ulPin > BoardToPin_MAX_COUNT) return;
	if(BoardToPinInfo[ulPin].type!=PWM_TYPE) return;
	ulPin=BoardToPinInfo[ulPin].num;
#else
	if(ulPin>PWM_MAX_COUNT || PWM_Desc[ulPin].P==NULL) return;
#endif
	
	ulValue = mapResolution(ulValue, _writeResolution, 8);
	
	ulValue=((ulValue+1)*100)/(1<<_writeResolution);
#if defined(__M451__)
	if(ulValue==100)
	{  
		int32_t pin=PWM_Desc[ulPin].pintype.num;
		GPIO_Config(GPIO_Desc[pin]);
		GPIO_SetMode(GPIO_Desc[pin].P, GPIO_Desc[pin].bit, GPIO_MODE_OUTPUT);
		(GPIO_Desc[pin].P)->DOUT |= GPIO_Desc[pin].bit;
		pinEnabled[ulPin]= 0;
		fixValue[ulPin]=ulValue;
		return;
	}
#elif defined(__M252__)
	if(ulValue==0)
	{  
		int32_t pin=PWM_Desc[ulPin].pintype.num;
		GPIO_Config(GPIO_Desc[pin]);
		GPIO_SetMode(GPIO_Desc[pin].P, GPIO_Desc[pin].bit, GPIO_MODE_OUTPUT);
		(GPIO_Desc[pin].P)->DOUT &= ~GPIO_Desc[pin].bit;
		pinEnabled[ulPin]= 0;
		fixValue[ulPin]=ulValue;
		return;
	}	
#elif defined(__M480__)|| defined(__M460__)
	if(ulValue==0)
	{  
		int32_t pin=PWM_Desc[ulPin].pintype.num;
		GPIO_Config(GPIO_Desc[pin]);
		GPIO_SetMode(GPIO_Desc[pin].P, GPIO_Desc[pin].bit, GPIO_MODE_OUTPUT);
		(GPIO_Desc[pin].P)->DOUT &= ~GPIO_Desc[pin].bit;
		pinEnabled[ulPin]= 0;
		fixValue[ulPin]=ulValue;
		return;
	}
#elif defined(__NUC240__)
	if(ulValue==0)
	{  
		int32_t pin=PWM_Desc[ulPin].pintype.num;
		GPIO_Config(GPIO_Desc[pin]);
		GPIO_SetMode(GPIO_Desc[pin].P, GPIO_Desc[pin].bit, GPIO_PMD_OUTPUT);
		(GPIO_Desc[pin].P)->DOUT &= ~GPIO_Desc[pin].bit;
		pinEnabled[ulPin]= 0;
		fixValue[ulPin]=ulValue;
		return;
	}
#elif defined(__NANO100__) || defined(__NANO1X2__)
	if(ulValue==0)
	{  
		int32_t pin=PWM_Desc[ulPin].pintype.num;
		GPIO_Config(GPIO_Desc[pin]);
		GPIO_SetMode(GPIO_Desc[pin].P, GPIO_Desc[pin].bit, GPIO_PMD_OUTPUT);
		(GPIO_Desc[pin].P)->DOUT &= ~GPIO_Desc[pin].bit;
		pinEnabled[ulPin]= 0;
		fixValue[ulPin]=ulValue;
		return;
	}
#elif defined(__NUC131__)
	if(ulValue==0)
	{  
		int32_t pin=PWM_Desc[ulPin].pintype.num;
		GPIO_Config(GPIO_Desc[pin]);
		GPIO_SetMode(GPIO_Desc[pin].P, GPIO_Desc[pin].bit, GPIO_PMD_OUTPUT);
		(GPIO_Desc[pin].P)->DOUT &= ~GPIO_Desc[pin].bit;
		pinEnabled[ulPin]= 0;
		fixValue[ulPin]=ulValue;
		return;
	}
#elif defined(__M032BT__)
	if(ulValue==0)
	{  
		int32_t pin=PWM_Desc[ulPin].pintype.num;
		GPIO_Config(GPIO_Desc[pin]);
		GPIO_SetMode(GPIO_Desc[pin].P, GPIO_Desc[pin].bit, GPIO_MODE_OUTPUT);
		(GPIO_Desc[pin].P)->DOUT &= ~GPIO_Desc[pin].bit;
		pinEnabled[ulPin]= 0;
		fixValue[ulPin]=ulValue;
		return;
	}
#endif	

#if defined(__M032BT__)

	/**** Combine Structure BPWM and PWM ****/
	typedef uint32_t (*pwmConfigChannel)(void *pwm, uint32_t , uint32_t , uint32_t);
	typedef uint32_t (*pwmEnableOutput)(void *pwm, uint32_t u32ChannelMask);
	typedef uint32_t (*pwmStart)(void *pwm, uint32_t u32ChannelMask);
	pwmConfigChannel pfn_pwmConfigChannel;
	pwmEnableOutput	pfn_pwmEnableOutput;
	pwmStart	pfn_pwmStart;

	if((PWM_Desc[ulPin].V==BPWM0)||(PWM_Desc[ulPin].V==BPWM1)){
		pfn_pwmConfigChannel	= (pwmConfigChannel)BPWM_ConfigOutputChannel;
		pfn_pwmEnableOutput		= (pwmEnableOutput)BPWM_EnableOutput;
		pfn_pwmStart			= (pwmStart)BPWM_Start;
	}else{ 
		pfn_pwmConfigChannel	= (pwmConfigChannel)PWM_ConfigOutputChannel;
		pfn_pwmEnableOutput		= (pwmEnableOutput)PWM_EnableOutput;
		pfn_pwmStart			= (pwmStart)PWM_Start;
	}
	/************************************/

	if (!pinEnabled[ulPin]){
		//Set Mutifunction pins
		PWM_Config(PWM_Desc[ulPin]);		

		//Config PWMs
		pfn_pwmConfigChannel(PWM_Desc[ulPin].P,PWM_Desc[ulPin].ch,PWM_Desc[ulPin].freq,ulValue);
		
		//Enable PWM output
		pfn_pwmEnableOutput(PWM_Desc[ulPin].P,(1<<PWM_Desc[ulPin].ch));
		
		//Start PWM
		pfn_pwmStart(PWM_Desc[ulPin].P,(1<<PWM_Desc[ulPin].ch));
		
		pinEnabled[ulPin] = 1;
	}
	
	//Config PWMs		
	if(fixValue[ulPin]!=ulValue)
	{
		pfn_pwmConfigChannel(PWM_Desc[ulPin].P,PWM_Desc[ulPin].ch,PWM_Desc[ulPin].freq,ulValue);
		fixValue[ulPin]=ulValue;
	}

#else
	if (!pinEnabled[ulPin]){
		
		//Set Mutifunction pins
		PWM_Config(PWM_Desc[ulPin]);		

#if defined(__M480__)|| defined(__M460__)	
       //Config PWMs
		EPWM_ConfigOutputChannel(PWM_Desc[ulPin].P,PWM_Desc[ulPin].ch,PWM_Desc[ulPin].freq,ulValue);
		
		//Enable PWM output
		EPWM_EnableOutput(PWM_Desc[ulPin].P,(1<<PWM_Desc[ulPin].ch));
		
		//Start PWM
		EPWM_Start(PWM_Desc[ulPin].P,(1<<PWM_Desc[ulPin].ch));  
#else
		//Config PWMs
		PWM_ConfigOutputChannel(PWM_Desc[ulPin].P,PWM_Desc[ulPin].ch,PWM_Desc[ulPin].freq,ulValue);
		
		//Enable PWM output
		PWM_EnableOutput(PWM_Desc[ulPin].P,(1<<PWM_Desc[ulPin].ch));
		
		//Start PWM
		PWM_Start(PWM_Desc[ulPin].P,(1<<PWM_Desc[ulPin].ch));
#endif		
		pinEnabled[ulPin] = 1;
	}
	
	//Config PWMs		
	if(fixValue[ulPin]!=ulValue)
	{
#if defined(__M480__)|| defined(__M460__)		
		EPWM_ConfigOutputChannel(PWM_Desc[ulPin].P,PWM_Desc[ulPin].ch,PWM_Desc[ulPin].freq,ulValue);
#else
        PWM_ConfigOutputChannel(PWM_Desc[ulPin].P,PWM_Desc[ulPin].ch,PWM_Desc[ulPin].freq,ulValue);
#endif
		fixValue[ulPin]=ulValue;
	}
#endif
	
	return;
}

#ifdef __cplusplus
}
#endif
