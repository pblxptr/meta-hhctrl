#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xb10b8b09, "module_layout" },
	{ 0x6c90306a, "cdev_del" },
	{ 0x8cc4d64d, "cdev_init" },
	{ 0x896267e0, "gpiod_direction_output" },
	{ 0x2b68bd2f, "del_timer" },
	{ 0xc48eb6ff, "pwm_apply_state" },
	{ 0xbe4255ff, "gpio_to_desc" },
	{ 0xa24727c4, "device_destroy" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
	{ 0xc6f46339, "init_timer_key" },
	{ 0x962d577b, "desc_to_gpio" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xf2db698b, "__platform_driver_register" },
	{ 0xda02d67, "jiffies" },
	{ 0xc5850110, "printk" },
	{ 0x83cf6072, "gpiod_direction_input" },
	{ 0x417f319e, "device_create" },
	{ 0xc38c83b8, "mod_timer" },
	{ 0x92d5838e, "request_threaded_irq" },
	{ 0x8e865d3c, "arm_delay_ops" },
	{ 0x1690856c, "_dev_err" },
	{ 0x74a90316, "cdev_add" },
	{ 0x58532f0b, "gpiod_unexport" },
	{ 0x86332725, "__stack_chk_fail" },
	{ 0xe783e261, "sysfs_emit" },
	{ 0xda47ef20, "gpiod_get_value" },
	{ 0x495b536f, "pwm_put" },
	{ 0x60482d92, "gpiod_export" },
	{ 0xdfb110b4, "gpiod_to_irq" },
	{ 0x9c33fbd5, "class_destroy" },
	{ 0x222e7ce2, "sysfs_streq" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0x2b0efad4, "platform_driver_unregister" },
	{ 0x7f02188f, "__msecs_to_jiffies" },
	{ 0x99fece7b, "gpiod_set_value" },
	{ 0x3f50e6f0, "__class_create" },
	{ 0xcf905939, "gpiod_put" },
	{ 0xed2b1b13, "pwm_get" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xc1514a3b, "free_irq" },
	{ 0xbd5498da, "gpiod_get" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Chatch2sr");
MODULE_ALIAS("of:N*T*Chatch2srC*");

MODULE_INFO(srcversion, "5C58550AE0221E1D5ED93B5");
