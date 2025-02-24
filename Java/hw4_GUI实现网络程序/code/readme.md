# Java 聊天室应用

本项目是一个基于 Java 开发的线上聊天室应用，支持群聊和私聊功能，使用 MySQL 数据库存储用户信息 :)

## 项目结构
```
chat-room/
│
├── src/
│ ├── client/
│ │ ├── ChatClient.java # 客户端主程序
│ │ └── ChatMessage.java # 消息实体类
│ │
│ ├── server/
│ │ ├── ChatServer.java # 服务器主程序
│ │ └── ClientHandler.java # 客户端连接处理器
│ │
│ └── database/
│ └── DatabaseUtil.java # 数据库工具类
| └── create_database.sql # 建表sql命令
│
├── lib/
│ └── mysql-connector-java-.jar # MySQL JDBC驱动
│
├── compile.bat # 编译脚本
├── run-server.bat # 服务器启动脚本
└── run-client.bat # 客户端启动脚本
```

## 环境要求
- JDK 8 或更高版本
- MySQL 5.7 或更高版本
- Windows 操作系统

## 编译和运行
- 利用提供的sql建表
- 编译：`compile.bat`
- 启动服务器：`run-server.bat`
- 启动客户端：`run-client.bat`
