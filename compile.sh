#!/bin/bash

# 设置颜色输出
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${YELLOW}开始清理和重新构建项目...${NC}"

# 检查是否在项目根目录
if [ ! -f "CMakeLists.txt" ]; then
    echo -e "${RED}错误: 请在项目根目录下运行此脚本${NC}"
    exit 1
fi

# 清理build目录
echo -e "${YELLOW}清理build目录...${NC}"
if [ -d "build" ]; then
    rm -rf build
    echo -e "${GREEN}build目录已清理${NC}"
else
    echo -e "${YELLOW}build目录不存在，跳过清理${NC}"
fi

# 清理bin目录
echo -e "${YELLOW}清理bin目录...${NC}"
if [ -d "bin" ]; then
    rm -rf bin
    echo -e "${GREEN}bin目录已清理${NC}"
else
    echo -e "${YELLOW}bin目录不存在，跳过清理${NC}"
fi

# 创建build目录
echo -e "${YELLOW}创建build目录...${NC}"
mkdir -p build
cd build

# 运行cmake配置
echo -e "${YELLOW}运行cmake配置...${NC}"
if cmake ..; then
    echo -e "${GREEN}cmake配置成功${NC}"
else
    echo -e "${RED}cmake配置失败${NC}"
    exit 1
fi

# 编译项目
echo -e "${YELLOW}开始编译...${NC}"
if make -j$(nproc); then
    echo -e "${GREEN}编译成功！${NC}"
    echo -e "${GREEN}可执行文件位于: bin/ 目录${NC}"
    
    # 显示生成的可执行文件
    if [ -d "../bin" ]; then
        echo -e "${YELLOW}生成的可执行文件:${NC}"
        ls -la ../bin/
    fi
else
    echo -e "${RED}编译失败${NC}"
    exit 1
fi

echo -e "${GREEN}构建完成！${NC}" 