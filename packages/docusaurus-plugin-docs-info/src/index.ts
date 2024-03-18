import path from "path";
import fs from "fs";
import simpleGit, { DefaultLogFields } from "simple-git";
import matter from "gray-matter";
import { LoadContext, Plugin } from "@docusaurus/types";

async function readDir(pathName: string) {
  return new Promise<string[]>((resolve) => {
    const filenames: Array<string> = [];
    fs.readdir(pathName, async (_, files) => {
      await Promise.all(
        files.map((val) => {
          return new Promise<void>((resolve) => {
            const filename = path.join(pathName, val);
            fs.stat(filename, async (_, stats) => {
              if (stats.isFile()) {
                filenames.push(filename);
              } else {
                const result = await readDir(filename);
                filenames.push(...result);
              }
              resolve();
            });
          });
        })
      );
      resolve(filenames);
    });
  });
}

export interface Article {
  filename: string;
  title: string;
  create_date?: Date;
  update_date?: Date;
}

export interface Articles {
  list: Article[];
  current: number;
  total: number;
}

export default function (
  context: LoadContext,
  options: { debug?: boolean }
): Plugin {
  const themePath = path.resolve(__dirname, "./theme");
  return {
    name: "docusaurus-plugin-docs-info",
    getThemePath() {
      return themePath;
    },
    async contentLoaded({ content, actions }): Promise<void> {
      const { addRoute, createData, setGlobalData } = actions;
      const isProd = process.env.NODE_ENV === "production";
      if (!isProd && !options.debug) {
        setGlobalData({
          current: 1,
          list: [],
          total: 0,
        });
        return;
      }

      // 读取docs下文件
      const files = await readDir("./docs");
      const git = simpleGit();
      const articles = [];

      await Promise.all(
        files.map((file) => {
          return new Promise<void>((resolve) => {
            if (
              !(
                file.startsWith("_") ||
                file.endsWith(".md") ||
                file.endsWith(".mdx")
              )
            ) {
              return resolve();
            }
            // 读取文件内容, 查看标题, 查看日期
            const meta = matter.read(file);
            const article: Article = {
              filename: file,
              title:
                meta.data["title"] ||
                path.basename(file).split(".").slice(0, -1).join("."),
            };
            // 读取git log文件时间
            git.log<DefaultLogFields>(
              {
                format: "%ad",
                file: file,
              },
              (_, data) => {
                if (meta.data["create_date"]) {
                  article.create_date = new Date(meta.data["create_date"]);
                } else {
                  article.create_date = data.all.length
                    ? new Date(data.all[data.all.length - 1].date)
                    : new Date();
                }
                if (meta.data["update_date"]) {
                  article.update_date = new Date(meta.data["update_date"]);
                } else {
                  article.update_date = data.latest
                    ? new Date(data.latest.date)
                    : new Date();
                }
                articles.push(article);
                resolve();
              }
            );
          });
        })
      );

      // 20篇为一页, 创建时间由新到旧
      articles.sort((a, b) => {
        return b.create_date.getTime() - a.create_date.getTime();
      });

      const pageSize = 21;
      const latest = {
        current: 1,
        list: articles.slice(0, pageSize),
        total: articles.length,
      };
      setGlobalData(latest);

      const pageData = await createData(
        "timeline-1.json",
        JSON.stringify(latest)
      );
      addRoute({
        path: "/timeline",
        component: "@theme/Timeline",
        modules: {
          articles: pageData,
        },
        exact: true,
      });

      for (let i = 1; i < Math.ceil(articles.length / pageSize); i++) {
        const page = i + 1;
        const pageData = await createData(
          `timeline-${page}.json`,
          JSON.stringify({
            current: i + 1,
            list: articles.slice(i * pageSize, (i + 1) * pageSize),
            total: articles.length,
          })
        );
        addRoute({
          path: `/timeline/${page}`,
          component: "@theme/Timeline",
          modules: {
            articles: pageData,
          },
          exact: true,
        });
      }
    },
  };
}
