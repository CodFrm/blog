entrypoint是容器执行的命令cmd是执行的命令或者参数.

```bash
docker run --entrypoint /bin/bash test bash
#          覆盖ENTRYPOINT  覆盖CMD
```

```dockerfile
ENTRYPOINT ["/bin/echo", "hello"]
CMD ["world"]
```

![image-20210114105711640](entrypoint.assets/image-20210114105711640.png)

像这样执行结果是:`Hello world`

覆盖CMD的参数

```bash
docker run test eicas
```

![image-20210114111521900](entrypoint.assets/image-20210114111521900.png)

感觉就像是将两个命令拼接起来

### 在k8s中的对应

[k8s文档地址](https://kubernetes.io/zh/docs/tasks/inject-data-application/define-command-argument-container/#notes)

![image-20210114112244500](entrypoint.assets/image-20210114112244500.png)



### CMD和ENTRYPOINT执行的两种形式

```dockerfile
CMD "命令/参数"
# 以shell执行,相当于会自动补全"/bin/sh -c ",等价于CMD ["/bin/sh -c ","\"命令/参数\""],ENTRYPOINT也是如此
```

![image-20210114112052386](entrypoint.assets/image-20210114112052386.png)

![image-20210114105605169](https://i.loli.net/2021/01/14/6LMPTFvfbrkXjd1.png)

![image-20210114111116997](entrypoint.assets/image-20210114111116997.png)

ENTRYPOINT的shell方式下,无法接受参数

![image-20210114111300414](entrypoint.assets/image-20210114111300414.png)

```dockerfile
CMD ["命令/参数","命令/参数"]
# 以exec形式执行
```

![image-20210114105711640](entrypoint.assets/image-20210114105711640.png)

