obj-m	+=	old_module.o
obj-m	+=	new_module.o
obj-m	+=	moduleparam.o
obj-m	+=	atomic.o
obj-m	+=	process.o
obj-m	+=	interrupt.o
obj-m	+=	manu_driver.o
obj-m	+=	auto_driver.o
obj-m	+=	auto.o
obj-m	+=	device_driver.o
obj-m	+=	demo.o
obj-m	+=	driver.o

all:
	make -C /lib/modules/`uname -r`/build M=`pwd` modules
clean:
	make -C /lib/modules/`uname -r`/build M=`pwd` clean
