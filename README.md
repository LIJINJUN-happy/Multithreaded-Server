# Multithreaded-Server
多线程服务器

此为一个单进程多线程服务器
一个定时器线程（处理计时间隔触发以及定时触发的事件）ClassTimer
一个epoll线程（获取最新动态，且监听端口）ClassNet
多个轮询任务列表线程（获取任务列表资源锁得到任务，从而共同派发执行）ClassPthread
以及一个监视线程（用于检测各个线程的状态的稳定以及存活否，以及每个客户的心跳反馈）



开发遇见的问题以及心得：

1.首先是库链接
由于用到线程头文件#include <pthread.h> ，编译程序包括 预编译, 编译,汇编,链接,包含头文件了,仅能说明有了线程函数的声明, 但是还没有实现, 加上-lpthread是在链接阶段,链接这个库

其次是-std=c++11,gcc是一个C 编译器 === -std= (c++11/gnu++11) -std指编译器可支持的C++标准类型。不包含链接的话，例如我们使用到c++11中的 to_string函数的时候会报错。

以及数据库mysql链接问题，即使包含了mysql.h文件也会报错====>mysql.h:没有那个文件或目录，是缺少 libmysqlclient-dev，OK安装就是了，centos下 : yum install mysql-devel 


2.线程问题
线程在编译的时候假如我们在非主函数内生产，但是非主函数内结束了，在其间生产的线程会出现报错，现解决方式是在主函数生产子线程，这样避免了在非主函数作用域（生命周期结束了）后子线程的崩溃

线程由于是统一进程内共享资源的，难免会有资源争抢的时候，这个时候我们可以使用互斥锁（pthread_mutex_t）来解决，获取锁（pthread_mutex_lock）的线程可以访问资源，而没有获得锁的线程会等待阻塞等到锁被解开（pthread_mutex_unlock）然后争抢到后才能访问资源。

由于线程在运行的时候需要占用CPU的资源，假如线程一直运行，但是又没事情可做会导致CPU的资源浪费，这时候我们可以让线程去通过睡眠以及唤醒来合理利用CPU资源（linux通过 top -u 用户名 查看）。我们可以通过使用（pthread_cond_t）条件变量来解决，也就是当条件达到要求了我们才去唤醒线程（pthread_cond_signal），其余时间让线程处于睡眠（阻塞等待pthread_cond_wait，不占用CPU资源）。要注意的是，pthread_cond_wait需要搭配我们的互斥锁（pthread_mutex_t）使用，pthread_cond_wait的机制是，把线程加入等待列队，当接收到唤醒（pthread_cond_signal）的适合他才会去工作，而且，在wait函数调用的时候，假如线程加入等待队列成功后，会自动解锁，以便让其他线程也获取资源，否者，其他线程无法进入睡眠或者是获取资源。当唤醒的时候，在沉睡的所有线程会去争夺这个锁，或许会根据睡眠时间，又或许是根据设置的比例，又或许根据.....，争夺到锁的线程被唤醒成功然后继续执行，然后解锁，让其余沉睡线程继续争夺锁。 
从何使得线程在有需要的时候才工作，其余时间可以睡眠减轻CPU的重担。


网络问题：
在网络编程，封装了TCP类模块，ocket bind listen函数的基操，然后利用epoll进行监视，头文件<sys/epoll.h>。
epoll 是 select 以及 poll 的进化体，select 和 poll都是轮询所有的连接对象（客户端的描述套接字fd）来查看是否有消息要处理，效率慢。而epoll则是检测出有信息的套接字然后装进容器内，并返回检测出的套接字数量，我们便可以通过数量做一个循环来从容器内取出套接字并且获取信息，这样就不用遍历所有的套接字描述符，减少了不必要的开支。
epll用法：
epoll_create 创建epoll的描述符
epoll_ctl 开始监视套接字（如服务端的监听套接字，客户端返回的套接字）都可以加入其中
然后通过 epoll_wait来阻塞监视，有信息获取的时候便会返回结果，我们可以设一个循环来获取epoll_wait的返回，它返回的是一个int 表示有变化的fd数量，通过数量我们循环获取事件列表，获取套接字描述符 （事件列表[i].data.fd） 以及事件 （事件列表[i].event），然后进行判断，假如fd是服务器以及event是（EPOLLIN）便进行 accept 函数接受客户端的连接，否者fd就是客户端的描述符，然后我们可以同理根据事件来处理客户端的事情。
