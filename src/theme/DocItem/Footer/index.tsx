import React from "react";
import Footer from "@theme-original/DocItem/Footer";
import { useDoc } from "@docusaurus/theme-common/internal";

export default function FooterWrapper(props) {
  const { metadata } = useDoc();

  return (
    <>
      <Footer {...props} />
    </>
  );
}
