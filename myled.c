// SPDX-License-Identifier: GPL-3.0-only
/*
    Copyright (C) 2020  Kazuki Shirasu. All right reserved.
*/
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/delay.h>

MODULE_AUTHOR("Ryuichi Ueda and Kazuki Shirasu");
MODULE_DESCRIPTION("driver for LED control");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.0.1");

static dev_t dev;
static struct cdev cdv;
static struct class *cls = NULL;
static volatile u32 *gpio_base = NULL;

static ssize_t led_write(struct file* filp, const char* buf, size_t count, loff_t* pos)
{
	char c;
	int n, i;
	if(copy_from_user(&c,buf,sizeof(char)))
		return -EFAULT;

	if(c == '1'){
		gpio_base[7] = 1 << 24;
		msleep(1500);
		gpio_base[10] = 1 << 24;
		gpio_base[7] = 1 << 24;
		gpio_base[7] = 1 << 23;
		msleep(1500);
		gpio_base[10] = 1 << 24;
		gpio_base[10] = 1 << 23;
		gpio_base[7] = 1 << 23;
		msleep(1500);
		gpio_base[10] = 1 << 23;


		for(i = 0; i < 35; i++){
			switch(i){
				case 1:
				case 5:
				case 9:
				case 11:
				case 13:
				case 23:
				case 25:	
				case 29:	
					gpio_base[7] = 1 << 25;
					msleep(1000);
					break;

				case 3:
				case 7:
				case 15:
				case 17:
				case 19:	
				case 21:	
				case 27:	
				case 31:	
				case 33:	
					gpio_base[7] = 1 << 25;
					msleep(500);
					break;

				case 6:
				case 10:	
				case 18:	
				case 24:	
				case 30:	
				case 34:	
					gpio_base[10] = 1 << 25;
					msleep(1000);
					break;

				default:
					gpio_base[10] = 1 << 25;
					msleep(500);
					break;
					
			}
		}

	}else if(c == '1'){
		gpio_base[10] = 1 << 25;
		gpio_base[10] = 1 << 24;
		gpio_base[10] = 1 << 23;
		gpio_base[10] = 1 << 22;
	}

	return 1;
}

static ssize_t sushi_read(struct file* filp, char* buf, size_t count, loff_t* pos)
{
	int size = 0;
	char sushi[] = {'s', 'u', 's', 'h', 'i', 0x0A};
	if(copy_to_user(buf + size, (const char *)sushi, sizeof(sushi))){
		printk(KERN_INFO "sushi : copy_to_user failed\n");
		return -EFAULT;
	}
	size += sizeof(sushi);
	return size;
}
static struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.write = led_write,
	.read = sushi_read
};
static int __init init_mod(void)
{
	int retval;
	retval = alloc_chrdev_region(&dev, 0, 1, "myled");
	if(retval < 0){
		printk(KERN_ERR "alloc_chrdev_region failed.\n");
		return retval;
	}

	printk(KERN_INFO "%s is loaded. major:%d\n",__FILE__,MAJOR(dev));
	
	cdev_init(&cdv, &led_fops);
	retval = cdev_add(&cdv, dev, 1);
	if(retval < 0){
		printk(KERN_ERR "cdev_add failed. major:%d, minor:%d",MAJOR(dev),MINOR(dev));
		return retval;
	}

	cls = class_create(THIS_MODULE,"myled");
	if(IS_ERR(cls)){
		printk(KERN_ERR "class_create failed");
		return PTR_ERR(cls);
	}
	device_create(cls, NULL, dev, NULL, "myled%d",MINOR(dev));
	gpio_base = ioremap_nocache(0xfe200000, 0xA0);

	const u32 led = 25;
	const u32 index = led/10;
	const u32 shift = (led%10)*3;
	const u32 mask = ~(0x7 << shift);
	gpio_base[index] = (gpio_base[index] & mask) | (0x1 << shift);
	const u32 led_r = 24;
	const u32 index_r = led_r/10;
	const u32 shift_r = (led_r%10)*3;
	const u32 mask_r = ~(0x7 << shift_r);
	gpio_base[index_r] = (gpio_base[index_r] & mask_r) | (0x1 << shift_r);
	const u32 led_g = 23;
	const u32 index_g = led_g/10;
	const u32 shift_g = (led_g%10)*3;
	const u32 mask_g = ~(0x7 << shift_g);
	gpio_base[index_g] = (gpio_base[index_g] & mask_g) | (0x1 << shift_g);
	const u32 led_b = 22;
	const u32 index_b = led_b/10;
	const u32 shift_b = (led_b%10)*3;
	const u32 mask_b = ~(0x7 << shift_b);
	gpio_base[index_b] = (gpio_base[index_b] & mask_b) | (0x1 << shift_b);

	return 0;
}
static void __exit cleanup_mod(void)
{
	cdev_del(&cdv);
	device_destroy(cls, dev);
	class_destroy(cls);
	unregister_chrdev_region(dev, 1);
	printk(KERN_INFO "%s is unloaded. major:%d\n" ,__FILE__,MAJOR(dev));
}

module_init(init_mod);
module_exit(cleanup_mod);
