/* GPIO Example
 
   This example code is in the Public Domain (or CC0 licensed, at your option.)
 
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/adc.h"
 
 
#include "driver/uart.h"
 
#include "sdkconfig.h"
#include "esp_log.h"
#include "cJSON.h"
#include "esp_adc_cal.h"
 
#include "driver/twai.h"
 
#include "esp_timer.h"
 
 
// #define Ignition 14
// #define Reverse 12
// #define Break 15
// #define ModeL 18
// #define ModeR 13
// #define SideStand 16
 
// #define temp5 23 // adc
// #define temp6 25 // adc
// #define temp7 26 // adc
// #define temp8 27 // adc
 
// int ingi =0;
// int modeL = 0;
// int modeR = 0;
 
// int brake = 0;
// int reve = 0;
// int sidestand = 0 ;
 
// int temp1 = 0;
// int temp2 =0;
// int temp3 = 0;
// int temp4 = 0;
// int temp5 = 0;
// int temp6 = 0;
// int temp7 = 0;
// int temp8 = 0;
//char mode[] = "";
//char arr[3][10] = {"ingi","modeL","modeR", "brake", "reve"};
 
 
//#include <ds18b20.h> // temperature sensor
 
/* --------------------- Definitions and static variables ------------------ */
// Example Configuration
#define PING_PERIOD_MS 250
#define NO_OF_DATA_MSGS 50
#define NO_OF_ITERS 3
#define ITER_DELAY_MS 500
#define RX_TASK_PRIO 9
#define TX_TASK_PRIO 11
#define CTRL_TSK_PRIO 8
#define TX_GPIO_NUM CONFIG_EXAMPLE_TX_GPIO_NUM
#define RX_GPIO_NUM CONFIG_EXAMPLE_RX_GPIO_NUM
#define EXAMPLE_TAG "TWAI Master"
 
#define ID_MASTER_STOP_CMD 0x0A0
#define ID_MASTER_START_CMD 0x0A1
#define ID_MASTER_PING 0x0A2
#define ID_SLAVE_STOP_RESP 0x0B0
#define ID_SLAVE_DATA 0x0B1
#define ID_SLAVE_PING_RESP 0x0B2
 
#define ID_BATTERY_3_VI 0x1725
#define ID_BATTERY_2_VI 0x1bc5
#define ID_BATTERY_1_VI 0x1f05
 
#define ID_BATTERY_3_SOC 0x1727
#define ID_BATTERY_2_SOC 0x1bc7
#define ID_BATTERY_1_SOC 0x1f07
 
#define ID_LX_BATTERY_VI 0x6
#define ID_LX_BATTERY_T 0xa
#define ID_LX_BATTERY_SOC 0x8
#define ID_LX_BATTERY_PROT 0x9
 
 
 
#define ID_MOTOR_RPM 0x230
#define ID_MOTOR_TEMP 0x233
#define ID_MOTOR_CURR_VOLT 0x32
#define ADC_WIDTH_BIT_DEFAULT (ADC_WIDTH_BIT_9 - 1)
 
 
static float Voltage_1 = 0;
static float Current_1 = 0;
static float SOC_1 = 0;
static float SOH_1 = 0;
 
static int Voltage_2 = 0;
static int Current_2 = 0;
static float SOC_2 = 0;
static float SOH_2 = 0;
 
static float Voltage_3 = 0;
static float Current_3 = 0;
static int SOC_3 = 0;
static int SOH_3 = 0;
 
static uint16_t iDs[ 2 ] = {0x1f00,0x1bc0,0x1720};
 
// static uint16_t iDs[2] = {0x1f00, 0x1bc0};
 
static uint8_t state = 0;
static int M_CONT_TEMP = 0;
static int M_MOT_TEMP = 0;
static int M_THROTTLE = 0;
 
static int M_AC_CURRENT = 0;
static int M_AC_VOLTAGE = 0;
static int M_DC_CURRENT = 0;
static int M_DC_VOLATGE = 0;
 
static int S_DC_CURRENT = 0;
static int S_AC_CURRENT1 = 0;
static int S_AC_CURRENT2 = 0;
 
static float TRIP_1 = 0;
static float TRIP1 = 0;
 
static int t_stamp = 0;
int adc_value = 0;
int adc_value1 = 0;
int adc_value2 = 0;
 
uint32_t RPM;
 
char motor_err[32];
 
