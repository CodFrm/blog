---
title: Git命令总结
---

> Git 是一款免费、开源的分布式版本控制系统，用于敏捷高效地处理任何或小或大的项目。[1] Git 的读音为/gɪt/。
> Git 是一个开源的分布式版本控制系统，可以有效、高速的处理从很小到非常大的项目版本管理。[2] Git 是 Linus Torvalds 为了帮助管理 Linux 内核开发而开发的一个开放源码的版本控制软件。
>
> 学习了一下 git,但是很多命令暂时都还没记住,于是将现在所学到的 git 命令记录下来,方便查询
>
> 至于它们的作用,可以从下面的链接去学习
>
> [GIT 教程跳转](http://www.liaoxuefeng.com/wiki/0013739516305929606dd18361248578c67b8067c8c017b000)

![img](img/Git命令总结.assets/0.jpg)

## 初始配置

配置用户名

git config --global user.name "CodFrm"

配置邮箱

git config --global user.email "yz@ggnb.top"

--global 表示全局 在整个 git 里生效

## Git 仓库命令

初始化 git 仓库

git init

将文件加入 git 仓库

git add -f file1 file2

-f 表示强制添加

将文件提交到仓库

git commit -m “提交说明”

查看仓库状态

git status

查看上一次文件变动

git diff

查看指定文件变动

git diff -- file

文件删除

git rm file

跳转到其他版本,如果是老的版本,使用 git log 将看不到之前的新版本,使用 git reflog

git reset --hard HEAD^

HEAD^表示上一个版本 HEAD^^表示上两个 HEAD~10 表示上 10 个

HEAD^ 也可以用 commit_id 表示想要跳转到的版本

### 分支

创建并切换分支

git checkout -b 分支名字

创建分支

git branch 分支

切换分支

git checkout 分支

查看分支,当前分支有\*

git branch

删除分支

git branch -d 分支

-D 强制删除

合并分支到当前分支

git merge 分支

### 日志

提交历史,查看每个版本的信息

git log

显示比较简单的信息 --pretty=oneline

显示分支合并图 --graph

命令历史,查看每个命令的信息

git reflog

撤销修改,回到该版本最初样式

git checkout -- file

将暂存区的文件撤回

git reset HEAD file

## 远程仓库

创建 SSH KEY

ssh-keygen -t rsa -C "code.farmer@qq.com"

关联远程库

git remote add origin git@github.com:CodFrm/StudyGit.git

克隆远程仓库

git clone git@github.com:CodFrm/StudyGit.git

克隆分支到本地

git checkout -b 本地分支名 origin/远程分支名

查看远程库的信息

git remote

git remote -v 显示更详细的信息

推送分支

git push origin 分支名字

第一次请加上-u 参数 git push -u origin 分支名字

从远程库获取新版本然后合并

git pull origin master

从远程库获取最新的分支版本

git fetch origin master

pull 和 fetch 理解还真有点困难....最好实践一下

fetch 是从远程拉取新加入的版本,要用 reset 跳转到这个新的版本上去才行,否则只是拉取

pull 就是从远程拉取了这些新的版本,然后将最新的和现在的合并

相当于 fetch 然后再 merge

### 暂存现场

储存现场

git stash

查看现场

git stash list

还原现场

git stash apply

删除现场

git stash drop

还原并删除现场

git stash pop

### 标签

设置标签

git tag 标签名字 commit_id

commit_id 可选

删除标签

git tag -d 标签名字

推送标签,推送标签到远程仓库

git push origin 标签名

git push origin --tags 推送所有标签

查看标签

git tag

查看标签信息

git show 标签名字

给标签说明

git tag -a 标签名字 -m "说明" commit_id

删除远程标签

git push origin :refs/tags/标签名字

## 其他

给 git 命令设置别名

git config --global alias.别名 命令
