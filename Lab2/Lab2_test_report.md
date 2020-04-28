# *Lab2 test report*

*from 我想开学队*

---

## 1 背景知识

---

### 1.1 超文本传输协议

超文本传输协议（HTTP）是当今Internet上最常用的应用程序协议。像许多网络协议一样，HTTP使用客户端-服务器模型。 HTTP客户端打开与HTTP服务器的网络连接，并发送HTTP请求消息。然后，服务器以HTTP响应消息进行回复，该消息通常包含客户端请求的某些资源（文件，文本，二进制数据）。为了方便起见，我们在本节中简要介绍HTTP消息格式和结构。 HTTP / 1.1的详细规范可以在RFC 2616-超文本传输协议-HTTP / 1.1中找到。

### 1.2 HTTP消息

HTTP消息是简单的格式化数据块。所有HTTP消息分为两种：请求消息和响应消息。请求消息请求来自Web服务器的操作。响应消息将请求的结果返回给客户端。请求和响应消息都具有相同的基本消息结构。

#### 1.2.1 消息格式

HTTP请求和响应消息包含3个组件：

- 描述消息的开始行
- 包含属性的标题块
- 以及包含数据的可选正文

每个组件的格式如下

##### 1.2.1.1 起始行

所有HTTP消息均以起始行开头。请求消息的起始行说明了怎么做。响应消息的起始行说明发生了什么。具体来说，起始行在请求消息中也称为请求行，在响应消息中也称为响应行。

请求行：请求行包含描述服务器应执行的操作的方法和描述执行该方法的资源的请求URL。请求行还包含一个HTTP版本，该版本告诉服务器客户端使用哪种HTTP方言。所有这些字段都由空格分隔。
请求行示例：

GET /index.html HTTP/1.1

响应行：响应行包含响应消息使用的HTTP版本，数字状态代码和描述操作状态的文本原因短语。
响应行示例：

HTTP/1.1 200 OK

##### 1.2.1.2 标头

在开始行之后是零个，一个或多个HTTP标头字段的列表。 HTTP标头字段将其他信息添加到请求和响应消息。它们基本上只是名称/值对的列表。每个HTTP标头都有一个简单的语法：名称，后跟冒号（:)，后跟可选的空格，后跟字段值，然后是CRLF。

HTTP标头分为：常规标头，请求标头，响应标头，实体标头和扩展标头。请注意，请求标头字段与响应标头字段不同。我们不会详细介绍这些字段，并且您无需在本实验中实施。您可以在RFC 2616-超文本传输协议-HTTP / 1.1中找到它们。

请求中的标头示例：

```
Host: 127.0.0.1:8888
User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:74.0) Gecko/20100101 Firefox/74.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate
Connection: keep-alive
Upgrade-Insecure-Requests: 1
Cache-Control: max-age=0
								     // CRLF
```

响应中的标头示例：

```
Server: Guo's Web Server
Content-length: 248
Content-type: text/html
									// CRLF
```

##### 1.2.1.3 实体主体

HTTP消息的第三部分是可选的实体主体。实体是HTTP消息的有效负载。它们是HTTP旨在传递的东西。

HTTP消息可以承载多种数字数据：图像，视频，HTML文档，软件应用程序，信用卡交易，电子邮件等等。

实体的示例：

```
<html><head>
<title>CS06142</title>
</head><body>
<h1>CS06142</h1>
<p>Welcome to Cloud Computing Course.<br />
</p>
<hr>
<address>Http Server at ip-127-0-0-1 Port 8888</address>
</body></html>
```

#### 1.2.2 HTTP请求结构

HTTP请求消息包含HTTP请求行（包含方法，查询字符串和HTTP协议版本），零个或多个HTTP标头行和空白行（即CRLF）。

HTTP请求消息示例：

```
GET /index.html HTTP/1.1   
Host: 127.0.0.1:8888
User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:74.0) Gecko/20100101 Firefox/74.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate
Connection: keep-alive
Upgrade-Insecure-Requests: 1
Cache-Control: max-age=0
								     // CRLF
```

