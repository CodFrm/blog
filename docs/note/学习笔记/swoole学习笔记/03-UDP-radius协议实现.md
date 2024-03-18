---
title: swoole学习笔记(三)-UDP radius协议实现
---

> 又开新坑-swoole 作为 radius 服务器,huanlphp 写业务 [php-radius](https://github.com/CodFrm/php-radius "php-radius") 主要还是为了学习 swoole 和实验我的框架,所以这里记录一下 radius 协议的结构和使用(原来用 python 实现过一次,容易崩溃还写得垃圾),文章中只写了 auth,没有写 account 的记录,openvpn 需要 auth 和 account 才能实现连接成功,可以看我完整的实现[python 实现](https://github.com/CodFrm/stuShare/blob/master/radius/main.py)

## 协议&工具

- [rfc2865 radius 身份认证](https://tools.ietf.org/html/rfc2865)
- [rfc2866 radius 计费](https://tools.ietf.org/html/rfc2866)

测试工具我用的 NTRadPing:[http://www.winsite.com/internet/server-tools/ntradping/](http://www.winsite.com/internet/server-tools/ntradping/)

![](img/03-UDP-radius%E5%8D%8F%E8%AE%AE%E5%AE%9E%E7%8E%B0.assets/TIM%E6%88%AA%E5%9B%BE20180622220548-300x188.png)

## 结构

### pack fromat

> 关于计费和验证的格式都是一样的

验证:[https://tools.ietf.org/html/rfc2865#page-13](https://tools.ietf.org/html/rfc2865#page-13)

计费:[https://tools.ietf.org/html/rfc2866#page-5](https://tools.ietf.org/html/rfc2866#page-5)

```
    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     Code      |  Identifier   |            Length             |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                                                               |
   |                         Authenticator                         |
   |                                                               |
   |                                                               |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |  Attributes ...
   +-+-+-+-+-+-+-+-+-+-+-+-+-
```

看得有些隐晦,我将它转换成 c 的 struct 结构吧
char 1 个字节,8bit short 两个字节,16bit

```c
struct radius_format{
	char code;//包类型,请求类型根据这个判断
    char identifier;//鉴定码,服务器和客户端这个值需要一样
    short lenght; //本次数据包的总长度(20-4096)
    char authenticator[16];//数据hash,用来校验数据是否正确,一个数据的md5码
};
```

在响应的时候 authenticator 的计算方式为:MD5(Code+ID+Length+RequestAuth+Attributes+**Secret**) 在计算中的**RequestAuth**为请求的时候的 authenticator,**Secret**记笔记,是双方的一个 key

后面的 Attributes 长度不一定(length-20),里面包含着 **用户/NAS** 的一些信息,比如账号,密码,ip,之类的,格式看后面,这里也可以用来扩展自己的协议,带上自己的一些奇奇怪怪的值- -

#### code 意义

主要就是前面 5 个了

```
      RADIUS Codes (decimal) are assigned as follows:

        1       Access-Request   //验证请求,一般由客户端使用
        2       Access-Accept    //验证通过,一般由服务端处理返回
        3       Access-Reject    //验证拒绝,一般由服务端处理返回
        4       Accounting-Request    //计费请求,客户端发起
        5       Accounting-Response   //计费返回,服务器发起
       11       Access-Challeng  //服务端主动请求再次验证,客户端要求必须返回
       12       Status-Server (experimental)//服务器状态???没看到说明
       13       Status-Client (experimental)//同上
      255       Reserved//保留
```

### Attributes

[https://tools.ietf.org/html/rfc2865#page-22](https://tools.ietf.org/html/rfc2865#page-22)

```
    0                   1                   2
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
   |     Type      |    Length     |  Value ...
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
```

```c
struct radius_attr{
	char type;//类型,有不少...
	char length;//type+length+value的长度
}
```

value 是不固定的(长度:1-253),根据 length 来判定(就是 length-2 咯)

#### type

> 偷懒了,不写意义了

验证属性:[https://tools.ietf.org/html/rfc2865#page-24](https://tools.ietf.org/html/rfc2865#page-24)

计费属性:[https://tools.ietf.org/html/rfc2866#page-11](https://tools.ietf.org/html/rfc2866#page-11)

```
      This specification concerns the following values:

          1      User-Name          //登录用户名
          2      User-Password      //登录密码
          3      CHAP-Password
          4      NAS-IP-Address     //NAS的IP地址
          5      NAS-Port           //NAS的端口
          6      Service-Type
          7      Framed-Protocol
          8      Framed-IP-Address
          9      Framed-IP-Netmask
         10      Framed-Routing
         11      Filter-Id
         12      Framed-MTU
         13      Framed-Compression
         14      Login-IP-Host
         15      Login-Service
         16      Login-TCP-Port
         17      (unassigned)
         18      Reply-Message
         19      Callback-Number
         20      Callback-Id
         21      (unassigned)
         22      Framed-Route
         23      Framed-IPX-Network
         24      State
         25      Class
         26      Vendor-Specific
         27      Session-Timeout
         28      Idle-Timeout
         29      Termination-Action
         30      Called-Station-Id
         31      Calling-Station-Id
         32      NAS-Identifier
         33      Proxy-State
         34      Login-LAT-Service
         35      Login-LAT-Node
         36      Login-LAT-Group
         37      Framed-AppleTalk-Link
         38      Framed-AppleTalk-Network
         39      Framed-AppleTalk-Zone
         40-59   (reserved for accounting)//计费的,包含流量信息和连接时间什么的
         60      CHAP-Challenge
         61      NAS-Port-Type
         62      Port-Limit
         63      Login-LAT-Port
```

## 实现

> 利用 NTR 工具发送测试数据,使用 php-swoole 来接收和处理

### 解包

php 中使用 unpack 来解包,还是很方便的

```php
    public function onPacket(swoole_server $serv, string $data, array $clientInfo) {
        //解包
        $radius = unpack('ccode/cidentifier/nlength/a16authenticator', $data);
        print_r($radius);
        echo bin2hex($radius['authenticator']) . "\n";
        echo bin2hex($data);
    }

```

![](img/03-UDP-radius%E5%8D%8F%E8%AE%AE%E5%AE%9E%E7%8E%B0.assets/TIM%E6%88%AA%E5%9B%BE20180623221807-300x101.png)

后面还需要处理 attr 属性,这是我解码的代码

```php
<?php

class radius {
    /**
     * @var swoole_server
     */
    public $server;

    /**
     * 密钥
     * @var string
     */
    public $secret_key;

    public static $ATTR_TYPE = [1 => 'User-Name',
        2 => 'User-Password', 3 => 'CHAP-Password', 4 => 'NAS-IP-Address', 5 => 'NAS-Port', 6 => 'Service-Type',
        7 => 'Framed-Protocol', 8 => 'Framed-IP-Address', 9 => 'Framed-IP-Netmask', 10 => 'Framed-Routing',
        11 => 'Filter-Id', 12 => 'Framed-MTU', 13 => 'Framed-Compression', 14 => 'Login-IP-Host', 15 => 'Login-Service',
        16 => 'Login-TCP-Port', 17 => '(unassigned)', 18 => 'Reply-Message', 19 => 'Callback-Number',
        20 => 'Callback-Id', 21 => '(unassigned)', 22 => 'Framed-Route', 23 => 'Framed-IPX-Network', 24 => 'State',
        25 => 'Class', 26 => 'Vendor-Specific', 27 => 'Session-Timeout', 28 => 'Idle-Timeout', 29 => 'Termination-Action',
        30 => 'Called-Station-Id', 31 => 'Calling-Station-Id', 32 => 'NAS-Identifier', 33 => 'Proxy-State',
        34 => 'Login-LAT-Service', 35 => 'Login-LAT-Node', 36 => 'Login-LAT-Group', 37 => 'Framed-AppleTalk-Link',
        38 => 'Framed-AppleTalk-Network', 39 => 'Framed-AppleTalk-Zone',
        60 => 'CHAP-Challenge', 61 => 'NAS-Port-Type', 62 => 'Port-Limit', 63 => 'Login-LAT-Port'];

    /**
     * 收到udp数据包
     * @param swoole_server $serv
     * @param string $data
     * @param array $clientInfo
     */
    public function onPacket(swoole_server $serv, string $data, array $clientInfo) {
        $attr = [];
        $struct = $this->unpack($data, $attr);
        print_r($struct);
        print_r($attr);
    }

    /**
     * 解码radius数据包
     * @param string $bin
     * @param array $attr
     * @return array|bool
     */
    public function unpack(string $bin, array &$attr): array {
        //一个正常的radius封包长度是绝对大于等于20的
        if (strlen($bin) < 20) {
            return [];
        }
        //解包
        $radius = unpack('ccode/cidentifier/nlength/a16authenticator', $bin);
        //获取后面的属性长度,并且对数据包进行验证
        if (strlen($bin) != $radius['length']) {
            return [];
        }
        $attr_len = $radius['length'] - 20;
        //处理得到后面的Attributes,并且解包
        $attr = $this->unpack_attr(substr($bin, 20, $attr_len));
        if ($attr == []) {
            return [];
        }
        return $radius;
    }

    /**
     * 处理Attributes
     * @param string $bin
     * @return array
     */
    public function unpack_attr(string $bin): array {
        $attr = [];
        $offset = 0;
        $len = strlen($bin);
        while ($offset < $len) {
            $attr_type = ord($bin[$offset]);//属性类型
            $attr_len = ord($bin[$offset + 1]);//属性长度
            $attr[static::$ATTR_TYPE[$attr_type]] = substr($bin, $offset + 2, $attr_len - 2);//属性值
            //跳到下一个
            $offset += $attr_len;
        }
        //判断offset和$len是否相等,不相等认为无效,抛弃这个封包
        if ($offset != $len) {
            return [];
        }
        return $attr;
    }


    /**
     * 运行服务器
     * @param int $authPort
     * @param int $accountPort
     */
    public function run(string $secret_key, int $authPort = 1812, int $accountPort = 1813) {
        $server = new swoole_server("0.0.0.0", $authPort, SWOOLE_PROCESS, SWOOLE_SOCK_UDP);
        $server->on('Packet', array($this, 'onPacket'));
        $server->start();
        $this->server = $server;
        $this->secret_key = $secret_key;
    }
}

$server = new radius();
$server->run('test123');
```

![](img/03-UDP-radius%E5%8D%8F%E8%AE%AE%E5%AE%9E%E7%8E%B0.assets/TIM%E6%88%AA%E5%9B%BE20180624151246-300x156.png)

### 密码验证

> 密码有两种的加密方式 `User-Password(PAP加密)`和`CHAP-Password(CHAP加密)`

### User-Password

[https://tools.ietf.org/html/rfc2865#page-27](https://tools.ietf.org/html/rfc2865#page-27)

> 在传输时，密码是隐藏的。首先在密码的末尾用空值填充 16 个字节的倍数。单向 MD5 哈希是通过由共享密钥组成的八位字节流来计算的，后面是请求身份验证器。该值与密码的前 16 个八位位组段进行异或并放置在用户密码属性的字符串字段的前 16 个八位位组中。
>
> 如果密码长度超过 16 个字符，则第二个单向 MD5 散列值将通过由共享密钥组成的八位字节流计算，然后是第一个异或结果。该散列与密码的第二个 16 位八位字节进行异或，并置于用户密码属性的字符串字段的第二个 16 位字节中。
>
> 调用**共享密钥 S**和伪随机 128 位请求认证器**RA** 。 将密码分成 16 个八位字节块 p1，p2 等。 最后一个填充为零，最终为 16 个八位字节的边界。 调用密文块 c（1），c（2）等，我们需要中间值 b1，b2 等。

```
         b1 = MD5(S + RA)       c(1) = p1 xor b1
         b2 = MD5(S + c(1))     c(2) = p2 xor b2
                .                       .
                .                       .
                .                       .
         bi = MD5(S + c(i-1))   c(i) = pi xor bi

      The String will contain c(1)+c(2)+...+c(i) where + denotes
      concatenation.
```

上面其实是 google 翻译来的- -...把我理解的说一下

当密码位数小于 16 位的时候,先计算出**md5(密钥(secret_key)+Authenticator)**的 md5,然后再与我们接收到的**User-Password**进行位运算,遇到`\x0`结尾字符串结束

如果超过了 16 位而且还没有遇到`\x0`那么继续和我们的**User-Password**进行位运算,但是这回的用于位运算的 md5 为**md5(密钥(secret_key)+前 16 个字符)**,这里有个坑....文档说的是加密的过程,我们需要实现的是解密,所以前 16 个字符串不是我们解密后的 16 个字符,而是加密后的 16 个字符,=\_=害我拿原文(解密后的)一直在算,不对

实现代码:

```php
    public function decode_user_passwd($bin, $Authenticator) {
        $passwd = '';
        $S = $this->secret_key;
        $len = strlen($bin);
        //b1 = MD5(S + RA)
        $hash_b = md5($S . $Authenticator, true);
        for ($offset = 0; $offset < $len; $offset += 16) {
            //每次拿16字符进行解码
            for ($i = 0; $i < 16; $i++) {
                $pi = ord($bin[$offset + $i]);
                $bi = ord($hash_b[$i]);
                //c(i) = pi xor bi
                $chr = chr($pi ^ $bi);
                if ($chr == "\x0") {
                    //文本标志\x0结尾
                    return $passwd;
                }
                $passwd .= $chr;
            }
            //判断一下是不是已经结束了,然后返回
            if ($len == $offset + 16) {
                return $passwd;
            }
            //bi = MD5(S + c(i-1))
            $hash_b = md5($S . substr($bin, $offset, 16), true);
        }
        //都循环完了,还没看见结束,返回空
        return '';
    }
```

### CHAP-Password

> 从上面的代码来看`User-Password`是不安全的,只要知道了**secret_key**,密码明文完全可以解密出来,CHAP 密码原文是无法解密出来的
>
> RADIUS 服务器根据用户名查找密码，使用 MD5 在 CHAP ID 八位位组，密码和 CHAP 质询（如果存在 CHAP-Challenge 属性，否则从请求身份验证器）加密质询，以及 将该结果与 CHAP 密码进行比较。 如果它们匹配，则服务器发回一个访问接受，否则它将发回一个访问拒绝。
>
> 此属性指示由 PPP 挑战握手认证协议（CHAP）用户响应挑战而提供的响应值。 它仅用于 Access-Request 数据包。
>
> 如果存在于数据包中，则在 CHAP-Challenge 属性（60）中找到 CHAP 质询值，否则在请求验证器字段中找到。

[https://tools.ietf.org/html/rfc2865#page-8](https://tools.ietf.org/html/rfc2865#page-8)

[https://tools.ietf.org/html/rfc2865#page-28](https://tools.ietf.org/html/rfc2865#page-28)

CHAP-Password 的结构是这样的,中间还有一个 CHAP Ident,后面 string 是一个 16 个字节的字符串,一听就觉得是 md5

```
   A summary of the CHAP-Password Attribute format is shown below.  The
   fields are transmitted from left to right.

    0                   1                   2
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
   |     Type      |    Length     |  CHAP Ident   |  String ...
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
```

感觉 CHAP 比 PAP 好写多了,直接验证**md5(chapid+passwd+CHAP-Challenge(没有则用 Authenticator))**和 String 是否相等就好了

```php
//调用
echo "isPass:" . (
	$this->verify_chap_passwd($attr['CHAP-Password'],
							  'qwe123',
							  $attr['CHAP-Challenge'] ?? $struct['authenticator']
							 ) ? 'true' : 'false');

    public function verify_chap_passwd(string $bin, string $pwd, string $chap): bool {
        if (strlen($bin) != 17) return false;
        $chapid = $bin[0];
        $string = substr($bin, 1);
        return md5($chapid . $pwd . $chap, true) == $string;
    }
```

### 封包

> 处理完后,总还得人家一个回信吧

[https://tools.ietf.org/html/rfc2865#page-19](https://tools.ietf.org/html/rfc2865#page-19)

都是一样的,只是吧解包变成封包一个逆向操作,值得注意的是**Authenticator**,在接收的时候这个是随机的,发送的时候,我们需要带进去计算**MD5(Code+ID+Length+RequestAuth+Attributes+Secret)**

```php
    /**
     * 封包
     * @param int $code
     * @param int $identifier
     * @param string $reqAuthenticator
     * @param array $attr
     * @return string
     */
    public function pack(int $code, int $identifier, string $reqAuthenticator, array $attr = []): string {
        $attr_bin = '';
        foreach ($attr as $key => $value) {
            $attr_bin .= $this->pack_attr($key, $value);
        }
        $len = 20 + strlen($attr_bin);
        //MD5(Code+ID+Length+RequestAuth+Attributes+Secret)
        $send = pack('ccna16',
                $code, $identifier, $len,
                md5(chr($code) . chr($identifier) . pack('n', $len) .
                    $reqAuthenticator . $attr_bin . $this->secret_key, true)
            ) . $attr_bin;
		//这里实际使用的时候有错误,因为NTR工具没有校验Response Authenticator...现在已经修改了
        return $send;
    }

    /**
     * 封包属性
     * @param  $code
     * @param string $data
     * @return string
     */
    public function pack_attr($code, string $data): string {
        return pack('cc', $code, 2 + strlen($data)) . $data;
    }
```

![](img/03-UDP-radius%E5%8D%8F%E8%AE%AE%E5%AE%9E%E7%8E%B0.assets/TIM%E6%88%AA%E5%9B%BE20180624194116-300x147.png)

> 完整代码中的封包有错误,请看上面的注释,在代码中修改了

完成,完整代码: [start.php](./img/03-UDP-radius协议实现.assets/start.zip)
