---
title: android 邀请链接,安装apk注册自动填写邀请人
---

> 今天又遇到一个 app 推广的问题,要求通过下载链接下载安装之后,注册的时候自动填写这个邀请链接的邀请人,但是又怎么吧这个邀请人的信息填入这个 apk 包呢?开始想着在文件的最后直接写入用户的 uid,结果破坏的 apk 的结构,后来发现 apk 其实就是一个 zip 包,通过网上的资料找到了思路,通过邀请链接下载的时候就修改这个 zip 包的注释将邀请者的 uid 填入

## ZIP 文件结构

随手一搜就可以找到 zip 的文件结构

**[官方文档](https://pkware.cachefly.net/webdocs/APPNOTE/APPNOTE-6.2.0.txt) [参考文章](https://blog.csdn.net/a200710716/article/details/51644421)** 这个文章后面还拿了一个简单的文件做讲解,挺不错的

其他的都不管他,我只想要注释那一段的结构,

### End of central directory record(EOCD) 目录结束标识

目录结束标识存在于整个归档包的结尾，用于标记压缩的目录数据的结束。每个压缩文件必须有且只有一个 EOCD 记录。

| Offset | Bytes | Description                                                                   | 翻译                          |
| ------ | ----- | ----------------------------------------------------------------------------- | ----------------------------- |
| 0      | 4     | End of central directory signature = 0x06054b50                               | 核心目录结束标记（0x06054b50  |
| 4      | 2     | Number of this disk                                                           | 当前磁盘编号                  |
| 6      | 2     | Number of the disk with the start of the central directory                    | 核心目录开始位置的磁盘编号    |
| 8      | 2     | total number of entries in the central directory on this disk                 | 该磁盘上所记录的核心目录数量  |
| 10     | 2     | total number of entries in the central directory                              | 核心目录结构总数              |
| 12     | 2     | size of central directory (bytes)                                             | 核心目录的大小                |
| 16     | 2     | offset of start of central directory with respect to the starting disk number | 核心相对于 archive 开始的位移 |
| 20     | 2     | .ZIP file comment length(n)                                                   | 注释长度                      |
| 22     | n     | .ZIP Comment                                                                  | 注释内容                      |

### 先用易语言测试一下

先打包一个文件

![](img/android-%E9%82%80%E8%AF%B7%E9%93%BE%E6%8E%A5.assets/TIM%E6%88%AA%E5%9B%BE20180401230107-300x194.png)

随手一读

```
.版本 2
.支持库 spec
.局部变量 data, 字节集
data ＝ 读入文件 (“test.zip”)
调试输出 (取字节集中间 (data, 寻找字节集 (data, { 80, 75, 5, 6 }, ), 50))
调试输出 (到字节集 (“test123”))
```

读取出来的数据中确实有 test123
然后看看怎么写注释进去,这回读取 apk 的

```
//80,75,5,6,0,0,0,0,18,0,18,0,18,5,0,0,55,14,17,0,0,0,60,105,110,118,62,50,60,47,105,110,118,62
//读出来这玩意,先理清楚结构
//80,75,5,6   EOCD
//0,0           磁盘编号
//0,0           开始位置的磁盘编号
//18,0          磁盘上所记录的核心目录数量
//18,0          核心目录结构总数
//18,5,0,0      核心目录的大小
//55,14,17,0    核心目录开始位置相对于archive开始的位移
//0,0           注释长度
//0,60,105,110,118,62,50,60,47,105,110,118,62未知内容,不管了,我们只需要修改注释

.版本 2

.局部变量 data, 字节集
.局部变量 tmpByte, 字节集
.局部变量 pos, 整数型

data ＝ 读入文件 (“test.apk”)
pos ＝ 寻找字节集 (data, { 80, 75, 5, 6 }, )
data [pos ＋ 20] ＝ 8
tmpByte ＝ 到字节集 (“12345678”) ＋ { 0 } ＋ 取字节集右边 (data, 取字节集长度 (data) － pos － 21)
data ＝ 取字节集左边 (data, 寻找字节集 (data, { 80, 75, 5, 6 }, ) ＋ 21)
data ＝ data ＋ tmpByte
写到文件 (“ddd.apk”, data)


成功写入压缩包,看看能不能在手机上安装
成功可以安装使用
```

### PHP 下载代码

php 的话就和前面一下,修改注释内容,直接贴 php 的代码了
php 的文件操作就是文本操作....十六进制要用双引号\0xff 表示

```php
    public function download_apk($inv = 1) {
		if (!userModel::existUser($inv)) {//判断用户是否存在
			$inv = 0;
		}
		$invStr = '<inv>' . $inv . '</inv>';//设置注释
		$data = file_get_contents('app/kana.apk');//读取apk源数据
		$pos = strpos($data, "\x50\x4b\x05\x06");
		//搜索标志位置(一般都是文件尾部,不考虑了)
		$data = substr($data, 0, $pos + 20);
		//取出标志+20左边,等下直接从注释长度那块合成
		$dec = dechex(strlen($invStr));
		//将长度转为十六进制,后面再将十六进制转为byte,只考虑一位,反正ff,255个长度也够了
		$data .= hex2bin(strlen($dec) <= 1 ? ('0' . $dec) : $dec) . "\x00$invStr";//合成注释
		header("Content-Type: application/octet-stream");
		header("Accept-Ranges: bytes");
		header("Accept-Length: " . strlen($data));
		header("Content-Disposition: attachment; filename=kana.apk");
		echo $data;//输出文件数据
    }
```

### android 读取

暂时空着,android 又不是我写 ┓( ´∀` )┏,贴一下思路吧
读取本地安装的 apk 包数据,然后在里面搜索这个注释(怎么感觉和没讲一样)
总之很简单的
