<h1 align="center"> 王一之的个人博客 </h1>
<p align="center">分享知识，记录生活，认识朋友</p>

---

这里是我的个人博客，使用 docusaurus 构建，主要用于记录一些技术知识，生活感悟，以及一些有趣的事情。欢迎大家来访，也欢迎大家留言，一起交流。

---

> 如果你想基于本仓库部署自己的博客，可以查看下面的内容。关于本博客对 docusaurus 的一些修改，你可以查看[我为什么换掉 Wordpress 选择 Docusaurus？](./docs/note//为什么换掉Wordpress.md)

## 运行

你可以直接克隆本仓库，然后在本地运行。

```bash
# 克隆与进入仓库
git clone https://github.com/codfrm/blog.git
cd blog
# 安装依赖
npm i
# 运行
npm start
```

## 目录结构

你需要根据自己的需求来对一些文件进行修改，一些没有标注出来的文件可能是与 docuasaurus 相关的一些配置文件，你可以根据官方文档来进行修改。

- archive 我的个人归档，你需要将它删除
- docs 博客文章，你的文章需要放在这里
- packages 一些自定义的插件
- src
  - components/Comments/index.tsx 评论组件，使用[Giscus](https://giscus.app/zh-CN)构建 如果你使用的话，你需要修改里面的配置
  - pages/index.tsx 首页文件
- docuasaurus.config.js docuasaurus 的配置文件，包含了博客的标题、描述等内容，你需要根据自己的需求来修改
- sidebars.js 侧边栏配置文件，你需要根据自己的需求来修改

## 部署

构建完成后，将构建好的静态文件部署到你的服务器上即可。你也可以选择 Github Pages、Vercel、Netlify 等等。

```bash
# 构建静态文件
npm run build
```
