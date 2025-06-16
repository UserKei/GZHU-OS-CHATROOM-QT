#!/bin/bash

# 历史消息功能测试脚本

echo "🔍 聊天室历史消息功能测试"
echo "========================"
echo ""

echo "🛠️ 修复内容："
echo "  ✅ 创建了ChatMessage结构体存储完整消息信息"
echo "  ✅ 修改数据库查询返回时间戳信息"
echo "  ✅ 优化历史消息显示格式"
echo "  ✅ 修复重新登录后看不到历史消息的问题"
echo ""

echo "🧪 测试步骤："
echo "  1. 启动程序并登录用户A"
echo "  2. 启动服务器"
echo "  3. 发送几条测试消息"
echo "  4. 关闭程序"
echo "  5. 重新启动程序并登录同一用户"
echo "  6. 检查是否能看到之前的历史消息"
echo ""

echo "📋 预期结果："
echo "  • 重新登录后应该看到历史消息"
echo "  • 历史消息应该有正确的时间戳"
echo "  • 历史消息区域有明确的标识"
echo ""

echo "🚀 开始测试？(y/n)"
read -r response
if [[ "$response" == "y" || "$response" == "Y" ]]; then
    echo "正在启动聊天室进行测试..."
    cd ..
    ./start.sh
else
    echo "可以手动运行 ./start.sh 进行测试"
fi
