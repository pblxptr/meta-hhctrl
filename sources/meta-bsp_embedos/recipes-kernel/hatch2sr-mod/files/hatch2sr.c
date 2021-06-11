/******************************************************************************
 *
 *   Copyright (C) 2011  Intel Corporation. All rights reserved.
 *
 *   This program is free software;  you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; version 2 of the License.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY;  without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 *   the GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program;  if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 *****************************************************************************/

#include <linux/input.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/pwm.h>
#include <linux/slab.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/err.h>
#include <linux/interrupt.h>

#define EN_DEBOUNCE

#ifdef EN_DEBOUNCE
#include <linux/jiffies.h>
extern unsigned long volatile jiffies;
unsigned long old_jiffie = 0;
#endif

#define BASE_MINOR 0
#define DEV_COUNT  1

#define PWM_ENGINE_PERIOD_NS  		 			 (10000000)
#define PWM_ENGINE_INITIAL_DUTY    			 (0)
#define OPEN_POSITION_SENSOR_IDX   			 (0)
#define CLOSED_POSITION_SENSOR_IDX 			 (1)
#define SENSOR_DEBOUNCE_MICROSECONDS     (200)

/*
**	Function prototypes for file operations
*/
static int 		 __init hatch2sr_driver_init(void);
static void 	 __exit hatch2sr_driver_exit(void);
static int 		 hatch2sr_open(struct inode*, struct file*);
static int  	 hatch2sr_release(struct inode*, struct file*);
static ssize_t hatch2sr_read(struct file*, char __user*, size_t, loff_t*);
static ssize_t hatch2sr_write(struct file*, const char __user*, size_t, loff_t*);

static struct file_operations fops = {
	.owner = 		THIS_MODULE,
	.open = 		hatch2sr_open,
	.release = 	hatch2sr_release,
	.read = 		hatch2sr_read,
	.write = 		hatch2sr_write
};

/*
** Function prototypes for gpio operations
*/
static irqreturn_t sensor_gpio_isr(int irq, void* dev_id);

struct miscdevice hatch2srmisc = {
	.name	= "hatch2sr",
	.minor	= MISC_DYNAMIC_MINOR,
	.fops	= &fops,
};
 

dev_t dev = 0;
struct pwm_device* pwm_engine_dev;

struct sensor_desc {
	struct gpio_desc* gpio_desc;
	int gpio_id;
	int irq;
};

struct sensors {
	struct sensor_desc open_position;
	struct sensor_desc closed_position;
} sensors;

/*
** This function is called when somebody has called open driver file.
*/
static int	hatch2sr_open(struct inode* inode, struct file* file)
{
	printk("%s\n", __FUNCTION__);

	return 0;
}

/*
** This function is called when somebody has called close driver file.
*/
static int hatch2sr_release(struct inode* inode, struct file* file)
{
	printk("%s\n", __FUNCTION__);

	return 0;
}

/*
** This function is called when somebody has called read driver file.
*/
static ssize_t hatch2sr_read(struct file* file, char __user* buf, size_t len, loff_t* off)
{
	printk("%s\n", __FUNCTION__);

	return 0;
}

/*
** This function is called when somebody has called write driver file.
*/
static ssize_t hatch2sr_write(struct file* file, const char __user* buf, size_t len, loff_t* off)
{
	printk("%s\n", __FUNCTION__);

	return len;
}

static int hatch2sr_driver_probe(struct platform_device *pdev)
{
	struct device* dev = &pdev->dev;

	printk("%s\n", __FUNCTION__);
	
	// Initialize pwm
	pwm_engine_dev = pwm_get(dev, "motor1");

	if (IS_ERR(pwm_engine_dev)) {
		dev_err(dev, "Cannot initialize pwm dev.\n");
		return -1;
	}
	// Configure -> pwm  
	pwm_config(pwm_engine_dev, PWM_ENGINE_INITIAL_DUTY, PWM_ENGINE_PERIOD_NS);
	pwm_enable(pwm_engine_dev);

	// Initialize gpios for sensors
	sensors.open_position.gpio_desc = gpiod_get_index(dev, NULL, OPEN_POSITION_SENSOR_IDX, GPIOD_IN);
	sensors.open_position.gpio_id = desc_to_gpio(sensors.open_position.gpio_desc);

	sensors.closed_position.gpio_desc = gpiod_get_index(dev, NULL, CLOSED_POSITION_SENSOR_IDX, GPIOD_IN);
	sensors.closed_position.gpio_id = desc_to_gpio(sensors.closed_position.gpio_desc);

	if (IS_ERR(sensors.open_position.gpio_desc) || IS_ERR(sensors.closed_position.gpio_desc)) {
		dev_err(dev, "Cannot initialize gpios for sensors.\n");
		return -1;
	}

	// Configure -> gpios
	gpiod_direction_input(sensors.open_position.gpio_desc);
	gpiod_export(sensors.open_position.gpio_desc, false);
	
	gpiod_direction_input(sensors.closed_position.gpio_desc);	
	gpiod_export(sensors.closed_position.gpio_desc, false);

	sensors.open_position.irq = gpio_to_irq(sensors.open_position.gpio_id);
	if (request_irq(sensors.open_position.irq, sensor_gpio_isr, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, 
			"hatch2sr_o_sensor.", sensor_gpio_isr)) {
		dev_err(dev, "Cannot request isr for open position sensor.\n");
		return -1;
	}

	// sensors.closed_position.irq = gpio_to_irq(sensors.closed_position.gpio_id);
	// if (request_irq(sensors.closed_position.irq, sensor_gpio_isr, IRQF_TRIGGER_LOW , 
	// 		"hatch2sr c_sensor.", sensor_gpio_isr)) {
	// 	dev_err(dev, "Cannot request isr for closed position sensor.\n");
	// 	return -1;
	// }

	printk("Irq for open position sensor: %d\n", sensors.open_position.irq);
	printk("Irq for closed position sensor: %d\n", sensors.closed_position.irq);


 	return 0;
}

static int hatch2sr_driver_remove(struct platform_device *pdev)
{
	printk("%s\n", __FUNCTION__);

	pwm_put(pwm_engine_dev);

	//TODO: Release gpios
	//TODO: Relase isr

	return 0;
}

// GPIO RELATED FUNCTIONS // 
static irqreturn_t sensor_gpio_isr(int irq, void* dev_id)
{
	static unsigned int counter = 0;

	#ifdef EN_DEBOUNCE
		unsigned long diff = jiffies - old_jiffie;
		if (diff < 100)
		{
			return IRQ_HANDLED;
		}
		
		old_jiffie = jiffies;
	#endif  

	++counter;

	if (dev_id == sensor_gpio_isr) {
		printk("Dev id is ok\n");
	} else {
		printk("Dev is IS NOT OK!!!\n");
	}

	printk("Handle interrupt\n");

	return IRQ_HANDLED;	
}


static const struct of_device_id hatch2sr_match[] = {
	{ .compatible = "hatch2sr", },
	{ },
};

MODULE_DEVICE_TABLE(of, hatch2sr_match);

static struct platform_driver hatch2sr_driver = {
	.probe	= hatch2sr_driver_probe,
	.remove = hatch2sr_driver_remove,
	.driver = {
		.name	= "hatch2sr",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(hatch2sr_match),
	},
};

module_platform_driver(hatch2sr_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Patryk Biel");
MODULE_DESCRIPTION("Driver used to control hatch through 8-pin relay.");
MODULE_VERSION("1:0.0");