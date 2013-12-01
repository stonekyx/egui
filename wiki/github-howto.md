#怎样在EGUI项目里使用Git[Hub]#

##基本知识##
术语：

<dl>
<dt>Local machine</dt>
<dd>本地电脑，就是你的计算机。</dd>
<dt>Remote server</dt>
<dd>服务器，也就是github。</dd>
<dt>Repository</dt>
<dd>代码库，在服务器上有一个，本地电脑上也有一个。</dd>
<dt>Branch</dt>
<dd>分支，本质是指向commit的指针。服务器上有每个分支的记录，本地电脑上也有，但是两者不一定同步。本地电脑上有两种分支，分别是远程分支和本地分支，解释见下。</dd>
<dt>Local branch</dt>
<dd>本地分支，就是你平常checkout的时候指定的分支，`git checkout -b <branch_name>`创建的也是本地分支。`git branch`可以列出本地分支。</dd>
<dt>Remote branch</dt>
<dd>远程分支，也是在<em>本地电脑</em>里，但是代表着服务器上的某个分支。`git branch -r`可以列出远程分支。上面提到的本地分支会追踪某个远程分支，但`git fetch`的时候只会更新远程分支，所以你checkout到本地分支的时候是看不到新代码的，而是会在`git status`里提示一句“xx commits behind”，表示该本地分支与对应远程分支的距离。</dd>
</dl>

##EGUI的git使用方法##

###本地跟踪多个服务器代码库###
* `git branch -vv`列出本地分支和它追踪的远程分支。远程分支都是像origin/master这样的，类似路径的格式，本地分支就没有那个/。
* 上面的origin/master中那个origin就是服务器的标识符。你可以任意增加其他的服务器，只要给它们起不同的名字即可。一般这个代码库被clone下来的时候，服务器的标识符默认是origin。
* `git remote add egui https://github.com/stonekyx/egui.git`可以增加一个叫做egui的服务器，它的地址就是后面那个。
* 此时就可以执行`git fetch egui`来获取这个新服务器上的分支记录了。`git branch -r`应该能看到若干以“egui/”开头的远程分支。
* `git branch egui-master egui/master`建立一个叫做egui-master的分支，它的起点是egui/master，并追踪这个远程分支。
* `git branch -vv`查看刚建立的新分支。

###更新本地分支记录###
预备知识：
* `git fetch`只更新origin（默认服务器）的记录
* `git fetch egui`更新egui这个服务器的记录
* `git fetch --all`更新所有服务器的记录

刚才说过，`git fetch`系列指令只更新远程分支记录，要怎样在本地分支里看到新的代码呢？
在`git checkout`到本地分支后，如果它追踪的远程分支有更新，git会提示说“xx commits behind”，并告诉你可以用`git pull`来合并修改。那么我们照做即可。（*注意！*如果上面提示的不只是“behind”，还有“xx different commits”的话，那么在`git pull`的时候会发生一个merge，这是不应该发生的。）

###Topic branch和Pull request###

<dl>
<dt>Topic branch</dt>
<dd>在开始写一个新功能或者修改一个bug的时候，应该建立一个新的本地分支。很多人把这个新的本地分支叫做topic branch，因为在这上面的修改都是为了同一个目的。</dd>
<dt>Pull request</dt>
<dd>在github项目主页右边应该能看到一个pull request按钮，这是多人合作开源项目的一种方法。主要的项目代码保存在某一个人的帐号里，其他人fork这个项目，并在自己fork出来的项目里修改代码。准备好一个新功能之后，向那个主要的维护人发起一个pull request（合并请求，其实就像论坛的帖子一样，可以和维护人进行讨论）。双方讨论完成以后，维护人就会把这份新代码合并到他的代码库里。</dd>
</dl>

Topic branch里的新功能还在开发的时候，这些代码是保存在开发人员（而非主要维护者）自己的项目里的，所以topic branch追踪的远程分支应该也在大家自己fork出去的代码库里。

###Rebase操作###
Topic branch一般是从某个代码库的master分支分出来的，并在上面开发新功能或者修改bug。但是这个修改期可能很长，在完成之前master很可能已经更新了，很可能master分支里的更新会影响到新功能的编写（比如基础架构发生了变化）。此时由于topic branch是另一个分支，通过它看不到master里的更新。怎么让它也能获得这些更新呢？

`git rebase`操作可以让一个分支重新构建在指定分支的顶端。比如，如果当前所在的分支是从master分出来的，那么`git rebase master`可以自动把分支建立以来的修改全部基于最新的master重新做一次。

但是rebase操作会改变历史（重新产生的commits编号将全部发生变化），如果要推送到服务器的话，万一有其他人也在追踪着这个分支，他们的本地分支就会发生混乱。所以对于一般的小修改，就不用让topic branch追踪远程分支了，只要全部准备好以后再push到服务器上即可（这次是必须，因为pull request只能合并服务器上的分支）。

##补充说明##
我并没有很丰富的使用git的经验，仍需要不断学习。EGUI使用的方法是我自己想出来的，不一定规范或有效。我能做的不过是把我掌握的全部都说出来而已，请大家自己学习git之后找到你自己的使用方法（很可能比我的要好）。我想大家的使用方法不同并不会导致项目代码发生混乱，因为最后还有pull request这一关守着，主要代码都是由一个人来控制的。
