import { usePluginData } from "@docusaurus/useGlobalData";
import { Articles } from "../..";
import ArticleList from "../ArticleList";
import Link from "@docusaurus/Link";

function LatestDocs() {
  const data = usePluginData("docusaurus-plugin-docs-info") as Articles;

  return (
    <div
      className="container flex flex-col"
      style={{
        gap: "1rem",
      }}
    >
      <ArticleList list={data.list} />
      <div
        className="w-full"
        style={{
          textAlign: "right",
        }}
      >
        <Link exact href="/timeline">
          <button className="button button--primary button--lg">
            查看更多
          </button>
        </Link>
      </div>
    </div>
  );
}

export default LatestDocs;
