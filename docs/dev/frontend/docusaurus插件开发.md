# docusaurus 的插件开发与自定义主题

我为了本博客的一些需求，开发了一些 docusaurus 的插件，这里记录一下开发过程。其中踩到了一些坑，折腾了我好几天，结果回过头来一看是那么简单，希望对你能有所帮助。

## 创建插件

你需要创建一个类似下面的 npm 包：

```json
{
  "name": "docusaurus-plugin-docs-info",
  "version": "1.0.0",
  "description": "",
  "main": "src/index.ts",
  "scripts": {
    "test": "echo \"Error: no test specified\" && exit 1"
  },
  "author": "",
  "license": "ISC",
  "dependencies": {
    "@docusaurus/core": "^3.1.1",
    "dayjs": "^1.11.10",
    "reading-time": "^1.5.0"
  }
}
```

目录结构就像这样：

![image-20240324133547293](img/docusaurus%E6%8F%92%E4%BB%B6%E5%BC%80%E5%8F%91.assets/image-20240324133547293.png)

其中源代码放在 src 目录下，theme 是相关主题，components 是组件

## 引用插件

创建完成插件后可以使用下面的方式进行引用

### 本地路径

可以通过本地相对路径来进行引用

```js
  plugins: [
  ["./packages/docusaurus-plugin-docs-info", {}]
  ],
```

### npm 包

也可以使用安装 npm 包的形式来引用

首先需要执行：`npm i ./packages/docusaurus-plugin-content-docs-ex`来安装包，然后再使用下方的配置进行配置

```json
plugins: [
 ["docusaurus-plugin-docs-info", {}]
],
```

## API

你可以看官方文档了解更多：[Plugin Method References](https://www.docusaurus.cn/docs/api/plugin-methods)，官方主要分为以下四个部分：

- Lifecycle APIs：生命周期 API，当到达构建的某些阶段，docusaurus 会调用插件的生命周期 API，你可以在这些 API 中做一些事情，比如修改配置，添加一些内容等。
- Extending infrastructure：扩展基础设施
- I18n lifecycles：i18n 生命周期，一些 i18n 翻译相关的内容
- Static methods：静态方法，主要是校验选项和校验主题两个方法

这里我简单的介绍一些常用的：

### loadContent

这个 API 可以用来预加载一些内容，比如读取文件，将某些数据预加载好，这样在后续的 API 中就可以使用这些内容了。
例如官方的`docusaurus-plugin-content-docs`插件就是在此预加载好所有的文档数据，然后在`contentLoaded`中再添加路由的。

### contentLoaded

这个 API 有两个参数，一个是 content，一个是 actions，content 就是 loadContent 返回的内容，
actions 里面包含了`addRoute`、`createData`、`setGlobalData`，3 个方法，
可以通过这三个方法添加数据和路由，这样就可以自定义一些页面了。

### getThemePath

获取主题路径，在`contentLoaded`添加路由时，会要求填写一个组件：这里的组件填写的是一个字符串，你可以用`@site/`前缀表示主路径下的组件，也可以使用`@theme/`前缀表示`getThemePath`返回的路径下的组件。

```ts
export default function (context: LoadContext, options: any): Plugin {
  const themePath = path.resolve(__dirname, "./theme");
  return {
    name: "docusaurus-plugin-docs-info",
    getThemePath() {
      return themePath;
    },
  };
}
// 添加路由
addRoute({
  path: "/timeline",
  component: "@theme/Timeline",
  modules: {
    articles: pageData,
  },
  exact: true,
});
```

### injectHtmlTags

在 body/head 中注入一些标签，比如添加一些统计代码，广告代码等。

### postBuild

构建完成时调用，会有生成路由的信息，可以在这里做一些事情，比如生成 sitemap， rss 等。

### validateOptions

校验选项，可以在这里校验一些配置是否正确。

## Hooks

除了上面的 API 之外，还有一些 hooks 可以使用，很多都是官方内部的，文档中并没有说明，也是我翻阅源码看到的，这里我列举一些我用到的：

### useGlobalData/PluginData

用于获取在`contentLoaded`中设置的全局数据与插件数据，可以在组件中使用。

```ts
import useGlobalData, {
  usePluginData,
  useAllPluginInstancesData,
} from "@docusaurus/useGlobalData";
```

### useDoc

获取文档数据，可以获取到文档的标题、内容、路径等。

```ts
import { useDoc } from "@docusaurus/theme-common/internal";
```

### useColorMode

获取颜色模式，例如黑夜/白天模式。

```ts
import { useColorMode } from "@docusaurus/theme-common";
```

## 自定义主题

如果你想要自定义你自己的主题，你可以使用`npm run swizzle --typescript`命令，这样可以让你替换插件的默认组件，来自定义你自己的样式。

官方文档：[Swizzling | Docusaurus](https://docusaurus.io/docs/swizzling)

例如我就包装了原版的官方 docs 插件，然后自定义了主题，使我的博客 docs 稳定支持了创建时间与阅读时间。

## 使用包装器模式重制官方 docs 插件

虽然使用其它模式，例如再新写一个插件，也可以实现，但是使用包装器模式，可以更好的保持原有的插件的功能，只是在原有的基础上添加一些新的功能。

此处需要注意一个坑，如果使用包装器模式需要在`docusaurus.config.ts`中将原本的 docs 插件关闭掉，否则会产生冲突。

```ts
  presets: [
    [
      "classic",
      {
        docs: false,
        // docs: {
        //   routeBasePath: "/",
        //   sidebarPath: "./sidebars.ts",
        //   // Please change this to your repo.
        //   // Remove this to remove the "edit this page" links.
        //   editUrl: "https://github.com/codfrm/blog",
        //   showLastUpdateTime: true,
        // },
      } satisfies Preset.Options,
    ],
  ],
```

引用时，在plugin单独引用：

```ts
  plugins: [
    [
      "docusaurus-plugin-content-docs-ex",
      {
        routeBasePath: "/",
        sidebarPath: "./sidebars.ts",
        // Please change this to your repo.
        // Remove this to remove the "edit this page" links.
        editUrl: "https://github.com/CodFrm/blog/edit/main",
        showLastUpdateTime: true,
      },
    ]
  ]
```

然后就可以在插件中使用原有的 docs 插件，然后在原有的基础上添加一些新的功能。

```ts
export default async function pluginContentDocs(
  context: LoadContext,
  options: PluginOptions & { debug?: boolean }
): Promise<Plugin<LoadedContent>> {
  const ret = (await docsPlugin.call(
    this,
    context,
    options
  )) as Plugin<LoadedContent>;
  const themePath = path.resolve(__dirname, "./theme");
  ret.getThemePath = () => {
    return themePath;
  };

  const warpLoadContent = ret.loadContent;
  ret.loadContent = async () => {
	const ret = await warpLoadContent();
	//  ....一些操作
	return ret;
  }

  return ret;
}
```

更多详细的内容你可以看我的插件：[docusaurus-plugin-content-docs-ex](https://github.com/CodFrm/blog/tree/main/packages/docusaurus-plugin-content-docs-ex)