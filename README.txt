修改CMakeList.txt文件的路径，GMP和OPENSSL的include和lib路径，包括输出路径。
然后在build文件夹Cmake ..命令，然后make命令即可。
运行时加GMP/gmp参数执行将调用自己写的代码，加OPENSSL/openssl调用OpenSSL库
什么都不加默认gmp
优化算法选择在control.h中宏定义

