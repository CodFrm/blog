import Layout from "@theme/Layout";
import { Article, Articles } from "docusaurus-plugin-docs-info";
import { Pagination } from "antd";
import Link from "@docusaurus/Link";
import ArticleList from "../../components/ArticleList";

export interface TimelineProps {
  articles: Articles;
}

function Timeline({ articles }: TimelineProps) {
  return (
    <Layout title={"最新文章 - 第" + articles.current + "页"}>
      <div
        className="container flex flex-col px-8"
        style={{
          paddingTop: "2rem",
          gap: "1rem",
        }}
      >
        <ArticleList list={articles.list} />
        <div
          className="flex flex-row"
          style={{
            justifyContent: "center",
          }}
        >
          <Pagination
            current={articles.current}
            total={articles.total}
            pageSize={20}
            showSizeChanger={false}
            style={{
              marginBottom: "10px",
            }}
            itemRender={(page, type, el) => {
              if (type !== "page") {
                return el;
              }
              if (page === 1) {
                return <Link to={"/timeline"}>1</Link>;
              }
              return <Link to={"/timeline/" + page}>{page}</Link>;
            }}
          />
        </div>
      </div>
    </Layout>
  );
}

export default Timeline;