#### 1.2.3 HTTP响应结构

HTTP响应消息包含HTTP响应状态行（包含HTTP协议版本，状态代码和状态代码的描述），零个或多个HTTP标头行，空行（即，CRLF本身）和内容HTTP请求所请求。

HTTP响应消息示例：

```
HTTP/1.1 200 OK  					
Server: Tiny Web Server
Content-length: 248
Content-type: text/html
									// CRLF
<html><head>
<title>CS06142</title>
</head><body>
<h1>CS06142</h1>
<p>Welcome to Cloud Computing Course.<br />
</p>
<hr>
<address>Http Server at ip-127-0-0-1 Port 8888</address>
</body></html>
```

### 1.3 HTTP代理

HTTP代理服务器是中介。代理位于客户端和服务器之间，并充当“中间人”，在各方之间来回往返移动HTTP消息。

HTTP代理服务器是代表客户端完成事务的中间人。如果没有HTTP代理，HTTP客户端将直接与HTTP服务器通信。客户端使用HTTP代理与代理进行对话，而代理本身又代表客户端与服务器通信。

HTTP代理服务器既是Web服务器又是Web客户端。因为HTTP客户端将请求消息发送到代理，所以代理服务器必须像Web服务器一样正确处理请求和连接并返回响应。同时，代理本身将请求发送到服务器，因此它也必须像正确的HTTP客户端一样工作，发送请求并接收响应。

HTTP代理的工作模式如下图所示：

```
                               +-----------+               +-----------+
                               |           |               |           |
   +----------+    Request     |           |   Request     |           |
   |          |+--------------->           |+-------------->           |
   |  Client  |                |   Proxy   |               |   Server  |
   |          <---------------+|           <--------------+|           |          
   +----------+	   Response    |           |   Response    |           |          
                               |           |               |           |
                               +-----------+               +-----------+
```

---

## 2.**实验概述**

您可以使用从我们的课程中学到的网络编程知识，从头开始构建基于HTTP / 1.1的HTTP服务器。另外，请尝试使用从该类中学到的高并发编程技巧来保证Web服务器的性能。

------

### 2.1 目标

练习基本的网络编程技能，例如使用套接字API，解析数据包；熟悉强大的高性能并发编程。

### 2.2 任务

实现自己的HTTP服务器，在本实验中，从头开始实现满足以下要求的HTTP服务器（基于HTTP / 1.1）

#### 2.2.1 HTTP服务器功能

从网络角度来看，您的HTTP服务器应实现以下功能：

1. 创建一个监听套接字并将其绑定到端口
2. 等待客户端连接到端口
3. 接受客户端并获取新的连接套接字
4. 读入并解析HTTP请求
5. 开始提供服务：1）处理HTTP GET / POST请求，如果发生错误，则返回错误消息。 2）将请求代理到另一个HTTP服务器（高级版本可选）。

服务器将处于非代理模式或代理模式（我们已在1.3节中介绍了代理）。它不会同时做两件事。

#### 2.2.2 处理**HTTP**请求

在本实验中，您只需要在HTTP服务器中实现GET方法和POST方法。也就是说，如果您的HTTP服务器接收到HTTP请求，但是请求方法既不是GET也不是POST。 HTTP服务器仅需要返回501 Not Implemented错误消息（响应消息的响应行的状态码为501，请参见2.2）。不需要处理请求中的标题行（但是您需要识别它，以便您不会将其与下一个请求的起始行混合使用）。另外，您可以随意在HTTP响应中填充任何（或零个）标题行。

##### 2.2.2.1 处理**HTTP GET**请求

