
#!/bin/bash

# 定义项目名称和目录
project_name="Server"
project_dir="./$project_name"

# 创建build
mkdir -p build
cd ./build


cmake ..
cmake --build . 

echo "CMake project created and built in $project_dir"
./Server