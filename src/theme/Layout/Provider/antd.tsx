import { ConfigProvider, theme } from "antd";
import { useColorMode } from "@docusaurus/theme-common";

export default function AntdProvider({ children }) {
  const colorMode = useColorMode();
  return (
    <ConfigProvider
      theme={{
        algorithm:
          colorMode.colorMode == "dark"
            ? theme.darkAlgorithm
            : theme.defaultAlgorithm,
      }}
    >
      {children}
    </ConfigProvider>
  );
}
