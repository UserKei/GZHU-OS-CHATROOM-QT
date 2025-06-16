#!/bin/bash

# 数据库位置检查脚本

echo "🔍 聊天室数据库位置检查"
echo "======================"
echo ""

echo "📂 数据库存储位置："
echo "应用使用Qt的QStandardPaths::AppDataLocation来存储数据"
echo ""

# 在macOS上，应用数据通常存储在：
MAC_APP_DATA="$HOME/Library/Application Support/ChatRoom System"
MAC_DB_PATH="$MAC_APP_DATA/ChatRoom/chatroom.db"
echo "🍎 macOS 预期位置: $MAC_APP_DATA"
echo "📄 数据库文件: $MAC_DB_PATH"

if [ -d "$MAC_APP_DATA" ]; then
    echo "✅ 找到应用数据目录"
    echo "📁 目录内容:"
    ls -la "$MAC_APP_DATA"
    
    if [ -f "$MAC_DB_PATH" ]; then
        echo ""
        echo "✅ 找到数据库文件: $MAC_DB_PATH"
        echo "📊 文件大小: $(ls -lh "$MAC_DB_PATH" | awk '{print $5}')"
        echo "🕒 修改时间: $(ls -lh "$MAC_DB_PATH" | awk '{print $6" "$7" "$8}')"
        
        echo ""
        echo "� 数据库内容:"
        echo "用户数量: $(sqlite3 "$MAC_DB_PATH" "SELECT COUNT(*) FROM users;")"
        echo "消息数量: $(sqlite3 "$MAC_DB_PATH" "SELECT COUNT(*) FROM messages;")"
        echo "敏感词数量: $(sqlite3 "$MAC_DB_PATH" "SELECT COUNT(*) FROM sensitive_words;")"
        
        echo ""
        echo "📝 最近的消息:"
        sqlite3 "$MAC_DB_PATH" "SELECT sender, message, timestamp FROM messages ORDER BY timestamp DESC LIMIT 5;" | while read line; do
            echo "  $line"
        done
        
        echo ""
        echo "🔧 数据库检查工具:"
        echo "查看所有消息: sqlite3 \"$MAC_DB_PATH\" \"SELECT * FROM messages;\""
        echo "查看用户: sqlite3 \"$MAC_DB_PATH\" \"SELECT username, email FROM users;\""
    else
        echo "❌ 未找到数据库文件 $MAC_DB_PATH"
    fi
else
    echo "❌ 未找到应用数据目录"
    echo ""
    echo "🔍 搜索可能的位置:"
    find ~/Library -name "chatroom.db" 2>/dev/null || echo "未找到数据库文件"
fi

echo ""
echo "🛠️ 调试信息:"
echo "如果数据库不存在，可能的原因："
echo "1. 程序还没有运行过"
echo "2. 程序运行时出现错误"
echo "3. 数据库创建失败"
echo ""
echo "💡 建议操作:"
echo "1. 运行程序并发送一些消息"
echo "2. 重新运行此脚本检查数据库"
echo "3. 如果问题持续，检查程序日志"
