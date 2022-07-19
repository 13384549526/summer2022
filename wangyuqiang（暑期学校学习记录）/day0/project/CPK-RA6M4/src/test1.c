/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-07-16     ASUS       the first version
 */
/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-06-21     Tom_onemore       the first version
 */
#include <rtthread.h>
#include <rtdevice.h>
#include "hal_data.h"

#define ADC_DEV_NAME        "adc0"                      /* ADC �豸���� */
#define ADC_DEV_CHANNEL_1   0                           /* ADC ͨ�� HC-SR501 */
//#define ADC_DEV_CHANNEL_2   1                           /* ADC ͨ�� MQ-2 */
#define BUZZER              BSP_IO_PORT_06_PIN_11       /* ������I/O���� */

static int adc_vol_sample(int argc, char *argv[])
{
    rt_adc_device_t adc_dev;
    rt_uint32_t value1, vol1;
    rt_err_t ret1 = RT_EOK;
//    rt_err_t ret2 = RT_EOK;

    /* �����豸 */
    adc_dev = (rt_adc_device_t)rt_device_find(ADC_DEV_NAME);
    if (adc_dev == RT_NULL)
    {
        rt_kprintf("adc sample run failed! can't find %s device!\n", ADC_DEV_NAME);
        return RT_ERROR;
    }

    /* ��ʼ������������Ϊ�͵�ƽ */
    rt_pin_write(BUZZER, PIN_LOW);

    while(1)
        {
            /* ʹ���豸 */
            ret1 = rt_adc_enable(adc_dev, ADC_DEV_CHANNEL_1);
//            ret2 = rt_adc_enable(adc_dev, ADC_DEV_CHANNEL_2);
            /* ��ȡ����ֵ */
            value1 = rt_adc_read(adc_dev, ADC_DEV_CHANNEL_1);
            rt_kprintf("the value1 is :%d \n", value1);
            rt_hw_us_delay(1000);
//            value2 = rt_adc_read(adc_dev, ADC_DEV_CHANNEL_2);
//            rt_kprintf("the value1 is :%d  the value2 is :%d \n", value1,value2);

            /* ת��Ϊ��Ӧ��ѹֵ��3.3V��Ӧ12λ���ֵ4096�����ݾ��ȳ���100����2λС�� */
            vol1 = value1 * 330 / 4096;
//            vol2 = value2 * 330 / 4096;
            rt_kprintf("the voltage1 is :%d.%02d\n", vol1 / 100, vol1 % 100);
            rt_hw_us_delay(1000);
            if (vol1 > 3.3 )
                {
                    rt_pin_write(BUZZER, PIN_HIGH); /*�򿪷�����*/

                }
            else
                {
                    rt_pin_write(BUZZER, PIN_LOW); /*�رշ�����*/
                }
        }

    /* �ر�ͨ�� */
    ret1 = rt_adc_disable(adc_dev, ADC_DEV_CHANNEL_1);
//    ret2 = rt_adc_disable(adc_dev, ADC_DEV_CHANNEL_2);
    return ret1;
}
/* ������ msh �����б��� */
MSH_CMD_EXPORT(adc_vol_sample, adc voltage convert sample);
//INIT_BOARD_EXPORT(adc_vol_sample);

