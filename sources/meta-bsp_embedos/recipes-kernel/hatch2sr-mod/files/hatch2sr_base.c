// /******************************************************************************
//  *
//  *   Copyright (C) 2011  Intel Corporation. All rights reserved.
//  *
//  *   This program is free software;  you can redistribute it and/or modify
//  *   it under the terms of the GNU General Public License as published by
//  *   the Free Software Foundation; version 2 of the License.
//  *
//  *   This program is distributed in the hope that it will be useful,
//  *   but WITHOUT ANY WARRANTY;  without even the implied warranty of
//  *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
//  *   the GNU General Public License for more details.
//  *
//  *   You should have received a copy of the GNU General Public License
//  *   along with this program;  if not, write to the Free Software
//  *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//  *
//  *****************************************************************************/

// #include <linux/input.h>
// #include <linux/module.h>
// #include <linux/kernel.h>
// #include <linux/of.h>
// #include <linux/platform_device.h>
// #include <linux/pwm.h>
// #include <linux/slab.h>
// #include <linux/miscdevice.h>
// #include <linux/uaccess.h>
// #include <linux/device.h>
// #include <linux/gpio.h>
// #include <linux/err.h>
// #include <linux/interrupt.h>
// #include "sensor.h"
// #include "engine.h"


// #define EN_DEBOUNCE

// #ifdef EN_DEBOUNCE
// #include <linux/jiffies.h>
// extern unsigned long volatile jiffies;
// unsigned long old_jiffie = 0;
// #endif

// #define BASE_MINOR 0
// #define DEV_COUNT  1

// #define PWM_ENGINE_PERIOD_NS  		 			 (10000000)
// #define PWM_ENGINE_INITIAL_DUTY    			 (0)
// #define OPEN_POSITION_SENSOR_IDX   			 (0)
// #define CLOSED_POSITION_SENSOR_IDX 			 (1)
// #define SENSOR_DEBOUNCE_MICROSECONDS     (200)

// /*
// **	Function prototypes for file operations
// */
// static int 		 __init hatch2sr_driver_init(void);
// static void 	 __exit hatch2sr_driver_exit(void);
// static int 		 hatch2sr_open(struct inode*, struct file*);
// static int  	 hatch2sr_release(struct inode*, struct file*);
// static ssize_t hatch2sr_read(struct file*, char __user*, size_t, loff_t*);
// static ssize_t hatch2sr_write(struct file*, const char __user*, size_t, loff_t*);

// static struct file_operations fops = {
// 	.owner = 		THIS_MODULE,
// 	.open = 		hatch2sr_open,
// 	.release = 	hatch2sr_release,
// 	.read = 		hatch2sr_read,
// 	.write = 		hatch2sr_write
// };

// /*
// ** Function prototypes for gpio operations
// */
// static int aquire_isr(void);
// static void release_isr(void);
// static irqreturn_t sensor_gpio_isr(int irq, void* dev_id);

// struct miscdevice hatch2srmisc = {
// 	.name	= "hatch2sr",
// 	.minor	= MISC_DYNAMIC_MINOR,
// 	.fops	= &fops,
// };

// dev_t dev = 0;
// static Engine engine;
// static Sensor open_pos_sensor;
// static Sensor closed_pos_sensor;

// /*
// ** This function is called when somebody has called open driver file.
// */
// static int	hatch2sr_open(struct inode* inode, struct file* file)
// {
// 	printk("%s\n", __FUNCTION__);

// 	return 0;
// }

// /*
// ** This function is called when somebody has called close driver file.
// */
// static int hatch2sr_release(struct inode* inode, struct file* file)
// {
// 	printk("%s\n", __FUNCTION__);

// 	return 0;
// }

// /*
// ** This function is called when somebody has called read driver file.
// */
// static ssize_t hatch2sr_read(struct file* file, char __user* buf, size_t len, loff_t* off)
// {
// 	printk("%s\n", __FUNCTION__);

// 	return 0;
// }

// /*
// ** This function is called when somebody has called write driver file.
// */
// static ssize_t hatch2sr_write(struct file* file, const char __user* buf, size_t len, loff_t* off)
// {
// 	printk("%s\n", __FUNCTION__);

// 	return len;
// }

