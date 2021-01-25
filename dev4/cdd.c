#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>

#define CDD_MINOR 0
#define CDD_COUNT 1

// 设备号
dev_t dev;

//声明cdev
struct cdev cdd_cdev;

//设备类的指针
struct class *cdd_class;
//设备的指针
struct device *cdd_device;



/**
 *inode是文件的结构节点，用来存储静态文件。
 * 文件创建时，内核中就有inode结构 
 * 文件打开时会创建一个file结构
 */
int cdd_open(struct inode *inode, struct file *filp){
	printk("cdd_open\n");
	return 0;
}
int cdd_release(struct inode *inode, struct file *filp){
	printk("cdd_release\n");
	return 0;
}
ssize_t cdd_read(struct file *filp, char __user *buf, size_t size, loff_t *offset){
	printk("cdd_read\n");
	return 0;
}
ssize_t cdd_write(struct file *filp, const char __user *buf, size_t size, loff_t *offset){
	printk("cdd_write\n");
}

long cdd_unlocked_ioctl (struct file *filp, unsigned int cmd, unsigned long data){
	printk("cdd_unlocked_ioctl\n");
}


struct file_operations cdd_fops={
	.owner = THIS_MODULE,
	.open = cdd_open,
	.read = cdd_read,
	.write = cdd_write,
	.unlocked_ioctl = cdd_unlocked_ioctl, //io ctrl 接口
	.release = cdd_release,
};


int cdd_init(void){

	// 设备号申请
	int ret = alloc_chrdev_region(&dev,CDD_MINOR,CDD_COUNT,"cdd_alloc_demo");
	if(ret<0){
		printk("register_chardev_region failed!\n");
		return ret;
	}
	// 初始化cdev
	cdev_init(&cdd_cdev,&cdd_fops);
	// 将cdev添加到内核并绑定设备号
	ret = cdev_add(&cdd_cdev,dev,CDD_COUNT);
	if(ret<0){
		unregister_chrdev_region(dev,CDD_COUNT);
		printk("cdev_add error!\n");
		return ret;
	}
	//创建设备类
	
	//会在/sys/class目录创建cdd_class子目录
	cdd_class =  class_create(THIS_MODULE,"cdd_class");
	if(IS_ERR(cdd_class)){
		cdev_del(&cdd_cdev);
		unregister_chrdev_region(dev,CDD_COUNT);
		

		printk("class_create error!\n");
		ret = PTR_ERR(cdd_class);
		return ret;
	}

	// 创建设备文件
	cdd_device = device_create(cdd_class,NULL,dev,NULL,"cdd");
	if(IS_ERR(cdd_device)){
		//销毁设备类
		class_destroy(cdd_class);
		cdev_del(&cdd_cdev);
		unregister_chrdev_region(dev,CDD_COUNT);
		

		printk("class_create error!\n");
		ret = PTR_ERR(cdd_device);
		return ret;
	}
	printk("device_create success!\n");
    return 0;
}

void cdd_exit(void){
	//销毁设备文件
	device_destroy(cdd_class,dev);
	//销毁设备类
	class_destroy(cdd_class);
	//销毁cdev
	cdev_del(&cdd_cdev);
	// 构造设备号
	unregister_chrdev_region(dev,CDD_COUNT);
	
	
	printk("unregister_chrdev_region\n");
}

MODULE_LICENSE("GPL");
module_init(cdd_init)
module_exit(cdd_exit)

