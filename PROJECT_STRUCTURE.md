# 聊天室系统 - 项目结构说明

## 📁 优化后的项目结构

```
ChatRoom/
├── CMakeLists.txt              # CMake构建配置
├── ChatRoom.pro               # Qt项目文件（备用）
├── build_cmake.sh.in          # CMake脚本模板
│
├── src/                       # 源代码目录
│   ├── main.cpp              # 程序入口点
│   │
│   ├── core/                 # 核心业务逻辑
│   │   ├── database.cpp      # 数据库管理
│   │   ├── database.h        
│   │   ├── userinfo.cpp      # 用户信息类
│   │   ├── userinfo.h        
│   │   ├── messagefilter.cpp # 消息过滤器
│   │   └── messagefilter.h   
│   │
│   ├── network/              # 网络通信模块
│   │   ├── chatclient.cpp    # 聊天客户端
│   │   ├── chatclient.h      
│   │   ├── chatserver.cpp    # 聊天服务器
│   │   └── chatserver.h      
│   │
│   └── ui/                   # 用户界面模块
│       ├── windows/          # 主要窗口
│       │   ├── loginwindow.cpp    # 登录窗口
│       │   ├── loginwindow.h      
│       │   ├── mainwindow.cpp     # 主界面窗口
│       │   └── mainwindow.h       
│       │
│       └── dialogs/          # 对话框
│           ├── registerdialog.cpp      # 注册对话框
│           ├── registerdialog.h        
│           ├── usermanagerdialog.cpp   # 用户管理对话框
│           ├── usermanagerdialog.h     
│           ├── settingsdialog.cpp      # 设置对话框
│           └── settingsdialog.h        
│
├── include/                   # 头文件目录（预留）
│   ├── core/                 # 核心模块头文件
│   ├── network/              # 网络模块头文件
│   └── ui/                   # UI模块头文件
│
├── docs/                     # 文档目录
│   ├── README.md             # 项目说明文档
│   └── PROJECT_SUMMARY.md    # 项目总结
│
├── scripts/                  # 脚本目录
│   ├── build_cmake.sh        # CMake构建脚本
│   ├── build.sh             # QMake构建脚本
│   ├── run.sh               # 程序启动脚本
│   └── test.sh              # 测试脚本
│
├── resources/                # 资源文件目录（预留）
│   └── (图标、样式等资源文件)
│
└── build/                    # 构建输出目录
    ├── bin/                  # 可执行文件
    └── (其他构建生成文件)
```

## 🎯 模块化设计优势

### 1. **清晰的职责分离**
- **core/**: 核心业务逻辑，不依赖UI
- **network/**: 网络通信，独立的网络层
- **ui/**: 用户界面，分为窗口和对话框

### 2. **易于维护**
- 相关功能文件组织在一起
- 减少文件查找时间
- 便于团队协作开发

### 3. **便于扩展**
- 新功能可以独立添加到对应模块
- 模块间依赖关系清晰
- 支持插件化扩展

### 4. **构建优化**
- CMake配置更加清晰
- 支持模块化编译
- 便于设置不同的编译选项

## 📋 各模块说明

### Core模块 (src/core/)
- **database**: SQLite数据库操作，用户管理，消息存储
- **userinfo**: 用户信息数据结构
- **messagefilter**: 敏感词过滤功能

### Network模块 (src/network/)
- **chatclient**: TCP客户端，处理与服务器的通信
- **chatserver**: TCP服务器，处理多客户端连接和消息转发

### UI模块 (src/ui/)
- **windows/**: 主要应用程序窗口
  - **loginwindow**: 用户登录界面
  - **mainwindow**: 主聊天界面
- **dialogs/**: 各种功能对话框
  - **registerdialog**: 用户注册
  - **usermanagerdialog**: 用户权限管理
  - **settingsdialog**: 应用程序设置

### Scripts模块 (scripts/)
- **build_cmake.sh**: 使用CMake构建项目
- **run.sh**: 启动应用程序
- **test.sh**: 功能测试指南

### Docs模块 (docs/)
- **README.md**: 完整的项目文档
- **PROJECT_SUMMARY.md**: 项目总结和技术说明

## 🚀 构建命令

```bash
# 使用优化后的构建脚本
./scripts/build_cmake.sh

# 启动程序
./scripts/run.sh

# 查看测试指南
./scripts/test.sh
```

## 🔧 开发指南

### 添加新功能
1. 确定功能属于哪个模块
2. 在对应的src/目录下创建文件
3. 更新CMakeLists.txt中的源文件列表
4. 添加必要的头文件包含

### 模块依赖规则
- **UI模块** 可以依赖 **Core** 和 **Network** 模块
- **Network模块** 可以依赖 **Core** 模块
- **Core模块** 应该保持独立，不依赖其他模块

这种结构化的组织方式使得项目更加专业和易于维护！
