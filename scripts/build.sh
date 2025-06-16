#!/bin/bash

# 聊天室系统构建脚本

echo "开始构建聊天室管理系统..."

# 检查Qt环境
if ! command -v qmake &> /dev/null; then
    echo "错误: 未找到qmake命令，请确保Qt开发环境已正确安装"
    echo "可以使用以下命令安装Qt:"
    echo "brew install qt"
    exit 1
fi

# 清理旧的构建文件
echo "清理旧的构建文件..."
make clean 2>/dev/null || true
rm -f Makefile
rm -f ChatRoom
rm -rf *.o
rm -rf moc_*
rm -rf ui_*

# 生成Makefile
echo "生成Makefile..."
qmake ChatRoom.pro

if [ $? -ne 0 ]; then
    echo "错误: qmake执行失败"
    exit 1
fi

# 编译项目
echo "编译项目..."
make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

if [ $? -eq 0 ]; then
    echo "构建成功！"
    echo "可执行文件: ./ChatRoom"
    echo ""
    echo "使用方法:"
    echo "1. 运行 ./ChatRoom"
    echo "2. 注册新用户或使用现有用户登录"
    echo "3. 如提示连接服务器失败，点击菜单栏'服务器' -> '启动服务器'"
    echo "4. 可以启动多个实例来测试多用户聊天"
else
    echo "构建失败！请检查错误信息"
    exit 1
fi
