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

#define BASE_MINOR 0
#define DEV_COUNT  1
#define ENGINE_PWM 0

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
static struct class* dev_class;

struct hatch_pwm_engine {
	struct pwm_device* pwm;
	unsigned long period;
};

struct hatch_pwm_engine* pwm_engine;

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
	printk("%s\n", __FUNCTION__);

  struct device* dev = &pdev->dev;

	int error;
	
	pwm_engine = kzalloc(sizeof(struct hatch_pwm_engine), GFP_KERNEL);

	if (!pwm_engine) {
		return -ENOMEM;
	}
   
	pwm_engine->pwm = pwm_get(dev, NULL); //Apply for pwm device

	if (IS_ERR(pwm_engine->pwm)) {
		error = PTR_ERR(pwm_engine->pwm);

		if (error != -EPROBE_DEFER) {
			dev_err(dev, "Failed to request PWM device: %d\n", error);
		}
		
		return error;
	}

	misc_register(&hatch2srmisc);

 	return 0;
}
 
static int hatch2sr_driver_remove(struct platform_device *pdev)
{
	printk("%s\n", __FUNCTION__);

	return 0;
}

static int __maybe_unused hatch2sr_suspend(struct device *dev)
{
	printk("%s\n", __FUNCTION__);
	
	struct hatch_pwm_engine* pwm_engine = dev_get_drvdata(dev);
 
	if (pwm_engine->period)
		pwm_disable(pwm_engine->pwm);
 
	return 0;
}

static int __maybe_unused hatch2sr_resume(struct device *dev)
{
	printk("%s\n", __FUNCTION__);	

	struct hatch_pwm_engine* pwm_engine = dev_get_drvdata(dev);
 
	if (pwm_engine->period) {
		pwm_config(pwm_engine->pwm, pwm_engine->period / 2, pwm_engine->period);
		pwm_enable(pwm_engine->pwm);
	}
 
	return 0;
}

static SIMPLE_DEV_PM_OPS(hatch2sr_pm_ops,
			 hatch2sr_suspend, 
			 hatch2sr_resume
);

#ifdef CONFIG_OF
static const struct of_device_id hatch2sr_match[] = {
	{ .compatible = "hatch2sr", },
	{ },
};

MODULE_DEVICE_TABLE(of, hatch2sr_match);
#endif

static struct platform_driver hatch2sr_driver = {
	.probe	= hatch2sr_driver_probe,
	.remove = hatch2sr_driver_remove,
	.driver = {
		.name	= "hatch2sr",
		.owner = THIS_MODULE,
		.pm	= &hatch2sr_pm_ops,
		.of_match_table = of_match_ptr(hatch2sr_match),
	},
};

module_platform_driver(hatch2sr_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Patryk Biel");
MODULE_DESCRIPTION("Driver used to control hatch through 8-pin relay.");
MODULE_VERSION("1:0.0");