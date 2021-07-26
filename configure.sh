#!/bin/bash
source Definitions.mk

for file in $(find . -regex ".*\.sh");do
  echo "allow $file to be executed" 
  chmod +x $file
done

for dataFile in $cosmoDat;do
  echo "format $dataFile" 
  bash src/FormatFile.sh $dataFile
done

# ---------these are needed for python and c++---------------- #
echo "make lib directory"
mkdir "lib" 2> /dev/null

echo "make exec directory"
mkdir "exec" 2> /dev/null

echo "make src/misc_dir directory"
mkdir "src/misc_dir" 2> /dev/null

PathHead=src/misc_dir/path.hpp
PathHeadPy=src/misc_dir/path.py
PathTypePy=src/misc_dir/type.py

echo "write type in $PathTypePy"
echo "from ctypes import c_"$LONGpy"double as cdouble" > $PathTypePy

echo "define _PATH_=$PWD in $PathHeadPy"
echo "_PATH_=\"$PWD\" "> $PathHeadPy

echo "write paths in $PathHead"

echo "#ifndef PATHS_HEAD
#define PATHS_HEAD

#define cosmo_PATH \"$PWD/$cosmoDat\" 
#define PWD \"$PWD\" 

#endif
">$PathHead




echo "Run configure.sh License in order to read the License, or read the file named LICENSE."
if test "$1" = "License" 
then 
  echo -e "License:"
  cat LICENSE

  echo ""
  echo ""
  echo ""
fi

echo  -e "\033[1;5;35m 
  _   _  _____  _____             
 | \ | |/ ____|/ ____|            
 |  \| | (___ | |     _ __  _ __  
 | .   |\___ \| |    | '_ \| '_ \ 
 | |\  |____) | |____| |_) | |_) |
 |_| \_|_____/ \_____| .__/| .__/ 
                     | |   | |    
                     |_|   |_|    
"


echo  -e "\033[0;97m 
You can run \"make\" to compile everything. After that, you will find several examples in UserSpace, 
and you can run the executables in exec/ in order to see if the code actually works.
"