HTTP服务器应该能够处理html页面的HTTP GET请求。如果HTTP请求的路径与html页面文件相对应，则以200 OK和文件的完整内容进行响应。例如，如果请求GET /index.html，并且在文件目录中存在一个名为index.html的文件。您还应该能够处理对文件目录子目录中文件的请求（例如GET /images/hero.jpg）。如果HTTP请求的路径与目录相对应，并且该目录包含index.html文件，则响应200 OK，并在该文件夹中显示index.html文件的全部内容。如果所请求的页面文件不存在，或者所请求的目录不包含index.html文件，则返回404 Not Found响应（HTTP正文是可选的）。

##### 2.2.2.2 处理**HTTP POST**请求

HTTP服务器应该能够处理HTTP POST请求。在本实验中，处理HTTP POST的方法非常简单。您应该构造一个包含2个键的HTTP POST请求（请参阅第3.1.7.2节）：“名称”和“ ID”（请分别填写您的姓名和学生编号），然后将POST请求发送到/ Post_show（即http： //127.0.0.1:8888/Post_show（如果服务器的IP为127.0.0.1，服务端口为8888）。然后，如果HTTP服务器收到此POST请求（请求URL为/ Post_show，并且键为“ Name”和“ ID”），则响应为200 OK，并回显已发送的“ Name”-“ ID”对（请参阅第3.1.7.2节），否则（即请求网址不是/ Post_show或键不是“名称”和“ ID”），则返回404 Not Found响应消息。

##### 2.2.2.3 其他要求

只需为其他请求方法返回501未实现错误消息（例如DELETE，PUT等，请参见第3.1.7.3节）。

#### 2.2.3 实施代理服务器（可选）

使您的服务器能够将HTTP请求代理到另一个HTTP服务器，并将响应转发给客户端。您应使用--proxy命令行参数的值，该参数包含上游HTTP服务器的地址和端口号。您的代理服务器应在两个套接字（HTTP客户端fd和上游HTTP服务器fd）上等待新数据。当数据到达时，您应该立即将其读取到缓冲区，然后将其写入另一个套接字。本质上，您正在维护HTTP客户端和上游HTTP服务器之间的双向通信。请注意，您的代理服务器必须支持多个请求/响应。如果其中一个套接字关闭，则通信无法继续，因此您应该关闭另一个套接字并退出子进程。

提示：1）这比写入文件或从stdin读取更为棘手，因为您不知道2路流的哪一侧先写入数据，或者不知道它们在收到响应后是否会写入更多数据。 2）您应该再次使用线程执行此任务。例如，考虑使用两个线程来促进双向通信，一个线程从A到B，另一个线程从B到A。

#### 2.2.4 使用多线程增加并发

您的HTTP服务器应使用多个线程来处理尽可能多的并发客户端的请求。您至少具有以下三个选项来构建多线程服务器：

按需线程：每当有新客户端进入时，您都可以创建一个新线程，并使用该线程来处理所有客户端的任务，包括解析HTTP请求，获取页面文件和发送响应。客户端完成后可以销毁线程，例如，通过TCP recv（）检测。但是，在HTTP层中检测客户端是否完成可能并不容易。

永远在线的线程池：您可以在HTTP服务器程序中使用固定大小的线程池来同时处理多个客户端请求。如果没有任务，则这些线程处于等待状态。如果有新客户端进入，请分配一个线程来处理该客户端的请求并发送响应。如果分配的线程繁忙，则可以使用工作队列来缓冲请求，然后让线程稍后对其进行处理。

合并：将以上两种样式结合在一起。例如，您可以使用线程池来接收请求和发送响应，并使用按需线程来获取大页面文件。

随意选择以上三个中的任何一个，或使用您认为很酷的其他多线程体系结构。

#### 2.2.5 指定参数

您的程序应启用长选项以接受参数并在启动期间指定这些参数。它们是--ip，-port和--proxy（可选）。

--ip-指定服务器IP地址。
--port-选择HTTP服务器侦听传入连接的端口。
--proxy-选择一个“上游” HTTP服务器进行代理。该参数可以在冒号后面有一个端口号（例如https://www.CS06142.com:80）。如果未指定端口号，则默认为端口80。

#### 2.2.6 运行HTTP服务器

