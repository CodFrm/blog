---
title: Dockerfile和docker-compose
---

## Dockerfile

> 使用 Dockerfile 构建自己的镜像,这里记录一些常用的命令,这里拿我尝试做的一个 nginx 镜像来记录
> [https://github.com/huanl-php/docker-nginx/blob/master/Dockerfile](https://github.com/huanl-php/docker-nginx/blob/master/Dockerfile)

### FROM alpine:latest

选择基础的镜像,这里的话我用的是 alpine,后面的 latest 表示最新的版本,alpine 是一个非常小的 linux 发行版,如果你对镜像大小很敏感的话推荐使用这个镜像作为基础镜像

### LABEL

这一行表示这个 Dockerfile 的作者

### ENV

环境变量,当你没配置的时候将使用这些作为默认值

### EXPOSE

声明要暴露的端口，但是并不会直接帮你暴露出去，至少告诉 docker 你会暴露那些

### RUN

运行命令,这里用了&和\表示连接命令和换行

### COPY

COPY 顾名思义的复制文件 从本地复制到容器里面去

### CMD

当你容器启动的时候，将运行的命令，除此之外还有**ENTRYPOINT**

暂时就先记录这些，以后用到了再继续记录

### 另外提一下

使用 alpine 这个的时候有下面这个方法,安装包,卸载包，但是当时我构建我的 nginx 项目的时候 del 之后提示一些包不存在，时间久远我也不记得当时的操作了

```shell
apk add --no-cache --virtual .build-deps gcc
apk del .build-deps
```

然后我尝试了这种方法，只删除构建时需要的包，之后我发现我的镜像更小了= =不知道这样做会有什么不好。。。。

```shell
apk add --no-cache gcc
apk del gcc
```

## docker-compose.xml

> compose 用于容器快速部署，我这里用我的云签到的项目来
> [https://github.com/CodFrm/cas/blob/master/docker-compose.yml](https://github.com/CodFrm/cas/blob/master/docker-compose.yml) > [文档](https://docs.docker.com/compose/compose-file/)

### version

要求版本，不多说了

### services

容器服务

### db 和 web

都只是一个名字而已

### image

容器的镜像名字

### environment

环境变量

### networks

所属网络环境

### ports

暴露端口 外部端口:容器内部端口

### depends_on

依赖，会先构建所依赖的容器

### 根部的 networks

声明网络，没有将会创建

### cas_network

网络的名字

### driver

网络驱动类型，我这里是 bridge 桥接

## 最近用到的

### build

build Dockerfile 路径，直接通过 dockerfile 来构建了，不用 image

### volumes

和本地路径绑定 本地路径:容器内部路径

### external_links

将外部容器加入网络 链接的容器名字:本容器内叫的名字

### external

networks 下的一个配置项,允许外部访问 external:true

先记录到这里，解释是自己的简单理解\>\_\<
