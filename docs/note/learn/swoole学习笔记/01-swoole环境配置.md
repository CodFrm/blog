---
title: swoole学习笔记(一)-swoole环境配置(树莓派安装)
---

> 打算开始学习 swoole 了(原来好像弄过,不过那次只是接触了一下,并未太过深入,这次重新来过 (° ー °〃)
> swoole 虽然能在 windows 上搭建,不过我觉得意义不大....需要安装 CygWin 这和在 linux 上有什么区别呢 ┑(￣ Д ￣)┍,刚好现在手上有一台空闲的树莓派 zero,试试在上面搭建

## 编译 php

> 之所以要编译安装是因为在 swoole 编译的时候需要用到 phpize,apt-get 安装的时候没发现有

现在这个上面什么东西都没有,先安装 php,我选最新的 php7.2.6,zero 配置是真的好低....解压和编译 cpu 都 100%了很慢....趁这个时间去干点别的吧

下载,解压源码,安装依赖

强烈建议使用国内镜像....不然可能一些依赖 lib 按照失败,导致编译错误

```
sudo -i
wget http://hk1.php.net/get/php-7.2.6.tar.gz/from/this/mirror
mv mirro php.tar.gz
tar -zxvf php.tar.gz
apt-get update
apt-get install libxml2* libbz2-dev libjpeg-dev libmcrypt-dev libssl-dev openssl libxslt1-dev libxslt1.1 libcurl4-gnutls-dev libpq-dev build-essential git make
```

编译配置,复制的网上的 lnmp 编译- -...去掉了和 Nginx 有关的编译项,我只需要编译出 php 就行,不需要 Nginx 那些环境,当然如果你之前已经有了这些,这一部分就可以跳过了

```
cd php-7.2.6
 ./configure \
--prefix=/usr/local/php \
--exec-prefix=/usr/local/php \
--bindir=/usr/local/php/bin \
--sbindir=/usr/local/php/sbin \
--includedir=/usr/local/php/include \
--libdir=/usr/local/php/lib/php \
--mandir=/usr/local/php/php/man \
--with-config-file-path=/usr/local/php/etc \
--with-mysql-sock=/var/lib/mysql/mysql.sock \
--with-mcrypt=/usr/include \
--with-mhash \
--with-openssl \
--with-mysql=shared,mysqlnd \
--with-mysqli=shared,mysqlnd \
--with-pdo-mysql=shared,mysqlnd \
--with-gd \
--with-iconv \
--with-zlib \
--enable-zip \
--enable-inline-optimization \
--disable-debug \
--disable-rpath \
--enable-shared \
--enable-xml \
--enable-bcmath \
--enable-shmop \
--enable-sysvsem \
--enable-mbregex \
--enable-mbstring \
--enable-ftp \
--enable-gd-native-ttf \
--enable-pcntl \
--enable-sockets \
--with-xmlrpc \
--enable-soap \
--without-pear \
--with-gettext \
--enable-session \
--with-curl \
--with-freetype-dir \
--enable-opcache \
--enable-redis \
--enable-fpm \
--enable-fastcgi \
--disable-fileinfo
```

![](img/01-swoole%E7%8E%AF%E5%A2%83%E9%85%8D%E7%BD%AE.assets/TIM%E6%88%AA%E5%9B%BE20180619143040-300x207.png)

CPU 100% 有点怕,树莓派 zero 性能确实是弱...编译好慢....解决了编译配置的问题后就开始编译,我是真的睡了一觉(第二天)才起来 make install

```
make && make install
```

设置一下 php.ini 文件

```
cp php.ini-production /usr/local/php/etc/php.ini
//我输入php -v之后发现没反应,但是php确实是成功了,在/usr/local/php/bin里面./php -v也有反应,想到可能是没有链接到/usr/bin 目录里,用ln命令链接一下
ln -s /usr/local/php/bin/php /usr/bin/php
//链接phpize
ln -s /usr/local/php/bin/phpize /usr/bin/phpize
```

成功之后,老套路

```
php -v
```

![](img/01-swoole%E7%8E%AF%E5%A2%83%E9%85%8D%E7%BD%AE.assets/TIM%E6%88%AA%E5%9B%BE20180620085032-300x87.png)

成功,终于可以下一步了,进入 swoole 编译配置

## swoole 编译

从 git 上下载源码[https://github.com/swoole/swoole-src/releases](https://github.com/swoole/swoole-src/releases),开始编译

```
wget https://github.com/swoole/swoole-src/archive/v4.0.0.zip
unzip v4.0.0.zip
mv swoole-src-4.0.0/ swoole
cd swoole
phpize
```

这里我提示了一个错误...
Cannot find autoconf. Please check your autoconf installation and the
$PHP_AUTOCONF environment variable. Then, rerun this script.
解决办法:

```
apt-get install m4 autoconf
```

phpize 成功之后继续运行编译配置和开始编译(但愿这次不用那么久了...)

开启一些需要的:[编译配置项](https://wiki.swoole.com/wiki/page/437.html)

```
./configure --with-php-config=/usr/local/php/bin/php-config --enable-sockets --enable-swoole-debug --enable-openssl --enable-mysqlnd --enable-coroutine
make && make install
```

![](img/01-swoole%E7%8E%AF%E5%A2%83%E9%85%8D%E7%BD%AE.assets/TIM%E6%88%AA%E5%9B%BE20180620095954-300x207.png)

然后需要在 php.ini 中配置下

```
vi /usr/local/php/etc/php.ini
//添加
extension=swoole.so
```

然后`php -m`

![](img/01-swoole%E7%8E%AF%E5%A2%83%E9%85%8D%E7%BD%AE.assets/TIM%E6%88%AA%E5%9B%BE20180620113804-300x207.png)

有这一项就代表成啦~

## 测试

> 安装编译都完成之后,当然来试试是不是真的能用了

复制官方的例子,嘿嘿嘿~

```php
<?php
//创建websocket服务器对象，监听0.0.0.0:9502端口
$ws = new swoole_websocket_server("0.0.0.0", 9502);
//监听WebSocket连接打开事件
$ws->on('open', function ($ws, $request) {
    var_dump($request->fd, $request->get, $request->server);
    $ws->push($request->fd, "hello, welcome\n");
});
//监听WebSocket消息事件
$ws->on('message', function ($ws, $frame) {
    echo "Message: {$frame->data}\n";
    $ws->push($frame->fd, "server: {$frame->data}");
});
//监听WebSocket连接关闭事件
$ws->on('close', function ($ws, $fd) {
    echo "client-{$fd} is closed\n";
});
$ws->start();
```

`php swoole.php`
web:

```html
<script>
  var ws = new WebSocket("ws://localhost:9502");
  ws.onopen = function () {
    ws.send("send data");
  };

  ws.onmessage = function (evt) {
    var received_msg = evt.data;
    console.log(received_msg);
  };

  ws.onclose = function () {
    console.log("连接关闭");
  };
</script>
```

成了~

![](img/01-swoole%E7%8E%AF%E5%A2%83%E9%85%8D%E7%BD%AE.assets/TIM%E6%88%AA%E5%9B%BE20180620114807-300x207.png)

---

## 问题解决

### redis 扩展安装

> 弄完后...并没有用,然后重新编译一次成了....= =,不过还是记着

在 swoole 编译完成后,又遇到了一个问题....

```
php: symbol lookup error: /usr/local/php/lib/php/extensions/no-debug-non-zts-20170718/swoole.so: undefined symbol: swoole_redis_coro_init
```

查资料后发现可能是需要给 php 安装 redis 扩展....[redis 源码下载](https://pecl.php.net/package/redis)

```
wget https://pecl.php.net/get/redis-4.0.2.tgz
tar -zxvf redis-4.0.2.tgz
cd redis-4.0.2
phpize
./configure --with-php-config=/usr/local/php/bin/php-config
make && make install
```

然后在 php.ini 中加上`extension = redis.so`就好了,注意这个配置一定要放在 swoole 的配置的前面,因为这些扩展都是按照顺序加载的

---

**历时一天,终于搞定了 编译真的是漫长的过程=\_=**