您的程序应该能够在终端启动。如果您的程序名为httpserver，则只需键入：

在非代理模式下：

./httpserver --ip 127.0.0.1 --port 8888 --number-thread 8

这意味着您的HTTP服务器的IP地址为127.0.0.1，服务端口为8888。--number-thread表示线程池中有8个线程可同时处理多个客户端请求。

---

在代理模式下：

./httpserver --ip 127.0.0.1 --port 8888 --number-thread 8 --proxy https://www.CS06142.com:80

这意味着这是一个HTTP代理。该代理的IP地址为127.0.0.1，服务端口为8888。该代理具有8个线程的线程池。 --proxy表示“上游” HTTP服务器为https://www.CS06142.COM:80。因此，如果您向该代理发送请求消息（即127.0.0.1:8888），它将将该请求消息转发至“上游” HTTP服务器（即https://www.CS06142.com:80）并转发给客户端的响应消息。

当您运行上面的命令时，您的HTTP服务器应该正确运行。

#### 2.2.7 访问HTTP服务器

##### 2.2.7.1 使用GET方法

您可以通过打开Web浏览器并转到相应的URL来检查HTTP服务器是否正常工作。[注] IP 127.0.0.1是指本地主机的IP。因此，您可以使用127.0.0.1在同一台本地计算机上测试您的HTTP服务器。
例如：



您还可以使用curl程序发送HTTP请求。如何使用curl的示例是：
curl -i -X GET http://127.0.0.1:8888/index.html



如果请求页面不存在，则您的HTTP服务器应返回404 Not Found错误消息。




##### 2.2.7.2 使用POST方法

您可以通过使用curl程序发送HTTP请求来检查POST方法是否有效。在终端上键入命令：
curl -i -X POST --data'名称= HNU＆ID = CS06142'http://127.0.0.1:8888/Post_show



如果请求URL不是/ Post_show或键不是“ Name”和“ ID”），则会收到404 Not Found错误消息。




您还可以构造POST HTTP请求，并使用某些浏览器插件工具将请求发送到HTTP服务器。

##### 2.2.7.3 其他方法

除GET请求和POST请求外，HTTP服务器将不处理HTTP请求。如果您发送HTTP DELETE（或PUT，HEAD等）请求以删除指定的资源，则会收到501 Not Implemented错误消息：



#### 2.2.8 实施版本

基本版本：每个TCP连接只能同时发送一个请求。客户端等待响应，当客户端获得响应时，也许将TCP连接重新用于新请求（或使用新的TCP连接）。这也是普通HTTP服务器支持的内容。

### 2.3 性能指标

服务器每秒可以处理多少个HTTP请求

### 2.4 实验环境

实验中共有4个不同的实验环境：

- **ENV1 周蜜**

  Linux内核版本为5.3.0-42-generic；2GB内存；20GB硬盘；供应商ID为AuthenticAMD；CPU型号为AMD Ryzen 5 2500U with Radeon Vega Mobile Gfx，共1个物理CPU；每个物理CPU有1个物理核心，共有1个物理核心；使用超线程技术，1个物理核心模拟出1个逻辑核心，共有1个逻辑核心。

- **ENV2 林子翔**

  Linux内核版本为4.15.0-91-generic；2GB内存；20GB硬盘；供应商ID为GenuineIntel；CPU型号为Intel(R) Core(TM) i5-9400F CPU @ 2.90GHz，共1个物理CPU；每个物理CPU有1个物理核心，共有1个物理核心；使用超线程技术，1个物理核心模拟出1个逻辑核心，共有1个逻辑核心。

- **ENV3 胡卓韬**

  Linux内核版本为5.3.0-42-generic；2GB内存；供应商ID为GenuineIntel；CPU型号为Intel(R) Core(TM) i7-7500U CPU @ 2.70GHz，共1个物理CPU；每个物理CPU有1个物理核心，共有1个物理核心；使用超线程技术，1个物理核心模拟出1个逻辑核心，共有1个逻辑核心。