char batt_err[32];
char batt_temp[32];
 
 
uint32_t THROTTLE;
uint32_t CONT_TEMP;
uint32_t MOT_TEMP;
 
uint32_t DC_CURRENT;
uint32_t AC_CURRENT;
uint32_t AC_VOLTAGE;
uint32_t DC_VOLTAGE;
 
uint32_t voltage1_hx;
uint32_t current1_hx;
 
uint32_t voltage2_hx;
uint32_t current2_hx;
 
uint32_t voltage3_hx;
uint32_t current3_hx;
 
uint32_t SOC1_hx;
uint32_t SOH1_hx;
 
uint32_t SOC2_hx;
uint32_t SOH2_hx;
 
uint32_t SOC3_hx;
uint32_t SOH3_hx;
 
uint32_t Motor_err;
 
char bat1[32];
char bat2[32];
char bat3[32];
char vol_avg[32];
char sOC_avg[32];
char cur_tot[32];
char CVoltage_1[32];
char CVoltage_3[32];
char CVoltage_2[32];
char CCurrent_1[32];
char CCurrent_2[32];
char CCurrent_3[32];
char CSOC_1[32];
char CSOC_2[32];
char CSOC_3[32];
char CSOH_1[32];
char CSOH_2[32];
char CSOH_3[32];
char T_stamp[32];
char sensor[32];
char rpm[64];
char speed[64];
char throttle[64];
 
char motorTemp[64];
char contTmep[64];
char acCurrent[64];
char acVoltage[64];
char dcCurrent[64];
char dcVoltage[64];
char s_dcCurrent[64];
char s_acCurrent1[64];
char s_acCurrent2[64];
char trip1[64];
 
/////////////////////////////////////////////////////////////////////////////////
 
static const twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
static const twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
 
static const twai_general_config_t g_config = {.mode = TWAI_MODE_NORMAL,
.tx_io = GPIO_NUM_21,
.rx_io = GPIO_NUM_22,
.clkout_io = TWAI_IO_UNUSED,
.bus_off_io = TWAI_IO_UNUSED,
.tx_queue_len = 10,
.rx_queue_len = 20,
.alerts_enabled = TWAI_ALERT_ALL,
.clkout_divider = 0};
 
static QueueHandle_t tx_task_queue;
static QueueHandle_t rx_task_queue;
static SemaphoreHandle_t stop_ping_sem;
static SemaphoreHandle_t cnt_Switch_start;
static SemaphoreHandle_t done_sem;
static SemaphoreHandle_t ctrl_task_Transmit;
static SemaphoreHandle_t ctrl_task_receive;
static SemaphoreHandle_t ctrl_task_send;
 
/* --------------------------- Tasks and Functions -------------------------- */
// void decToBinary(int n)
// {
//    int binaryNum[32];
//    int i =0 ;
//    while(n>0){
//       binaryNum[i] = n % 2 ;
//       n = n/2 ;
//       i++ ;
 
//    }
 
//    for(int j= i -1 ; j>=0 ; j--)
//    {
//       printf("%d\n", binaryNum[j]);
//    }
// }
uint8_t arr[8];
uint8_t combinedValue = 0;
//int val_of_temp = 0;
int new_val ;
 
