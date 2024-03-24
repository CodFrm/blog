import React from "react";
import Heading from "@theme-original/Heading";
import { useSyntheticTitle } from "../DocItem/Content";
import { useDoc } from "@docusaurus/theme-common/internal";
import dayjs from "dayjs";
import { Detail } from "docusaurus-plugin-content-docs-ex/src";

export default function HeadingWrapper(props) {
  const syntheticTitle = useSyntheticTitle();
  const doc = useDoc();
  const detail = (doc.metadata as any).detail as Detail;
  return (
    <>
      <Heading {...props}>
        {props.children}
        {detail && !syntheticTitle && props.as.toString() === "h1" && doc.contentTitle==props.children && (
          <span
            style={{
              display: "block",
              fontSize: "14px",
              fontWeight: "normal",
              marginBottom: "10px",
              marginTop: "10px",
            }}
          >
            {dayjs(detail.create_date).format("YYYY年MM月DD日")} · 阅读需{" "}
            {Math.ceil(detail.reading_time.minutes)} 分钟
          </span>
        )}
      </Heading>
    </>
  );
}
