# Redux Toolkit 入门

在此之前，我的 React 从来都是一把梭，可能是项目写得比较小的原因，也没感觉到过什么问题。

上一次自己手动封装了一个`useFetch`请求 API 获取数据，感觉这种写法还不错，觉得那些库是不是有更好的解决方案，于是来了解一下[Redux Toolkit](https://redux-toolkit.js.org/introduction/getting-started)。

官方已经不推荐使用`redux`了，而是推荐使用`redux-toolkit`，`RTK`对`redux`进行了封装，提供了一些工具函数，让我们更方便地使用`redux`。它们主要用于状态管理。当应用中有很多组件需要共享状态时，就可以使用`redux`来管理这些状态。

平常使用`useContext`也是足够的，但是当应用变得复杂时，就需要使用`redux`这类的框架来管理状态了，虽然我还没遇到这么复杂的应用，但是简单的了解一下也是不错的。

在请求 API 接口时，我们也可以使用`RTK Query`来管理数据获取，这样就不用自己封装`useFetch`了。

## 创建模板

首先，我们使用官方提供的模板来创建一个项目：

```bash
npx degit reduxjs/redux-templates/packages/vite-template-redux my-app
cd my-app
npm i
```

创建完成后，我们就可以使用`npm start`来启动项目，可以看到一个这样的页面

![image-20240416155405105](img/redux-toolkit.assets/image-20240416155405105.png)

可以进入`src/App.tsx`文件里看一下，具体的实现代码。

另外，我们也可以下载[Redux DevTools](https://chromewebstore.google.com/detail/redux-devtools/lmhkpmbekcpmknklioeibfkpmmfibljd)这个浏览器扩展来查看 Redux 的状态变化。

## 基础概念

我们根据上述创建的模板项目来学习下面的一些概念。

### Store

`store`是一个对象，它包含了应用中所有的`state`，在 Redux 中，只会有一个`store`。在示例中处于：`src/app/store.ts`文件中。

我们可以使用`configureStore`来创建一个`store`，并且可以传入一些参数，比如`reducer`、`middleware`等。`configureStore`是`createStore`的一个封装。

```ts
const store = configureStore({
  reducer: rootReducer,
  // Adding the api middleware enables caching, invalidation, polling,
  // and other useful features of `rtk-query`.
  middleware: (getDefaultMiddleware) => {
    return getDefaultMiddleware().concat(quotesApiSlice.middleware);
  },
  preloadedState,
});
```

### Action

`action`是一个对象，它描述了发生了什么事情。必须包含`type`字段，用来描述`action`的类型。另外，还可以包含`payload`字段，用来描述`action`的数据。

例如：

```ts
const addTodoAction = {
  type: "todos/todoAdded",
  payload: "Buy milk",
};
```

### Reducer

`reducer`是一个函数，它接收两个参数：`state`和`action`，然后返回一个新的`state`。它根据`action`的`type`来决定如何更新`state`。

例如：

```ts
const initialState = { value: 0 };

function counterReducer(state = initialState, action) {
  // 检查 reducer 是否关心这个 action
  if (action.type === "counter/increment") {
    // 如果是，复制 `state`
    return {
      ...state,
      // 使用新值更新 state 副本
      value: state.value + 1,
    };
  }
  // 返回原来的 state 不变
  return state;
}
```

### Dispatch

`dispatch`是一个函数，它用来发送`action`，然后`store`会调用`reducer`来更新`state`。

例如：

```ts
store.dispatch({ type: "counter/increment" });
```

### Selector

`selector`是一个函数，它用来从`state`中获取数据。`selector`可以接收`state`作为参数，然后返回需要的数据。当`state`过于复杂时，我们可以使用`selector`来简化获取数据的过程。

例如：

```ts
const selectCount = (state) => state.counter.value;

const count = selectCount(store.getState());
```

### Middleware

`middleware`是一个函数，可以用于记录 action 日志、异步请求等。`middleware`可以拦截`dispatch`，然后执行一些操作，最后再调用`dispatch`，这在`RTK Query`中经常使用。

### Slice

`slice`是一个对象，它包含了`reducer`、`action`和`selector`。`slice`可以用来封装`reducer`、`action`和`selector`，然后导出给其他模块使用。

可以使用`createSlice`来创建一个`slice`，它接收一个对象，包含`name`、`initialState`、`reducers`等字段。

例如：

```ts
const counterSlice = createSlice({
  name: "counter",
  initialState: { value: 0 },
  reducers: {
    increment: (state) => {
      state.value += 1;
    },
    decrement: (state) => {
      state.value -= 1;
    },
  },
});
```

## RTK Query

`RTK Query`是一个用于管理数据获取的库，它可以帮助我们管理数据获取的状态、缓存、轮询等。平常调用 API 接口时，我们就可以使用`RTK Query`来请求调用。

`RTK Query`提供了一些工具函数，比如`createApi`、`createApiSlice`等，用来创建一个`API`。

## 创建一个简单的示例

了解了上面的基础概念与模板项目后，我们可以来写一个简单的示例。

将模板项目的操作抽离出来了，只有单独的一个文件，这样看起来会比较清晰一点。

像下面的示例，实现了基本的`incr`和延迟`incr`的功能，同时也实现了一个简单的`API`请求。

`RTK Query`我实现了一个简单的`API`请求，获取`products`数据，然后展示在页面上，同时添加了 put 方法，`invalidatesTags`可以使缓存失效。

你可以先点击 view 按钮，来回切换多个`product`，你可以看到缓存的效果。当你点击 put 按钮时，会使缓存失效，再次点击 view 按钮，会重新请求数据。

```ts
import type { PayloadAction } from "@reduxjs/toolkit";
import {
  asyncThunkCreator,
  buildCreateSlice,
  combineSlices,
  configureStore,
} from "@reduxjs/toolkit";
import "./App.css";
// eslint-disable-next-line @typescript-eslint/no-restricted-imports
import { Provider, useDispatch, useSelector } from "react-redux";
import { useState } from "react";
import { createApi, fetchBaseQuery } from "@reduxjs/toolkit/query/react";

// `buildCreateSlice` allows us to create a slice with async thunks.
const createAppSlice = buildCreateSlice({
  creators: { asyncThunk: asyncThunkCreator },
});

interface Product {
  id: number;
  title: string;
  description?: string;
}

interface ProductsApiResponse {
  products: Product[];
  total: number;
  skip: number;
  limit: number;
}

// Define a service using a base URL and expected endpoints
const apiSlice = createApi({
  baseQuery: fetchBaseQuery({
    baseUrl: "https://dummyjson.com/",
    headers: {
      Authorization:
        "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZCI6MTUsInVzZXJuYW1lIjoia21pbmNoZWxsZSIsImVtYWlsIjoia21pbmNoZWxsZUBxcS5jb20iLCJmaXJzdE5hbWUiOiJKZWFubmUiLCJsYXN0TmFtZSI6IkhhbHZvcnNvbiIsImdlbmRlciI6ImZlbWFsZSIsImltYWdlIjoiaHR0cHM6Ly9yb2JvaGFzaC5vcmcvSmVhbm5lLnBuZz9zZXQ9c2V0NCIsImlhdCI6MTcxMzYyNTQ5OSwiZXhwIjoxNzEzNjI3Mjk5fQ.LiRqhAj26lexQJ5kHBxeVFo4ry1zTYnHXSDerrOH7T4",
    },
  }),
  reducerPath: "api",
  // Tag types are used for caching and invalidation.
  tagTypes: ["Products"],
  endpoints: (build) => ({
    // Supply generics for the return type (in this case `ProductsApiResponse`)
    // and the expected query argument. If there is no argument, use `void`
    // for the argument type instead.
    getProducts: build.query<ProductsApiResponse, number>({
      query: (limit = 10) => `/products?limit=${limit}`,
      // `providesTags` determines which 'tag' is attached to the
      // cached data returned by the query.
      providesTags: (result, error, id) => [{ type: "Products", id }],
    }),
    getProduct: build.query<Product, number>({
      query: (id) => `/products/${id}`,
      providesTags: (result, error, id) => [{ type: "Products", id }],
    }),
    putProduct: build.mutation<Product, Product>({
      query: (args: { id: number; title: string }) => ({
        url: "/products/" + args.id,
        method: "PUT",
        body: { title: args.title },
      }),
      invalidatesTags: (result, error, product) => [
        { type: "Products", id: product.id },
      ],
    }),
  }),
});

const { useGetProductsQuery, useGetProductQuery, usePutProductMutation } =
  apiSlice;

// 创建slice
const slice = createAppSlice({
  // slice名
  name: "blog",
  // 初始化state
  initialState: {
    title: "Redux Toolkit",
    readCount: 0,
  },
  // 创建一个reducer
  reducers: (create) => ({
    // 添加阅读数
    incrementReadCount: create.reducer((state) => {
      state.readCount++;
    }),
    // 根据传入的参数增加阅读数
    incrementReadCountBy: create.reducer(
      (state, action: PayloadAction<number>) => {
        state.readCount += action.payload;
      }
    ),
    // 延迟增加阅读数
    incrementAsyncReadCount: create.asyncThunk(
      async (count: number) => {
        return new Promise<number>((resolve) => {
          setTimeout(() => {
            resolve(count);
          }, 1000);
        });
      },
      {
        pending: (state) => {},
        fulfilled: (state, action) => {
          state.readCount += action.payload;
        },
        rejected: (state) => {},
      }
    ),
  }),
  // 创建一个selector
  selectors: {
    selectReadCount: (state) => state.readCount,
    selectTitle: (state) => state.title,
  },
});

// 导出selectors
const { selectReadCount, selectTitle } = slice.selectors;

// 导出actions
const { incrementReadCount, incrementReadCountBy, incrementAsyncReadCount } =
  slice.actions;

// 创建一个store
const store = configureStore({
  reducer: combineSlices(slice, apiSlice),
  // 添加一个中间件
  middleware: (getDefaultMiddleware) => {
    return getDefaultMiddleware()
      .concat((storeAPI) => {
        return (next) => (action) => {
          console.log(action);
          return next(action);
        };
      })
      .concat(apiSlice.middleware);
  },
});

// Infer the type of `store`
export type AppStore = typeof store;
// Infer the `AppDispatch` type from the store itself
export type AppDispatch = AppStore["dispatch"];

export const useAppDispatch = useDispatch.withTypes<AppDispatch>();

const App = () => {
  return (
    <Provider store={store}>
      <Blog />
    </Provider>
  );
};

const Blog = () => {
  // 使用useSelector来获取数据
  const readCount = useSelector(selectReadCount);
  const title = useSelector(selectTitle);
  const [incrBy, setIncrBy] = useState(2);
  const dispatch = useAppDispatch();
  const data = useGetProductsQuery(10);
  const [putProduct, { isLoading }] = usePutProductMutation();
  const [viewId, setViewId] = useState<number>(0);

  return (
    <div className="App">
      <h1>{title}</h1>
      <h2>ReadCount: {readCount}</h2>
      <input
        value={incrBy}
        onChange={(e) => setIncrBy(Number(e.target.value))}
      />
      <button
        onClick={() => {
          dispatch(incrementReadCount());
        }}
      >
        read
      </button>
      <button onClick={() => dispatch(incrementReadCountBy(incrBy))}>
        read by
      </button>
      <button onClick={() => dispatch(incrementAsyncReadCount(incrBy))}>
        async read by
      </button>
      <ul>
        {data.isLoading && <div>Loading...</div>}
        {data.isSuccess &&
          data.data?.products.map((product) => (
            <div>
              <li key={product.id}>{product.title}</li>
              <button
                onClick={() => {
                  setViewId(product.id);
                }}
              >
                view
              </button>
              <button
                onClick={() => {
                  // 变更
                  putProduct({
                    id: product.id,
                    title: product.title + " New",
                  });
                }}
              >
                put
              </button>
            </div>
          ))}
      </ul>
      <br />
      {viewId && <View id={viewId} />}
    </div>
  );
};

const View: React.FC<{ id: number }> = ({ id }) => {
  const view = useGetProductQuery(id);
  return (
    <div>
      <h1>{view.data?.title}</h1>
    </div>
  );
};

export default App;
```

## 最后

简单了看了模板项目与官方教程，然后写了一个简单示例，大概的了解了`RTK`的使用，感觉还是挺方便的，尤其是`RTK Query`，下次写项目时有机会用上再深入了解一下。

另外感觉`RTK`的文档组织得不太好，有些混乱，而且概念很多，不太容易理解，建议大家多看看官方的示例，多动手写一下。
