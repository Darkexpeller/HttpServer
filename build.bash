
#!/bin/bash

# 定义项目名称和目录
project_name="Server"
project_dir="./$project_name"

for file in ./src/*;do
    git add ${file}
done
git add ./CMakeLists.txt
git add ./build.bash


# 创建build
mkdir -p build
cd ./build


cmake ..
cmake --build . 

echo "CMake project created and built in $project_dir"
./Server