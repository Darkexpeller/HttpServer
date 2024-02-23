cmake_minimum_required(VERSION 3.15)


project(Server)
set(CMAKE_CXX_STANDARD 17)
set(PROJECT_SOURCE

    ./src/main.cpp

    ./src/is_error.h 
    ./src/is_error.cpp

    ./src/CreateTcpSocket.cpp
    ./src/CreateTcpSocket.h
    

    ./src/HttpMes.h
    ./src/HttpMes.cpp

    ./src/HttpResolution.cpp
    ./src/HttpResolution.h

)
add_executable(Server ${PROJECT_SOURCE})
####11111