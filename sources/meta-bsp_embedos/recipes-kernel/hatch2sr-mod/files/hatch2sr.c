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

#define BASE_MINOR 0
#define DEV_COUNT  1

#define PWM_ENGINE_PERIOD_NS  		 (10000000)
#define PWM_ENGINE_INITIAL_DUTY    (0)
#define OPEN_POSITION_SENSOR_IDX   (0)
#define CLOSED_POSITION_SENSOR_IDX (1)

/*
**	Function prototypes
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

struct miscdevice hatch2srmisc = {
	.name	= "hatch2sr",
	.minor	= MISC_DYNAMIC_MINOR,
	.fops	= &fops,
};
 

dev_t dev = 0;
struct pwm_device* pwm_engine_dev;
struct gpio_desc* open_position_sensor;
struct gpio_desc* closed_position_sensor;


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
	int closed_position_sensor_val;
	int open_position_sensor_val;

	closed_position_sensor_val = gpiod_get_value(closed_position_sensor);
	open_position_sensor_val = gpiod_get_value(open_position_sensor);

	printk("%s\n", __FUNCTION__);
	printk("Sensor closed value: %d\n", closed_position_sensor_val);
	printk("Sensor open value: %d\n", open_position_sensor_val);

	gpiod_set_value(open_position_sensor, 1);

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
	
	//Initialize pwm
	pwm_engine_dev = pwm_get(dev, "motor1");

	if (IS_ERR(pwm_engine_dev)) {
		dev_err(dev, "Cannot initialize pwm dev.\n");
		return -1;
	}

	//Initialize gpios for sensors
	open_position_sensor = gpiod_get_index(dev, NULL, OPEN_POSITION_SENSOR_IDX, GPIOD_IN);
	closed_position_sensor = gpiod_get_index(dev, NULL, CLOSED_POSITION_SENSOR_IDX, GPIOD_IN);

	if (IS_ERR(open_position_sensor) || IS_ERR(closed_position_sensor)) {
		dev_err(dev, "Cannot initialize gpios for sensors.\n");
		return -1;
	}

	//Configure pwm && gpios  
	pwm_config(pwm_engine_dev, PWM_ENGINE_INITIAL_DUTY, PWM_ENGINE_PERIOD_NS);
	pwm_enable(pwm_engine_dev);

	gpiod_export(open_position_sensor, false);
	gpiod_export(closed_position_sensor, false);

 	return 0;
}
static int hatch2sr_driver_remove(struct platform_device *pdev)
{
	printk("%s\n", __FUNCTION__);

	pwm_put(pwm_engine_dev);
	gpiod_put(open_position_sensor);
	gpiod_put(closed_position_sensor);

	return 0;
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