// esp_adc_cal_characteristics_t *adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
// esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);
//     //Check type of calibration value used to characterize ADC
// if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
//     printf("eFuse Vref");
// } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
//     printf("Two Point");
// } else {
//     printf("Default");
// }
static void switch_ip(void *arg)
{
 
//xSemaphoreTake(cnt_Switch_start, portMAX_DELAY);
// Wait for completion
vTaskDelay(pdMS_TO_TICKS(50));
// int arr[8];
// int i,x,pos,n=8 ;
// uint8_t arr[8];
// uint8_t combinedValue = 0;
 
while(1)
{
 
 
vTaskDelay(pdMS_TO_TICKS(50));
 
 
ingi = !gpio_get_level(Ignition);
brake = !gpio_get_level(Break);
modeL = !gpio_get_level(ModeL);
modeR = !gpio_get_level(ModeR);
reve = !gpio_get_level(Reverse);
sidestand = !gpio_get_level(SideStand);
 
//printf("Iginiton = %d, Break = %d, Mode+ = %d, Moe- = %d, Reverse =%d \n",ingi,brake,modeR,modeL,reve);
 
// arr[0]= 0;
// arr[1]= modeR ;
// arr[2] = modeL ;
// arr[3] = brake ;
// arr[4] = ingi ;
// arr[5] = reve ;
// arr[6] = 0;
// arr[7] = 0 ;
// //printf(arr);
// int byte3 = 0;
 
// for (int i = 0; i < 7; i++) {
//    byte3 = byte3 * 10 + arr[i];
//     }
 
 
//     arr[0] = 0;
//     arr[1] = modeR;  // Replace with your actual modeR value
//     arr[2] = modeL;  // Replace with your actual modeL value
//     arr[3] = brake;  // Replace with your actual brake value
//     arr[4] = ingi;  // Replace with your actual ingi value
//     arr[5] = reve;  // Replace with your actual reve value
//     arr[6] = 0;
//     arr[7] = 0;
 
//     // Combining values into a single 8-bit variable
//     for (int i = 0; i < 8; i++) {
//         combinedValue |= arr[i] << i;
//     }
// printf("%u\n", combinedValue);
//decToBinary(combinedValue);
struct ControlBits {
    unsigned int b0 : 1;
    unsigned int modeR : 1;
    unsigned int modeL : 1;
    unsigned int brake : 1;
    unsigned int ingi : 1;
    unsigned int reve : 1;
    unsigned int sidestand : 1;
    unsigned int b8 : 1;
};
 
// Define a union to share the same memory space for the bit field and an 8-bit integer
union ControlUnion {
    struct ControlBits bits;
    uint8_t combinedValue;
};
 
 
// not needed
union ControlUnion control;
    // Assign values to the bit field members
    control.bits.b0 = 0;
    control.bits.modeR = modeR;  // Replace with your actual modeR value
    control.bits.modeL = modeL;  // Replace with your actual modeL value
    control.bits.brake = brake;  // Replace with your actual brake value
    control.bits.ingi = ingi;   // Replace with your actual ingi value
    control.bits.reve = reve;   // Replace with your actual reve value
    control.bits.sidestand = sidestand;
    control.bits.b8 = 0;
 
    // Print the combined value
    // printf("Combined value: %d\n", control.combinedValue);
   
union
{
int b;
uint32_t f;
} u; // crazy
u.b = control.combinedValue;
 
state = u.f;  // converted to hexa
 
 
 
    // Print the binary representation
   //  printf("Binary representation: ");
   //  for (int i = 7; i >= 0; i--) {
   //    int myVal = (control.combinedValue >> i) & 1 ;
   //      printf("%d", myVal);
   //  }
    printf("\n");
}
vTaskDelete(NULL);
 
}
 
 
// int Dout ;
// int Vmax = 100 ;
// int Dmax = 4095 ;
// static float batt_tmp = 0 ;
// static void battery_temp(void *arg)     //Conversion of analog to digital signal
// {
//     adc1_config_width(ADC_WIDTH_BIT_12);
//     adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_0); //gpio 32
//     while(1)
//     {
//         int val_of_temp = adc1_get_raw(ADC1_CHANNEL_4);
//         //uint32_t voltage = esp_adc_cal_raw_to_voltage(val_of_temp, adc_chars);
//         //'float Vout = (float)3000 * (float)(Vmax / Dmax);
//         //Vout = (float)((int)(Vout * 100.0)) / 100.0;
//         batt_tmp = (float)val_of_temp * ((float)Vmax / (float)Dmax);
//         //printf("the value shown %d \n", val_of_temp);//val_of_temp);
//         printf("Battery Temperature : %.4f \n", batt_tmp);
//         //ESP_LOGD(EXAMPLE_TAG, "Failed to queue message for transmission\n");
//         vTaskDelay(250/portTICK_PERIOD_MS);
//             //new_val = val_of_temp ;
//     }
//     vTaskDelay(NULL);
// }
 
 
// // int Dout ;
// // int Vmax = 80 ;
// // int Dmax = 4095 ;
// static float cnt_tmp = 0;
// static void controller_temp(void *arg)     //Conversion of analog to digital signal
// {
//     adc1_config_width(ADC_WIDTH_BIT_12);
//     adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_0);
//     while(1)
//     {
//         int val_temp_controller = adc1_get_raw(ADC1_CHANNEL_6);//gpio34
//         printf("cnt_tmp_analog---------> %d \n",val_temp_controller);
//         //uint32_t voltage = esp_adc_cal_raw_to_voltage(val_of_temp, adc_chars);
//         //'float Vout = (float)3000 * (float)(Vmax / Dmax);
//         //Vout = (float)((int)(Vout * 100.0)) / 100.0;
//         cnt_tmp = (float)val_temp_controller * ((float)Vmax / (float)Dmax);
//         //printf("the value shown %d \n", val_temp_controller);//val_of_temp);
//         printf("Controller(MCU) temperature : %.4f \n", cnt_tmp);
//         //ESP_LOGD(EXAMPLE_TAG, "Failed to queue message for transmission\n");
//         vTaskDelay(100/portTICK_PERIOD_MS);
//             //new_val = val_of_temp ;
//     }
//     vTaskDelay(NULL);
// }
 
