import path from "path";
import fs from "fs";
import simpleGit, { DefaultLogFields } from "simple-git";
import matter from "gray-matter";
import { LoadContext, Plugin } from "@docusaurus/types";
import readingTime, { ReadTimeResults } from "reading-time";
import { LoadedVersion } from "@docusaurus/plugin-content-docs";
import { DocMetadata } from "docusaurus-plugin-content-docs/src";

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
  reading_time: ReadTimeResults;
  create_date?: Date;
  update_date?: Date;
}

export interface Articles {
  list: DocMetadata[];
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
    async contentLoaded({ content, actions, allContent }): Promise<void> {
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

      const docsData = allContent["docusaurus-plugin-content-docs"] as {
        default: { loadedVersions: LoadedVersion };
      };
      let docs = docsData.default.loadedVersions[0].docs as DocMetadata[];
      // 文章时间排序
      docs = docs.sort((a, b) => {
        //@ts-ignore
        return b.detail.create_date.getTime() - a.detail.create_date.getTime();
      });

      // 20篇为一页, 创建时间由新到旧
      const pageSize = 21;
      const latest = {
        current: 1,
        list: docs.slice(0, pageSize),
        total: docs.length,
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

      for (let i = 1; i < Math.ceil(docs.length / pageSize); i++) {
        const page = i + 1;
        const pageData = await createData(
          `timeline-${page}.json`,
          JSON.stringify({
            current: i + 1,
            list: docs.slice(i * pageSize, (i + 1) * pageSize),
            total: docs.length,
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
