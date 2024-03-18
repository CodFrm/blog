## resolver.Resolver
请求解析器,当一个http请求过来,解析到对应的服务

https://github.com/micro/go-micro/blob/master/api/resolver/resolver.go
```go
type Resolver interface {
	Resolve(r *http.Request) (*Endpoint, error)
	String() string
}
```
deme:(来自grpc)
```go
func (r *Resolver) Resolve(req *http.Request) (*resolver.Endpoint, error) {
	// /foo.Bar/Service
	if req.URL.Path == "/" {
		return nil, errors.New("unknown name")
	}
	// [foo.Bar, Service]
	parts := strings.Split(req.URL.Path[1:], "/")
	// [foo, Bar]
	name := strings.Split(parts[0], ".")
	// foo
	return &resolver.Endpoint{
		Name:   strings.Join(name[:len(name)-1], "."),//foo
		Host:   req.Host,
		Method: req.Method,
		Path:   req.URL.Path,
	}, nil
}
```

## Namespace

https://github.com/micro/go-micro/blob/master/api/handler/options.go
```go
type Option func(o *Options)
```


## Registry
服务注册发现接口

https://github.com/micro/go-micro/blob/master/registry/registry.go
```go
type Registry interface {
	Init(...Option) error
	Options() Options
	Register(*Service, ...RegisterOption) error
	Deregister(*Service) error
	GetService(string) ([]*Service, error)
	ListServices() ([]*Service, error)
	Watch(...WatchOption) (Watcher, error)
	String() string
}
```

## api.Router
路由接口
```go
type Router interface {
	// Returns options
	Options() Options
	// Stop the router
	Close() error
	// Endpoint returns an api.Service endpoint or an error if it does not exist
	Endpoint(r *http.Request) (*api.Service, error)
	// Route returns an api.Service route
	Route(r *http.Request) (*api.Service, error)
}
```