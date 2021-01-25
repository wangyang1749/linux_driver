#include <linux/init.h>
#include <linux/module.h>

int hello_init(void){
	printk("Hello init\n");
    return 0;
}
void hello_exit(void){
	printk("Hello exit\n");
}

module_init(hello_init)
module_exit(hello_exit)