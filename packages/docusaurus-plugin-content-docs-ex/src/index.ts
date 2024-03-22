import docsPlugin, {
  PluginOptions,
  LoadedContent,
  DocMetadata as OfficialDocMetadata,
} from "@docusaurus/plugin-content-docs";
import { Plugin, LoadContext } from "@docusaurus/types";
import matter from "gray-matter";
//@ts-ignore
export { validateOptions } from "@docusaurus/plugin-content-docs/src/index";
import path from "path";
import fs from "fs";
import simpleGit, { DefaultLogFields } from "simple-git";
import readingTime, { ReadTimeResults } from "reading-time";

export type DocMetadata = OfficialDocMetadata & { detail: Detail };

export interface Detail {
  filename: string;
  reading_time: ReadTimeResults;
  create_date?: Date;
  update_date?: Date;
}

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
    // 注入时间信息
    const docs: DocMetadata[] = [];
    const git = simpleGit();
    ret.loadedVersions.forEach((version) => {
      version.docs.forEach((doc) => {
        docs.push(doc as any);
      });
    });

    await Promise.all(
      docs.map(async (doc) => {
        // 读取文件内容, 查看标题, 查看日期
        const filename = doc.source.replace("@site", ".");
        const meta = matter.read(filename);
        const detail: Detail = {
          filename: filename,
          reading_time: readingTime(meta.content),
        };
        return new Promise<void>((resolve) => {
          // 读取git log文件时间
          git.log<DefaultLogFields>(
            {
              format: "%ad",
              file: filename,
            },
            (_, data) => {
              if (meta.data["create_date"]) {
                detail.create_date = new Date(meta.data["create_date"]);
              } else {
                detail.create_date = data.all.length
                  ? new Date(data.all[data.all.length - 1].date)
                  : new Date();
              }
              if (meta.data["update_date"]) {
                detail.update_date = new Date(meta.data["update_date"]);
              } else {
                detail.update_date = data.latest
                  ? new Date(data.latest.date)
                  : new Date();
              }
              doc.detail = detail;
              resolve();
            }
          );
        });
      })
    );
    // 排序创造时间列表

    return ret;
  };

  const warpContentLoaded = ret.contentLoaded;

  ret.contentLoaded = async ({ content, actions, allContent }) => {
    const ret = await warpContentLoaded({
      content,
      actions,
      allContent,
    });
    return ret;
  };

  return ret;
}
