/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

import React from "react";
import Heading from "@theme/Heading";
import type { Props } from "@theme/MDXComponents/Heading";
import { useDoc } from "@docusaurus/theme-common/internal";
import { Detail } from "packages/docusaurus-plugin-content-docs/src";
import dayjs from "dayjs";

export default function MDXHeading(props: Props): JSX.Element {
  const doc = useDoc();
  const detail = (doc.metadata as any).detail as Detail;
  return (
    <>
      <Heading
        {...props}
        style={{
          marginBottom: "6px",
        }}
      >
        {props.children}
      </Heading>
      {detail && (
        <span
          style={{
            display: "block",
            fontSize: ".9rem",
            fontWeight: "normal",
            marginBottom: "1rem",
          }}
        >
          {dayjs(detail.create_date).format("YYYY年MM月DD日")} · 阅读需{" "}
          {Math.ceil(detail.reading_time.minutes)} 分钟
        </span>
      )}
    </>
  );
}
