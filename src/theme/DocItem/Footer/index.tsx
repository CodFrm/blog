import React from "react";
import Footer from "@theme-original/DocItem/Footer";
// @ts-ignore
import Comments from "/src/components/Comments";
import { useDoc } from "@docusaurus/theme-common/internal";

export default function FooterWrapper(props) {
  const { metadata } = useDoc();
  const { comments = true } = metadata.frontMatter;

  return (
    <>
      <Footer {...props} />
      {comments && <Comments />}
    </>
  );
}
