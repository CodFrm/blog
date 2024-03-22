/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

import React from "react";
import clsx from "clsx";
import { ThemeClassNames } from "@docusaurus/theme-common";
import { useDoc } from "@docusaurus/theme-common/internal";
import Heading from "@theme/Heading";
import MDXContent from "@theme/MDXContent";
import type { Props } from "@theme/DocItem/Content";
import { Detail } from "packages/docusaurus-plugin-content-docs-ex/src";
import dayjs from "dayjs";

/**
 Title can be declared inside md content or declared through
 front matter and added manually. To make both cases consistent,
 the added title is added under the same div.markdown block
 See https://github.com/facebook/docusaurus/pull/4882#issuecomment-853021120

 We render a "synthetic title" if:
 - user doesn't ask to hide it with front matter
 - the markdown content does not already contain a top-level h1 heading
*/
export function useSyntheticTitle(): string | null {
  const { metadata, frontMatter, contentTitle } = useDoc();
  const shouldRender =
    !frontMatter.hide_title && typeof contentTitle === "undefined";
  if (!shouldRender) {
    return null;
  }
  return metadata.title;
}

export default function DocItemContent({ children }: Props): JSX.Element {
  const syntheticTitle = useSyntheticTitle();
  const doc = useDoc();
  const detail = (doc.metadata as any).detail as Detail;

  return (
    <div className={clsx(ThemeClassNames.docs.docMarkdown, "markdown")}>
      {syntheticTitle && (
        <header>
          <Heading
            as="h1"
            style={{
              marginBottom: "8px",
            }}
          >
            {syntheticTitle}
          </Heading>
          {detail && (
            <span
              style={{
                display: "block",
                fontSize: "14px",
                fontWeight: "normal",
                marginBottom: "10px",
              }}
            >
              {dayjs(detail.create_date).format("YYYY年MM月DD日")} · 阅读需{" "}
              {Math.ceil(detail.reading_time.minutes)} 分钟
            </span>
          )}
        </header>
      )}
      <MDXContent>{children}</MDXContent>
    </div>
  );
}