- **ENV4 邓彬**

  linux内核版本为4.4.0-18362-Microsoft；8GB内存；供应商ID为Samsung；CPU型号为 Intel(R) Core(TM) i5-7300HQ CPU @2.50Ghz，共1个物理CPU；每个物理CPU共有1个物理核心，共有1个物理核心；使用超线程技术，1个物理核心模拟出1个逻辑核心，共有1个逻辑核心。

如无特别说明，默认使用ENV1。

---

## 3.**性能测试**

程序的性能会受到诸多因素的影响，其中包括软件层面的因素和硬件层面的因素。本节将分析比较在不同硬件环境下的服务器CPU内核数对性能指标的影响、同时向服务器发送请求的并发客户端数对性能指标的影响。

本测试遵循单一变量原则，变量包括：服务器CPU内核数，启用/禁用超线程，同时向服务器发送请求的并发客户端数。请注意，客户可能是性能瓶颈。因此，在测试性能时，最好使用多台计算机。例如，您可以在（三个组成员的）三台计算机上运行多个客户端进程，并在（另一个组成员的）另一台计算机上运行服务器进程。而且，网络也可能成为瓶颈。您可以根据网络环境的物理带宽来估计性能极限，并查看您的实现是否可以达到性能极限。

-----

### 3.1 在不同硬件环境下的服务器CPU内核数

测试在各种服务器机器环境上运行时，服务器每秒可以处理多少HTTP请求。

变量改动:更改虚拟机处理器内核的数量(限于物理机CPU核数，最高为2)

总任务数10000并发2个请求

| 内核数 | 1      | 2      |
| ------ | ------ | ------ |
| ENV1   | 3561.4 | 4015.3 |
| ENV2   | 4132.1 | 4977.5 |
| ENV3   | 3379.7 | 4092.4 |
| ENV4   | 3088.3 | 3417.5 |

![01](C:\Users\24950\Desktop\CloudComputingLabs-master\Lab2\01.png)

分析：

可以看到，随着虚拟机内核处理器数量的上升，服务器每秒处理的 http 请求数也在稳定上升，呈线性规律。

受制于电脑本身的 CPU 核数限制，最多只能设置虚拟机的处理器内核数量到 4，但可以预见，随着内核数上升，服务器每秒处理 http 请求数也会稳定上升。

原因在于，处理器内核数量在根本上决定了服务器的自身性能，也必然会带来处理能力的改变。

### 3.2 同时向服务器发送请求的并发客户端数

通过改变同时向服务器发送请求的并发客户端的数量，测试服务器每秒可以处理多少HTTP请求。

变量改动：改变同时向服务器端发送 http 请求的并发客户端数目（1-1000）

下方线状横轴代表同时向服务器端发送 http 请求的并发客户端数目(即-c 后的参数)，纵轴代表每秒服务器端处理的 http 请求数。

| 每秒请求数量\并发客户端数 | 100  | 200  | 300  | 400  | 500  |
| ------------------------- | ---- | ---- | ---- | ---- | ---- |
| 任务数1000                | 1972 | 2153 | 2234 | 2312 | 2298 |
| 任务数5000                | 2341 | 2253 | 2434 | 2312 | 1348 |

![02](C:\Users\24950\Desktop\CloudComputingLabs-master\Lab2\02.png)

分析：

可以看到，在任务量为1000的时候，并发客户端数的增加并没有对每秒请求数量有太大影响。但是在任务量为5000时，随着并发客户端数的增加，每秒请求数量会明显降低。

对于随着并发请求数增长，每秒处理请求数下降有两个方面解释：

①并发请求数的增大，带来了对系统内存资源的消耗，使得在使用 apache 模拟使用情况时出现了对系统内存资源更多的消耗，导致其每秒无力处理更多的请求。

②并发请求数的增大，对 CPU 的处理能力带来了更高的压力，相比较较小的并发请求数而言，服务器甚至无法达到原先的每秒处理请求数的能力。