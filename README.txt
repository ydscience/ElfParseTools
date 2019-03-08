 ElfParseTools
模仿readellf 解析软件对so文件进行解析
1.查看Elf文件头信息
  ElfParseTools -h  filePath;
  ElfParseTools -h "E:\C++ project\ElfParseTools\Release\libnative-lib.so"
 2.查看Elf程序头信息
  ElfParseTools -p filePath
  ElfParseTools -p "E:\C++ project\ElfParseTools\Release\libnative-lib.so"
 3.查看Elf节区头信息
  ElfParseTools -s filePath
  ElfParseTools -s "E:\C++ project\ElfParseTools\Release\libnative-lib.so"
  4.查看函数的偏移地址
   ELFParseTools -m  "E:\C++ project\ElfParseTools\Release\libnative-lib.so"
 5.增加节区信息
   ElfParseTools -a sectionName sectionSize filePath
   ElfParseTools -a "yuanding" 1000 "E:\C++ project\ElfParseTools\Release\libnative-lib.so"

 6.hook指定函数病插入执行代码
   ElfParseTools -f mathName newMathSaveOffset newMathOffset newMathSize  newProgramPath fileNamePath
