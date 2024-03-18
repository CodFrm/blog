import React from "react";
import Giscus from "@giscus/react";
import { useColorMode } from "@docusaurus/theme-common";

export default function Comments(): JSX.Element {
  const { colorMode } = useColorMode();

  return (
    <div>
      <Giscus
        id="comments"
        repo="codfrm/blog"
        repoId="MDEwOlJlcG9zaXRvcnkxOTY2ODAyNzk="
        category="Blog Message"
        categoryId="DIC_kwDOC7kaV84CeCC_"
        mapping="og:title"
        strict="0"
        reactionsEnabled="1"
        emitMetadata="0"
        inputPosition="top"
        theme={colorMode === "dark" ? "dark_tritanopia" : "light_tritanopia"}
        lang="zh-CN"
        loading="lazy"
      />
    </div>
  );
}
