hariboteos
==========

『30日でできる！ OS自作入門』川合 秀実氏(著)のLinux開発環境を整えることができます



## Develop a OS in linux according to the book


## prepare

QEMU is a generic and open source machine emulator and virtualizer.

When used as a machine emulator, QEMU can run OSes and programs made for one machine (e.g. an ARM board) on a different machine (e.g. your own PC). By using dynamic translation, it achieves very good performance.

When used as a virtualizer, QEMU achieves near native performances by executing the guest code directly on the host CPU. QEMU supports virtualization when executing under the Xen hypervisor or using the KVM kernel module in Linux. When using KVM, QEMU can virtualize x86, server and embedded PowerPC, and S390 guests.


     //opensuse
    zypper in qemu  
    
    // CentOS6.3
    wget http://wiki.qemu-project.org/download/qemu-1.7.0.tar.bz2

    tar xvf gemu-1.7.0.tar.bz2
    cd gemu-1.7.0
    ./configure
    cd
    make 
    make install