// static int hatch2sr_driver_probe(struct platform_device *pdev)
// {
// 	struct device* dev = &pdev->dev;
// 	struct pwm_device* pwm_dev;
// 	struct gpio_desc* gpio_sensor_open;
// 	struct gpio_desc* gpio_sensor_closed;

// 	printk("%s\n", __FUNCTION__);
	
// 	// Configure engine
// 	pwm_dev = pwm_get(dev, "motor1");

// 	if (IS_ERR(pwm_dev)) {
// 		dev_err(dev, "Cannot initialize pwm dev.\n");
// 		return -1;
// 	}
// 	engine_init(&engine, pwm_dev);

// 	// Configure sensors
// 	gpio_sensor_open = gpiod_get_index(dev, NULL, OPEN_POSITION_SENSOR_IDX, GPIOD_IN);
// 	gpio_sensor_closed = gpiod_get_index(dev, NULL, CLOSED_POSITION_SENSOR_IDX, GPIOD_IN);
	
// 	if (IS_ERR(gpio_sensor_open) || IS_ERR(gpio_sensor_closed)) {
// 		dev_err(dev, "Cannot initialize gpio for sensors.\n");
// 		return -1;
// 	}
// 	sensor_init(&open_pos_sensor, gpio_sensor_open);
// 	sensor_init(&closed_pos_sensor, gpio_sensor_closed);

// 	// Configure interrupts
// 	if (aquire_isr()) {
// 		dev_err(dev, "Cannot initialize interrupts for sensors.\n");
// 		return -1;
// 	}

//  	return 0;
// }

// static int hatch2sr_driver_remove(struct platform_device *pdev)
// {
// 	printk("%s\n", __FUNCTION__);

// 	engine_deinit(&engine);
// 	sensor_deinit(&open_pos_sensor);
// 	sensor_deinit(&closed_pos_sensor);
// 	release_isr();

// 	return 0;
// }

// // GPIO RELATED FUNCTIONS // 

// int aquire_isr(void)
// {
// 	if (request_irq((sensor_irq(&open_pos_sensor)), sensor_gpio_isr, IRQF_TRIGGER_FALLING, 
// 			"hatch2sr_o_sensor.", sensor_gpio_isr) ||
// 		 (request_irq((sensor_irq(&closed_pos_sensor)), sensor_gpio_isr, IRQF_TRIGGER_FALLING, 
// 			"hatch2sr_o_sensor.", sensor_gpio_isr))
// 		) {
// 			return -1;
// 		}

// 	return 0;
// }

// void release_isr(void)
// {
// 	free_irq(sensor_irq(&open_pos_sensor), sensor_gpio_isr);
// 	free_irq(sensor_irq(&closed_pos_sensor), sensor_gpio_isr);
// }

// static irqreturn_t sensor_gpio_isr(int irq, void* dev_id)
// {
// 	static unsigned int counter = 0;

// 	#ifdef EN_DEBOUNCE
// 		unsigned long diff = jiffies - old_jiffie;
// 		if (diff < 100)
// 		{
// 			return IRQ_HANDLED;
// 		}
		
// 		old_jiffie = jiffies;
// 	#endif  

// 	++counter;

// 	if (dev_id == sensor_gpio_isr) {
// 		printk("Dev id is ok\n");
// 	} else {
// 		printk("Dev is IS NOT OK!!!\n");
// 	}

// 	printk("Handle interrupt\n");

// 	return IRQ_HANDLED;	
// }


// static const struct of_device_id hatch2sr_match[] = {
// 	{ .compatible = "hatch2sr", },
// 	{ },
// };

// MODULE_DEVICE_TABLE(of, hatch2sr_match);

// static struct platform_driver hatch2sr_driver = {
// 	.probe	= hatch2sr_driver_probe,
// 	.remove = hatch2sr_driver_remove,
// 	.driver = {
// 		.name	= "hatch2sr",
// 		.owner = THIS_MODULE,
// 		.of_match_table = of_match_ptr(hatch2sr_match),
// 	},
// };

// module_platform_driver(hatch2sr_driver);

// MODULE_LICENSE("GPL");
// MODULE_AUTHOR("Patryk Biel");
// MODULE_DESCRIPTION("Driver used to control hatch through 8-pin relay.");
// MODULE_VERSION("1:0.0");