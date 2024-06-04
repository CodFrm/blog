# manifest v3 脚本猫何去何从

从 2024 年 6 月的 Chrome 127 开始，Chrome 的不稳定版本就不能再使用 Manifest V2 了，正式版会根据反馈再决定停止时间，完全停止目前看公告是在 2025 年 6 月开始。[了解更多关于 Manifest V3](https://developer.chrome.com/docs/extensions/develop/migrate/what-is-mv3?hl=zh-cn)

脚本猫也不得不开始考虑 Manifest V3 的问题了，标题有些夸张了，虽然 Manifest V3 限制很多，但还不至于到无法使用的地步，尽管还是有挺多蛋疼的问题的。

目前`Tampermonkey`的 Beta 版本已经更新为 MV3，从更新日志上来看除了需要用户手动开启开发者模式以外好像并没有太大的影响。对于需要开启开发者模式的问题，这点实在难以接受，希望后续可以取消，`violentmonkey`的作者看起来十分反对，并决定[抗争](https://github.com/violentmonkey/violentmonkey/issues/1934#issuecomment-1954310945)。

后面就来详细说说 Manifest V3 对脚本猫（Userscript 类扩展）的影响，不过目前处于调研阶段，可能会有错误，欢迎指正。实际开发时也可能会有一些其它的感悟或者实现方法，后续会继续更新。

## Service Worker

首先最大的变化就是，v2 的 background 页被取消了，取而代之的是 Service Worker。background 页是一个常驻的页面，可以监听事件，还可以操作 DOM，和一些`window`才有的 API，像`GM_xhr/GM_download`就用到了`URL.createObjectURL`方法来处理`blob`数据。

而 Service Worker 是一个独立的线程，并且拥有很多限制，比如：无法使用 XHR API、无 DOM 和 window 对象、无法操作 localStorage 等等。

由于这个变化，脚本猫的很多功能几乎全部需要重写，通信方面也要重新设计，一些功能也要重新考虑。虽然说都有替代方案，但还是有些烦躁。

另外我也还没有接触过 Service Worker，还需要学习一下，开发过程中可能还会遇到一些问题。

## DNR

其次就是围绕着网络请求修改方面的，Manifest V2，可以使用[`webRequest API`](https://developer.chrome.com/docs/extensions/reference/api/webRequest?hl=zh-cn)，可以非常自由的修改网络请求，然而 V3 版本这个 API 被移除了，取而代之的是 [`declarativeNetRequest API`](https://developer.chrome.com/docs/extensions/reference/api/declarativeNetRequest?hl=zh-cn),使用的是声明式的规则列表，甚至还有条数限制。这直接影响了 GM_xhr 的相关功能，例如：Forbidden header、匿名 cookie、set-cookie 等。

Tampermonkey 并没有放弃这个功能，我不知道它是如何实现的，目前我想到的就是每一个 GM xhr 请求，如果有相关的 header，就为这个请求添加一个规则，这样做不知道有没有问题，但是确实十分不优雅。

另外检测脚本安装，也是依赖 webRequest API 的，这个功能也需要重新考虑方案。

## Userscript 和后台脚本

mv3 引入了两个新的 API：[chrome.userScripts](https://developer.chrome.com/docs/extensions/reference/api/userScripts?hl=zh-cn)和[chrome.offscreen](https://developer.chrome.com/docs/extensions/reference/api/offscreen?hl=zh-cn)，对于实现 Userscript 和后台脚本的功能上来说，更方便了，而且 chrome.userScripts 甚至可以让脚本在页面加载前进行注入，感觉又可以诞生一些新的玩法。但是必须要求用户开启开发者模式，这个实在是太难接受了。而且这个 API 可以突破 CSP 限制，在此之前是直接使用 webRequest API 将 CSP 策略移除，这样是更加安全的。

现在脚本的注入完全依赖这个 API，而不需要自己管理了，这个算是一个好消息，但是这样的话对于@match/@include 的表达式会产生一些影响。

另外就是 chrome.offscreen 这个 API，可以创建一个屏幕外的文档，目前我是打算是使用这个 API 创建一个屏幕外文档，然后用来运行后台脚本与定时脚本，有点感觉像是为了之前的 background 页损失而妥协的，也许有些功能也可以依靠它来实现。

## 最后

总之从 mv2 重构到 mv3 是一个庞大的工程，整个架构需要进行调整，有些功能也需要考虑新的实现方案，如果不是强制要求，我真的一点都不想升级，尤其是要求用户开启开发者模式这个，希望以后可以放开。

从开发角度来讲，mv3 对 userscript 类扩展的影响并没有那么大，甚至有些方面还有所改善。另外再一次重构，有之前的经验，也会更加顺利一些，有些功能也可以重新设计，更加合理。

后续脚本猫将会开始进行 mv3 的重构工作，希望能够顺利完成，如果有什么新的发现或者问题，我会继续更新这篇文章，如果是实现方案或者感悟，我会另外写文章。
