修改CMakeList.txt文件的路径，GMP和OPENSSL的include和lib路径，包括输出路径。
然后在build文件夹Cmake ..命令，然后make命令即可。
实现坐标转换，wNAF,Fixed-base,Burrett reduction,NIST Fast reduction优化算法。
运行时增加参数控制算法使用，OPENSSL/openssl使用openssl标准库，不加参数或GMP/gmp使用全部优化算法，可选参数wNAF,FixedBase,BurrMod,FastMod禁用对应算法。

