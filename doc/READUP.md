# Introduction

This work is inspired by https://github.com/jamesfreeman959/selinux-hands-on-labs which provides a rich overview of SELinux features.
Readup of the lab is recommended.
This lab focusses rather on applying the tools for creating a security system design.


## Setup
For this lab I am using Fedora 44 in a Virtual Machine. 


Installed packages


```
sudo dnf install selinux-policy-devel \
                 policycoreutils \
                 policycoreutils-python-utils \
                 setools-console \
                 checkpolicy


python3-setools
apol
python3-pygraphviz 
graphviz

seinfoflow
setools-gui


```


# Scenario Layout 

We start out with a simple layout:

A server and client want to establish a trusted communication channel using SELinux.

Both the client and the server use a configuration file, which should only be accessible by the corresponding party.

Hence the target system layout, which we try to model using SELinux comprises of an outer Trust Boundary, which isolates the IPC from the 
operating environment, while at the same time both client and server also implement their own trust boundary.


```
+----------------------- System Trust Boundary --------------------------+
|                                                                        |
|  +--- Client Trust Boundary ---+    +--- Server Trust Boundary ---+    |
|  |                             |    |                             |    |
|  |  Client                     |    |  Server                     |    |
|  |  client_t                   |    |  server_t                   |    |
|  |                             |    |                             |    |
|  |  +-----------------------+  |    |  +-----------------------+  |    |
|  |  | client.json           |  |    |  | server.json           |  |    |
|  |  +-----------------------+  |    |  +-----------------------+  |    |
|  |                             |    |                             |    |
|  +-------------+---------------+    +---------------+-------------+    |
|                |                                    ^                  |
|                |         Unix Socket IPC            |                  |
|                +------------------------------------+                  |
|                                  |                                     |
|                          /tmp/uds.sock                                 |
|                          socket_t                                      |
|                                                                        |
+------------------------------------------------------------------------+
```
















- Objects: 
    - Config: client.json 
    - Config: server.json 
    - IPC: /tmp/uds.sock


We will investigate and show a way to systematically analyze shortcomings of specific system designs.




## Setup

After copying the client and server to /usr/local/bin, let's first assign the corresponding SELinux labels:

```
dev@fedora:/usr/local/bin$ ls -alZ
total 796
drwxr-xr-x. 1 root root system_u:object_r:bin_t:s0         24 Oct 21 10:52 .
drwxr-xr-x. 1 root root system_u:object_r:usr_t:s0         90 Apr 22 15:58 ..
-rwxr-xr-x. 1 root root unconfined_u:object_r:bin_t:s0 397624 Oct 21 10:51 client
-rwxr-xr-x. 1 root root unconfined_u:object_r:bin_t:s0 412016 Oct 21 10:52 server
```




Further ressources:

- https://github.com/jamesfreeman959/selinux-hands-on-labs