// // int Dout ;
// // int Vmax = 80 ;
// // int Dmax = 4095 ;
// static float pcb = 0 ;
// static void pcb_temp(void *arg)     //Conversion of analog to digital signal
// {
//     adc1_config_width(ADC_WIDTH_BIT_12);
//     adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_0);
//     while(1)
//     {
//         int val_temp_pcb = adc1_get_raw(ADC1_CHANNEL_7); //GPIO -35
       
//         pcb = (float)val_temp_pcb * ((float)Vmax / (float)Dmax);
//         //printf("the value shown %d \n", val_temp_pcb);//val_of_temp);
//         printf("PCB temperature : %.4f \n", pcb);
//         //ESP_LOGD(EXAMPLE_TAG, "Failed to queue message for transmission\n");
//         vTaskDelay(100/portTICK_PERIOD_MS);
//             //new_val = val_of_temp ;
//     }
//     vTaskDelay(NULL);
// }
 
// static float soc = 0 ;
// static void soc_battery(void *arg)     //Conversion of analog to digital signal
// {
//     adc1_config_width(ADC_WIDTH_BIT_12);
//     adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_0);
//     while(1)
//     {
//         int soc_of_batt = adc1_get_raw(ADC1_CHANNEL_3); // GPIO - 39
//         //uint32_t voltage = esp_adc_cal_raw_to_voltage(val_of_temp, adc_chars);
//         //'float Vout = (float)3000 * (float)(Vmax / Dmax);
//         //Vout = (float)((int)(Vout * 100.0)) / 100.0;
//         soc = (float)soc_of_batt * ((float)Vmax / (float)Dmax);
//         //printf("the value shown %d \n", soc_of_batt);//val_of_temp);
//         printf("SoC of the battery is : %.4f  \n", soc);
//         //ESP_LOGD(EXAMPLE_TAG, "Failed to queue message for transmission\n");
//         vTaskDelay(100/portTICK_PERIOD_MS);
//             //new_val = val_of_temp ;
//     }
//     vTaskDelay(NULL);
// }
 
// int Dout ;
// int V_motor_max = 150 ;
// int D_motor_max = 4095 ;
// static float V_motor_out = 0;
// static void motor_temp(void *arg)     //Conversion of analog to digital signal
// {
//     adc1_config_width(ADC_WIDTH_BIT_12);
//     adc1_config_channel_atten(ADC1_CHANNEL_5, ADC_ATTEN_DB_0);
//     while(1)
//     {
//         int val_temp_motor = adc1_get_raw(ADC1_CHANNEL_5); // gpio33
//         //uint32_t voltage = esp_adc_cal_raw_to_voltage(val_of_temp, adc_chars);
//         //'float Vout = (float)3000 * (float)(Vmax / Dmax);
//         //Vout = (float)((int)(Vout * 100.0)) / 100.0;
//         V_motor_out = (float)val_temp_motor * ((float)V_motor_max / (float)D_motor_max);
//         //printf("the value shown %d \n", val_temp_motor);//val_of_temp);
//         printf("Motor Temperature : %.4f \n", V_motor_out);
//         //ESP_LOGD(EXAMPLE_TAG, "Failed to queue message for transmission\n");
//         vTaskDelay(100/portTICK_PERIOD_MS);
//             //new_val = val_of_temp ;
//     }
//     vTaskDelay(NULL);
// }
 
