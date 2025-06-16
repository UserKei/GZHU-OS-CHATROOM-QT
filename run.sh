#!/bin/bash

# 聊天室系统启动脚本

echo "启动聊天室管理系统..."

# 检查构建是否存在
if [ ! -d "build/bin/ChatRoom.app" ]; then
    echo "错误: 未找到构建文件，请先运行构建命令:"
    echo "./build_cmake.sh"
    exit 1
fi

# 启动应用程序
echo "正在启动应用程序..."
open build/bin/ChatRoom.app

echo "应用程序已启动！"
echo ""
echo "使用说明："
echo "1. 首次使用请先注册用户"
echo "2. 登录后如果提示连接服务器失败，请点击菜单栏'服务器' -> '启动服务器'"
echo "3. 可以启动多个实例来测试多用户聊天功能"
echo "4. 支持公开聊天和私聊功能"
echo "5. 可以通过'用户管理'设置权限和敏感词过滤"
