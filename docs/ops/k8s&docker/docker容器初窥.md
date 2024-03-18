---
title: docker容器初窥
---

> 了解一下，了解一下- -！
> [官网](https://www.docker.com/) [英文文档](https://docs.docker.com/install/overview/) [中文文档](https://yeasy.gitbooks.io/docker_practice/)

## 安装

> 我吧我原来那台腾讯云 windows 的服务器换成了 centos 来尝试
> [参考教程](https://yeasy.gitbooks.io/docker_practice/install/centos.html)

```shell
# 安装docker-ce
## 依赖
sudo yum install -y yum-utils device-mapper-persistent-data lvm2
## 切换国内源
sudo yum-config-manager --add-repo https://mirrors.ustc.edu.cn/docker-ce/linux/centos/docker-ce.repo
## 安装
sudo yum makecache fast
sudo yum install docker-ce -y
##  开机启动和开启docker
sudo systemctl enable docker
sudo systemctl start docker
```

执行完上面的步骤安装就算完成了,执行一次`docker run hello-world`没有提示错误就是成功了

## 镜像

> 这获取镜像感觉和 git 的命令有些相似,我先注册了一个账号,以后再看怎么使用
> [镜像仓库](https://hub.docker.com/explore/)

### pull

使用 pull 获取镜像，我们可以在镜像仓库中找一个我们喜欢的镜像，然后拉去下来，之后运行

```shell
# 命令格式
docker pull [选项] [Docker Registry 地址[:端口号]/]仓库名[:标签]
# 这里我弄一个redis
docker pull redis
```

### 开启一个 redis 服务器

--name 是为这个容器命名，不然之后我们就只能用 UUID 来找了
-d 是以守护态运行，也就是后台运行，不然会直接的打印到我们终端上
我们可以用
docker logs test-redis 看运行日志
docker container ls 查看容器信息
docker attach test-redis 重新进入容器
docker container rm test-redis 删除容器

```shell
docker run --name test-redis -d redis
```

### 使用 cli 连接 redis

使用下面这个命令创建一个 redis 的容器 运行 redis-cli 这个命令
-i 使用交互式的操作
-t 打开一个终端
--rm 退出之后就删除这个容器
后面的-h 和-p 是主机(redis 就是主机名)和端口

```shell
docker run -it --rm redis redis-cli -h test-redis -p 6379
```

不过上面这个命令是不足够的 - -。。。运行之后并没有链接上我们之前创建的 redis 服务器，还需要加上一个 --link 的参数，使两个容器建立链接，然后我们就可以使用 redis 客户端了

```shell
docker run -it --link test-redis:redis --rm redis redis-cli -h test-redis
```

### 外部使用容器

我先给我的服务器装一个 redis-cli，等会儿好测试

```shell
yum install redis -y
```

在容器没有创建前，我们可以用`-p`来映射端口

```shell
# -P 随机分配一个49000~49900的端口
# 可以用docker container ls来查看
# -p可以指定分配，例如:
# -p 6379:6379 (映射所有地址到6379) -p 127.0.0.1:6300:6379 (映射到127.0.0.1上的6300端口)
# -p 127.0.0.1::6379(映射所有端口到ip上) -p 1813:1813/udp(指定udp端口)
# 之后可以使用docker port test-redis 查看映射情况
docker run --name test-redis -d -P redis
```

容器创建完成之后，我们可以用 iptables 来操作，不过这是很不好的，只是提一下可以使用这个方法

```shell
# 获取ip地址，我们也可以直接的用这个ip来连接
docker inspect test-redis | grep IPAddress
# iptables映射端口
iptables -t nat -A  DOCKER -p tcp --dport 6379 -j DNAT --to-destination 172.17.0.2:6379
```

推荐下面这个方法:
使用 commit 创建镜像，然后重新运行

```shell
# 创建了个my-redis的镜像
docker commit test-redis my-redis
# 我吧原来的停止删除，重新创建，这回用-p参数
docker stop test-redis
docker container rm test-redis
docker run --name test-redis -d -p 6379:6379 redis
```

完成，之后我们可以使用`redis-cli`直接访问了

```shell
[root@VM_92_235_centos ~]# redis-cli
127.0.0.1:6379> set haha qwe123
OK
127.0.0.1:6379> get haha
"qwe123"
127.0.0.1:6379> exit
[root@VM_92_235_centos ~]# docker run -it --link test-redis:redis --rm redis redis-cli -h test-redis
test-redis:6379> get haha
"qwe123"
test-redis:6379> exit
```

上面是我玩的，感觉还不错吧

## End

越用到后面越觉得容器的强大，最开始我想感觉有些占空间，又没什么用，用不到，所以用的云服务器学习，现在我想在我的电脑上装一个玩了。

还有很多内容，这一篇文章只是记录一些基本的操作，还有一个**Dockerfile**和 Makefile 类似，可以用来定制我们需要的镜像，这也是一个很重要的内容，后面再继续研究。
