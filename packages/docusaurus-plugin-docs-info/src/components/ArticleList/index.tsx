import { Article } from "../..";
import { FullVersion } from "@docusaurus/plugin-content-docs/lib/types.d.ts";
import { DocMetadata } from "@docusaurus/plugin-content-docs";
import { usePluginData } from "@docusaurus/useGlobalData";
import dayjs from "dayjs";
import Link from "@docusaurus/Link";

const ArticleList: React.FC<{
  list: Article[];
}> = ({ list: data }) => {
  const docsData = usePluginData("docusaurus-plugin-content-docs") as {
    versions: FullVersion[];
  };
  const list: Array<Array<Article>> = [];
  const map: Map<string, DocMetadata> = new Map();
  for (let i = 0; i < docsData.versions[0].docs.length; i++) {
    // @ts-ignore
    map.set(docsData.versions[0].docs[i].path, docsData.versions[0].docs[i]);
  }
  // 一行两个
  const num = 3;
  for (let i = 0; i < data.length && i < 8; i += num) {
    list.push(data.slice(i, i + num));
  }
  // 构建与docs的连接
  for (let i = 0; i < list.length; i++) {
    for (let j = 0; j < list[i].length; j++) {
      const val = list[i][j];
      let path = val.filename.replace(/\.(md|mdx)/, "").substring(4);
      if (
        val.filename.endsWith("index.md") ||
        val.filename.endsWith("index.mdx")
      ) {
        path = path.split("/").slice(0, -1).join("/") + "/";
      }
      const doc = map.get(path);
      if (doc) {
        // @ts-ignore
        list[i][j].link = doc.path;
      }
    }
  }

  return (
    <>
      {list.map((val, index) => (
        <div className="row" key={"row-" + index}>
          {val.map((val, index) => (
            <div className="col col--4" key={"col-" + index}>
              <div
                className="card"
                style={{
                  borderWidth: 1,
                  borderColor: "var(--ifm-color-primary)",
                }}
              >
                <div className="card__header">
                  <Link
                    // @ts-ignore
                    to={val.link}
                    className="text-left"
                    style={{
                      display: "block",
                      fontSize: "18px",
                      maxWidth: "80%",
                      textOverflow: "ellipsis",
                      overflow: "hidden",
                      whiteSpace: "nowrap",
                    }}
                  >
                    {val.title}
                  </Link>
                </div>
                <div className="card__body">
                  <div
                    className="flex flex-row w-full items-center"
                    style={{
                      justifyContent: "space-between",
                      fontSize: "12px",
                    }}
                  >
                    <p>{dayjs(val.create_date).format("YYYY年MM月DD日")}</p>
                    <Link
                      // @ts-ignore
                      to={val.link}
                    >
                      <button className="button button--link">阅读</button>
                    </Link>
                  </div>
                </div>
              </div>
            </div>
          ))}
        </div>
      ))}
    </>
  );
};

export default ArticleList;