// // int Dout ;
// // int Vmax = 80 ;
// // int Dmax = 4095 ;
// static float thr_per = 0 ;
// static void throttle_percentage(void *arg)     //Conversion of analog to digital signal
// {
//     adc1_config_width(ADC_WIDTH_BIT_12);
//     adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_0);// gpio 36
//     while(1)
//     {
//         int throttle_per = adc1_get_raw(ADC1_CHANNEL_0);//,ADC_WIDTH_BIT_12, NULL);
//         //uint32_t voltage = esp_adc_cal_raw_to_voltage(val_of_temp, adc_chars);
//         //'float Vout = (float)3000 * (float)(Vmax / Dmax);
//         //Vout = (float)((int)(Vout * 100.0)) / 100.0;
//         thr_per = (float)throttle_per * ((float)Vmax / (float)Dmax);
//         //printf("the value shown %d \n", throttle_per);//val_of_temp);
//         printf("Throttle percentage : %.4f \n", thr_per);
//         //ESP_LOGD(EXAMPLE_TAG, "Failed to queue message for transmission\n");
//         vTaskDelay(100/portTICK_PERIOD_MS);
//             //new_val = val_of_temp ;
//     }
//     vTaskDelay(NULL);
// }
 
 
 
static void twai_transmit_task(void *arg)
{
//tx_task_action_t action;
//xQueueReceive(tx_task_queue, &action, portMAX_DELAY);
 
//xSemaphoreTake(ctrl_task_Transmit, portMAX_DELAY); // Wait for completion
 
// xSemaphoreGive(ctrl_task_receive); // Start control task
int aa = 0;
 
ESP_LOGI(EXAMPLE_TAG, "Transmitting to battery");
 
vTaskDelay(pdMS_TO_TICKS(100));
int arr[8];
int i,x,pos,n=8 ;
 
while (1)
{
 
 
 
//printf("this is my %d", new_val);
twai_message_t transmit_message_switch = {.identifier = (0x18530902), .data_length_code = 8, .extd = 1, .data = {thr_per, 0x03, 0x00, state, 0x00, 0x00, 0x00, 0x00}};
if (twai_transmit(&transmit_message_switch, 1000) == ESP_OK)
{
ESP_LOGI(EXAMPLE_TAG, "Message queued for transmission\n");
vTaskDelay(pdMS_TO_TICKS(100));
}
else
{
 
ESP_LOGE(EXAMPLE_TAG, "Failed to queue message for transmission\n");
}
vTaskDelay(pdMS_TO_TICKS(100));
 
// twai_message_t transmit_message_switch1 = {.identifier = (0x18530902), .data_length_code = 8, .extd = 1, .data = {0x00, 0x03, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00}};
// if (twai_transmit(&transmit_message_switch1, 1000) == ESP_OK)
// {
// ESP_LOGI(EXAMPLE_TAG, "Message queued for transmission\n");
// vTaskDelay(pdMS_TO_TICKS(250));
// }
// else
// {
 
// ESP_LOGE(EXAMPLE_TAG, "Failed to queue message for transmission\n");
// }
// vTaskDelay(pdMS_TO_TICKS(250));
 
// twai_message_t transmit_message_switch2 = {.identifier = (0x18530902), .data_length_code = 8, .extd = 1, .data = {0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
// if (twai_transmit(&transmit_message_switch2, 1000) == ESP_OK)
// {
// ESP_LOGI(EXAMPLE_TAG, "Message queued for transmission\n");
// vTaskDelay(pdMS_TO_TICKS(250));
// }
// else
// {
 
// ESP_LOGE(EXAMPLE_TAG, "Failed to queue message for transmission\n");
// }
// vTaskDelay(pdMS_TO_TICKS(250));
 
 
twai_message_t transmit_message_batteryTemp = {.identifier = (0x000000A), .data_length_code = 8, .extd = 1, .data = {batt_tmp, batt_tmp, batt_tmp, batt_tmp, batt_tmp, batt_tmp, batt_tmp, batt_tmp}};
if (twai_transmit(&transmit_message_batteryTemp, 10000) == ESP_OK)
{
ESP_LOGI(EXAMPLE_TAG, "Message queued for transmission\n");
vTaskDelay(pdMS_TO_TICKS(100));
}
else
{
 
ESP_LOGE(EXAMPLE_TAG, "Failed to queue message for transmission\n");
}
vTaskDelay(pdMS_TO_TICKS(100));
 
 
twai_message_t transmit_message_otherTemp = {.identifier = (0x18530903), .data_length_code = 8, .extd = 1, .data = {cnt_tmp, V_motor_out, V_motor_out, 0x00 , 0x00 , 0x00 , 0x00 , 0x00 }};
if (twai_transmit(&transmit_message_otherTemp, 10000) == ESP_OK)
{
    ESP_LOGI(EXAMPLE_TAG, "Message queued for transmission\n");
vTaskDelay(pdMS_TO_TICKS(100));
}
else
{
 
ESP_LOGE(EXAMPLE_TAG, "Failed to queue message for transmission\n");
}
vTaskDelay(pdMS_TO_TICKS(100));
 
 
 
 
twai_message_t transmit_message_SoC= {.identifier = ID_LX_BATTERY_SOC , .data_length_code = 8, .extd = 1, .data = {soc, 0x00, 0x00, 0x9A , 0xB0 , 0x63 , 0x1D , 0x01 }};
if (twai_transmit(&transmit_message_SoC, 10000) == ESP_OK)
{
ESP_LOGI(EXAMPLE_TAG, "Message queued for transmission\n");
vTaskDelay(pdMS_TO_TICKS(100));
}
else
{
 
ESP_LOGE(EXAMPLE_TAG, "Failed to queue message for transmission\n");
}
vTaskDelay(pdMS_TO_TICKS(100));
 
 
}
 
 
 
vTaskDelete(NULL);
}
 
 
 
 
void app_main(void)
{
gpio_set_direction(Ignition,GPIO_MODE_INPUT);
gpio_set_pull_mode(Ignition,GPIO_PULLUP_ONLY);
 
gpio_set_direction(Reverse,GPIO_MODE_INPUT);
gpio_set_pull_mode(Reverse,GPIO_PULLUP_ONLY);
 
gpio_set_direction(ModeL,GPIO_MODE_INPUT);
gpio_set_pull_mode(ModeL,GPIO_PULLUP_ONLY);
 
gpio_set_direction(ModeR,GPIO_MODE_INPUT);
gpio_set_pull_mode(ModeR,GPIO_PULLUP_ONLY);
 
gpio_set_direction(Break,GPIO_MODE_INPUT);
gpio_set_pull_mode(Break,GPIO_PULLUP_ONLY);
 
gpio_set_direction(SideStand,GPIO_MODE_INPUT);
gpio_set_pull_mode(SideStand,GPIO_PULLUP_ONLY);
 
//xTaskCreatePinnedToCore(switch_ip, "TWAI_tx", 4096, NULL, TX_TASK_PRIO, NULL, tskNO_AFFINITY);
 
 
ESP_ERROR_CHECK(twai_driver_install(&g_config, &t_config, &f_config)); /// driver install
ESP_LOGI(EXAMPLE_TAG, "Driver installed");
 
ESP_ERROR_CHECK(twai_start()); /// driver start
 
ESP_LOGI(EXAMPLE_TAG, "Driver started");
 
vTaskDelay(pdMS_TO_TICKS(500)) ;
 
xTaskCreate(throttle_percentage , "throttle", 4096, NULL, 8, NULL);
xTaskCreate(pcb_temp , "pcb temp", 4096, NULL, 8, NULL);
xTaskCreate(motor_temp , "motor temp", 4096, NULL, 8, NULL);
xTaskCreate(battery_temp , "battery temperature", 4096, NULL, 8, NULL);
xTaskCreate(controller_temp , "motor controller temperature", 4096, NULL, 8, NULL);
xTaskCreate(soc_battery, "battery SoC", 4096, NULL, 8, NULL);
xTaskCreate(switch_ip, "Swicth_Tsk", 4096, NULL, 8, NULL);
//xTaskCreate(twai_transmit_task, "transmit_Tsk", 4096, NULL, 8, NULL);
xTaskCreate(twai_transmit_task, "Transmit_Tsk", 4096, NULL, 8, NULL);
//xSemaphoreGive(cnt_Switch_start); // Start control task
 
//gpio_set_intr_type(Ignition,GPIO_INTR_LOW_LEVEL);
//gpio_intr_enable(Ignition);
 
//xSemaphoreTake(done_sem, portMAX_DELAY); // Wait for completion
 
//ESP_ERROR_CHECK(twai_driver_uninstall());
//ESP_LOGI(EXAMPLE_TAG, "Driver uninstalled");
 
//vSemaphoreDelete(cnt_Switch_start);
//vSemaphoreDelete(done_sem);
 
 
}