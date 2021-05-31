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

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/pwm.h>

#define BASE_MINOR 0
#define DEV_COUNT  1

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

dev_t dev = 0;
static struct class* dev_class;
static struct cdev hatch2sr_cdev;

struct pwm_hatch_engine {
	struct pwm_device* pwm;
	
};

/*
** This function is called when somebody has called open driver file.
*/
static int	hatch2sr_open(struct inode* inode, struct file* file)
{
	pr_info("%s\n", __FUNCTION__);

	return 0;
}

/*
** This function is called when somebody has called close driver file.
*/
static int hatch2sr_release(struct inode* inode, struct file* file)
{
	pr_info("%s\n", __FUNCTION__);

	return 0;
}

/*
** This function is called when somebody has called read driver file.
*/
static ssize_t hatch2sr_read(struct file* file, char __user* buf, size_t len, loff_t* off)
{
	pr_info("%s\n", __FUNCTION__);

	return 0;
}

/*
** This function is called when somebody has called write driver file.
*/
static ssize_t hatch2sr_write(struct file* file, const char __user* buf, size_t len, loff_t* off)
{
	pr_info("%s\n", __FUNCTION__);

	return len;
}

/*
** Module init function
*/
int static __init hatch2sr_driver_init(void)
{
	pr_info("%s\n", __FUNCTION__);

	if (alloc_chrdev_region(&dev, BASE_MINOR, DEV_COUNT, "hatch2sr") < 0) {
		pr_info("Cannot allocate major number for device.\n");

		return -1;
	}

	/* Initializing cdev structutre */
	cdev_init(&hatch2sr_cdev, &fops);
	
	/* Adding character device to the system */
	if (cdev_add(&hatch2sr_cdev, dev, DEV_COUNT) < 0) {
		pr_err("Cannot add the cdev to the system.\n");

		goto r_class_fail;
	}

	/* Creating class */
	if ((dev_class = class_create(THIS_MODULE, "hatch2sr_class")) == NULL) {
		pr_err("Cannot create the struct class for device.\n");
		
		goto r_class_fail;
	}

	/* Creating device */
	if (device_create(dev_class, NULL, dev, NULL, "hatch2sr_device") == NULL) {
		pr_err("Cannot create the device.\n");

		goto r_device_fail;
	}

	pr_info("Major: %d, Minor: %d", MAJOR(dev), MINOR(dev));
	pr_info("Kernel Module inserted successfully...\n");

	return 0;

	r_device_fail:
		class_destroy(dev_class);
	r_class_fail:
		unregister_chrdev_region(dev, DEV_COUNT);
		return -1;
}

/*
** Module exit function
*/
static void __exit hatch2sr_driver_exit(void)
{
	pr_info("%s\n", __FUNCTION__);

	device_destroy(dev_class, dev);
	class_destroy(dev_class);
	cdev_del(&hatch2sr_cdev);
	unregister_chrdev_region(dev, DEV_COUNT);
	pr_info("Kernel Module removed successfully...\n");

}

module_init(hatch2sr_driver_init);
module_exit(hatch2sr_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Patryk Biel");
MODULE_DESCRIPTION("Driver used to control hatch through 8-pin relay.");
MODULE_VERSION("1:0.0");