修改CMakeList.txt文件的路径，GMP和OPENSSL的include和lib路径，包括输出路径。
然后在build文件夹Cmake ..命令，然后make命令即可。
实现坐标转换，wNAF,Fixed-base,Burrett reduction,NIST Fast reduction,多点乘 优化算法。
运行时增加参数控制算法使用，OPENSSL/openssl使用openssl标准库，不加参数或GMP/gmp使用全部优化算法，加spy使用cache侧信道攻击（存在问题），可选参数wNAF,FixedBase禁用对应算法。BurrMod,FastMod使用对应算法。

