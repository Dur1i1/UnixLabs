#include <linux/init.h> /* Needed for the macros */  
#include <linux/module.h> /* Needed by all modules */
#include <linux/printk.h> /* Needed for pr_info() */  
  
static int __init ModuleInit(void)
{  
    pr_info("Welcome to the Tomsk State University\n");  
    return 0;  
}  
  
static void __exit ModuleExit(void)  
{
    pr_info("Tomsk State University forever!\n");  
}  
  
module_init(ModuleInit);  
module_exit(ModuleExit);
 
MODULE_LICENSE("GPL");
