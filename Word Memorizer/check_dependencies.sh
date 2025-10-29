#!/bin/bash

echo "检查系统依赖..."

# 检查基本构建工具
check_tool() {
    if command -v $1 &> /dev/null; then
        echo "✓ $1 已安装"
        return 0
    else
        echo "✗ $1 未安装"
        return 1
    fi
}

# 检查 pkg-config 包
check_pkg() {
    if pkg-config --exists $1; then
        version=$(pkg-config --modversion $1)
        echo "✓ $1 已安装 (版本: $version)"
        return 0
    else
        echo "✗ $1 未安装"
        return 1
    fi
}

echo ""
echo "基本构建工具:"
check_tool g++
check_tool cmake
check_tool make
check_tool pkg-config

echo ""
echo "GTKMM 相关依赖:"
check_pkg gtkmm-3.0
check_pkg sigc++-2.0
check_pkg glibmm-2.4
check_pkg giomm-2.4
check_pkg pangomm-1.4
check_pkg cairomm-1.0

echo ""
echo "如果需要安装缺失的依赖，请运行:"
echo "sudo apt update && sudo apt install build-essential cmake pkg-config libgtkmm-3.0-dev"
