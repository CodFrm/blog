# ACME 协议流程详解与实现

ACME 协议全名为 “Automatic Certificate Management Environment”，[RFC 8555](https://datatracker.ietf.org/doc/html/rfc8555)，
意为自动证书管理环境。它是一个由 IETF 制定的协议，用于自动化证书颁发和更新过程。

在了解 ACME 之前，我们先来了解一下一些相关的名词：

- **CA**：Certificate Authority，证书颁发机构，负责颁发数字证书。
- **CSR**：Certificate Signing Request，证书签名请求，是由申请者生成的包含公钥的数据块，用于向 CA 申请数字证书。
- **CRT**：Certificate，证书文件，通常使用.crt 扩展名，可以是 PEM 或 DER 格式。.crt 文件包含公钥并由证书颁发机构（CA）签发，用于身份验证和数据加密。
- **PEM**：Privacy-Enhanced Mail，一种证书文件格式，可用于存储证书（.crt、.pem）、证书请求（.csr）和私钥（.key）。
- **DER**：Distinguished Encoding Rules，一种证书文件格式，可用于存储证书（.cer、.der）。
- **DV 证书**：Domain Validation Certificate，域名验证证书，证书颁发机构验证申请者对域名的控制权后颁发的证书。
- **OV 证书**：Organization Validation Certificate，组织验证证书，除了验证域名所有权外，CA 还验证申请组织的身份。
- **EV 证书**：Extended Validation Certificate，扩展验证证书，提供最高级别的验证，包括域名所有权、组织信息以及组织的实体存在性。

ACME 协议通过 HTTPS 与 JSON 来进行通信，它定义了一套标准的 API，用于客户端与证书颁发机构之间通信，实现证书的自动化颁发和更新。

## ACME 提供商

常见的免费的 ACME 提供商有：

- [Let's Encrypt](https://letsencrypt.org/)
- [ZeroSSL](https://zerossl.com/)

大多数系统都是支持的 Let's Encrypt，因此我们这里通过 Let's Encrypt 来进行介绍。

Let’s Encrypt 提供域名验证型（DV）证书。不提供组织验证（OV）或扩展验证（EV），OV 和 EV 证书一般需要验证真实实体才能颁发，不能做到自动化的颁发，自动化也会削弱这种信任。[Introduction](https://datatracker.ietf.org/doc/html/rfc8555#section-1)

## ACME 协议流程

ACME 协议的流程如下：

### 注册账户

> [Account Management](https://datatracker.ietf.org/doc/html/rfc8555#autoid-28)

通过`new-account`向 ACME 服务商注册账户，获取账户的唯一标识符（在发送请求时的 kid 字段）。有的 ACME 提供商可能还需要验证邮箱等信息，需要先注册 CA 账户，然后在请求 ACME 注册账户时携带上指定信息。

### 创建订单

> [Applying for Certificate Issuance](https://datatracker.ietf.org/doc/html/rfc8555#autoid-35)

通过`new-order`向 ACME 服务商创建一个新的订单，指定需要颁发证书的域名。如果通过，此时处于`pending`状态，ACME 服务商会返回一个订单标识符（finalize）和授权（authorizations），用于后续的操作。

### 验证域名

> [Identifier Authorization](https://datatracker.ietf.org/doc/html/rfc8555#section-7.5)

> [DNS Challenge](https://datatracker.ietf.org/doc/html/rfc8555#section-8.4)

在获取到 ACME 服务商返回的授权信息后，此时还只是处于`pending`，后续需要验证域名的所有权。此时我们请求上一步返回的`authorizations`URL，ACME 服务商会返回一个验证信息（challenges），客户端需要根据这个信息来验证域名的所有权。一般情况下会有`http-01`和`dns-01`两种验证方式。

我们只介绍`dns-01`的方式。

#### DNS-01 验证

> [Key Authorizations](https://datatracker.ietf.org/doc/html/rfc8555#section-8.1)

我们需要在 DNS 中添加一条 TXT 记录，记录名为 `_acme-challenge`加上域名，例如：`docs.scriptcat.org`那么记录名为 `_acme-challenge.docs.scriptcat.org`。

但在 dns 管理平台中请注意，添加的 TXT 记录名为：`_acme-challenge.docs`，你可以使用命令：`dig -t txt _acme-challenge.docs.scriptcat.org`来查看是否添加成功。

记录值需要计算你的账户指纹，然后与返回的 token 一起组成一个字符串，然后对这个字符串进行 SHA256 哈希，然后 base64 编码，最后得到的值就是记录值，十分复杂。。。。

keyAuthorization = token || '.' || base64url(Thumbprint(accountKey))

这里我们是用的 ES256 算法，所以 Thumbprint 就是对公钥进行 SHA256 哈希，然后 base64 编码。

```go
func ES256Jwk(publicKey ecdsa.PublicKey) string {
 return fmt.Sprintf(`{"crv":"%s","kty":"EC","x":"%s","y":"%s"}`,
  "P-256",
  base64.RawURLEncoding.EncodeToString(publicKey.X.Bytes()),
  base64.RawURLEncoding.EncodeToString(publicKey.Y.Bytes()),
 )
}

func (c *Client) thumbprint() string {
 sha256Bytes := sha256.Sum256([]byte(ES256Jwk(c.options.privateKey.PublicKey)))
 return base64.RawURLEncoding.EncodeToString(sha256Bytes[:])
}

func (c *Client) keyAuthorization(token string) string {
 return token + "." + c.thumbprint()
}

func (c *Client) DNS01ChallengeRecord(token string) string {
 hash := sha256.Sum256([]byte(c.keyAuthorization(token)))
 return base64.RawURLEncoding.EncodeToString(hash[:])
}
```

### 请求验证

> [Responding to Challenges](https://datatracker.ietf.org/doc/html/rfc8555#section-7.5.1)

当我们配置好`challenge`后，我们需要先请求一次`challenges`URL，发送一个`{}`空对象来通知 ACME 服务器已经准备好挑战了，我们可以轮训`authorizations`URL 来查看验证状态，也可以轮询`challenges`URL，更推荐`authorizations`URL，如果验证成功，那么我们的订单就会变成`valid`状态，这时候我们也可以将 dns 等记录删除了。

当变为`valid`状态后，我们就可以请求证书了。

### 请求颁发证书

> [Applying for Certificate Issuance](https://datatracker.ietf.org/doc/html/rfc8555#section-7.4)

当我们的订单变成`valid`状态后，我们就可以请求颁发证书了，通过`finalize`URL 来请求颁发证书，ACME 服务商会返回我们的证书链接。其中必须要携带上我们的 CSR 信息。

申请成功后，会返回一个`certificate`URL

### 下载证书

> [Downloading the Certificate](https://datatracker.ietf.org/doc/html/rfc8555#section-7.4.2)

当我们请求颁发证书成功后，我们就可以通过`certificate`URL 来下载我们的证书了。然后就可以部署到我们的服务器上了。

## ACME 协议的实现

> [Request Authentication](https://datatracker.ietf.org/doc/html/rfc8555#section-6.2)

> [JWS RFC7515](https://datatracker.ietf.org/doc/html/rfc7515)

ACME 使用 jws（JSON Web Signature）来进行签名，但是 ACME 依赖非对称算法，所以请注意不能使用`HS256`，RFC 中推荐使用`ES256`，我们可以先实现一个 JWS 的签名和验证的工具类。

在这里我就不详细展开了，可以看看我的 acme 包中的 jws 实现： [algorithm_test.go](https://github.com/CodFrm/dns-kit/blob/6dcac9b084a8487188af9eb58c5e411b489cedbe/pkg/jws/algorithm_test.go)

这里为 acme 的请求做了一层封装，方便使用：

```go
func (c *Client) newRequest(url string, payload any) (*http.Request, error) {
 nonce, err := c.NewNonce()
 if err != nil {
  return nil, err
 }
 // 注册账户需要签名
 if c.options.privateKey == nil {
  return nil, ErrPrivateKeyNotFound
 }
 var header *jws.Header
 // 如果有kid则使用kid签名
 if c.options.kid != "" {
  header = jws.NewHeader(newEs256(c.options.kid, c.options.privateKey))
 } else {
  header = jws.NewHeader(jws.ES256(c.options.privateKey))
 }
 data, err := jws.Encode(header.Set("nonce", nonce).Set("url", url),
  payload, jws.WithSerialization(jws.JSONSerialization))
 if err != nil {
  return nil, err
 }
 req, err := http.NewRequest(http.MethodPost, url, bytes.NewBuffer([]byte(data)))
 if err != nil {
  return nil, err
 }
 req.Header.Set("Content-Type", "application/jose+json")
 return req, nil
}

func (c *Client) do(url string, payload any) ([]byte, *http.Response, error) {
 req, err := c.newRequest(url, payload)
 if err != nil {
  return nil, nil, err
 }
 resp, err := c.options.httpClient.Do(req)
 if err != nil {
  return nil, nil, err
 }
 defer resp.Body.Close()
 body, err := io.ReadAll(resp.Body)
 if err != nil {
  return nil, nil, err
 }
 return body, resp, nil
}
```

### 获取目录

首先我们需要通过`GET`请求来获取 ACME 服务商的目录信息，目录信息中包含了 ACME 服务商的一些信息，例如：`new-account`、`new-order`等等。

我们以 Let's Encrypt 为例，其中 DirectoryUrl 为`https://acme-v02.api.letsencrypt.org/directory`：

```go
type Directory struct {
 NewNonce   string `json:"newNonce"`
 NewAccount string `json:"newAccount"`
 NewOrder   string `json:"newOrder"`
 NewAuthz   string `json:"newAuthz"`
 RevokeCert string `json:"revokeCert"`
 KeyChange  string `json:"keyChange"`
 Meta       struct {
  TermsOfService          string   `json:"termsOfService"`
  Website                 string   `json:"website"`
  CaaIdentities           []string `json:"caaIdentities"`
  ExternalAccountRequired bool     `json:"externalAccountRequired"`
 } `json:"meta"`
}

func (c *Client) GetDirectory() (*Directory, error) {
 // 请求目录
 req, err := http.NewRequest(http.MethodGet, c.options.directoryUrl, nil)
 if err != nil {
  return nil, err
 }
 resp, err := c.options.httpClient.Do(req)
 if err != nil {
  return nil, err
 }
 defer resp.Body.Close()
 directory := &Directory{}
 // 解析目录
 if err := json.NewDecoder(resp.Body).Decode(directory); err != nil {
  return nil, err
 }
 c.options.directory = directory
 return directory, nil
}
```

### 注册账户

此处对应的是注册账户，我们需要在请求中携带上我们的邮箱信息，ACME 服务商会返回一个账户的唯一标识符（kid）。我们需要保存这个 kid 和私钥，后续的请求都需要携带上这个 kid，然后用私钥来签名。

注册账户请求需要带上 jws 信息，在`jws.ES256.PreCompute`中有对应实现

```go
func (c *Client) NewAccount(contact []string) (string, error) {
 body, resp, err := c.do(c.options.directory.NewAccount, map[string]interface{}{
  "termsOfServiceAgreed": true,
  "contact":              contact,
 })
 if err != nil {
  return "", err
 }
 if resp.StatusCode != http.StatusCreated {
  return "", fmt.Errorf("NewAccount failed: %s", body)
 }
 if resp.Header.Get("Location") == "" {
  return "", fmt.Errorf("location not found: %s", body)
 }
 return resp.Header.Get("Location"), nil
}
```

### 创建订单

此步骤对应创建订单，我们需要在请求中携带上我们的域名信息，ACME 服务商会返回一个订单的标识符（finalize）和授权（authorizations）。

```go
type Identifiers struct {
 Type  string `json:"type"`
 Value string `json:"value"`
}

type NewOrderResponse struct {
 Status      string    `json:"status"`
 Expires     time.Time `json:"expires"`
 NotBefore   time.Time `json:"notBefore"`
 NotAfter    time.Time `json:"notAfter"`
 Identifiers []struct {
  Type  string `json:"type"`
  Value string `json:"value"`
 } `json:"identifiers"`
 Authorizations []string `json:"authorizations"`
 Finalize       string   `json:"finalize"`
}

func (c *Client) NewOrder(identifiers []*Identifiers) (*NewOrderResponse, error) {
 body, resp, err := c.do(c.options.directory.NewOrder, map[string]interface{}{
  "identifiers": identifiers,
 })
 if err != nil {
  return nil, err
 }
 if resp.StatusCode != http.StatusCreated {
  return nil, fmt.Errorf("NewOrder failed: %s", body)
 }
 order := &NewOrderResponse{}
 if err := json.Unmarshal(body, order); err != nil {
  return nil, err
 }
 return order, nil
}
```

### 获取授权

在上一步创建成功订单后，ACME 会返回一个授权（authorizations）URL，我们需要请求这个 URL 来获取授权信息（challenges），然后我们需要根据这个信息来验证域名的所有权。

```go
type AuthorizationResponse struct {
 Identifier struct {
  Type  string `json:"type"`
  Value string `json:"value"`
 } `json:"identifier"`
 Status     string    `json:"status"`
 Expires    time.Time `json:"expires"`
 Challenges []struct {
  Type   string `json:"type"`
  Status string `json:"status"`
  Url    string `json:"url"`
  Token  string `json:"token"`
 } `json:"challenges"`
}

func (c *Client) GetAuthorization(url string) (*AuthorizationResponse, error) {
 body, resp, err := c.do(url, nil)
 if err != nil {
  return nil, err
 }
 if resp.StatusCode != http.StatusOK {
  return nil, fmt.Errorf("GetAuthorization failed: %s", body)
 }
 auth := &AuthorizationResponse{}
 if err := json.Unmarshal(body, auth); err != nil {
  return nil, err
 }
 return auth, nil
}
```

### 应对挑战

在上一步获取到授权信息后，我们需要根据这个信息来验证域名的所有权，我们需要在 DNS 中添加一条 TXT 记录，记录名为 `_acme-challenge`加上域名。

例如：`docs.scriptcat.org`那么记录名为 `_acme-challenge.docs.scriptcat.org`。

```go
func (c *Client) thumbprint() string {
 sha256Bytes := sha256.Sum256([]byte(jws.ES256Jwk(c.options.privateKey.PublicKey)))
 return base64.RawURLEncoding.EncodeToString(sha256Bytes[:])
}

func (c *Client) keyAuthorization(token string) string {
 return token + "." + c.thumbprint()
}

func (c *Client) ChallengeRecord(token string) string {
 hash := sha256.Sum256([]byte(c.keyAuthorization(token)))
 return base64.RawURLEncoding.EncodeToString(hash[:])
}
```

dns 记录设置好了之后，我们需要请求一次`challenges`URL，发送一个`{}`空对象来通知 ACME 服务器已经准备好挑战了，然后我们可以轮询`authorizations`URL 来查看验证状态，如果验证成功，那么我们的订单就会变成`valid`状态，这时候我们也可以将 dns 等记录删除了。

```go
type ChallengeResponse struct {
 Type             string `json:"type"`
 Status           string `json:"status"`
 Url              string `json:"url"`
 Token            string `json:"token"`
 ValidationRecord []struct {
  Hostname      string   `json:"hostname"`
  ResolverAddrs []string `json:"resolverAddrs"`
 } `json:"validationRecord"`
 Validated time.Time `json:"validated"`
}

// GetChallenge 获取挑战
func (c *Client) GetChallenge(url string) (*ChallengeResponse, error) {
 body, resp, err := c.do(url, nil)
 if err != nil {
  return nil, err
 }
 if resp.StatusCode != http.StatusOK {
  return nil, fmt.Errorf("GetChanllenge failed: %s", body)
 }
 challenge := &ChallengeResponse{}
 if err := json.Unmarshal(body, challenge); err != nil {
  return nil, err
 }
 return challenge, nil
}

// RequestChallenge 请求挑战
// 当你当http-01/dns-01记录准备好后，调用此接口
// 然后使用GetChallenge或者GetAuthorization轮询查看状态
func (c *Client) RequestChallenge(url string) (*ChallengeResponse, error) {
 body, resp, err := c.do(url, "{}")
 if err != nil {
  return nil, err
 }
 if resp.StatusCode != http.StatusOK {
  return nil, fmt.Errorf("RequestChallenge failed: %s", body)
 }
 challenge := &ChallengeResponse{}
 if err := json.Unmarshal(body, challenge); err != nil {
  return nil, err
 }
 return challenge, nil
}
```

### 请求颁发证书

请求颁发证书需要先生成一个 CSR 信息，然后请求`finalize`URL，ACME 服务商会返回我们的证书。其中必须要携带上我们的 CSR 信息。

下载下来的证书是一个 PEM 格式的证书，我们可以通过`x509.ParseCertificate`来解析证书，然后就可以使用了。

```go

type FinalizeResponse struct {
 Status      string    `json:"status"`
 Expires     time.Time `json:"expires"`
 Identifiers []struct {
  Type  string `json:"type"`
  Value string `json:"value"`
 } `json:"identifiers"`
 Authorizations []string `json:"authorizations"`
 Finalize       string   `json:"finalize"`
 Certificate    string   `json:"certificate"`
}

func (c *Client) Finalize(url string, csr []byte) (*FinalizeResponse, error) {
 body, resp, err := c.do(url, map[string]interface{}{
  "csr": base64.RawURLEncoding.EncodeToString(csr),
 })
 if err != nil {
  return nil, err
 }
 if resp.StatusCode != http.StatusOK {
  return nil, fmt.Errorf("FinalizeOrder failed: %s", body)
 }
 finalize := &FinalizeResponse{}
 if err := json.Unmarshal(body, finalize); err != nil {
  return nil, err
 }
 return finalize, nil
}

func CreateCertificateRequest(auth []*Identifiers) ([]byte, *ecdsa.PrivateKey, error) {
 csr := x509.CertificateRequest{}
 for _, v := range auth {
  switch v.Type {
  case "dns":
   csr.DNSNames = append(csr.DNSNames, v.Value)
  }
 }
 k, err := ecdsa.GenerateKey(elliptic.P256(), rand.Reader)
 if err != nil {
  return nil, nil, err
 }
 b, err := x509.CreateCertificateRequest(rand.Reader, &csr, k)
 if err != nil {
  return nil, nil, err
 }
 return b, k, nil
}

func (c *Client) GetCertificate(url string) ([]byte, error) {
 body, resp, err := c.do(url, nil)
 if err != nil {
  return nil, err
 }
 if resp.StatusCode != http.StatusOK {
  return nil, fmt.Errorf("GetCertificate failed: %s", body)
 }
 return body, nil
}
```

## 测试

我们可以写一个方法来测试一下上面的流程是否可行，代码我就不贴这里了，大家可以到这里查看：[acme_test.go](https://github.com/CodFrm/dns-kit/blob/6dcac9b084a8487188af9eb58c5e411b489cedbe/pkg/acme/acme_test.go)。

这是最开始与本文写的一致的一个版本，后续会有所改动，可以回到主分支查看。

## 结尾

其中也参考了官方写的`acme`包：golang.org/x/crypto/acme，虽然已经有很多现成的包可以使用，也有很多文档，但是理论和实践差距还是挺大的，其中很多坑还是得自己趟一遍才能理解。

这次实现又接触到了不少东西：CSR、JWS、JWK、ES256、x509 等等，大家有兴趣也可以再深入一下，有空我也再写几篇，整理一下这些内容。
