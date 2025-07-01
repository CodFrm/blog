import docsPlugin, {
  PluginOptions,
  LoadedContent,
  DocMetadata as OfficialDocMetadata,
  Options,
} from "@docusaurus/plugin-content-docs";
import {
  Plugin,
  LoadContext,
  OptionValidationContext,
} from "@docusaurus/types";
import matter from "gray-matter";
import path from "path";
import simpleGit, { DefaultLogFields } from "simple-git";
import readingTime, { ReadTimeResults } from "reading-time";
//@ts-ignore
import { validateOptions as officialValidateOptions } from "@docusaurus/plugin-content-docs/src/index";

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
  const isProd = process.env.NODE_ENV === "production";

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
        if (!isProd && !options.debug) {
          // 非生产、非debug模式 直接生产假数据
          detail.create_date = new Date();
          detail.update_date = new Date();
          doc.detail = detail;
          return;
        }
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

  const warpAllContentLoaded = ret.allContentLoaded;
  if (warpAllContentLoaded) {
    ret.allContentLoaded = async ({ actions, allContent }) => {
      const ret = await warpAllContentLoaded({
        actions,
        allContent,
      });
      return ret;
    };
  }

  return ret;
}

export function validateOptions({
  validate,
  options: userOptions,
}: OptionValidationContext<Options, PluginOptions>): PluginOptions {
  // @ts-ignore
  const oldDebug = userOptions.debug;
  //@ts-ignore
  delete userOptions.debug;
  const ret = officialValidateOptions({ validate, options: userOptions });
  // @ts-ignore
  userOptions.debug = oldDebug;
  return ret;
}
