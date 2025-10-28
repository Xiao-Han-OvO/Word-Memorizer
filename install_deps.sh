#!/bin/bash

echo "安装单词背诵工具所需依赖..."

# 更新包列表
sudo apt update

# 安装基本构建工具
sudo apt install -y \
    build-essential \
    cmake \
    make \
    pkg-config

# 安装 GTKMM 开发包
sudo apt install -y \
    libgtkmm-3.0-dev \
    libglibmm-2.4-dev \
    libgiomm-2.4-dev \
    libpangomm-1.4-dev \
    libcairomm-1.0-dev \
    libsigc++-2.0-dev

# 安装运行时依赖（如果需要）
sudo apt install -y \
    libgtk-3-0 \
    libglibmm-2.4-1v5 \
    libgiomm-2.4-1v5 \
    libpangomm-1.4-1v5 \
    libcairomm-1.0-1v5 \
    libsigc++-2.0-0v5

echo ""
echo "依赖安装完成!"
echo "现在可以运行 ./build.sh 来构建程序"
