> 最近在使用 webpack 打包工具的时候，使用 watch 模式不管用，最开始以为是配置的问题，将就着写者，后面同一个项目在 windows 上可以，但是我用我的 ubuntu 运行 watch 模式怎么也不管用，通过 google 找到了解决方案。

## 解决方案

Debian，RedHat 或其他类似的 Linux 发行版

```shell
echo fs.inotify.max_user_watches=524288 | sudo tee -a /etc/sysctl.conf && sudo sysctl -p
```

Arch Linux

```shell
echo fs.inotify.max_user_watches=524288 | sudo tee /etc/sysctl.d/40-max-user-watches.conf && sudo sysctl --system
```

在终端中输入上面的命令

## Why？

这是因为在 Linux 下监控的文件有一定的限制，当我们项目的文件超过这个数量的时候，其他的文件就不会再监控，webpack 无法监测到文件的变化，所以 watch 模式就失效了

使用下面命令可以查看限制的数量：

```shell
cat /proc/sys/fs/inotify/max_user_watches
```

## 参考

[https://github.com/guard/listen/blob/master/README.md#increasing-the-amount-of-inotify-watchers](https://github.com/guard/listen/blob/master/README.md#increasing-the-amount-of-inotify-watchers)
