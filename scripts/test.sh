#!/bin/bash

# 聊天室测试脚本

echo "聊天室功能测试指南"
echo "===================="
echo ""

echo "修复内容："
echo "- 现在发送消息后立即在本地显示"
echo "- 无需等待服务器回显"
echo "- 离线状态下也能看到自己发送的消息"
echo ""

echo "测试步骤："
echo "1. 启动程序: ./run.sh"
echo "2. 登录用户（如kei）"
echo "3. 在消息框输入测试消息，按回车或点击发送"
echo "4. 应该立即看到自己的消息显示在聊天区域"
echo ""

echo "服务器测试："
echo "1. 点击菜单栏'服务器' -> '启动服务器'"
echo "2. 启动第二个程序实例测试多用户聊天"
echo "3. 两个用户可以相互发送消息"
echo ""

echo "功能特性："
echo "✅ 立即显示自己发送的消息"
echo "✅ 离线状态下也能发送消息（标记为离线）"
echo "✅ 避免重复显示消息"
echo "✅ 支持公开聊天和私聊"
echo "✅ 消息时间戳显示"
echo ""

echo "启动程序进行测试？(y/n)"
read -r response
if [[ "$response" == "y" || "$response" == "Y" ]]; then
    echo "启动聊天室..."
    ./run.sh
else
    echo "可以手动运行 ./run.sh 启动程序"
fi
