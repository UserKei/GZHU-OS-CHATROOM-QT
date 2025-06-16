#!/bin/bash

# 快速启动脚本 - 适应新的项目结构

echo "🚀 聊天室管理系统 - 优化版"
echo "=========================="
echo ""

# 检查构建是否存在
if [ ! -d "build/bin/ChatRoom.app" ]; then
    echo "📦 未找到构建文件，正在自动构建..."
    ./scripts/build_cmake.sh
    
    if [ $? -ne 0 ]; then
        echo "❌ 构建失败，请检查错误信息"
        exit 1
    fi
    echo ""
fi

# 启动应用程序
echo "🎯 正在启动聊天室应用程序..."
open build/bin/ChatRoom.app

echo "✅ 应用程序已启动！"
echo ""
echo "📋 新项目结构特点："
echo "  • 模块化设计：core、network、ui 分离"
echo "  • 更清晰的文件组织"
echo "  • 便于维护和扩展"
echo ""
echo "🎮 使用指南："
echo "  1. 注册新用户或登录现有用户"
echo "  2. 点击'服务器'菜单启动内置服务器"
echo "  3. 可以启动多个实例测试多用户聊天"
echo "  4. 支持消息过滤、权限管理等高级功能"
