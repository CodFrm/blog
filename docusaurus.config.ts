import { themes as prismThemes } from "prism-react-renderer";
import type { Config } from "@docusaurus/types";
import type * as Preset from "@docusaurus/preset-classic";
import docsToBlog from "./packages/docusaurus-plugin-docs-info/src";

const config: Config = {
  title: "王一之",
  tagline: "王一之的个人博客：分享知识，记录生活，认识朋友",
  favicon: "img/favicon.ico",

  // Set the production url of your site here
  url: "https://blog.icodef.com",
  // Set the /<baseUrl>/ pathname under which your site is served
  // For GitHub pages deployment, it is often '/<projectName>/'
  baseUrl: "/",

  // GitHub pages deployment config.
  // If you aren't using GitHub pages, you don't need these.
  organizationName: "codfrm", // Usually your GitHub org/user name.
  projectName: "blog", // Usually your repo name.

  onBrokenLinks: "warn",
  onBrokenMarkdownLinks: "warn",

  plugins: [
    ["@gracefullight/docusaurus-plugin-tailwind", {}],
    [
      "docusaurus-plugin-content-docs",
      {
        routeBasePath: "/",
        sidebarPath: "./sidebars.ts",
        // Please change this to your repo.
        // Remove this to remove the "edit this page" links.
        editUrl: "https://github.com/CodFrm/blog/edit/main",
        showLastUpdateTime: true,
      },
    ],
    ["docusaurus-plugin-docs-info", { debug: true }],
  ],

  // Even if you don't use internationalization, you can use this field to set
  // useful metadata like html lang. For example, if your site is Chinese, you
  // may want to replace "en" with "zh-Hans".
  i18n: {
    defaultLocale: "zh-CN",
    locales: ["zh-CN"],
  },

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
        theme: {
          customCss: "./src/css/custom.css",
        },
        sitemap: {
          // @ts-ignore
          changefreq: "weekly",
          priority: 0.5,
          ignorePatterns: ["/tags/**"],
          filename: "sitemap.xml",
        },
      } satisfies Preset.Options,
    ],
  ],

  themeConfig: {
    // Replace with your project's social card
    image: "img/docusaurus-social-card.jpg",
    navbar: {
      title: "王一之的博客",
      logo: {
        alt: "Logo",
        src: "img/avatar.png",
      },
      items: [
        {
          position: "right",
          label: "主页",
          href: "/",
        },
        {
          position: "right",
          label: "全部",
          href: "/blog",
        },
        {
          position: "right",
          label: "Dev",
          to: "/dev",
          // items: [
          //   {
          //     to: "/dev/golang",
          //     label: "Golang",
          //   },
          // ],
        },
        {
          type: "docSidebar",
          sidebarId: "ops",
          position: "right",
          label: "Ops",
          to: "/ops",
        },
        {
          type: "docSidebar",
          sidebarId: "note",
          position: "right",
          label: "笔记",
          to: "/note",
        },
        {
          href: "https://github.com/codfrm/blog",
          label: "GitHub",
          position: "right",
        },
      ],
      hideOnScroll: true,
    },
    footer: {
      style: "dark",
      copyright: `<p>Copyright © ${new Date().getFullYear()} icodef.com, Built with Docusaurus.</p>
      <p><a href="https://beian.miit.gov.cn/" target="_blank" rel="nofollow">湘ICP备19008073号</a></p>
      <p>若无特殊声明，本站所有内容遵循 <a rel="license" href="https://creativecommons.org/licenses/by-nc-sa/4.0/deed.zh-hans" >CC BY-NC-SA 4.0 协议</a></p>`,
    },
    prism: {
      theme: prismThemes.github,
      darkTheme: prismThemes.dracula,
      // https://prismjs.com/download.html
      additionalLanguages: ["php", "bash", "docker"],
    },
  } satisfies Preset.ThemeConfig,
};

export default config;
