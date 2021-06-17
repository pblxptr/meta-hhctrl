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
#include <linux/cdev.h>
#include <linux/sysfs.h>
#include<linux/kobject.h> 
#include "sensor.h"
#include "engine.h"


#define EN_DEBOUNCE

#ifdef EN_DEBOUNCE
#include <linux/jiffies.h>
extern unsigned long volatile jiffies;
unsigned long old_jiffie = 0;
#endif

#define DEV_BASE_MINOR (0)
#define DEV_COUNT  (1)

/*
** Function prototypes for attributes
*/
ssize_t hatch2sr_show_status(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t hatch2sr_store_status(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);

/*
**	Function prototypes for file operations
*/
static int 		 __init hatch2sr_driver_init(void);
static void 	 __exit hatch2sr_driver_exit(void);
static int 		 hatch2sr_open(struct inode*, struct file*);
static int  	 hatch2sr_release(struct inode*, struct file*);
static ssize_t hatch2sr_read(struct file*, char __user*, size_t, loff_t*);
static ssize_t hatch2sr_write(struct file*, const char __user*, size_t, loff_t*);

/*
** Driver struct 
*/
struct hatch2sr_device {
	dev_t num;
	struct cdev cdev;
	struct device* dev;
} hatch2sr_dev;

/*
** File operations
*/ 
static struct file_operations fops = {
	.owner = 		THIS_MODULE,
	.open = 		hatch2sr_open,
	.release = 	hatch2sr_release,
	.read = 		hatch2sr_read,
	.write = 		hatch2sr_write
};

/*
** Attributtes 
*/
static DEVICE_ATTR(status, S_IWUSR | S_IRUGO, hatch2sr_show_status, hatch2sr_store_status);

static struct attribute* hatch2sr_attrs[] = {
	&dev_attr_status.attr,
	NULL
};

static const struct attribute_group hatch2sr_group = {
	.attrs = hatch2sr_attrs,
};

static const struct attribute_group* hatch2sr_groups[] = {
	&hatch2sr_group,
	NULL
};


/* Function definitions for attributes
** This function is called when somebody reads the status attribute.
*/
ssize_t hatch2sr_show_status(struct device *dev, struct device_attribute *attr, char *buf)
{
	return 0;
}

/*
** This function is called when somebody writes the status attribute.
*/
ssize_t hatch2sr_store_status(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	return 0;
}


/* Function definitions for file operations
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

static int hatch2sr_driver_probe(struct platform_device* pdev)
{
	hatch2sr_dev.dev = &pdev->dev;

	// Allocate Major number 
	if (alloc_chrdev_region(&hatch2sr_dev.num, DEV_BASE_MINOR, DEV_COUNT, "hatch2sr") < 0) {
		dev_err(hatch2sr_dev.dev, "Cannot allocate major number for device.\n");
		return -1;
	}
	pr_info("Major: %d, Minor: %d \n", MAJOR(hatch2sr_dev.num), MINOR(hatch2sr_dev.num));

	//Initializing cdev for driver
	cdev_init(&hatch2sr_dev.cdev, &fops);

	//Add character device
	if ((cdev_add(&hatch2sr_dev.cdev, hatch2sr_dev.num, DEV_COUNT)) < 0){
			dev_err(hatch2sr_dev.dev, "Cannot add the device to the system\n");
			goto r_cdev;
		}

	//Create device class 
	if ((hatch2sr_dev.dev->class = class_create(THIS_MODULE, "hatch2sr")) == NULL){ // unregister_chrdev_region + cdev_del
			dev_err(hatch2sr_dev.dev, "Cannot create the struct class for device\n");
			goto r_class;
	}
	hatch2sr_dev.dev->class->dev_groups = hatch2sr_groups;

	//Create device nodes
	if ((device_create(hatch2sr_dev.dev->class, NULL, hatch2sr_dev.num, NULL, "hatch2sr")) == NULL) {  // unregister_chrdev_region + cdev_del + class_destroy
			dev_err(hatch2sr_dev.dev, "Cannot create the Device\n");
			goto r_device;
	}

	pr_info("Hatch2sr Kernel Module probed successfully...\n");
	return 0;

	r_device:
		class_destroy(hatch2sr_dev.dev->class);	
	r_class:
		cdev_del(&hatch2sr_dev.cdev);	
	r_cdev: 
		unregister_chrdev_region(hatch2sr_dev.num, DEV_COUNT);	
		return -1;
}

static int hatch2sr_driver_remove(struct platform_device *pdev)
{
	printk("%s\n", __FUNCTION__);

	device_destroy(hatch2sr_dev.dev->class, hatch2sr_dev.num);
	class_destroy(hatch2sr_dev.dev->class);
	cdev_del(&hatch2sr_dev.cdev);
	unregister_chrdev_region(hatch2sr_dev.num, DEV_COUNT);

	pr_info("Hatch2sr Kernel Module removed successfully...\n");

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

/**
 * 
 * Hatch functions:
 * 	- open 
 *  - close
 *  - status
 * 		- attributes: status
 * 		- values: closed | open | changing_position | faulty
 * 
 * Engine functions:
 * 	- start
 * 	- stop
 *  - set_speed
 * 		- attribute: speed
 *  - get_speed
 * 		- attribute: speed
 * 
 * Sensor functions:
 *  - get_status 
 *  	- attributes: open_pos_status | closed_pos_status
 *    - values: open | closed
 *   
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 */