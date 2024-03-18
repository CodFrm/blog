import Layout from "@theme/Layout";
import clsx from "clsx";
import styles from "./index.module.css";
import useDocusaurusContext from "@docusaurus/useDocusaurusContext";
import { Icon } from "@iconify/react/dist/iconify.js";
import Link from "@docusaurus/Link";
import LatestDocs from "docusaurus-plugin-docs-info/src/components/LatestDocs";

function HomepageHeader() {
  const { siteConfig } = useDocusaurusContext();
  return (
    <header
      className={clsx("hero", styles.heroBanner) + " flex flex-col gap-2 !pb-0"}
    >
      <div className="flex flex-row text-left px-8 items-center">
        <div className="flex flex-col flex-1">
          <h1 className="text-4xl mb-8">
            Hi！我是
            <span style={{ color: "var(--ifm-color-primary)" }}>王一之</span>
          </h1>
          <p className="text-lg">
            欢迎来到我的个人博客，一个什么都会点的全
            <span className={styles["hide-zhan"]}>
              <span className={styles["hide-zhan-1"]}>栈</span>
              <span className={styles["hide-zhan-2"]}>沾</span>
            </span>
            工程师，这里记录了我的一些学习笔记和技术分享。目前主要使用
            <Link to="https://go.dev/">
              <code>Golang</code>
            </Link>
            与
            <Link to="https://www.typescriptlang.org/">
              <code>TypeScript</code>
            </Link>
            ✖️
            <Link to="https://react.dev/">
              <code>React</code>
            </Link>
            进行开发，也会写一些关于
            <Link to="/ops">
              <code>运维</code>
            </Link>
            、
            <Link to="/ops">
              <code>工具分享</code>
            </Link>
            之类的文章。
            <br />
            平常喜欢做开源项目，目前我正在开发
            <Link to="https://docs.scriptcat.org" rel="">
              <code>脚本猫</code>
            </Link>
            与运营
            <Link to="https://bbs.tampermonkey.net.cn/" rel="">
              <code>油猴中文网</code>
            </Link>
            ，你可以在我的
            <Link to="https://github.com/codfrm">
              <code>Github</code>
            </Link>
            中找到更多关于我的开源项目。
            <br />
            <br />
            博客源码托管在
            <Link
              to={
                "https://github.com/" +
                siteConfig.organizationName +
                "/" +
                siteConfig.projectName
              }
              rel=""
            >
              <code>GitHub</code>
            </Link>
            上，如果觉得还不错，顺手点个 Star 吧！
          </p>
          <div className={"flex flex-row gap-3 " + styles.social}>
            <a href="https://space.bilibili.com/17047572" target="_blank">
              <Icon icon="ri:bilibili-fill" />
            </a>
            <a href="https://github.com/CodFrm" target="_blank">
              <Icon icon="ri:github-fill" />
            </a>
            <a href="mailto:yz@ggnb.top">
              <Icon icon="ri:mail-fill" />
            </a>
          </div>
          <br />
          <div className="flex flex-row gap-2 items-center">
            <Link to="/blog">
              <button className="button button--primary button--lg">
                开始阅读
              </button>
            </Link>
            {/* <a
              href="https://github.com/CodFrm"
              style={{
                height: "20px",
              }}
            >
              <img src="https://img.shields.io/github/stars/codfrm/blog?style=social" />
            </a> */}
          </div>
        </div>
        <div className="flex-1">
          <img src="/img/home-1.svg" />
        </div>
      </div>
      <div
        className="flex flex-col gap-3 w-full p-4"
        style={{
          background: "var(--ifm-color-emphasis-0)",
          borderTop: "1px solid var(--ifm-color-emphasis-100)",
        }}
      >
        <h1
          className="w-full text-2xl"
          style={{
            fontWeight: "bold",
          }}
        >
          最新文章
        </h1>
        <LatestDocs />
      </div>
    </header>
  );
}

function Index() {
  const { siteConfig } = useDocusaurusContext();
  return (
    <Layout title="首页" description={siteConfig.tagline}>
      <HomepageHeader />
    </Layout>
  );
}

export default Index;
