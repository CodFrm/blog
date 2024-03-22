import { FullVersion } from "@docusaurus/plugin-content-docs/lib/types.d.ts";
import { usePluginData } from "@docusaurus/useGlobalData";
import dayjs from "dayjs";
import Link from "@docusaurus/Link";
import { DocMetadata } from "docusaurus-plugin-content-docs/src";

const ArticleList: React.FC<{
  list: DocMetadata[];
}> = ({ list: data }) => {
  const docsData = usePluginData("docusaurus-plugin-content-docs") as {
    versions: FullVersion[];
  };
  const list: Array<Array<DocMetadata>> = [];
  // 一行两个
  const num = 3;
  for (let i = 0; i < data.length && i < 8; i += num) {
    list.push(data.slice(i, i + num));
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
                    to={val.permalink}
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
                    <p>
                      {dayjs(val.detail.create_date).format("YYYY年MM月DD日")}
                    </p>
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
