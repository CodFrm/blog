---
title: 从根本上解决鼠标手,chrome 插件"Vimium"分享
---

> 安利一个 chrome 插件,`Vimium`,全键盘上网(不用鼠标就不会出现鼠标手了啦\~),再想象一下电影里面啪啪啪啪那种敲键盘的感觉,so cool?~

Chrome 商店安装地址: https://chrome.google.com/webstore/detail/vimium/dbepggeogbaibhgnhhndojpepiihcmeb

github 开源地址: https://github.com/philc/vimium

## 常用命令

> 命令其实挺多的,我先写出我比较常用的一些命令,也能够覆盖到大部分使用场景了(全部一下子也记不住,记住了也没办法熟练的使用)

- 页面滚动命令: h(左)j(下)k(上)l(右)
- 页面滚动命令 2: gg(滚动到最顶部) shift+g(其实就是大写的`G`,shift 大小写转换而已) u(相当于 PgUp,向上滚动) d(向下滚动).这个命令挺好用的
- 左右切换 tab: Shift+j/J(左边 tab) shift+k/K(右边 tab)
- tab 切换命令 2: shift+t/T,会弹出一个框可以选择 tab 进行跳转
- 前进/后退:shift+h/H(后退) shift+l/L(前进)
- 关闭/恢复页面: x(关闭当前页面) shift+x(恢复上一个关闭的页面)
- 打开新页面命令: o(当前页面打开) shift+o/O(新页面打开) b(当前页面) shift+b/B(新页面打开),o 是会在历史记录/书签栏/关键字搜索,b 是在书签栏中搜索.这个命令我也很喜欢,直接的替换了我原来的一个书签管理工具,因为记录得实在是太多了...自己都找不到.
- 点击链接: f(当前页面),shift+f/F(新页面打开).按下后会有很多链接的指示,按照指示输入字母就可以点击链接了.
- tab 切换命令 2: g0(切换到第一个 tab) g$(shift+4)(切换到最后一个 tab) ^(shift+6)(切换到上一个打开的 tab)

## 进阶命令

> 还有一些命令,如果能熟练就更好了,虽然我自己也用得很少,因为有浏览器自己的快捷键(ctrl+cv,复制粘贴),或者我觉得没有鼠标习惯/方便,我也不是很熟练,还是习惯的去动鼠标- -...

插件和 vim 一样,有很多模式,按 esc 退出当前模式,在右下角会有提示

- v 进入视图模式,输入之后会发现有一个地方被选中,这时按 hjkl 可以进行移动操作
- y 复制选中的文本
- gi 选中输入框,进行输入,按 tab 可以进行切换
- ma 可以给当前位置做一个标记
- \`a 可以快速翻到标记位置(小写是当前页面的标记,大写是全浏览器的标记),一个超级方便的指令!
- yy 复制当前页面的链接,也是一个很方便的指令

## 其它命令

> 如果你记住了以上命令,已经可以完全的摆脱鼠标了(但是有些地方的操作我觉得鼠标更方便一点,或许是我已经习惯鼠标了吧),剩下的就是去熟练的使用了

- ? 打开帮助(但是好像有些命令没有写),万一忘记了,可以看看命令记一下,也会比我这里的更全,还有 wiki:https://github.com/philc/vimium/wiki
- / 进入搜索模式(感觉鸡肋...我喜欢 ctrl+f)
- n/N 进入搜索模式后查找下/上一个
