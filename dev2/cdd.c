#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>


#define CDD_MINOR 0
#define CDD_COUNT 1

// 设备号
dev_t dev;

int cdd_init(void){

	// 静态设备号申请

	int ret = alloc_chrdev_region(&dev,CDD_MINOR,CDD_COUNT,"cdd_alloc_demo");
	if(ret<0){
		printk("register_chardev_region failed!\n");
		return ret;
	}
	printk("register_chardev_region success!\n");
    return 0;
}
void cdd_exit(void){
	// 构造设备号

	unregister_chrdev_region(dev,CDD_COUNT);
	printk("unregister_chrdev_region\n");
}

module_init(cdd_init)
module_exit(cdd_exit)
