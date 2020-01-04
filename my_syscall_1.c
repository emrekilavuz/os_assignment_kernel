#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/linkage.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <asm/unistd.h>
#include <asm/uaccess.h>
#include <linux/types.h>
#include <linux/stat.h>

SYSCALL_DEFINE1(my_syscall_1, char __user *, proc_id_str){
    struct file *f;
    struct file *f2;
    char pid[6];
    char path1[100];
    char* text;
    char starttime[30];
    char* text2;
    char uptime[15];

    int counter1;
    int start_of_starttime=0;
    int finish_of_starttime=0;
    int i;
    int start_of_uptime;
    int finish_of_uptime=0;
    int space_counter;
    int kk;
    int rc=-1;
    int rcc=-1;
    int read_count = 0;
    int read_count2 = 0;

    unsigned long long start_time_long=0;
    unsigned long long uptime_long=0;
    unsigned long long execution_time =0;
    unsigned long long minutes = 0;

    text = kmalloc(700, GFP_KERNEL);
    text2 = kmalloc(100, GFP_KERNEL);

    memset(pid, '\0', sizeof(char)*6);
    memset(path1, '\0', sizeof(char)*100);
    memset(text, '\0', sizeof(char)*700);
    memset(starttime, '\0', sizeof(char)*30);
    memset(text2, '\0', sizeof(char)*100);
    memset(uptime, '\0', sizeof(char)*15);

    copy_from_user(pid, proc_id_str, 5);
    printk("process is is %s\n",pid);
    
    
    strcpy(path1,"/proc/");
    strcat(path1,pid);
    strcat(path1,"/stat");
    printk("%s\n",path1);
    
    f = filp_open(path1, O_RDONLY, 0);
    if (IS_ERR(f)) {
        printk("simplekey: File can not open");
        return(PTR_ERR(f));
    }
    //read(fd,text,699);

    read_count = kernel_read(f, text, 699, &(f->f_pos));
    printk("read character count is : %d\n",read_count);    
    printk("%s\n",text);

    counter1 = 0;
    i = 0;
    while(text[i] != '\0'){
        if(counter1==20){
            start_of_starttime = i+1;
        }
        if(counter1==21){
            finish_of_starttime = i;
        }
        if(text[i] == ' '){
            counter1++;
        }
        i++;    
    }

    strncpy(starttime,&text[start_of_starttime], finish_of_starttime - start_of_starttime);
    printk("start time string : %s\n", starttime);
    
    kstrtoull(starttime, 10, &start_time_long);
    printk("start time of the process : %llu\n", start_time_long);
    //close(fd);
    rc = filp_close(f, NULL);
    if (rc == 0) {
        printk(KERN_INFO "Device successfully closed\n");
    }

    // read uptime file
    
    
    //int fd2 = open("/proc/uptime",O_RDONLY);
    f2 = filp_open("/proc/uptime", O_RDONLY, 0);
    if (IS_ERR(f2)) {
        printk("simplekey: File can not open");
        return(PTR_ERR(f2));
    }

    //read(fd2,text2,99);
    read_count2 = kernel_read(f2, text2, 99, &(f2->f_pos));
    printk("read character count 2 is : %d\n",read_count2);
    printk("uptime file content is : %s\n",text2);
    
    start_of_uptime = 0;
    space_counter = 0;
    kk = 0; //index-counter
    while(text2[kk] != '\0'){
       if(text2[kk] == ' ' || text2[kk] == '.'){
           space_counter++;
       }
       if(space_counter==1){
            finish_of_uptime = kk;
            break;
        }
        kk++;
    }
  
    strncpy(uptime, &text2[start_of_uptime], finish_of_uptime - start_of_uptime);
    
    //close(fd2);
    rcc = filp_close(f2, NULL);
    if (rcc == 0) {
        printk(KERN_INFO "Device successfully closed\n");
    }
	
    
    printk("uptime string is : %s\n",uptime);
    kstrtoull(uptime, 10, &uptime_long);
    printk("uptime long is: %llu\n",uptime_long);
    execution_time = uptime_long - (start_time_long / 100);
    printk("execution time in second is : %llu\n",execution_time);
    minutes = execution_time / 60;
    printk("execution time in minutes : %llu\n",minutes);
    return 0;
}
