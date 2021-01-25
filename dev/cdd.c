#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>


#define CDD_MAJOR 265
#define CDD_MINOR 0
#define CDD_COUNT 1

// 设备号
dev_t dev;

int cdd_init(void){
	// 构造设备号
	dev = MKDEV(CDD_MAJOR,CDD_MINOR);
	// 静态设备号申请

	int ret = register_chrdev_region(dev,CDD_COUNT,"cdd_demo2");
	if(ret<0){
		printk("register_chardev_region failed!");
		return ret;
	}
	printk("register_chardev_region success!");
    return 0;
}
void cdd_exit(void){
	// 构造设备号
	dev = MKDEV(CDD_MAJOR,CDD_MINOR);
	unregister_chrdev_region(dev,CDD_COUNT);
	printk("unregister_chrdev_region");
}

module_init(cdd_init)
module_exit(cdd_exit)
